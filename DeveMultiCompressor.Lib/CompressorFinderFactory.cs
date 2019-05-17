using DeveMultiCompressor.Lib.Config;
using DeveMultiCompressor.Lib.Logging;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;

namespace DeveMultiCompressor.Lib
{
    public class CompressorFinderFactory
    {
        private readonly ILogger _logger;
        private readonly ConfigStringFiller _configStringFiller;
        private readonly ProcessRunner _processRunner;
        private readonly CompressorDirectoryConfiguration _compressorDirectoryConfiguration;

        public CompressorFinderFactory(ILogger logger, ConfigStringFiller configStringFiller, ProcessRunner processRunner, CompressorDirectoryConfiguration compressorDirectoryConfiguration)
        {
            _logger = logger;
            _configStringFiller = configStringFiller;
            _processRunner = processRunner;
            _compressorDirectoryConfiguration = compressorDirectoryConfiguration;
        }

        public IEnumerable<Compressor> GetCompressors()
        {
            var allCompressors = new List<Compressor>();
            var allCompressorDirectories = Directory.GetDirectories(Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, _compressorDirectoryConfiguration.CompressorDirectory));

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
            var precompressorDirectory = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, _compressorDirectoryConfiguration.PrecompressorDirectory);

            var configPath = Path.Combine(precompressorDirectory, Constants.CompressorConfigFileName);
            var config = JsonConvert.DeserializeObject<CompressorConfig>(File.ReadAllText(configPath));

            var precompressor = new Compressor(_logger, _configStringFiller, _processRunner, precompressorDirectory, config);
            return precompressor;
        }
    }
}
