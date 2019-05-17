using DeveMultiCompressor.Config;
using DeveMultiCompressor.Lib;
using DryIoc;
using System.IO;
using Xunit;

namespace DeveMultiCompressor.Tests
{
    public class TestAllCompressors
    {
        [Fact]
        public void AllCompressorsTest()
        {
            Constants.CompressorFolder = "Compressors";

            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik5.txt");

            using (var container = DryContainer.CreateDryContainer())
            {
                var runner = container.Resolve<CompressorRunner>();

                var commandLineOptions = new CommandLineOptions()
                {
                    InputFile = inputFile,
                    Verify = true
                };

                runner.GoCompress(commandLineOptions);
            }
        }
    }
}
