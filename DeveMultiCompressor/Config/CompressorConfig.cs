﻿namespace DeveMultiCompressor.Config
{
    public class CompressorConfig
    {
        public string CompressorExe { get; set; }
        public string CompressorLinuxExe { get; set; }
        public string CompressorArguments { get; set; }
        public string CompressedOutputFile { get; set; }
        public string DecompressArguments { get; set; }
        public string CompressedFileExtension { get; set; }
        public string Description { get; set; }
        public string Tags { get; set; }
    }
}
