using DeveMultiCompressor.Config;
using DeveMultiCompressor.Logging;
using System;
using System.Diagnostics;
using System.IO;

namespace DeveMultiCompressor
{
    public class ProcessRunner
    {
        private readonly ILogger _logger;

        public ProcessRunner(ILogger logger)
        {
            _logger = logger;
        }

        public void RunProcess(string directoryToRunFrom, ProcessRunInfo processRunInfo, string arguments)
        {
            Directory.SetCurrentDirectory(directoryToRunFrom);

            ProcessStartInfo psi;

            if (!processRunInfo.ShouldUseWine)
            {
                _logger.Write($"Running command: {processRunInfo.ExecutableFullPath} {arguments}");
                psi = new ProcessStartInfo(processRunInfo.ExecutableFullPath, arguments)
                {
                    //WorkingDirectory = Path.GetDirectoryName(_toolExePath)
                };
            }
            else
            {
                _logger.Write($"Running command: wine \"{processRunInfo.ExecutableFullPath}\" {arguments}");
                psi = new ProcessStartInfo("wine", $"\"{processRunInfo.ExecutableFullPath}\" {arguments}")
                {
                    //If you use a working directory, paths in Linux with wine don't work anymore
                    //WorkingDirectory = Path.GetDirectoryName(_toolExePath)
                };
            }

            var process = Process.Start(psi);

            process.WaitForExit();

            _logger.Write($"Exit code of command: {process.ExitCode}");
        }
    }
}
