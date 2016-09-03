using CommandLine;

namespace DeveMultiCompressor.Config
{
    public class CommandLineOptions
    {
        [Option(HelpText = "If this is set, the program will try to decompress the file instead of compressing it.")]
        public bool Decompress { get; set; }

        [Option(HelpText = "The path to the input file to compress.", Required = true)]
        public string InputFile { get; set; }

        [Option(HelpText = "Precompresses all files before compressing them. (When compressing, first precomp the files. When decompressing, extract all .pcf files).")]
        public bool UsePrecomp { get; set; }

        [Option(HelpText = "Unpacks the archive afterwards and compares the recreated file hash with the input file. (Only for compressing).")]
        public bool Verify { get; set; }
    }
}