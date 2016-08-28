using Devedse.DeveImagePyramid.Logging;
using DeveMultiCompressor.Config;
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
                var compressor = new Compressor(_logger, _configStringFiller, _processRunner, compressorDir);
                allCompressors.Add(compressor);
            }

            return allCompressors;
        }

        public Compressor GetPreCompressor()
        {
            var precompressorDirectory = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.PrecompressorFolder);
            var precompressor = new Compressor(_logger, _configStringFiller, _processRunner, precompressorDirectory);
            return precompressor;
        }
    }
}
