using CommandLine;

namespace DeveMultiCompressor.Config
{
    public class CommandLineOptions
    {
        [Option(HelpText = "The path to the input file to compress.", Required = true)]
        public string InputFile { get; set; }

        [Option(HelpText = "Precompresses all files before compressing them.")]
        public bool UsePrecomp { get; set; }

        [Option(HelpText = "Unpacks the archive afterwards and compares the recreated file hash with the input file.")]
        public bool Verify { get; set; }
    }
}
