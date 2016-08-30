using System;
using System.IO;
using System.Security.Cryptography;

namespace DeveMultiCompressor
{
    public class CompressorFileInfo
    {
        public string FullPath { get; private set; }

        public string DirectoryPath
        {
            get
            {
                return Path.GetDirectoryName(FullPath);
            }
        }

        public string FileNameWithoutExtension
        {
            get
            {
                return Path.GetFileNameWithoutExtension(FullPath);
            }
        }

        public string FileName
        {
            get
            {
                return Path.GetFileName(FullPath);
            }
        }

        public CompressorFileInfo(string path)
        {
            if (string.IsNullOrWhiteSpace(path)) throw new ArgumentException($"Path is null or empty, Path: {path}", nameof(path));
            if (!File.Exists(path)) throw new ArgumentException($"File with path '{path}' does not exist.");

            this.FullPath = path;
        }

        public void MoveToDirectory(string destinationDirectory)
        {
            Directory.CreateDirectory(destinationDirectory);
            var totalDestinationPath = Path.Combine(destinationDirectory, FileName);
            if (File.Exists(totalDestinationPath))
            {
                File.Delete(totalDestinationPath);
            }
            File.Move(FullPath, totalDestinationPath);
            FullPath = totalDestinationPath;
        }

        public CompressorFileInfo CopyToDirectory(string destinationDirectory)
        {
            Directory.CreateDirectory(destinationDirectory);
            var totalDestinationPath = Path.Combine(destinationDirectory, FileName);
            if (File.Exists(totalDestinationPath))
            {
                File.Delete(totalDestinationPath);
            }
            File.Copy(FullPath, totalDestinationPath);
            return new CompressorFileInfo(totalDestinationPath);
        }

        public string GenerateHash()
        {
            //Using SHA256 would be best because it's 4x as fast as 512. Also using Managed is like 10% slower then the SHA256CryptoServiceProvider, but this one works on all systems. (Not only XP with SP3+)
            //However we use SHA1, because almost everyone uses it and its easier to do manual verification too.
            using (var sha = SHA1Managed.Create())
            {
                using (var filestream = new FileStream(FullPath, FileMode.Open, FileAccess.Read, FileShare.Read))
                {
                    byte[] hashValue = sha.ComputeHash(filestream);
                    var hashResult = BitConverter.ToString(hashValue).Replace("-", string.Empty);
                    return hashResult;
                }
            }
        }

        public string GetFileSize()
        {
            var fileInfo = new FileInfo(FullPath);
            var sizeInMb = fileInfo.Length / 1000.0 / 1000.0;
            return $"{Math.Round(sizeInMb, 3)} MB";
        }

        public void Delete()
        {
            File.Delete(FullPath);
        }
    }
}
