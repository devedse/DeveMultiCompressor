# DeveMultiCompressor
A tool to compress files with multiple compressors to find out which one gives the best compression.

## Build status

| AppVeyor (Windows build) |
|:------------------------:|
| [![Build status](https://ci.appveyor.com/api/projects/status/c40u7g3kwhol8uk7?svg=true)](https://ci.appveyor.com/project/devedse/devemulticompressor) |

## Arguments and usage

The tool should be used with the following arguments:

| Short | Argument | Description |
| -- | -- | -- |
| -d | --decompress | If this is set, the program will try to decompress the file instead of compressing it. |
| -i | --inputfile | Required. The path to the input file to compress. |
| -p | --precomp | Precompresses all files before compressing them. (When compressing, first precomp the files. When decompressing, extract all .pcf files). |
| -v | --verify | Unpacks the archive afterwards and compares the recreated file hash with the input file. (Only for compressing). |
| | --include | Included compressors (List of archive extensions seperated by a comma, e.g.: nz,7z,zpaq). |
| | --exclude | Excluded compressors (List of archive extensions seperated by a comma, e.g.: nz,7z,zpaq). |
| | --help | Display the help screen. |
| | --version | Display version information. |

See the sample #SampleCompressEnwik6.cmd in downloaded package for a sample command.

The output (created archives) will be written to a folder named "Output".

## Download

The latest version is automatically build from the sources and can be found on the releases page:
https://github.com/devedse/DeveMultiCompressor/releases

## License

All my own code is licensed under the "Unlicense" (See LICENSE file).

Important note: additional exe's + dll's included in the Release/GIT repository (Compressors/Decompressors folder) might have different licenses then the rest of the csharp code. Please handle this according to their own licensing model.