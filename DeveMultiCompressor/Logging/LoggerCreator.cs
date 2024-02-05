using DeveMultiCompressor.Lib.Logging.Appenders;
using System;
using System.Collections.Generic;
using System.IO;

namespace DeveMultiCompressor.Lib.Logging
{
    public static class LoggerCreator
    {
        public static DateTimeLoggerAppender CreateLogger(bool useDifferentFileNamesForLogs)
        {
            var consoleLogger = new ConsoleLogger(LogLevel.Verbose);

            var logFileName = useDifferentFileNamesForLogs ? DateTime.Now.ToString("yyyy-MM-dd--HH-mm-ss") + ".txt" : "Log.txt";
            var fileLoggerPath = Path.Combine(FolderHelperMethods.AssemblyDirectory.Value, "Logs", logFileName);
            var fileLogger = new FileLogger(fileLoggerPath, LogLevel.Verbose);

            var multiLogger = new MultiLoggerAppender(new List<ILogger>() { consoleLogger, fileLogger });

            var loggingLevelAppender = new LoggingLevelLoggerAppender(multiLogger, " <:>");
            var dateTimeAppender = new DateTimeLoggerAppender(loggingLevelAppender, ":");
            return dateTimeAppender;
        }
    }
}
