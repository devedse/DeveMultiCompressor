using DeveMultiCompressor.Config;
using DeveMultiCompressor.Lib.Logging;
using DryIoc;
using System;
using System.Linq;

namespace DeveMultiCompressor.Lib
{
    public static class DryContainer
    {
        public static Container CreateDryContainer()
        {
            var container = new Container();

            container.RegisterInstance(container);
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
