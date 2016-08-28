using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeveMultiCompressor.Config
{
    public class CompressorConfig
    {
        public string CompressorExe { get; set; }
        public string CompressorArguments { get; set; }
        public string CompressedOutputFile { get; set; }
    }
}
