using CommandLine;
using DeveMultiCompressor.Config;
using DeveMultiCompressor.Lib;
using DeveMultiCompressor.Lib.Logging;
using DryIoc;
using System;
using System.Threading;

namespace DeveMultiCompressor
{
    public class Program
    {
        public static int Main(string[] args)
        {
            using (var container = DryContainer.CreateDryContainer())
            {
                var compressorRunner = container.Resolve<CompressorRunner>();
                var logger = container.Resolve<ILogger>();

                var result = Parser.Default.ParseArguments<CommandLineOptions>(args);
                var exitCode = result.MapResult(options =>
                    {
                        if (options.Decompress == false)
                        {
                            compressorRunner.GoCompress(options);
                        }
                        else
                        {
                            if (options.Verify)
                            {
                                logger.Write("--verify ignored because this argument is not valid for decompressing.", LogLevel.Warning, ConsoleColor.Yellow);
                            }
                            compressorRunner.GoDecompress(options);
                        }
                        return 0;
                    },
                    errors =>
                    {
                        foreach (var error in errors)
                        {
                            logger.WriteError(error.ToString());
                        }
                        Thread.Sleep(2000);
                        return 1;
                    });
                return exitCode;
            }
        }
    }
}
