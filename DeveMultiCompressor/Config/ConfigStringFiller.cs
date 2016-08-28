using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeveMultiCompressor.Config
{
    public class ConfigStringFiller
    {
        public string FillString(string input, CompressorFileInfo fileInfo)
        {
            input = input.Replace("{inputfilewithoutextension}", fileInfo.FileNameWithoutExtension);
            input = input.Replace("{inputfilepath}", fileInfo.FileName);
            return input;
        }
    }
}
