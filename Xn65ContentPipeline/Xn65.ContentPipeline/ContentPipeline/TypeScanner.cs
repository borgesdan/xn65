using System.Collections;
using System.Reflection;

namespace Xn65.Csharp.ContentPipeline
{
    internal class TypeScanner : IEnumerable<Type>, IEnumerable
    {
        private readonly Type baseType;
        private readonly Type attributeType;
        private readonly bool includeGenericDefinitions;
        private readonly Action<Exception>? customErrorHandler;

        public TypeScanner(Type baseType,
            Type attributeType,
            bool includeGenericDefinitions,
            Action<Exception>? customErrorHandler)
        {
            ArgumentNullException.ThrowIfNull(baseType);
            ArgumentNullException.ThrowIfNull(attributeType);

            this.baseType = baseType;
            this.attributeType = attributeType;
            this.includeGenericDefinitions = includeGenericDefinitions;
            this.customErrorHandler = customErrorHandler;
        }

        public IEnumerator<Type> GetEnumerator()
        {
            IEnumerable<Type> types = !WorkerAppDomain.InsideWorkerDomain
                ? GetAllLoadedTypes()
                : AssemblyScanner.GetSearchAssemblyTypes();

            foreach (Type type in types)
            {
                try
                {
                    if (type.IsDefined(attributeType, false))
                    {
                        if (!IsSuitableType(type))
                            throw new PipelineException(SR.TypeScannerError,
                            [
                                attributeType,
                                type,
                                baseType
                            ]);
                    }
                    else
                        continue;
                }
                catch (Exception ex)
                {
                    if (customErrorHandler != null)
                    {
                        customErrorHandler(ex);
                        continue;
                    }
                    throw;
                }
                yield return type;
            }
        }

        private IEnumerable<Type> GetAllLoadedTypes()
        {
            foreach (Assembly assembly in new AssemblyScanner(true))
            {
                Type[] types;
                try
                {
                    types = assembly.GetTypes();
                }
                catch
                {
                    continue;
                }
                foreach (Type allLoadedType in types)
                    yield return allLoadedType;
            }
        }

        private bool IsSuitableType(Type type)
        {
            if (!type.IsClass || type.IsAbstract || !includeGenericDefinitions && type.IsGenericTypeDefinition || type.GetConstructor(Type.EmptyTypes) == null)
                return false;

            if (!this.baseType.IsGenericTypeDefinition)
                return this.baseType.IsAssignableFrom(type);

            var baseType = type.BaseType;

            while (baseType != null && (!baseType.IsGenericType || baseType.GetGenericTypeDefinition() != this.baseType))
                baseType = baseType.BaseType;

            return baseType != null;
        }

        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}
