using Devedse.DeveImagePyramid.Logging;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeveMultiCompressor
{
    public class ProcessRunner
    {
        private ILogger _logger;

        public ProcessRunner(ILogger logger)
        {
            this._logger = logger;
        }

        public void RunProcess(string directoryToRunFrom, string pathToExe, string arguments)
        {
            Directory.SetCurrentDirectory(directoryToRunFrom);

            var psi = new ProcessStartInfo(pathToExe, arguments);
            var process = Process.Start(psi);

            process.WaitForExit();
        } 
    }
}
