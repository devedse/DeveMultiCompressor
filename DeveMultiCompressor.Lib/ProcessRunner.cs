using DeveMultiCompressor.Lib.Logging;
using System.Diagnostics;
using System.IO;

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

            var psi = new ProcessStartInfo(pathToExe, arguments);
            var process = Process.Start(psi);

            process.WaitForExit();

            _logger.Write($"Exit code of command: {process.ExitCode}");
        }
    }
}
