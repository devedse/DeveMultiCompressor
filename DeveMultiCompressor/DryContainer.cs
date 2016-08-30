using Devedse.DeveImagePyramid.Logging;
using DeveMultiCompressor.Config;
using DeveMultiCompressor.Logging;
using DryIoc;
using System;
using System.Linq;

namespace DeveMultiCompressor
{
    public static class DryContainer
    {
        public static Container CreateDryContainer()
        {
            var container = new Container();

            container.RegisterInstance(container, Reuse.Singleton);
            container.RegisterInstance<ILogger>(LoggerCreator.CreateLogger(false), Reuse.Singleton);
            container.Register<CompressorRunner>();
            container.Register<CompressorFinderFactory>(Reuse.Singleton);
            container.Register<ConfigStringFiller>(Reuse.Singleton);
            container.Register<ProcessRunner>(Reuse.Singleton);

            var failedResolutions = container.VerifyResolutions();

            if (failedResolutions.Any())
            {
                throw new AggregateException(failedResolutions.Select(t => t.Value));
            }

            return container;
        }
    }
}
