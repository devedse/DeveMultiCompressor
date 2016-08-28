using System;

namespace Devedse.DeveImagePyramid.Logging
{
    public interface ILogger
    {
        void Write(string str, LogLevel logLevel = LogLevel.Information, ConsoleColor color = ConsoleColor.Gray);
        void WriteError(string str, LogLevel logLevel = LogLevel.Error);
        void EmptyLine();
    }
}
