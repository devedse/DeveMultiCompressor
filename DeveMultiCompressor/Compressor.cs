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
        private CompressorConfig _config;

        public Compressor(ILogger logger, ConfigStringFiller configStringFiller, ProcessRunner processRunner, string compressorDir)
        {
            this._logger = logger;
            this._configStringFiller = configStringFiller;
            this._processRunner = processRunner;
            this.CompressorDir = compressorDir;

            var configPath = Path.Combine(compressorDir, Constants.CompressorConfigFileName);
            _config = JsonConvert.DeserializeObject<CompressorConfig>(File.ReadAllText(configPath));
        }

        public CompressorFileInfo CompressFile(CompressorFileInfo input)
        {
            var arguments = _configStringFiller.FillString(_config.CompressorArguments, input);
            _processRunner.RunProcess(CompressorDir, _config.CompressorExe, arguments);

            var outputFilePath = _configStringFiller.FillString(_config.CompressedOutputFile, input);
            var outputFileTotalPath = Path.Combine(CompressorDir, outputFilePath);
            return new CompressorFileInfo(outputFileTotalPath);
        }
    }
}
