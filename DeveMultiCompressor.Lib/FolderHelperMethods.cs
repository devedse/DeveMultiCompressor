using System;
using System.IO;
using System.Reflection;

namespace DeveMultiCompressor.Lib
{
    public static class FolderHelperMethods
    {
        public static Lazy<string> AssemblyDirectory { get; } = new Lazy<string>(() => CreateLocationOfImageProcessorAssemblyDirectory());

        private static string CreateLocationOfImageProcessorAssemblyDirectory()
        {
            var assembly = typeof(FolderHelperMethods).GetTypeInfo().Assembly;
            var assemblyDir = Path.GetDirectoryName(assembly.Location);
            return assemblyDir;
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
