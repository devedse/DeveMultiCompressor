﻿using DeveCoolLib.Conversion;
using System;
using System.Collections.Generic;

namespace DeveMultiCompressor.Lib.Compression
{
    public class CompressionResult
    {
        public CompressorFileInfo CompressedFile { get; }
        public Compressor UsedCompressor { get; }
        public TimeSpan Duration { get; }
        public long OriginalFileSize { get; }
        public long CompressedFileSize { get; }
        public VerificationStatus VerificationStatus { get; set; }
        public TimeSpan DecompressionDuration { get; set; }

        public CompressionResult(CompressorFileInfo compressedFile, Compressor usedCompressor, TimeSpan duration, long originalFileSize, long compressedFileSize)
        {
            CompressedFile = compressedFile;
            UsedCompressor = usedCompressor;
            Duration = duration;
            OriginalFileSize = originalFileSize;
            CompressedFileSize = CompressedFileSize;
            VerificationStatus = VerificationStatus.NotVerified;
        }

        public List<string> ToStringList()
        {
            return new List<string>()
            {
                UsedCompressor.CompressorConfig.CompressedFileExtension,
                UsedCompressor.CompressorConfig.Description,
                ValuesToStringHelper.MiliSecondsToString((int)Duration.TotalMilliseconds),
                ValuesToStringHelper.BytesToString(OriginalFileSize),
                ValuesToStringHelper.BytesToString(CompressedFileSize),
                VerificationStatus.ToString(),
                ValuesToStringHelper.MiliSecondsToString((int)DecompressionDuration.TotalMilliseconds)
            };
        }
    }
}
