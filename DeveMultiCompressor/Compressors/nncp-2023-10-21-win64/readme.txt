Lossless Data Compression with Neural Networks
==============================================

1) Overview
-----------

NNCP is an experimental lossless data compressor using Neural
Networks. Two models are available: Transformer (slower but best
ratio) and LSTM (faster). A text preprocessor and tokenizer can
optionally be enabled. More information is available at
https://bellard.org/nncp .

Thanks to LibNC, it supports both the GPU (NVIDIA CUDA version 11.x or
12.x required with a Ampere, ADA or Hopper GPU) and CPU. For an
acceptable speed with large models, a GPU is required.

2) Compilation
--------------

A Linux system is assumed. Just type 'make' to compile the program. A
binary DLL of the LibNC library is included in the archive. Change the
symbolic link to libnc_cuda.so if you are using cuda 12.x instead of
11.x with:

ln -sf libnc_cuda-12.so libnc_cuda.so

Windows cross-compilation from Linux is supported provided the
libnc*.dll files are copied from the Windows version.

5) Current best models for enwik8/enwik9
----------------------------------------

enwik8:

  ./nncp --cuda --profile enwik8 --preprocess 4096,512 c enwik8 out.bin
  
  Result: 14915298 bytes (13.2 hours)

enwik9:

  ./nncp --cuda --profile enwik9 --preprocess 16384,512 c enwik9 out.bin

  Result: 106632363 bytes (2.8 days)

Decompression:

  ./nncp --cuda d out.bin out.txt

  Decompression has a similar speed than compression.

Test system: AMD Ryzen 9 3900 + RTX 4090 NVIDIA GPU

Memory usage: CPU: 1 GB, GPU: 6.6 GB

6) Licence
----------

The source code is released under the MIT licence.

The LibNC library is provided in binary form and can be freely redistributed.
