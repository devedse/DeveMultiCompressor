using DeveMultiCompressor.Compression;
using DeveMultiCompressor.Config;
using DeveMultiCompressor.Logging;
using System;
using System.Diagnostics;
using System.IO;

namespace DeveMultiCompressor
{
    public class Compressor
    {
        public string CompressorDir { get; private set; }
        public CompressorConfig CompressorConfig { get; private set; }

        private readonly ILogger _logger;
        private readonly ConfigStringFiller _configStringFiller;
        private readonly ProcessRunner _processRunner;
        private readonly ProcessRunInfo _processRunInfo;

        public Compressor(ILogger logger, ConfigStringFiller configStringFiller, ProcessRunner processRunner, string compressorDir, CompressorConfig compressorConfig)
        {
            _logger = logger;
            _configStringFiller = configStringFiller;
            _processRunner = processRunner;
            CompressorDir = compressorDir;
            CompressorConfig = compressorConfig;
            _processRunInfo = new ProcessRunInfo(CompressorDir, CompressorConfig);
        }

        public CompressionResult CompressFile(CompressorFileInfo input)
        {
            _logger.Write($"Compressing with {_processRunInfo}");

            var outputFilePath = _configStringFiller.FillString(CompressorConfig.CompressedOutputFile, input);
            var outputFileTotalPath = Path.Combine(CompressorDir, outputFilePath);

            if (File.Exists(outputFileTotalPath))
            {
                File.Delete(outputFileTotalPath);
            }

            var arguments = _configStringFiller.FillString(CompressorConfig.CompressorArguments, input);
            var w = Stopwatch.StartNew();
            bool success = false;
            try
            {
                _processRunner.RunProcess(CompressorDir, _processRunInfo, arguments);
                success = File.Exists(outputFileTotalPath);
            }
            catch (Exception ex)
            {
                _logger.WriteError($"Error occured when compressing with {this.CompressorDir}. Error: {ex}");
            }
            w.Stop();

            var compressedFile = new CompressorFileInfo(outputFileTotalPath);
            var result = new CompressionResult(success, compressedFile, this, w.Elapsed, input.FileSize, success ? compressedFile.FileSize : input.FileSize);
            return result;
        }

        public CompressorFileInfo DecompressFile(CompressorFileInfo inputFile, string expectedOutputFilePath)
        {
            _logger.Write($"Extracting with {_processRunInfo}");

            if (File.Exists(expectedOutputFilePath))
            {
                File.Delete(expectedOutputFilePath);
            }

            var arguments = _configStringFiller.FillString(CompressorConfig.DecompressArguments, inputFile);
            _processRunner.RunProcess(CompressorDir, _processRunInfo, arguments);

            return new CompressorFileInfo(expectedOutputFilePath);
        }

        public void DecompressFileToDir(CompressorFileInfo inputFile, string outputDir)
        {
            _logger.Write($"Extracting to dir with {_processRunInfo}");

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
            _processRunner.RunProcess(outputDir, _processRunInfo, arguments);

            inputFileInOutputDir.Delete();
        }

        public override string ToString()
        {
            return $"{CompressorConfig.Description} ({CompressorConfig.CompressedFileExtension})";
        }
    }
}
