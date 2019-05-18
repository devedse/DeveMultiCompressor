using DeveMultiCompressor.Lib;
using DeveMultiCompressor.Lib.Config;
using DryIoc;
using System.IO;
using Xunit;

namespace DeveMultiCompressor.Tests
{
    public class FullFlowTests
    {
        [Fact]
        public void CompressSimpleFile()
        {
            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA.7z");

            using (var container = DryContainer.CreateDryContainer("TestCompressors"))
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

                Assert.True(File.Exists(outputFileLzma2));
                Assert.True(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                Assert.NotNull(lzma2.GenerateHash());
                Assert.NotNull(lzma.GenerateHash());
            }
        }

        [Fact]
        public void CompressSimpleFileRelativePath()
        {
            var previousDirectory = Directory.GetCurrentDirectory();
            try
            {
                var curDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles");
                Directory.SetCurrentDirectory(curDir);

                var inputFile = "enwik6.txt";
                var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA2.7z");
                var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Output", "enwik6_LZMA.7z");

                using (var container = DryContainer.CreateDryContainer("TestCompressors"))
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

                    Assert.True(File.Exists(outputFileLzma2));
                    Assert.True(File.Exists(outputFileLzma));

                    var lzma2 = new CompressorFileInfo(outputFileLzma2);
                    var lzma = new CompressorFileInfo(outputFileLzma);

                    Assert.NotNull(lzma2.GenerateHash());
                    Assert.NotNull(lzma.GenerateHash());
                }
            }
            finally
            {
                Directory.SetCurrentDirectory(previousDirectory);
            }
        }

        [Fact]
        public void CompressSimpleFileWithPrecomp()
        {
            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

            using (var container = DryContainer.CreateDryContainer("TestCompressors"))
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

                Assert.True(File.Exists(outputFileLzma2));
                Assert.True(File.Exists(outputFileLzma));

                var lzma2 = new CompressorFileInfo(outputFileLzma2);
                var lzma = new CompressorFileInfo(outputFileLzma);

                Assert.NotNull(lzma2.GenerateHash());
                Assert.NotNull(lzma.GenerateHash());
            }
        }

        [Fact]
        public void CompressAndDecompressWithoutPrecomp()
        {
            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

            var inputFileInfo = new CompressorFileInfo(inputFile);
            var inputFileHash = inputFileInfo.GenerateHash();

            using (var container = DryContainer.CreateDryContainer("TestCompressors"))
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

                Assert.True(File.Exists(outputFileLzma2));
                Assert.True(File.Exists(outputFileLzma));

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

                Assert.True(File.Exists(expectedFileLzma2));
                Assert.True(File.Exists(expectedFileLzma));

                var compFileInfoLzma2 = new CompressorFileInfo(expectedFileLzma2);
                var compFileInfoLzma = new CompressorFileInfo(expectedFileLzma);

                Assert.Equal(inputFileHash, compFileInfoLzma2.GenerateHash());
                Assert.Equal(inputFileHash, compFileInfoLzma.GenerateHash());
            }
        }

        [Fact]
        public void CompressAndDecompressWithoutPrecompWithAllRelativePaths()
        {
            var previousDirectory = Directory.GetCurrentDirectory();
            try
            {
                var curDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles");
                Directory.SetCurrentDirectory(curDir);

                var inputFile = "enwik6.txt";
                var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
                var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

                var inputFileInfo = new CompressorFileInfo(inputFile);
                var inputFileHash = inputFileInfo.GenerateHash();

                using (var container = DryContainer.CreateDryContainer("TestCompressors"))
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

                    Assert.True(File.Exists(outputFileLzma2));
                    Assert.True(File.Exists(outputFileLzma));

                    var lzma2 = new CompressorFileInfo(outputFileLzma2);
                    var lzma = new CompressorFileInfo(outputFileLzma);

                    var outputDirDecompressionLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2");
                    var outputDirDecompressionLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA");

                    FolderHelperMethods.ClearDirectory(outputDirDecompressionLzma2);
                    FolderHelperMethods.ClearDirectory(outputDirDecompressionLzma);

                    Directory.SetCurrentDirectory(Path.GetDirectoryName(lzma2.FullPath));
                    var commandLineOptionsDecompressLzma2 = new CommandLineOptions()
                    {
                        InputFile = Path.GetFileName(lzma2.FullPath),
                        Decompress = true,
                        UsePrecomp = false
                    };
                    runner.GoDecompress(commandLineOptionsDecompressLzma2);

                    Directory.SetCurrentDirectory(Path.GetDirectoryName(lzma.FullPath));
                    var commandLineOptionsDecompressLzma = new CommandLineOptions()
                    {
                        InputFile = Path.GetFileName(lzma.FullPath),
                        Decompress = true,
                        UsePrecomp = false
                    };

                    runner.GoDecompress(commandLineOptionsDecompressLzma);

                    var expectedFileLzma2 = Path.Combine(outputDirDecompressionLzma2, "enwik6.txt");
                    var expectedFileLzma = Path.Combine(outputDirDecompressionLzma, "enwik6.txt");

                    Assert.True(File.Exists(expectedFileLzma2));
                    Assert.True(File.Exists(expectedFileLzma));

                    var compFileInfoLzma2 = new CompressorFileInfo(expectedFileLzma2);
                    var compFileInfoLzma = new CompressorFileInfo(expectedFileLzma);

                    Assert.Equal(inputFileHash, compFileInfoLzma2.GenerateHash());
                    Assert.Equal(inputFileHash, compFileInfoLzma.GenerateHash());
                }
            }
            finally
            {
                Directory.SetCurrentDirectory(previousDirectory);
            }
        }

        [Fact]
        public void CompressAndDecompressWithPrecomp()
        {
            var inputFile = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "TestFiles", "enwik6.txt");
            var outputFileLzma2 = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA2.7z");
            var outputFileLzma = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, "enwik6_LZMA.7z");

            var inputFileInfo = new CompressorFileInfo(inputFile);
            var inputFileHash = inputFileInfo.GenerateHash();

            using (var container = DryContainer.CreateDryContainer("TestCompressors"))
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

                Assert.True(File.Exists(outputFileLzma2));
                Assert.True(File.Exists(outputFileLzma));

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

                Assert.False(File.Exists(expectedFileLzma2));
                Assert.False(File.Exists(expectedFileLzma));

                //Precomp decompresses all found .pcf files into a subfolder with the same name.
                var expectedFilePrecompLzma2 = Path.Combine(outputDirDecompressionLzma2, "enwik6", "enwik6.txt");
                var expectedFilePrecompLzma = Path.Combine(outputDirDecompressionLzma, "enwik6", "enwik6.txt");

                var compFileInfoLzma2 = new CompressorFileInfo(expectedFilePrecompLzma2);
                var compFileInfoLzma = new CompressorFileInfo(expectedFilePrecompLzma);

                Assert.Equal(inputFileHash, compFileInfoLzma2.GenerateHash());
                Assert.Equal(inputFileHash, compFileInfoLzma.GenerateHash());
            }
        }

        [Fact]
        public void AllCompressorsTest()
        {
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
