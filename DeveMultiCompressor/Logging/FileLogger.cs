using System;
using System.IO;

namespace Devedse.DeveImagePyramid.Logging
{
    public class FileLogger : ILogger, IDisposable
    {
        private readonly StreamWriter _logWriter;
        private readonly LogLevel _levelToLog;

        public FileLogger(string logFilePath, LogLevel levelToLog)
        {
            if (string.IsNullOrWhiteSpace(logFilePath)) throw new ArgumentException("logFilePath is null or white space", nameof(logFilePath));

            var logFileDirectory = Path.GetDirectoryName(logFilePath);
            if (!Directory.Exists(logFileDirectory))
            {
                Directory.CreateDirectory(logFileDirectory);
            }

            var fileStream = new FileStream(logFilePath, FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
            _logWriter = new StreamWriter(fileStream)
            {
                AutoFlush = true
            };
            _levelToLog = levelToLog;
        }


        public void Write(string str, LogLevel logLevel = LogLevel.Information, ConsoleColor color = ConsoleColor.Gray)
        {
            if ((int)logLevel >= (int)_levelToLog)
            {
                _logWriter.WriteLine(str);
            }
        }

        public void WriteError(string str, LogLevel logLevel = LogLevel.Error)
        {
            Write(str, logLevel, ConsoleColor.Red);
        }

        public void EmptyLine()
        {
            Write(string.Empty, LogLevel.LogAlways);
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                    _logWriter.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~FileLogger() {
        //   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
        //   Dispose(false);
        // }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above.
            // GC.SuppressFinalize(this);
        }
        #endregion
    }
}
