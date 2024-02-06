using DeveCoolLib.Conversion;
using System;
using System.Collections.Generic;

namespace DeveMultiCompressor.Compression
{
    public class CompressionResult
    {
        public bool Success { get; }
        public CompressorFileInfo CompressedFile { get; }
        public Compressor UsedCompressor { get; }
        public TimeSpan Duration { get; }
        public long OriginalFileSize { get; }
        public long CompressedFileSize { get; }
        public VerificationStatus VerificationStatus { get; set; }
        public TimeSpan DecompressionDuration { get; set; }

        public CompressionResult(bool success, CompressorFileInfo compressedFile, Compressor usedCompressor, TimeSpan duration, long originalFileSize, long compressedFileSize)
        {
            Success = success;
            CompressedFile = compressedFile;
            UsedCompressor = usedCompressor;
            Duration = duration;
            OriginalFileSize = originalFileSize;
            CompressedFileSize = compressedFileSize;
            VerificationStatus = VerificationStatus.NotVerified;
        }

        public List<string> ToStringList()
        {
            return new List<string>()
            {
                UsedCompressor.CompressorConfig.CompressedFileExtension,
                UsedCompressor.CompressorConfig.Description,
                Success.ToString(),
                ValuesToStringHelper.MiliSecondsToString((int)Duration.TotalMilliseconds),
                ValuesToStringHelper.BytesToString(OriginalFileSize),
                ValuesToStringHelper.BytesToString(CompressedFileSize),
                CompressedFileSize.ToString(),
                VerificationStatus.ToString(),
                ValuesToStringHelper.MiliSecondsToString((int)DecompressionDuration.TotalMilliseconds)
            };
        }
    }
}
