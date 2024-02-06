using DeveMultiCompressor.Config;
using DeveMultiCompressor.Logging;
using DryIoc;
using System;
using System.Linq;

namespace DeveMultiCompressor
{
    public static class DryContainer
    {
        public static Container CreateDryContainer(string compressorDirectory = Constants.DefaultCompressorFolder, string precompressorDirectory = Constants.DefaultPrecompressorFolder)
        {
            var container = new Container();

            var compressorDirectoryConfig = new CompressorDirectoryConfiguration()
            {
                CompressorDirectory = compressorDirectory,
                PrecompressorDirectory = precompressorDirectory
            };

            container.RegisterInstance(container);
            container.RegisterInstance(compressorDirectoryConfig);
            container.RegisterInstance<ILogger>(LoggerCreator.CreateLogger(false));
            container.Register<CompressorRunner>();
            container.Register<CompressorFinderFactory>(Reuse.Singleton);
            container.Register<ConfigStringFiller>(Reuse.Singleton);
            container.Register<ProcessRunner>(Reuse.Singleton);

            var failedResolutions = container.Validate();

            if (failedResolutions.Any())
            {
                throw new AggregateException(failedResolutions.Select(t => t.Value));
            }

            return container;
        }
    }
}
