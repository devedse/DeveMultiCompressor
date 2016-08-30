using Devedse.DeveImagePyramid.Logging;
using DeveMultiCompressor.Config;
using System.IO;

namespace DeveMultiCompressor
{
    public class Compressor
    {
        public string CompressorDir { get; private set; }
        private ILogger _logger;
        private ConfigStringFiller _configStringFiller;
        private ProcessRunner _processRunner;
        private CompressorConfig _compressorConfig;

        public Compressor(ILogger logger, ConfigStringFiller configStringFiller, ProcessRunner processRunner, string compressorDir, CompressorConfig compressorConfig)
        {
            this._logger = logger;
            this._configStringFiller = configStringFiller;
            this._processRunner = processRunner;
            this.CompressorDir = compressorDir;
            this._compressorConfig = compressorConfig;
        }

        public CompressorFileInfo CompressFile(CompressorFileInfo input)
        {
            _logger.Write($"Compressing with {_compressorConfig.CompressorExe}");

            var outputFilePath = _configStringFiller.FillString(_compressorConfig.CompressedOutputFile, input);
            var outputFileTotalPath = Path.Combine(CompressorDir, outputFilePath);

            if (File.Exists(outputFileTotalPath))
            {
                File.Delete(outputFileTotalPath);
            }

            var arguments = _configStringFiller.FillString(_compressorConfig.CompressorArguments, input);
            _processRunner.RunProcess(CompressorDir, _compressorConfig.CompressorExe, arguments);

            return new CompressorFileInfo(outputFileTotalPath);
        }

        public CompressorFileInfo DecompressFile(CompressorFileInfo input, string expectedOutputFilePath)
        {
            _logger.Write($"Extracting with {_compressorConfig.CompressorExe}");

            if (File.Exists(expectedOutputFilePath))
            {
                File.Delete(expectedOutputFilePath);
            }

            var arguments = _configStringFiller.FillString(_compressorConfig.DecompressArguments, input);
            _processRunner.RunProcess(CompressorDir, _compressorConfig.CompressorExe, arguments);

            return new CompressorFileInfo(expectedOutputFilePath);
        }
    }
}
