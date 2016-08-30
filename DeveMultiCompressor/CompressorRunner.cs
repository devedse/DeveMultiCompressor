using Devedse.DeveImagePyramid.Logging;
using System;
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
                _logger.Write("Using precomp...");
                var preCompressor = _compressionFinderFactory.GetPreCompressor();
                var copiedFile = inputFile.CopyToDirectory(preCompressor.CompressorDir);
                inputFile = preCompressor.CompressFile(inputFile);
                copiedFile.Delete();
            }

            _logger.Write("Starting actual compression...");
            _logger.Write($"Handling input file: '{options.InputFile}'. Generating hash...");
            var hash = inputFile.GenerateHash();
            _logger.Write($"Generated hash of input file: {hash}");

            foreach (var compressor in allCompressors)
            {
                var newInputFile = inputFile.CopyToDirectory(compressor.CompressorDir);
                var outputFile = compressor.CompressFile(newInputFile);
                newInputFile.Delete();

                if (options.Verify)
                {
                    var expectedPath = newInputFile.FullPath;
                    var decompressedFile = compressor.DecompressFile(outputFile, expectedPath);
                    var decompressedFileHash = decompressedFile.GenerateHash();
                    if (hash != decompressedFileHash)
                    {
                        throw new Exception($"Hash of decompressed file: '{decompressedFile.FullPath}': '{decompressedFileHash}' does not match has of input file: '{inputFile.FullPath}': '{hash}'.");
                    }
                    else
                    {
                        _logger.Write($"File verified. Hash is equal to input file: {decompressedFileHash}", color: ConsoleColor.Green);
                    }
                }

                outputFile.MoveToDirectory(outputDir);
                _logger.Write($"File compressed to '{outputFile.FileName}'. Size: {outputFile.GetFileSize()}", color: ConsoleColor.Green);
            }

            if (options.UsePrecomp)
            {
                //If we use precomp, delete the .pcf file
                inputFile.Delete();
            }

            _logger.Write("Completed compression :)");
        }
    }
}
