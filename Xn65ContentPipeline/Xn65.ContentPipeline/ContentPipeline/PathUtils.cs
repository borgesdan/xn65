using System.Globalization;

namespace Xn65.Csharp.ContentPipeline
{
    internal static class PathUtils
    {
        public static string GetFullPath(string path)
        {
            if (string.IsNullOrEmpty(path))
                throw new ArgumentNullException(nameof(path));
            try
            {
                return Path.GetFullPath(path);
            }
            catch (ArgumentException ex)
            {
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)path), ex);
            }
            catch (NotSupportedException ex)
            {
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)path), ex);
            }
            catch (PathTooLongException ex)
            {
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.PathTooLong, (object)path), ex);
            }
        }

        public static void ThrowIfPathNotAbsolute(string path)
        {
            if (!GetFullPath(path).Equals(path.Replace(Path.AltDirectorySeparatorChar, Path.DirectorySeparatorChar), StringComparison.OrdinalIgnoreCase))
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.FilenameNotAbsolute, (object)path));
        }

        public static Uri GetAbsoluteUri(string path)
        {
            string fullPath = GetFullPath(path);
            Uri result;
            if (!Uri.TryCreate(fullPath, UriKind.Absolute, out result))
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)fullPath));
            return result.IsFile ? result : throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)fullPath));
        }

        public static string GetAbsolutePath(Uri baseUri, string filename)
        {
            if (string.IsNullOrEmpty(filename))
                throw new ArgumentNullException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)filename));
            Uri result;
            if (baseUri != null)
            {
                if (!Uri.TryCreate(baseUri, filename, out result))
                    throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)filename));
            }
            else if (!Uri.TryCreate(filename, UriKind.Absolute, out result))
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)filename));
            if (!result.IsAbsoluteUri)
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.FilenameNotAbsolute, (object)result));
            return result.IsFile ? result.LocalPath : throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)result));
        }

        public static string GetRelativePath(Uri baseUri, string filename)
        {
            Uri result;
            if (!Uri.TryCreate(filename, UriKind.Absolute, out result))
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)result));
            if (!result.IsFile)
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.InvalidFilename, (object)result));
            if (baseUri != null)
                result = baseUri.MakeRelativeUri(result);
            string str1 = Uri.UnescapeDataString(result.ToString());
            string[] strArray = new string[2]
            {
        "file:///",
        "file:"
            };
            foreach (string str2 in strArray)
            {
                if (str1.StartsWith(str2, StringComparison.OrdinalIgnoreCase))
                {
                    str1 = str1.Substring(str2.Length);
                    break;
                }
            }
            return str1.Replace(Path.AltDirectorySeparatorChar, Path.DirectorySeparatorChar);
        }

        public static DateTime GetTimestamp(string filename)
        {
            try
            {
                FileInfo fileInfo = new FileInfo(filename);
                if (fileInfo.Exists)
                    return fileInfo.LastWriteTime;
            }
            catch
            {
            }
            return DateTime.MinValue;
        }

        public static void SafeDeleteFile(string filename)
        {
            try
            {
                File.Delete(filename);
            }
            catch
            {
            }
        }

        public static void SafeDeleteFileAndDirectories(
          string filename,
          params string[] rootDirectories)
        {
            try
            {
                File.Delete(filename);
                while (true)
                {
                    filename = Path.GetDirectoryName(filename);
                    if (!string.IsNullOrEmpty(filename))
                    {
                        foreach (string rootDirectory in rootDirectories)
                        {
                            if (rootDirectory.StartsWith(filename, StringComparison.OrdinalIgnoreCase))
                                return;
                        }
                        if (Directory.GetFileSystemEntries(filename).Length == 0)
                            Directory.Delete(filename);
                        else
                            goto label_11;
                    }
                    else
                        break;
                }
                return;
            label_11:;
            }
            catch
            {
            }
        }

        public static void CreateDirectory(string directory)
        {
            try
            {
                Directory.CreateDirectory(directory);
            }
            catch (Exception ex)
            {
                throw new PipelineException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.CantCreateDirectory, (object)directory), ex);
            }
        }

        public static string GetFullDirectoryName(string path)
        {
            try
            {
                return GetFullPath(path + Path.DirectorySeparatorChar);
            }
            catch (ArgumentException ex)
            {
                throw new PipelineException(ex.Message);
            }
        }
    }
}
