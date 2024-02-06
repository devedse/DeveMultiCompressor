using DeveMultiCompressor.Lib.Logging;
using DryIoc;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace DeveMultiCompressor.Lib
{
    public class ProcessRunner
    {
        private readonly ILogger _logger;

        public ProcessRunner(ILogger logger)
        {
            _logger = logger;
        }

        public void RunProcess(string directoryToRunFrom, string pathToExe, string arguments)
        {
            Directory.SetCurrentDirectory(directoryToRunFrom);

            _logger.Write($"Running command: {pathToExe} {arguments}");

            ProcessStartInfo psi;

            if (OperatingSystem.IsWindows())
            {
                psi = new ProcessStartInfo(pathToExe, arguments)
                {
                    //WorkingDirectory = Path.GetDirectoryName(_toolExePath)
                };
            }
            else
            {
                psi = new ProcessStartInfo("wine", $"\"{pathToExe}\" {arguments}")
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
