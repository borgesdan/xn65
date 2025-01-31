using System.Globalization;

namespace Xn65.Csharp.ContentPipeline
{
    internal abstract class TypeHandlerFactory<T> where T : class
    {
        readonly Dictionary<Type, Type> genericHandlers = [];

        protected abstract Type AttributeType { get; }
        protected abstract Type GenericType { get; }

        public List<T> Initialize()
        {
            var objList = new List<T>();

            foreach (Type type in new TypeScanner(typeof(T), AttributeType, true, null))
            {
                if (type.IsGenericTypeDefinition)
                    RegisterGenericHandler(type);
                else
                    objList.Add((T)Activator.CreateInstance(type)!);
            }

            return objList;
        }

        private void RegisterGenericHandler(Type handler)
        {
            var baseType = handler.BaseType
                ?? throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.BadGenericTypeHandler, typeof(T).Name, handler));

            while (!baseType.IsGenericType || baseType.GetGenericTypeDefinition() != GenericType)
            {
                baseType = baseType.BaseType
                    ?? throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.BadGenericTypeHandler, typeof(T).Name, handler));
            }

            var genericArgument = baseType.GetGenericArguments()[0];

            if (!genericArgument.IsGenericType)
                throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.BadGenericTypeHandler, typeof(T).Name, handler));

            Type key = TypeHandlerFactory<T>.TypeArraysAreEqual(handler.GetGenericArguments(), genericArgument.GetGenericArguments())
                ? genericArgument.GetGenericTypeDefinition()
                : throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.BadGenericTypeHandler, typeof(T).Name, handler));

            if (genericHandlers.ContainsKey(key))
                throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.DuplicateTypeHandler, typeof(T).Name, handler.AssemblyQualifiedName, genericHandlers[key].AssemblyQualifiedName, key));

            genericHandlers.Add(key, handler);
        }

        private static bool TypeArraysAreEqual(Type[] a, Type[] b)
        {
            if (a.Length != b.Length)
                return false;

            for (int index = 0; index < a.Length; ++index)
            {
                if (a[index] != b[index])
                    return false;
            }

            return true;
        }

        protected T? CreateHandler(Type type)
        {
            if (type.IsByRef || type.IsPointer)
                throw new ArgumentException(string.Format(CultureInfo.CurrentCulture, SR.BadTypePointerOrReference, type));

            if (type.ContainsGenericParameters)
                throw new ArgumentException(string.Format(CultureInfo.CurrentCulture, SR.BadTypeOpenGeneric, type));

            if (type.IsSubclassOf(typeof(Delegate)))
                throw new ArgumentException(string.Format(CultureInfo.CurrentCulture, SR.BadTypeDelegate, type));

            Type type1;

            return type.IsGenericType && genericHandlers.TryGetValue(type.GetGenericTypeDefinition(), out type1)
                ? (T?)Activator.CreateInstance(type1.MakeGenericType(type.GetGenericArguments()))
                : default;
        }
    }
}
