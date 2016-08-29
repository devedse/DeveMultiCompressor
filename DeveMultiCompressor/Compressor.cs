using Devedse.DeveImagePyramid.Logging;
using DeveMultiCompressor.Config;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

            var arguments = _configStringFiller.FillString(_compressorConfig.CompressorArguments, input);
            var outputFilePath = _configStringFiller.FillString(_compressorConfig.CompressedOutputFile, input);
            var outputFileTotalPath = Path.Combine(CompressorDir, outputFilePath);

            if (File.Exists(outputFileTotalPath))
            {
                File.Delete(outputFileTotalPath);
            }
            _processRunner.RunProcess(CompressorDir, _compressorConfig.CompressorExe, arguments);

            return new CompressorFileInfo(outputFileTotalPath);
        }
    }
}
