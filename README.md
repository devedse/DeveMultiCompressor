# DeveMultiCompressor
A tool to compress files with multiple compressors to find out which one gives the best compression.

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

After the compression is done the tool will show the statistics on all the compressors used.

Here's an example for `Pokemon Red.gb`:

|   Extension   |      Description      | Success |    Duration     | Original File Size | Compressed File Size | Compressed File Size (Bytes) | Verification Status | Decompression time |
| ------------- | --------------------- | ------- | --------------- | ------------------ | -------------------- | ---------------------------- | ------------------- | ------------------ |
| paq8px208fix1 | paq8px208fix1 (-12LT) |  True   |   9.8 Minutes   |        1MB         |       261.9KB        |            268161            |       Success       |    9.8 Minutes     |
| paq8px206fix1 | paq8px206fix1 (-12LT) |  True   |   9.9 Minutes   |        1MB         |        262KB         |            268329            |       Success       |    10.1 Minutes    |
|    cmix20     |        cmix20         |  True   |  28.5 Minutes   |        1MB         |       262.2KB        |            268492            |       Success       |    28.7 Minutes    |
|   paq8pxd47   |  paq8pxd_v47 (-s11)   |  True   |   1.6 Minutes   |        1MB         |       272.2KB        |            278742            |       Success       |    1.6 Minutes     |
|   paq8pxd18   |  paq8pxd_v18 (-s11)   |  True   |   1.1 Minutes   |        1MB         |       272.8KB        |            279387            |       Success       |      1 Minute      |
|   paq8hp12    |     paq8hp12 (-8)     |  True   |   50 Seconds    |        1MB         |       289.6KB        |            296532            |       Success       |     49 Seconds     |
|      nz       |     NanoZip (-cc)     |  True   | 712 Miliseconds |        1MB         |       295.8KB        |            302936            |       Success       |  715 Miliseconds   |
|     zpaq      |      zpaq (-m5)       |  True   |    2 Seconds    |        1MB         |       303.7KB        |            310947            |       Success       |      1 Second      |
|      mcm      |          mcm          |  True   |    9 Seconds    |        1MB         |       307.5KB        |            314885            |       Success       |     3 Seconds      |
|      rz       |          rz           |  True   |    1 Second     |        1MB         |       315.3KB        |            322839            |       Success       |  241 Miliseconds   |
|      7z       |       7z (LZMA)       |  True   | 363 Miliseconds |        1MB         |       322.9KB        |            330631            |       Success       |  353 Miliseconds   |
|      7z       |      7z (LZMA2)       |  True   | 742 Miliseconds |        1MB         |        323KB         |            330712            |       Success       |  356 Miliseconds   |
|      bsc      |          bsc          |  True   | 533 Miliseconds |        1MB         |       359.4KB        |            367990            |       Success       |  426 Miliseconds   |
|     nncp      |    Nncp (default)     |  True   |   4.2 Minutes   |        1MB         |       372.5KB        |            381416            |       Success       |    4.4 Minutes     |

## Download

The latest version is automatically build from the sources and can be found on the releases page:
https://github.com/devedse/DeveMultiCompressor/releases

## License

All my own code is licensed under the "Unlicense" (See LICENSE file).

Important note: additional exe's + dll's included in the Release/GIT repository (Compressors/Decompressors folder) might have different licenses then the rest of the csharp code. Please handle this according to their own licensing model.


## Build status

| GitHubActions Builds |
|:--------------------:|
| [![GitHubActions Builds](https://github.com/devedse/DeveMultiCompressor/workflows/GitHubActionsBuilds/badge.svg)](https://github.com/devedse/DeveMultiCompressor/actions/workflows/githubactionsbuilds.yml) |

## DockerHub

| Docker Hub |
|:----------:|
| [![Docker pulls](https://img.shields.io/docker/v/devedse/devemulticompressorconsoleapp)](https://hub.docker.com/r/devedse/devemulticompressorconsoleapp/) |

## Code Coverage Status

| CodeCov |
|:-------:|
| [![codecov](https://codecov.io/gh/devedse/DeveMultiCompressor/branch/master/graph/badge.svg)](https://codecov.io/gh/devedse/DeveMultiCompressor) |

## Code Quality Status

| SonarQube |
|:---------:|
| [![Quality Gate](https://sonarcloud.io/api/project_badges/measure?project=DeveMultiCompressor&metric=alert_status)](https://sonarcloud.io/dashboard?id=DeveMultiCompressor) |

## Package

| NuGet |
|:-----:|
| [![NuGet](https://img.shields.io/nuget/v/DeveMultiCompressor.svg)](https://www.nuget.org/packages/DeveMultiCompressor/) |