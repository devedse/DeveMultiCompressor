using DeveMultiCompressor.Lib.Config;
using DeveMultiCompressor.Lib.Logging;
using System.IO;

namespace DeveMultiCompressor.Lib
{
    public class Compressor
    {
        public string CompressorDir { get; private set; }
        public CompressorConfig CompressorConfig { get; private set; }

        private readonly ILogger _logger;
        private readonly ConfigStringFiller _configStringFiller;
        private readonly ProcessRunner _processRunner;

        public Compressor(ILogger logger, ConfigStringFiller configStringFiller, ProcessRunner processRunner, string compressorDir, CompressorConfig compressorConfig)
        {
            _logger = logger;
            _configStringFiller = configStringFiller;
            _processRunner = processRunner;
            CompressorDir = compressorDir;
            CompressorConfig = compressorConfig;
        }

        public CompressorFileInfo CompressFile(CompressorFileInfo input)
        {
            _logger.Write($"Compressing with {CompressorConfig.CompressorExe}");

            var outputFilePath = _configStringFiller.FillString(CompressorConfig.CompressedOutputFile, input);
            var outputFileTotalPath = Path.Combine(CompressorDir, outputFilePath);

            if (File.Exists(outputFileTotalPath))
            {
                File.Delete(outputFileTotalPath);
            }

            var arguments = _configStringFiller.FillString(CompressorConfig.CompressorArguments, input);
            _processRunner.RunProcess(CompressorDir, CompressorConfig.CompressorExe, arguments);

            return new CompressorFileInfo(outputFileTotalPath);
        }

        public CompressorFileInfo DecompressFile(CompressorFileInfo inputFile, string expectedOutputFilePath)
        {
            _logger.Write($"Extracting with {CompressorConfig.CompressorExe}");

            if (File.Exists(expectedOutputFilePath))
            {
                File.Delete(expectedOutputFilePath);
            }

            var arguments = _configStringFiller.FillString(CompressorConfig.DecompressArguments, inputFile);
            _processRunner.RunProcess(CompressorDir, CompressorConfig.CompressorExe, arguments);

            return new CompressorFileInfo(expectedOutputFilePath);
        }

        public void DecompressFileToDir(CompressorFileInfo inputFile, string outputDir)
        {
            _logger.Write($"Extracting with {CompressorConfig.CompressorExe}");

            if (Directory.Exists(outputDir))
            {
                FolderHelperMethods.ClearDirectory(outputDir);
            }
            else
            {
                Directory.CreateDirectory(outputDir);
            }

            var inputFileInOutputDir = inputFile.CopyToDirectory(outputDir);

            var totalCompressorExe = Path.Combine(CompressorDir, CompressorConfig.CompressorExe);
            var arguments = _configStringFiller.FillStringToFullPath(CompressorConfig.DecompressArguments, inputFileInOutputDir);
            _processRunner.RunProcess(outputDir, totalCompressorExe, arguments);

            inputFileInOutputDir.Delete();
        }
    }
}
