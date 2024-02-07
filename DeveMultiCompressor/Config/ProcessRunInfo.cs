using System;
using System.IO;

namespace DeveMultiCompressor.Config
{
    public class ProcessRunInfo
    {
        public string CompressorDir { get; private set; }
        public bool ShouldUseWine { get; private set; }
        public string Executable { get; private set; }
        public string ExecutableFullPath => Path.Combine(CompressorDir, Executable);

        public ProcessRunInfo(string compressorDir, CompressorConfig compressorConfig)
        {
            CompressorDir = compressorDir;
            if (OperatingSystem.IsWindows())
            {
                Executable = compressorConfig.CompressorExe;
            }
            else if (!string.IsNullOrWhiteSpace(compressorConfig.CompressorLinuxExe))
            {
                Executable = compressorConfig.CompressorLinuxExe;
            }
            else
            {
                Executable = compressorConfig.CompressorExe;
                ShouldUseWine = true;
            }
        }

        public override string ToString()
        {
            var wineString = ShouldUseWine ? "(wine) " : "";
            return $"{Executable} {wineString}";
        }
    }
}
