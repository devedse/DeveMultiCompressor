using Devedse.DeveImagePyramid.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeveMultiCompressor.Config;
using System.IO;

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
            var outputDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir);
            var allCompressors = _compressionFinderFactory.GetCompressors();

            var inputFile = new CompressorFileInfo(options.InputFile);

            if (options.UsePrecomp)
            {
                var preCompressor = _compressionFinderFactory.GetPreCompressor();
                var copiedFile = inputFile.CopyToDirectory(preCompressor.CompressorDir);
                inputFile = preCompressor.CompressFile(inputFile);
                copiedFile.Delete();
            }

            foreach (var compressor in allCompressors)
            {
                var newInputFile = inputFile.CopyToDirectory(compressor.CompressorDir);
                var outputFile = compressor.CompressFile(newInputFile);
                outputFile.MoveToDirectory(outputDir);
                newInputFile.Delete();
            }

            if (options.UsePrecomp)
            {
                //If we use precomp, delete the .pcf file
                inputFile.Delete();
            }
        }
    }
}
