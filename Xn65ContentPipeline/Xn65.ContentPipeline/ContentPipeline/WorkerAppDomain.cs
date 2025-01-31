using System.Globalization;
using System.Reflection;

namespace Xn65.Csharp.ContentPipeline
{
    internal class WorkerAppDomain : IDisposable
    {
        private AppDomain? appDomain;
        private RemoteProxy? remoteProxy;
        public static bool InsideWorkerDomain;

        public WorkerAppDomain()
        {
            appDomain = AppDomain.CurrentDomain;

            try
            {
                remoteProxy = Create<RemoteProxy>();
            }
            catch
            {
                Dispose();
                throw;
            }
        }

        public void Dispose()
        {
            if (remoteProxy != null)
            {
                remoteProxy = null;
            }

            if (appDomain == null)
                return;

            appDomain = null;
        }

        public T Create<T>()
        {
            if (appDomain == null)
                throw new InvalidOperationException();

            var type = typeof(T) ?? throw new InvalidOperationException();
            var obj = appDomain.CreateInstanceAndUnwrap(type.Assembly.FullName!, type.FullName!);

            return obj != null ? (T)obj : throw new InvalidOperationException();
        }


        public void LoadAssemblies(string[] searchAssemblies, string[] dependencies)
            => remoteProxy?.LoadAssemblies(searchAssemblies, dependencies);

        private class RemoteProxy : MarshalByRefObject
        {
            public RemoteProxy()
            {
                InsideWorkerDomain = true;
            }

            public void LoadAssemblies(string[] searchAssemblies, string[] dependencies)
            {
                LoadAssemblies(dependencies, false);
                LoadAssemblies(searchAssemblies, true);

                try
                {
                    AssemblyScanner.GetSearchAssemblyTypes();
                }
                catch (TypeLoadException ex)
                {
                    throw new PipelineException(string.Format(CultureInfo.CurrentCulture, SR.CantLoadPipelineAssembly, ex.TypeName));
                }

                VerifyVersionOfReferenceAssemblies();
            }

            private void VerifyVersionOfReferenceAssemblies()
            {
                var version1 = GetType().Assembly.GetName().Version;

                foreach (Assembly assembly in new AssemblyScanner(false))
                {
                    var name1 = assembly.GetName().Name;

                    foreach (AssemblyName referencedAssembly in assembly.GetReferencedAssemblies())
                    {
                        var name2 = referencedAssembly.Name;

                        if (StringMatchesXnaAssembly(name2!))
                        {
                            var version2 = referencedAssembly.Version;

                            if (version2 != version1)
                                throw new PipelineException(string.Format(CultureInfo.CurrentCulture, SR.AssemblyReferenceWrongVersion, name1, name2, version2?.ToString(), version1?.ToString()));
                        }
                    }
                }
            }

            private static bool StringMatchesXnaAssembly(string assemblyName)
            {
                switch (assemblyName)
                {
                    case "Microsoft.Xna.Framework":
                    case "Microsoft.Xna.Framework.Game":
                    case "Microsoft.Xna.Framework.GamerServices":
                    case "Microsoft.Xna.Framework.Graphics":
                    case "Microsoft.Xna.Framework.Xact":
                    case "Microsoft.Xna.Framework.Net":
                    case "Microsoft.Xna.Framework.Storage":
                    case "Microsoft.Xna.Framework.Input.Touch":
                    case "Microsoft.Xna.Framework.Video":
                    case "Microsoft.Xna.Framework.Avatar":
                    case "Microsoft.Xna.Content.Pipeline":
                        return true;
                    default:
                        return false;
                }
            }

            private static void LoadAssemblies(string[] assemblies, bool includeInSearch)
            {
                if (assemblies == null)
                    return;

                foreach (string assembly in assemblies)
                {
                    try
                    {
                        AssemblyScanner.LoadAssembly(assembly, includeInSearch);
                    }
                    catch (PipelineException)
                    {
                        throw;
                    }
                    catch (Exception)
                    {
                        throw new PipelineException(string.Format(CultureInfo.CurrentCulture, SR.CantLoadPipelineAssembly, assembly));
                    }
                }
            }
        }
    }
}
