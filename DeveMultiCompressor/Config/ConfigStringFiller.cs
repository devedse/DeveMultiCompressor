using DeveMultiCompressor.Lib.Compression;

namespace DeveMultiCompressor.Lib.Config
{
    public class ConfigStringFiller
    {
        public string FillString(string input, CompressorFileInfo fileInfo)
        {
            input = input.Replace("{inputfilewithextension}", fileInfo.FileName);
            input = input.Replace("{inputfilewithoutextension}", fileInfo.FileNameWithoutExtension);
            input = input.Replace("{inputfilewithonelessextension}", fileInfo.FileNameWithOneLessExtension);
            input = input.Replace("{inputfilepath}", fileInfo.FileName);
            return input;
        }

        public string FillStringToFullPath(string input, CompressorFileInfo fileInfo)
        {
            input = input.Replace("{inputfilewithextension}", fileInfo.FileName);
            input = input.Replace("{inputfilewithoutextension}", fileInfo.FileNameWithoutExtension);
            input = input.Replace("{inputfilewithonelessextension}", fileInfo.FileNameWithOneLessExtension);
            input = input.Replace("{inputfilepath}", fileInfo.FullPath);
            return input;
        }
    }
}
