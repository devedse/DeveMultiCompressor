using System;

namespace Devedse.DeveImagePyramid.Logging
{
    public class ConsoleLogger : ILogger
    {
        private readonly LogLevel _levelToLog;
        private readonly static object Lockject = new object();

        public ConsoleLogger(LogLevel levelToLog)
        {
            _levelToLog = levelToLog;
        }

        public void Write(string str, LogLevel logLevel = LogLevel.Information, ConsoleColor color = ConsoleColor.Gray)
        {
            if ((int)logLevel >= (int)_levelToLog)
            {
                lock (Lockject)
                {
                    Console.ForegroundColor = color;
                    Console.WriteLine(str);
                }
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
    }
}
