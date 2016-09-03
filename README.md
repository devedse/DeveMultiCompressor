# DeveMultiCompressor
A tool to compress files with multiple compressors to find out which one gives the best compression.

Travis build:
[![Build Status](https://travis-ci.org/devedse/DeveMultiCompressor.svg?branch=master)](https://travis-ci.org/devedse/DeveMultiCompressor)

AppVeyor build:
[![Build status](https://ci.appveyor.com/api/projects/status/c40u7g3kwhol8uk7?svg=true)](https://ci.appveyor.com/project/devedse/devemulticompressor)


## Arguments

The tool should be ran with the following arguments:

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
	
## Download

The latest version is automatically build from the sources and can be found on the releases page:
https://github.com/devedse/DeveMultiCompressor/releases
