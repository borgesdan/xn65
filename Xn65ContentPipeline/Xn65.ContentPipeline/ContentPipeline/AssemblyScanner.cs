using System.Collections;
using System.Globalization;
using System.Reflection;

namespace Xn65.Csharp.ContentPipeline
{
    internal class AssemblyScanner(bool includeAllAssemblies) : IEnumerable<Assembly>, IEnumerable
    {
        private readonly bool includeAllAssemblies = includeAllAssemblies;
        private static readonly List<Assembly> loadedAssemblies =
        [
            typeof (AssemblyScanner).Assembly
        ];

        public static void LoadAssembly(string assembly, bool includeInSearch)
        {
            ArgumentNullException.ThrowIfNull(assembly);

            Assembly assembly1;

            if (File.Exists(assembly))
                assembly1 = !Path.IsPathRooted(assembly) || !new Uri(assembly).IsUnc
                    ? Assembly.LoadFrom(assembly)
                    : throw new PipelineException(string.Format(CultureInfo.CurrentCulture, SR.CantLoadPipelineAssemblyOffShare, assembly));
            else
                assembly1 = Assembly.Load(assembly);

            if (!includeInSearch || loadedAssemblies.Contains(assembly1))
                return;

            loadedAssemblies.Add(assembly1);
        }

        public static IEnumerable<Type> GetSearchAssemblyTypes()
        {
            var searchAssemblyTypes = new List<Type>();

            foreach (var loadedAssembly in loadedAssemblies)
            {
                try
                {
                    searchAssemblyTypes.AddRange(loadedAssembly.GetTypes());
                }
                catch (ReflectionTypeLoadException)
                {
                    throw new PipelineException(string.Format(CultureInfo.CurrentCulture, SR.CantLoadPipelineAssembly, loadedAssembly.FullName));
                }
            }

            return searchAssemblyTypes;
        }

        public IEnumerator<Assembly> GetEnumerator()
        {
            var pending = new Stack<Assembly>();
            var seen = new Dictionary<Assembly, bool>();

            IEnumerable<Assembly> searchRoots = !includeAllAssemblies
                ? loadedAssemblies
                : AppDomain.CurrentDomain.GetAssemblies();

            foreach (Assembly key in searchRoots)
            {
                if (!IsSystemAssembly(key.GetName().Name!))
                {
                    pending.Push(key);
                    seen[key] = true;
                }
            }

            while (pending.Count > 0)
            {
                var assembly = pending.Pop();
                yield return assembly;
                foreach (var referencedAssembly in assembly.GetReferencedAssemblies())
                {
                    if (!IsSystemAssembly(referencedAssembly.Name!))
                    {
                        try
                        {
                            Assembly key = Assembly.Load(referencedAssembly);
                            if (!seen.ContainsKey(key))
                            {
                                pending.Push(key);
                                seen[key] = true;
                            }
                        }
                        catch
                        {
                        }
                    }
                }
            }
        }

        private bool IsSystemAssembly(string name)
        {
            if (includeAllAssemblies)
                return false;
            if (name == "mscorlib" || name == "System" || name.StartsWith("System.") || name.StartsWith("Microsoft.Build.") || name == "Microsoft.VisualC")
                return true;
            return name.StartsWith("Microsoft.Xna.Framework") && !name.StartsWith("Microsoft.Xna.Framework.Content.Pipeline");
        }

        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}
