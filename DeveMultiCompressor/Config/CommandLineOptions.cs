using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeveMultiCompressor.Config
{
    public class CommandLineOptions
    {
        [Option(HelpText = "The path to the input file to compress.", Required = true)]
        public string InputFile { get; set; }

        [Option(HelpText = "Precompresses all files before compressing them.")]
        public bool UsePrecomp { get; set; }
    }
}
