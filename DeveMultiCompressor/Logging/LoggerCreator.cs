using Devedse.DeveImagePyramid.Logging;
using Devedse.DeveImagePyramid.Logging.Appenders;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DeveMultiCompressor.Logging
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
