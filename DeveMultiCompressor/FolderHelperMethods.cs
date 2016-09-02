using System;
using System.IO;
using System.Reflection;

namespace DeveMultiCompressor
{
    public static class FolderHelperMethods
    {
        public static Lazy<string> AssemblyDirectory = new Lazy<string>(() => CreateAssemblyDirectory());

        private static string CreateAssemblyDirectory()
        {
            string codeBase = Assembly.GetExecutingAssembly().CodeBase;
            UriBuilder uri = new UriBuilder(codeBase);
            string path = Uri.UnescapeDataString(uri.Path);
            return Path.GetDirectoryName(path);
        }

        public static void ClearDirectory(string dir)
        {
            if (Directory.Exists(dir))
            {
                foreach (var file in Directory.GetFiles(dir))
                {
                    File.Delete(file);
                }

                foreach (var subDir in Directory.GetDirectories(dir))
                {
                    Directory.Delete(subDir, true);
                }
            }
        }
    }
}
