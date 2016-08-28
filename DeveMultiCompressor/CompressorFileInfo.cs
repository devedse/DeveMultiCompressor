using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeveMultiCompressor
{
    public class CompressorFileInfo
    {
        private string _path;

        public string DirectoryPath
        {
            get
            {
                return Path.GetDirectoryName(_path);
            }
        }

        public string FileNameWithoutExtension
        {
            get
            {
                return Path.GetFileNameWithoutExtension(_path);
            }
        }

        public string FileName
        {
            get
            {
                return Path.GetFileName(_path);
            }
        }

        public CompressorFileInfo(string path)
        {
            if (string.IsNullOrWhiteSpace(path)) throw new ArgumentException($"Path is null or empty, Path: {path}", nameof(path));
            if (!File.Exists(path)) throw new ArgumentException($"File with path '{path}' does not exist.");

            this._path = path;
        }

        public void MoveToDirectory(string destinationDirectory)
        {
            Directory.CreateDirectory(destinationDirectory);
            var totalDestinationPath = Path.Combine(destinationDirectory, FileName);
            if (File.Exists(totalDestinationPath))
            {
                File.Delete(totalDestinationPath);
            }
            File.Move(_path, totalDestinationPath);
            _path = totalDestinationPath;
        }

        public CompressorFileInfo CopyToDirectory(string destinationDirectory)
        {
            Directory.CreateDirectory(destinationDirectory);
            var totalDestinationPath = Path.Combine(destinationDirectory, FileName);
            if (File.Exists(totalDestinationPath))
            {
                File.Delete(totalDestinationPath);
            }
            File.Copy(_path, totalDestinationPath);
            return new CompressorFileInfo(totalDestinationPath);
        }

        public void Delete()
        {
            File.Delete(_path);
        }
    }
}
