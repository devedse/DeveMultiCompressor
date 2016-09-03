# DeveMultiCompressor
A tool to compress files with multiple compressors to find out which one gives the best compression.

AppVeyor build:
[![Build status](https://ci.appveyor.com/api/projects/status/c40u7g3kwhol8uk7?svg=true)](https://ci.appveyor.com/project/devedse/devemulticompressor)


## Arguments and usage

The tool should be used with the following arguments:

--decompress
> If this is set, the program will try to decompress the file instead of compressing it.

--inputfile
> Required. The path to the input file to compress.

--useprecomp
> Precompresses all files before compressing them. (When compressing, first precomp the files. When decompressing, extract all .pcf files).

--verify
> Unpacks the archive afterwards and compares the recreated file hash with the input file. (Only for compressing).

--help
> Display this help screen.

--version
> Display version information.

See the sample #SampleCompressEnwik6.cmd in downloaded package for a sample command.

The output (created archives) will be written to a folder named "Output".

## Download

The latest version is automatically build from the sources and can be found on the releases page:
https://github.com/devedse/DeveMultiCompressor/releases
