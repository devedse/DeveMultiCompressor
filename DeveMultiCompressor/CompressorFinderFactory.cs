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
    public class CompressorFinderFactory
    {
        private ILogger _logger;
        private ConfigStringFiller _configStringFiller;
        private ProcessRunner _processRunner;

        public CompressorFinderFactory(ILogger logger, ConfigStringFiller configStringFiller, ProcessRunner processRunner)
        {
            this._logger = logger;
            this._configStringFiller = configStringFiller;
            this._processRunner = processRunner;
        }

        public IEnumerable<Compressor> GetCompressors()
        {
            var allCompressors = new List<Compressor>();
            var allCompressorDirectories = Directory.GetDirectories(Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.CompressorFolder));

            foreach (var compressorDir in allCompressorDirectories)
            {
                var configPath = Path.Combine(compressorDir, Constants.CompressorConfigFileName);
                var configs = JsonConvert.DeserializeObject<List<CompressorConfig>>(File.ReadAllText(configPath));

                foreach (var config in configs)
                {
                    var compressor = new Compressor(_logger, _configStringFiller, _processRunner, compressorDir, config);
                    allCompressors.Add(compressor);
                }
            }

            return allCompressors;
        }

        public Compressor GetPreCompressor()
        {
            var precompressorDirectory = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.PrecompressorFolder);

            var configPath = Path.Combine(precompressorDirectory, Constants.CompressorConfigFileName);
            var config = JsonConvert.DeserializeObject<CompressorConfig>(File.ReadAllText(configPath));

            var precompressor = new Compressor(_logger, _configStringFiller, _processRunner, precompressorDirectory, config);
            return precompressor;
        }
    }
}
