using DeveMultiCompressor.Config;
using DeveMultiCompressor.Lib.Logging;
using System;
using System.IO;
using System.Linq;

namespace DeveMultiCompressor.Lib
{
    public class CompressorRunner
    {
        private readonly ILogger _logger;
        private readonly CompressorFinderFactory _compressionFinderFactory;

        public CompressorRunner(ILogger logger, CompressorFinderFactory compressorFinderFactory)
        {
            _logger = logger;
            _compressionFinderFactory = compressorFinderFactory;
        }

        public void GoCompress(CommandLineOptions options)
        {
            var inputFileFullPath = Path.GetFullPath(options.InputFile);

            var outputDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir);

            var allCompressors = _compressionFinderFactory.GetCompressors();

            if (options.IncludedCompressors.Any())
            {
                allCompressors = allCompressors.Where(t => options.IncludedCompressors.Any(z => z.Equals(t.CompressorConfig.CompressedFileExtension, StringComparison.OrdinalIgnoreCase))).ToList();
            }
            allCompressors = allCompressors.Where(t => options.ExcludedCompressors.All(z => !z.Equals(t.CompressorConfig.CompressedFileExtension))).ToList();

            var inputFile = new CompressorFileInfo(inputFileFullPath);

            _logger.Write($"Handling input file: '{inputFileFullPath}'. Generating hash...");
            var hash = inputFile.GenerateHash();
            _logger.Write($"Generated hash of input file: {hash}");

            if (options.UsePrecomp)
            {
                //If we use precomp, the new inputFile will be the output of precomp

                _logger.Write("Using precomp...");
                var preCompressor = _compressionFinderFactory.GetPreCompressor();
                var copiedFile = inputFile.CopyToDirectory(preCompressor.CompressorDir);
                inputFile = preCompressor.CompressFile(inputFile);
                copiedFile.Delete();

                if (options.Verify)
                {
                    var expectedPath = copiedFile.FullPath;
                    var decompressedFile = preCompressor.DecompressFile(inputFile, expectedPath);
                    var decompressedFileHash = decompressedFile.GenerateHash();
                    if (hash != decompressedFileHash)
                    {
                        throw new Exception($"Hash of decompressed file (with precomp): '{decompressedFile.FullPath}': '{decompressedFileHash}' does not match has of input file: '{inputFile.FullPath}': '{hash}'.");
                    }
                    else
                    {
                        _logger.Write($"File verified (with precomp). Hash is equal to input file: {decompressedFileHash}", color: ConsoleColor.Green);
                    }
                }

                _logger.Write($"Generating hash of precomp file: '{inputFile.FullPath}'...");
                hash = inputFile.GenerateHash();
                _logger.Write($"Generated hash of precomp input file: {hash}");
            }

            _logger.Write("Starting actual compression...");


            foreach (var compressor in allCompressors)
            {
                var newInputFile = inputFile.CopyToDirectory(compressor.CompressorDir);

                try
                {
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
                    _logger.Write($"File compressed to '{outputFile.FileName}'. Size: {outputFile.GetFileSize()}. Hash: {outputFile.GenerateHash()}", color: ConsoleColor.Green);
                }
                catch (Exception ex)
                {
                    _logger.WriteError($"Error occured when compressing with {compressor.CompressorDir}. Error: {ex.ToString()}");
                }
            }

            if (options.UsePrecomp)
            {
                //If we use precomp, delete the .pcf file
                inputFile.Delete();
            }

            _logger.Write("Completed compression :)");
        }

        public void GoDecompress(CommandLineOptions options)
        {
            var inputFileFullPath = Path.GetFullPath(options.InputFile);

            var lastPathOfOutputDir = Path.GetFileNameWithoutExtension(inputFileFullPath);
            var outputDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir, lastPathOfOutputDir);
            var allCompressors = _compressionFinderFactory.GetCompressors();

            var inputFile = new CompressorFileInfo(inputFileFullPath);

            var inputFileExtension = Path.GetExtension(inputFile.FullPath);
            var validDecompressors = allCompressors.Where(t => string.Equals(inputFileExtension, "." + t.CompressorConfig.CompressedFileExtension, StringComparison.OrdinalIgnoreCase) || string.Equals(inputFileExtension, t.CompressorConfig.CompressedFileExtension, StringComparison.OrdinalIgnoreCase)).ToList();

            if (validDecompressors.Count == 0)
            {
                _logger.WriteError("No valid decompressor found, exiting...");
                return;
            }
            if (validDecompressors.Count > 1)
            {
                _logger.Write($"Found {validDecompressors.Count} decompressors. Taking first.", LogLevel.Warning, ConsoleColor.Yellow);
            }

            var firstDecompressor = validDecompressors.First();

            firstDecompressor.DecompressFileToDir(inputFile, outputDir);

            if (options.UsePrecomp)
            {
                var precomp = _compressionFinderFactory.GetPreCompressor();

                foreach (var file in Directory.GetFiles(outputDir))
                {
                    var extension = Path.GetExtension(file);
                    if (string.Equals(extension, "." + precomp.CompressorConfig.CompressedFileExtension, StringComparison.OrdinalIgnoreCase) || string.Equals(extension, precomp.CompressorConfig.CompressedFileExtension, StringComparison.OrdinalIgnoreCase))
                    {
                        var precompFile = new CompressorFileInfo(file);
                        var lastPartOfPrecompFile = Path.GetFileNameWithoutExtension(precompFile.FullPath);
                        var outputDirPrecomp = Path.Combine(outputDir, lastPartOfPrecompFile);
                        if (Directory.Exists(outputDirPrecomp))
                        {
                            _logger.Write($"Output directory '{outputDirPrecomp}' already exist. Possible file collisions...", LogLevel.Warning, ConsoleColor.Yellow);
                        }
                        precomp.DecompressFileToDir(precompFile, outputDirPrecomp);
                        //We could remove the .pcf file here, but I leave it just for clarity
                    }
                }
            }
        }
    }
}
