using System;

namespace Devedse.DeveImagePyramid.Logging.Appenders
{
    public class LoggingLevelLoggerAppender : ILogger, IDisposable
    {
        private readonly ILogger _innerLogger;
        private readonly string _seperator;

        private int _longestLengthOfLogLevel;

        public LoggingLevelLoggerAppender(ILogger innerLogger, string seperator)
        {
            _innerLogger = innerLogger;
            _seperator = seperator;

            foreach (var val in Enum.GetValues(typeof(LogLevel)))
            {
                var valLength = val.ToString().Length;
                if (valLength > _longestLengthOfLogLevel)
                {
                    _longestLengthOfLogLevel = valLength;
                }
            }
        }

        public void EmptyLine()
        {
            _innerLogger.EmptyLine();
        }

        public void Write(string str, LogLevel logLevel = LogLevel.Information, ConsoleColor color = ConsoleColor.Gray)
        {
            var strToWrite = AppendLogLevel(str, logLevel);
            _innerLogger.Write(strToWrite, logLevel, color);
        }

        public void WriteError(string str, LogLevel logLevel = LogLevel.Error)
        {
            var strToWrite = AppendLogLevel(str, logLevel);
            _innerLogger.WriteError(strToWrite, logLevel);
        }

        private string AppendLogLevel(string str, LogLevel logLevel)
        {
            var logLevelpadded = logLevel.ToString().PadRight(_longestLengthOfLogLevel);
            return $"{logLevelpadded}{_seperator} {str}";
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
                    var disposableLogger = _innerLogger as IDisposable;
                    if (disposableLogger != null)
                    {
                        disposableLogger.Dispose();
                    }
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~LoggingLevelLoggerAppender() {
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
