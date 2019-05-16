using DeveMultiCompressor.Lib;

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

        public string FillStringToFullPath(string input, CompressorFileInfo fileInfo)
        {
            input = input.Replace("{inputfilewithoutextension}", fileInfo.FileNameWithoutExtension);
            input = input.Replace("{inputfilepath}", fileInfo.FullPath);
            return input;
        }
    }
}
