using DeveCoolLib.Conversion;
using DeveCoolLib.TextFormatting;
using DeveMultiCompressor.Compression;
using DeveMultiCompressor.Config;
using DeveMultiCompressor.Logging;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace DeveMultiCompressor
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
            CompressionResult precompResult = null;
            var allCompressionResults = new List<CompressionResult>();

            var inputFileFullPath = Path.GetFullPath(options.InputFile);

            var outputDir = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, Constants.OutputDir);

            var allCompressors = _compressionFinderFactory.GetCompressors();

            if (options.IncludedCompressors != null && options.IncludedCompressors.Any())
            {
                allCompressors = allCompressors.Where(t => options.IncludedCompressors.Any(z => z.Equals(t.CompressorConfig.CompressedFileExtension, StringComparison.OrdinalIgnoreCase))).ToList();
            }
            if (options.ExcludedCompressors != null && options.ExcludedCompressors.Any())
            {
                allCompressors = allCompressors.Where(t => options.ExcludedCompressors.All(z => !z.Equals(t.CompressorConfig.CompressedFileExtension))).ToList();
            }

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
                precompResult = preCompressor.CompressFile(inputFile);
                inputFile = precompResult.CompressedFile;
                copiedFile.Delete();

                if (!precompResult.Success)
                {
                    throw new Exception($"Precomp failed, output file was not found: {precompResult.CompressedFile.FullPath}");
                }

                if (options.Verify)
                {
                    var expectedPath = copiedFile.FullPath;

                    //TODO: Move this to DecompressFile Method just like it's done in the CompressFile method
                    var w = Stopwatch.StartNew();
                    var decompressedFile = preCompressor.DecompressFile(inputFile, expectedPath);
                    w.Stop();

                    if (!File.Exists(decompressedFile.FullPath))
                    {
                        throw new Exception($"Precomp verification failed, output file was not found: {decompressedFile.FullPath}");
                    }
                    var decompressedFileHash = decompressedFile.GenerateHash();
                    if (hash != decompressedFileHash)
                    {
                        precompResult.VerificationStatus = VerificationStatus.Failed;
                        precompResult.DecompressionDuration = w.Elapsed;
                        throw new Exception($"Hash of decompressed file (with precomp): '{decompressedFile.FullPath}': '{decompressedFileHash}' does not match has of input file: '{inputFile.FullPath}': '{hash}'.");
                    }
                    else
                    {
                        precompResult.VerificationStatus = VerificationStatus.Success;
                        precompResult.DecompressionDuration = w.Elapsed;
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
                    var compressionResult = compressor.CompressFile(newInputFile);
                    allCompressionResults.Add(compressionResult);
                    var outputFile = compressionResult.CompressedFile;

                    newInputFile.Delete();

                    if (compressionResult.Success)
                    {
                        if (options.Verify)
                        {
                            var expectedPath = newInputFile.FullPath;

                            //TODO: Move this to DecompressFile Method just like it's done in the CompressFile method
                            var w = Stopwatch.StartNew();
                            var decompressedFile = compressor.DecompressFile(outputFile, expectedPath);
                            w.Stop();

                            var decompressedFileHash = decompressedFile.GenerateHash();
                            if (hash != decompressedFileHash)
                            {
                                compressionResult.VerificationStatus = VerificationStatus.Failed;
                                compressionResult.DecompressionDuration = w.Elapsed;
                                _logger.WriteError($"Hash of decompressed file: '{decompressedFile.FullPath}': '{decompressedFileHash}' does not match has of input file: '{inputFile.FullPath}': '{hash}'.");
                            }
                            else
                            {
                                compressionResult.VerificationStatus = VerificationStatus.Success;
                                compressionResult.DecompressionDuration = w.Elapsed;
                                _logger.Write($"File verified. Hash is equal to input file: {decompressedFileHash}", color: ConsoleColor.Green);
                            }
                        }
                        outputFile.MoveToDirectory(outputDir);
                        _logger.Write($"File compressed to '{outputFile.FileName}'. Size: {ValuesToStringHelper.BytesToString(outputFile.FileSize)}. Hash: {outputFile.GenerateHash()}", color: ConsoleColor.Green);
                    }
                    else
                    {
                        _logger.WriteError($"Compression failed for {compressor.CompressorConfig.Description}. Output file {outputFile.FullPath} not found");
                    }

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


            var logString = ToLogString(Path.GetFileName(inputFileFullPath), precompResult, allCompressionResults);
            _logger.Write(logString);

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

        public string ToLogString(string inputFileName, CompressionResult preCompCompressionResult, List<CompressionResult> otherCompressionResults)
        {
            var sb = new StringBuilder();
            sb.AppendLine($"Compression results for file {inputFileName}");

            var theLogList = new List<List<string>>();
            theLogList.Add(new List<string>() { "Extension", "Description", "Success", "Duration", "Original File Size", "Compressed File Size", "Compressed File Size (Bytes)", "Verification Status", "Decompression time" });
            theLogList.Add(null);
            if (preCompCompressionResult != null)
            {
                theLogList.Add(preCompCompressionResult.ToStringList());
                theLogList.Add(null);
            }
            theLogList.AddRange(otherCompressionResults.OrderBy(t => t.CompressedFileSize).Select(t => t.ToStringList()));

            var outputString = TableToTextPrinter.TableToText(theLogList);
            sb.AppendLine(outputString);
            return sb.ToString();
        }
    }
}
