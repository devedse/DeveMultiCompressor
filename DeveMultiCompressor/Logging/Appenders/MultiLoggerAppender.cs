using System;
using System.Collections.Generic;

namespace Devedse.DeveImagePyramid.Logging.Appenders
{
    public class MultiLoggerAppender : ILogger, IDisposable
    {
        public List<ILogger> Loggers { get; private set; }

        public MultiLoggerAppender()
            : this(new List<ILogger>())
        {

        }

        public MultiLoggerAppender(List<ILogger> loggers)
        {
            Loggers = loggers;
        }


        public void Write(string str, LogLevel logLevel = LogLevel.Information, ConsoleColor color = ConsoleColor.Gray)
        {
            foreach (var logger in Loggers)
            {
                logger.Write(str, logLevel, color);
            }
        }

        public void WriteError(string str, LogLevel logLevel = LogLevel.Error)
        {
            foreach (var logger in Loggers)
            {
                logger.WriteError(str, logLevel);
            }
        }

        public void EmptyLine()
        {
            foreach (var logger in Loggers)
            {
                logger.EmptyLine();
            }
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    if (Loggers != null)
                    {
                        foreach (var logger in Loggers)
                        {
                            var disposableLogger = logger as IDisposable;
                            if (disposableLogger != null)
                            {
                                disposableLogger.Dispose();
                            }
                        }
                    }
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~MultiLogger() {
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
