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

                runner.GoCompress(commandLineOptions);

                Assert.IsTrue(File.Exists(outputFileLzma2));
                Assert.IsTrue(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                Assert.IsNotNull(lzma2.GenerateHash());
                Assert.IsNotNull(lzma.GenerateHash());
            }
        }

        [TestMethod]
        public void CompressSimpleFileRelativePath()
        {
            Constants.CompressorFolder = "TestCompressors";

            var previousDirectory = Directory.GetCurrentDirectory();
            try
            {
                var curDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles");
                Directory.SetCurrentDirectory(curDir);

                var inputFile = "enwik6.txt";
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

                    runner.GoCompress(commandLineOptions);

                    Assert.IsTrue(File.Exists(outputFileLzma2));
                    Assert.IsTrue(File.Exists(outputFileLzma));

                    var lzma2 = new CompressorFileInfo(outputFileLzma2);
                    var lzma = new CompressorFileInfo(outputFileLzma);

                    Assert.IsNotNull(lzma2.GenerateHash());
                    Assert.IsNotNull(lzma.GenerateHash());
                }
            }
            finally
            {
                Directory.SetCurrentDirectory(previousDirectory);
            }
        }

        [TestMethod]
        public void CompressSimpleFileWithPrecomp()
        {
            Constants.CompressorFolder = "TestCompressors";

            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

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

                runner.GoCompress(commandLineOptions);

                Assert.IsTrue(File.Exists(outputFileLzma2));
                Assert.IsTrue(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                Assert.IsNotNull(lzma2.GenerateHash());
                Assert.IsNotNull(lzma.GenerateHash());
            }
        }

        [TestMethod]
        public void CompressAndDecompressWithoutPrecomp()
        {
            Constants.CompressorFolder = "TestCompressors";

            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

            var inputFileInfo = new CompressorFileInfo(inputFile);
            var inputFileHash = inputFileInfo.GenerateHash();

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
                    UsePrecomp = false,
                    Verify = true
                };

                runner.GoCompress(commandLineOptions);

                Assert.IsTrue(File.Exists(outputFileLzma2));
                Assert.IsTrue(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                var outputDirDecompressionLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2");
                var outputDirDecompressionLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA");

                FolderHelperMethods.ClearDirectory(outputDirDecompressionLzma2);
                FolderHelperMethods.ClearDirectory(outputDirDecompressionLzma);


                var commandLineOptionsDecompressLzma2 = new CommandLineOptions()
                {
                    InputFile = lzma2.FullPath,
                    Decompress = true,
                    UsePrecomp = false
                };

                var commandLineOptionsDecompressLzma = new CommandLineOptions()
                {
                    InputFile = lzma.FullPath,
                    Decompress = true,
                    UsePrecomp = false
                };

                runner.GoDecompress(commandLineOptionsDecompressLzma2);
                runner.GoDecompress(commandLineOptionsDecompressLzma);

                var expectedFileLzma2 = Path.Combine(outputDirDecompressionLzma2, "enwik6.txt");
                var expectedFileLzma = Path.Combine(outputDirDecompressionLzma, "enwik6.txt");

                Assert.IsTrue(File.Exists(expectedFileLzma2));
                Assert.IsTrue(File.Exists(expectedFileLzma));

                var compFileInfoLzma2 = new CompressorFileInfo(expectedFileLzma2);
                var compFileInfoLzma = new CompressorFileInfo(expectedFileLzma);

                Assert.AreEqual(inputFileHash, compFileInfoLzma2.GenerateHash());
                Assert.AreEqual(inputFileHash, compFileInfoLzma.GenerateHash());
            }
        }

        [TestMethod]
        public void CompressAndDecompressWithPrecomp()
        {
            Constants.CompressorFolder = "TestCompressors";

            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

            var inputFileInfo = new CompressorFileInfo(inputFile);
            var inputFileHash = inputFileInfo.GenerateHash();

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

                runner.GoCompress(commandLineOptions);

                Assert.IsTrue(File.Exists(outputFileLzma2));
                Assert.IsTrue(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                var outputDirDecompressionLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2");
                var outputDirDecompressionLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA");

                FolderHelperMethods.ClearDirectory(outputDirDecompressionLzma2);
                FolderHelperMethods.ClearDirectory(outputDirDecompressionLzma);


                var commandLineOptionsDecompressLzma2 = new CommandLineOptions()
                {
                    InputFile = lzma2.FullPath,
                    Decompress = true,
                    UsePrecomp = true
                };

                var commandLineOptionsDecompressLzma = new CommandLineOptions()
                {
                    InputFile = lzma.FullPath,
                    Decompress = true,
                    UsePrecomp = true
                };

                runner.GoDecompress(commandLineOptionsDecompressLzma2);
                runner.GoDecompress(commandLineOptionsDecompressLzma);

                var expectedFileLzma2 = Path.Combine(outputDirDecompressionLzma2, "enwik6.txt");
                var expectedFileLzma = Path.Combine(outputDirDecompressionLzma, "enwik6.txt");

                Assert.IsFalse(File.Exists(expectedFileLzma2));
                Assert.IsFalse(File.Exists(expectedFileLzma));

                //Precomp decompresses all found .pcf files into a subfolder with the same name.
                var expectedFilePrecompLzma2 = Path.Combine(outputDirDecompressionLzma2, "enwik6", "enwik6.txt");
                var expectedFilePrecompLzma = Path.Combine(outputDirDecompressionLzma, "enwik6", "enwik6.txt");

                var compFileInfoLzma2 = new CompressorFileInfo(expectedFilePrecompLzma2);
                var compFileInfoLzma = new CompressorFileInfo(expectedFilePrecompLzma);

                Assert.AreEqual(inputFileHash, compFileInfoLzma2.GenerateHash());
                Assert.AreEqual(inputFileHash, compFileInfoLzma.GenerateHash());
            }
        }
    }
}
