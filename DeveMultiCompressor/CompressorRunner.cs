using Devedse.DeveImagePyramid.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeveMultiCompressor.Config;

namespace DeveMultiCompressor
{
    class CompressorRunner
    {
        private ILogger _logger;
        private CompressorFinderFactory _compressionFinderFactory;

        public CompressorRunner(ILogger logger, CompressorFinderFactory compressorFinderFactory)
        {
            this._logger = logger;
            this._compressionFinderFactory = compressorFinderFactory;
        }

        public void Go(CommandLineOptions options)
        {
            var allCompressors = _compressionFinderFactory.GetCompressors();

            var inputFile = new CompressorFileInfo(options.InputFile);

            if (options.UsePrecomp)
            {
                var preCompressor = _compressionFinderFactory.GetPreCompressor();
                inputFile.MoveToDirectory(preCompressor.CompressorDir);
                inputFile = preCompressor.CompressFile(inputFile);
            }
            
            foreach (var compressor in allCompressors)
            {
                var newInputFile = inputFile.CopyToDirectory(compressor.CompressorDir);
                compressor.CompressFile(newInputFile);
            }
        }
    }
}
