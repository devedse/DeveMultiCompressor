using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using DeveMultiCompressor;
using DryIoc;
using DeveMultiCompressor.Config;
using System.IO;

namespace DeveMultiCompressorTests
{
    [TestClass]
    public class FullFlowTests
    {
        [TestMethod]
        public void CompressSimpleFile()
        {
            Constants.CompressorFolder = "TestCompressors";

            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA.7z");

            using (var container = DryContainer.CreateDryContainer())
            {
                var runner = container.Resolve<CompressorRunner>();

                if (File.Exists(outputFileLzma2))
                {
                    File.Delete(outputFileLzma2);
                }
                if (File.Exists(outputFileLzma))
                {
                    File.Delete(outputFileLzma);
                }

                var commandLineOptions = new CommandLineOptions()
                {
                    InputFile = inputFile,
                    Verify = true
                };

                runner.Go(commandLineOptions);

                Assert.IsTrue(File.Exists(outputFileLzma2));
                Assert.IsTrue(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                Assert.IsNotNull(lzma2.GenerateHash());
                Assert.IsNotNull(lzma.GenerateHash());
            }
        }

        [TestMethod]
        public void CompressSimpleFileWithPrecomp()
        {
            Constants.CompressorFolder = "TestCompressors";

            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA.7z");

            using (var container = DryContainer.CreateDryContainer())
            {
                var runner = container.Resolve<CompressorRunner>();

                if (File.Exists(outputFileLzma2))
                {
                    File.Delete(outputFileLzma2);
                }
                if (File.Exists(outputFileLzma))
                {
                    File.Delete(outputFileLzma);
                }

                var commandLineOptions = new CommandLineOptions()
                {
                    InputFile = inputFile,
                    UsePrecomp = true,
                    Verify = true
                };

                runner.Go(commandLineOptions);

                Assert.IsTrue(File.Exists(outputFileLzma2));
                Assert.IsTrue(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                Assert.IsNotNull(lzma2.GenerateHash());
                Assert.IsNotNull(lzma.GenerateHash());
            }
        }
    }
}
