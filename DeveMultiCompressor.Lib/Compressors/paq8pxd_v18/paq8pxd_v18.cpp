/* paq8pxd18 file compressor/archiver.  Release by Kaido Orav, Jul. 19, 2016

    Copyright (C) 2008-2014 Matt Mahoney, Serge Osnach, Alexander Ratushnyak,
    Bill Pettis, Przemyslaw Skibinski, Matthew Fite, wowtiger, Andrew Paterson,
    Jan Ondrus, Andreas Morphis, Pavel L. Holoborodko, Kaido Orav, Simon Berger,
    Neill Corlett

    LICENSE

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details at
    Visit <http://www.gnu.org/copyleft/gpl.html>.

To install and use in Windows:

- To install, put paq8pxd_v18.exe or a shortcut to it on your desktop.
- To compress a file or folder, drop it on the paq8pxd18 icon.
- To decompress, drop a .paq8pxd18 file on the icon.

A .paq8pxd18 extension is added for compression, removed for decompression.
The output will go in the same folder as the input.

While paq8pxd18 is working, a command window will appear and report
progress.  When it is done you can close the window by pressing
ENTER or clicking [X].


COMMAND LINE INTERFACE

- To install, put paq8pxd_v18.exe somewhere in your PATH.
- To compress:      paq8pxd_v18 [-N] file1 [file2...]
- To decompress:    paq8pxd_v18 [-d] file1.paq8pxd18 [dir2]
- To view contents: paq8pxd_v18 -l file1.paq8pxd18

The compressed output file is named by adding ".paq8pxd18" extension to
the first named file (file1.paq8pxd18).  Each file that exists will be
added to the archive and its name will be stored without a path.
The option -N specifies a compression level ranging from -0
(fastest) to -8 (smallest).  The default is -5.  If there is
no option and only one file, then the program will pause when
finished until you press the ENTER key (to support drag and drop).
If file1.paq8pxd18 exists then it is overwritten. Level -0 only
transforms or decompresses data.

If the first named file ends in ".paq8pxd18" then it is assumed to be
an archive and the files within are extracted to the same directory
as the archive unless a different directory (dir2) is specified.
The -d option forces extraction even if there is not a ".paq8pxd18"
extension.  If any output file already exists, then it is compared
with the archive content and the first byte that differs is reported.
No files are overwritten or deleted.  If there is only one argument
(no -d or dir2) then the program will pause when finished until
you press ENTER.

For compression, if any named file is actually a directory, then all
files and subdirectories are compressed, preserving the directory
structure, except that empty directories are not stored, and file
attributes (timestamps, permissions, etc.) are not preserved.
During extraction, directories are created as needed.  For example:

  paq8pxd_v18 -4 c:\tmp\foo bar

compresses foo and bar (if they exist) to c:\tmp\foo.paq8pxd18 at level 4.

  paq8pxd_v18 -d c:\tmp\foo.paq8pxd18 .

extracts foo and compares bar in the current directory.  If foo and bar
are directories then their contents are extracted/compared.

There are no commands to update an existing archive or to extract
part of an archive.  Files and archives larger than 2GB are not
supported (but might work on 64-bit machines, not tested).
File names with nonprintable characters are not supported (spaces
are OK).


TO COMPILE

There are 2 files: paq8pxd_v18.cpp (C++) and wrtpre.cpp (C++).
paq8pxd_v18.cpp recognizes the following compiler options:

  -DWINDOWS           (to compile in Windows)
  -DUNIX              (to compile in Unix, Linux, etc)
  -DMT                (to compile with multithreading support)
  -DDEFAULT_OPTION=N  (to change the default compression level from 5 to N).

If you compile without -DWINDOWS or -DUNIX, you can still compress files,
but you cannot compress directories or create them during extraction.
You can extract directories if you manually create the empty directories
first.

Use -DEFAULT_OPTION=N to change the default compression level to support
drag and drop on machines with less than 256 MB of memory.  Use
-DDEFAULT_OPTION=4 for 128 MB, 3 for 64 MB, 2 for 32 MB, etc.


Recommended compiler commands and optimizations:

  MINGW g++ (x86,x64):
   with multithreading:
    g++ paq8pxd_v18.cpp -DWINDOWS -DMT -msse2 -O3 -s -static -o paq8pxd_v18.exe 
   without multithreading:
    g++ paq8pxd_v18.cpp -DWINDOWS -msse2 -O3 -s -static -o paq8pxd_v18.exe 

  UNIX/Linux (PC x86,x64):
   with multithreading:
    g++ paq8pxd_v18.cpp -DUNIX -DMT -msse2 -O3 -s -static -lpthread -o paq8pxd_v18
   without multithreading:
    g++ paq8pxd_v18.cpp -DUNIX -msse2 -O3 -s -static -lpthread -o paq8pxd_v18

  Non PC (e.g. PowerPC under MacOS X)
    g++ paq8pxd_v18.cpp -O2 -DUNIX -s -o paq8pxd_v18


ARCHIVE FILE FORMAT

An archive has the following format.  

  paq8pxd18 -N 
  segment size 
  segment offset
  \0 file list size
  compressed file list(
    size TAB filename CR LF
    size TAB filename CR LF
    ...)
  compressed binary data
  file segmentation data
  stream data sizes[11]

-N is the option (-0 to -15) and mode, even if a default was used.
00LMNNNN bit M is set if fast mode, 
         bit L is set if quick mode,
         if L or M are not set default to slow mode.

segment size is total size of file(s) segmentation data in bytes
at segmnet offset after compressed binary data.

file segmentation data is full list of detected blocks:
type size info
type size info
type size 
type size info
.....

info is present if block type needs extra info like in image or audio.

Plain file names are stored without a path.  Files in compressed
directories are stored with path relative to the compressed directory
(using UNIX style forward slashes "/").  For example, given these files:

  123 C:\dir1\file1.txt
  456 C:\dir2\file2.txt

Then

  paq8pxd_v18 archive \dir1\file1.txt \dir2

will create archive.paq8pxd18 

The command:

  paq8pxd_v18 archive.paq8pxd18 C:\dir3

will create the files:

  C:\dir3\file1.txt
  C:\dir3\dir2\file2.txt

Decompression will fail if the first 10 bytes are not "paq8pxd18 -".  Sizes
are stored as decimal numbers.  CR, LF, TAB are ASCII codes
13, 10, 9 respectively.


ARITHMETIC CODING

The binary data is arithmetic coded as the shortest base 256 fixed point
number x = SUM_i x_i 256^-1-i such that p(<y) <= x < p(<=y), where y is the
input string, x_i is the i'th coded byte, p(<y) (and p(<=y)) means the
probability that a string is lexicographcally less than (less than
or equal to) y according to the model, _ denotes subscript, and ^ denotes
exponentiation.

The model p(y) for y is a conditional bit stream,
p(y) = PROD_j p(y_j | y_0..j-1) where y_0..j-1 denotes the first j
bits of y, and y_j is the next bit.  Compression depends almost entirely
on the ability to predict the next bit accurately.


MODEL MIXING

paq8pxd_v18 uses a neural network to combine a large number of models.  The
i'th model independently predicts
p1_i = p(y_j = 1 | y_0..j-1), p0_i = 1 - p1_i.
The network computes the next bit probabilty

  p1 = squash(SUM_i w_i t_i), p0 = 1 - p1                        (1)

where t_i = stretch(p1_i) is the i'th input, p1_i is the prediction of
the i'th model, p1 is the output prediction, stretch(p) = ln(p/(1-p)),
and squash(s) = 1/(1+exp(-s)).  Note that squash() and stretch() are
inverses of each other.

After bit y_j (0 or 1) is received, the network is trained:

  w_i := w_i + eta t_i (y_j - p1)                                (2)

where eta is an ad-hoc learning rate, t_i is the i'th input, (y_j - p1)
is the prediction error for the j'th input but, and w_i is the i'th
weight.  Note that this differs from back propagation:

  w_i := w_i + eta t_i (y_j - p1) p0 p1                          (3)

which is a gradient descent in weight space to minimize root mean square
error.  Rather, the goal in compression is to minimize coding cost,
which is -log(p0) if y = 1 or -log(p1) if y = 0.  Taking
the partial derivative of cost with respect to w_i yields (2).


MODELS

Most models are context models.  A function of the context (last few
bytes) is mapped by a lookup table or hash table to a state which depends
on the bit history (prior sequence of 0 and 1 bits seen in this context).
The bit history is then mapped to p1_i by a fixed or adaptive function.
There are several types of bit history states:

- Run Map. The state is (b,n) where b is the last bit seen (0 or 1) and
  n is the number of consecutive times this value was seen.  The initial
  state is (0,0).  The output is computed directly:

    t_i = (2b - 1)K log(n + 1).

  where K is ad-hoc, around 4 to 10.  When bit y_j is seen, the state
  is updated:

    (b,n) := (b,n+1) if y_j = b, else (y_j,1).

- Stationary Map.  The state is p, initially 1/2.  The output is
  t_i = stretch(p).  The state is updated at ad-hoc rate K (around 0.01):

    p := p + K(y_j - p)

- Nonstationary Map.  This is a compromise between a stationary map, which
  assumes uniform statistics, and a run map, which adapts quickly by
  discarding old statistics.  An 8 bit state represents (n0,n1,h), initially
  (0,0,0) where:

    n0 is the number of 0 bits seen "recently".
    n1 is the number of 1 bits seen "recently".
    n = n0 + n1.
    h is the full bit history for 0 <= n <= 4,
      the last bit seen (0 or 1) if 5 <= n <= 15,
      0 for n >= 16.

  The primaty output is t_i := stretch(sm(n0,n1,h)), where sm(.) is
  a stationary map with K = 1/256, initialized to
  sm(n0,n1,h) = (n1+(1/64))/(n+2/64).  Four additional inputs are also
  be computed to improve compression slightly:

    p1_i = sm(n0,n1,h)
    p0_i = 1 - p1_i
    t_i   := stretch(p_1)
    t_i+1 := K1 (p1_i - p0_i)
    t_i+2 := K2 stretch(p1) if n0 = 0, -K2 stretch(p1) if n1 = 0, else 0
    t_i+3 := K3 (-p0_i if n1 = 0, p1_i if n0 = 0, else 0)
    t_i+4 := K3 (-p0_i if n0 = 0, p1_i if n1 = 0, else 0)

  where K1..K4 are ad-hoc constants.

  h is updated as follows:
    If n < 4, append y_j to h.
    Else if n <= 16, set h := y_j.
    Else h = 0.

  The update rule is biased toward newer data in a way that allows
  n0 or n1, but not both, to grow large by discarding counts of the
  opposite bit.  Large counts are incremented probabilistically.
  Specifically, when y_j = 0 then the update rule is:

    n0 := n0 + 1, n < 29
          n0 + 1 with probability 2^(27-n0)/2 else n0, 29 <= n0 < 41
          n0, n = 41.
    n1 := n1, n1 <= 5
          round(8/3 lg n1), if n1 > 5

  swapping (n0,n1) when y_j = 1.

  Furthermore, to allow an 8 bit representation for (n0,n1,h), states
  exceeding the following values of n0 or n1 are replaced with the
  state with the closest ratio n0:n1 obtained by decrementing the
  smaller count: (41,0,h), (40,1,h), (12,2,h), (5,3,h), (4,4,h),
  (3,5,h), (2,12,h), (1,40,h), (0,41,h).  For example:
  (12,2,1) 0-> (7,1,0) because there is no state (13,2,0).

- Match Model.  The state is (c,b), initially (0,0), where c is 1 if
  the context was previously seen, else 0, and b is the next bit in
  this context.  The prediction is:

    t_i := (2b - 1)Kc log(m + 1)

  where m is the length of the context.  The update rule is c := 1,
  b := y_j.  A match model can be implemented efficiently by storing
  input in a buffer and storing pointers into the buffer into a hash
  table indexed by context.  Then c is indicated by a hash table entry
  and b can be retrieved from the buffer.


CONTEXTS

High compression is achieved by combining a large number of contexts.
Most (not all) contexts start on a byte boundary and end on the bit
immediately preceding the predicted bit.  The contexts below are
modeled with both a run map and a nonstationary map unless indicated.

- Order n.  The last n bytes, up to about 16.  For general purpose data.
  Most of the compression occurs here for orders up to about 6.
  An order 0 context includes only the 0-7 bits of the partially coded
  byte and the number of these bits (255 possible values).

- Sparse.  Usually 1 or 2 of the last 8 bytes preceding the byte containing
  the predicted bit, e.g (2), (3),..., (8), (1,3), (1,4), (1,5), (1,6),
  (2,3), (2,4), (3,6), (4,8).  The ordinary order 1 and 2 context, (1)
  or (1,2) are included above.  Useful for binary data.

- Text.  Contexts consists of whole words (a-z, converted to lower case
  and skipping other values).  Contexts may be sparse, e.g (0,2) meaning
  the current (partially coded) word and the second word preceding the
  current one.  Useful contexts are (0), (0,1), (0,1,2), (0,2), (0,3),
  (0,4).  The preceding byte may or may not be included as context in the
  current word.

- Formatted text.  The column number (determined by the position of
  the last linefeed) is combined with other contexts: the charater to
  the left and the character above it.

- Fixed record length.  The record length is determined by searching for
  byte sequences with a uniform stride length.  Once this is found, then
  the record length is combined with the context of the bytes immediately
  preceding it and the corresponding byte locations in the previous
  one or two records (as with formatted text).

- Context gap.  The distance to the previous occurrence of the order 1
  or order 2 context is combined with other low order (1-2) contexts.

- FAX.  For 2-level bitmapped images.  Contexts are the surrounding
  pixels already seen.  Image width is assumed to be 1728 bits (as
  in calgary/pic).

- Image.  For uncompressed 8,24-bit color BMP, TIFF and TGA images.
  Contexts are the high order bits of the surrounding pixels and linear
  combinations of those pixels, including other color planes.  The
  image width is detected from the file header.  When an image is
  detected, other models are turned off to improve speed.

- JPEG.  Files are further compressed by partially uncompressing back
  to the DCT coefficients to provide context for the next Huffman code.
  Only baseline DCT-Huffman coded files are modeled.  (This ia about
  90% of images, the others are usually progresssive coded).  JPEG images
  embedded in other files (quite common) are detected by headers.  The
  baseline JPEG coding process is:
  - Convert to grayscale and 2 chroma colorspace.
  - Sometimes downsample the chroma images 2:1 or 4:1 in X and/or Y.
  - Divide each of the 3 images into 8x8 blocks.
  - Convert using 2-D discrete cosine transform (DCT) to 64 12-bit signed
    coefficients.
  - Quantize the coefficients by integer division (lossy).
  - Split the image into horizontal slices coded independently, separated
    by restart codes.
  - Scan each block starting with the DC (0,0) coefficient in zigzag order
    to the (7,7) coefficient, interleaving the 3 color components in
    order to scan the whole image left to right starting at the top.
  - Subtract the previous DC component from the current in each color.
  - Code the coefficients using RS codes, where R is a run of R zeros (0-15)
    and S indicates 0-11 bits of a signed value to follow.  (There is a
    special RS code (EOB) to indicate the rest of the 64 coefficients are 0).
  - Huffman code the RS symbol, followed by S literal bits.
  The most useful contexts are the current partially coded Huffman code
  (including S following bits) combined with the coefficient position
  (0-63), color (0-2), and last few RS codes.

- Match.  When a context match of 400 bytes or longer is detected,
  the next bit of the match is predicted and other models are turned
  off to improve speed.

- Exe.  When a x86 file (.exe, .obj, .dll) is detected, sparse contexts
  with gaps of 1-12 selecting only the prefix, opcode, and the bits
  of the modR/M byte that are relevant to parsing are selected.
  This model is turned off otherwise.

- Indirect.  The history of the last 1-3 bytes in the context of the
  last 1-2 bytes is combined with this 1-2 byte context.

- DMC. A bitwise n-th order context is built from a state machine using
  DMC, described in http://plg.uwaterloo.ca/~ftp/dmc/dmc.c
  The effect is to extend a single context, one bit at a time and predict
  the next bit based on the history in this context.  The model here differs
  in that two predictors are used.  One is a pair of counts as in the original
  DMC.  The second predictor is a bit history state mapped adaptively to
  a probability as as in a Nonstationary Map.

ARCHITECTURE

The context models are mixed by several of several hundred neural networks
selected by a low-order context.  The outputs of these networks are
combined using a second neural network, then fed through several stages of
adaptive probability maps (APM) before arithmetic coding.

For images, only one neural network is used and its context is fixed.

An APM is a stationary map combining a context and an input probability.
The input probability is stretched and divided into 32 segments to
combine with other contexts.  The output is interpolated between two
adjacent quantized values of stretch(p1).  There are 2 APM stages in series:

  p1 := (p1 + 3 APM(order 0, p1)) / 4.
  p1 := (APM(order 1, p1) + 2 APM(order 2, p1) + APM(order 3, p1)) / 4.

PREPROCESSING

paq8pxd_v18 uses preprocessing transforms on certain data types to improve
compression.  To improve reliability, the decoding transform is
tested during compression to ensure that the input file can be
restored.  If the decoder output is not identical to the input file
due to a bug, then the transform is abandoned and the data is compressed
without a transform so that it will still decompress correctly.

The input is split into blocks with the format <type> <decoded size> <info>
where <type> is 1 byte (0 = no transform), <decoded size> is the size
of the data after decoding, which may be different than the size of <data>.
Data is stored uncompressed after compressed data ends.
The preprocessor has 3 parts:

- Detector.  Splits the input into smaller blocks depending on data type.

- Coder.  Input is a block to be compressed.  Output is a temporary
  file.  The coder determines whether a transform is to be applied
  based on file type, and if so, which one.  A coder may use lots
  of resources (memory, time) and make multiple passes through the
  input file.  The file type is stored (as one byte) during compression.

- Decoder.  Performs the inverse transform of the coder.  It uses few
  resorces (fast, low memory) and runs in a single pass (stream oriented).
  It takes input either from a file or the arithmetic decoder.  Each call
  to the decoder returns a single decoded byte.

The following transforms are used:

- EXE:  CALL (0xE8) and JMP (0xE9) address operands are converted from
  relative to absolute address.  The transform is to replace the sequence
  E8/E9 xx xx xx 00/FF by adding file offset modulo 2^25 (signed range,
  little-endian format).  Data to transform is identified by trying the
  transform and applying a crude compression test: testing whether the
  byte following the E8/E8 (LSB of the address) occurred more recently
  in the transformed data than the original and within 4KB 4 times in
  a row.  The block ends when this does not happen for 4KB.

- JPEG: detected by SOI and SOF and ending with EOI or any nondecodable
  data.  No transform is applied.  The purpose is to separate images
  embedded in execuables to block the EXE transform, and for a future
  place to insert a transform.
  
- BASE64: Decodes BASE64 encoded data and recursively transformed
  up to level 5. Input can be full stream or end-of-line coded.
  
- BASE85: Decodes Ascii85 encoded data and recursively transformed
  up to level 5. Input can be full stream or end-of-line coded.
  Supports: https://en.wikipedia.org/wiki/Ascii85#Adobe_version

- 24-bit images: 24-bit image data uses simple color transform
  (b, g, r) -> (g, g-r, g-b)
  
- ZLIB:  Decodes zlib encoded data and recursively transformed
  up to level 5. Supports zlib compressed images (4/8/24 bit) in pdf

- GIF:  Gif (8 bit) image  recompression. 
  
- CD: mode 1 and mode 2 form 1+2 - 2352 bytes

- MDF: wraped around CD, re-arranges subchannel  and CD data

- TEXT: All detected text blocks are transformed using dynamic dictionary
  preprocessing (based on XWRT). If transformed block is larger from original
  then transform is skipped.
  
- LZSS: (Haruhiko Okumura's LZSS): Decompresses Microsoft compress.exe archives.

- TTA: audio filter 

- MRB: 8 bit images with RLE compression


IMPLEMENTATION

Hash tables are designed to minimize cache misses, which consume most
of the CPU time.

Most of the memory is used by the nonstationary context models.
Contexts are represented by 32 bits, possibly a hash.  These are
mapped to a bit history, represented by 1 byte.  The hash table is
organized into 64-byte buckets on cache line boundaries.  Each bucket
contains 7 x 7 bit histories, 7 16-bit checksums, and a 2 element LRU
queue packed into one byte.  Each 7 byte element represents 7 histories
for a context ending on a 3-bit boundary plus 0-2 more bits.  One
element (for bits 0-1, which have 4 unused bytes) also contains a run model
consisting of the last byte seen and a count (as 1 byte each).

Run models use 4 byte hash elements consisting of a 2 byte checksum, a
repeat count (0-255) and the byte value.  The count also serves as
a priority.

Stationary models are most appropriate for small contexts, so the
context is used as a direct table lookup without hashing.

The match model maintains a pointer to the last match until a mismatching
bit is found.  At the start of the next byte, the hash table is referenced
to find another match.  The hash table of pointers is updated after each
whole byte.  There is no checksum.  Collisions are detected by comparing
the current and matched context in a rotating buffer.

The inner loops of the neural network prediction (1) and training (2)
algorithms are implemented in MMX assembler, which computes 4 elements
at a time.  Using assembler is 8 times faster than C++ for this code
and 1/3 faster overall.  (However I found that SSE2 code on an AMD-64,
which computes 8 elements at a time, is not any faster).


DIFFERENCES FROM PAQ8PXD_V16
+zlib recompression from paq8px_v71
+gif recompression from paq8px_v72+ 
+ascii85 decode/encode filter
-base64 detection changes (fix freezing up on text files)
-more adaptive 8bit image model
-better 1bit image model
-contextmap SSE changes
-szdd size incrased
-wav all sample rates
+mdf cd filter
+mrb filter (8 bit rle image)
+ppm model  (http://encode.ru/threads/2515-mod_ppmd)
*/

#define PROGNAME "paq8pxd18"  // Please change this if you change the program.
#define SIMD_GET_SSE  //uncomment to use SSE2 in ContexMap
#define MT            //uncomment for multithreading, compression only

#ifdef WINDOWS                       
#ifdef MT
//#define PTHREAD       //uncomment to force pthread to igore windows native threads
#endif
#endif

#ifdef unix
#ifdef MT 
#define PTHREAD 1
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "zlib.h"
#define NDEBUG  // remove for debugging (turns on Array bound checks)
#include <assert.h>

#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif

#ifdef WINDOWS
#include <windows.h>
#endif

#ifndef DEFAULT_OPTION
#define DEFAULT_OPTION 5
#endif
#include <stdint.h>
#ifdef _MSC_VER

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#endif
// 8, 16, 32 bit unsigned types (adjust as appropriate)
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef uint64_t   U64;

// min, max functions
#if  !defined(WINDOWS) || !defined (min)
inline int min(int a, int b) {return a<b?a:b;}
inline int max(int a, int b) {return a<b?b:a;}
#endif

#if defined(WINDOWS) || defined(_MSC_VER)
    #define atoll(S) _atoi64(S)
#endif

#ifdef _MSC_VER  
#define fseeko(a,b,c) _fseeki64(a,b,c)
#define ftello(a) _ftelli64(a)
#else
#ifndef unix
#ifndef fseeko
#define fseeko(a,b,c) fseeko64(a,b,c)
#endif
#ifndef ftello
#define ftello(a) ftello64(a)
#endif
#endif
#endif


#ifdef PTHREAD
#include "pthread.h"
#endif

// Error handler: print message if any, and exit
void quit(const char* message=0) {
  throw message;
}

// strings are equal ignoring case?
int equals(const char* a, const char* b) {
  assert(a && b);
  while (*a && *b) {
    int c1=*a;
    if (c1>='A'&&c1<='Z') c1+='a'-'A';
    int c2=*b;
    if (c2>='A'&&c2<='Z') c2+='a'-'A';
    if (c1!=c2) return 0;
    ++a;
    ++b;
  }
  return *a==*b;
}

//////////////////////// Program Checker /////////////////////

// Track time and memory used
class ProgramChecker {
    private:
  U64 memused;  // bytes allocated by Array<T> now
  U64 maxmem;   // most bytes allocated ever
  clock_t start_time;  // in ticks
public:
   void alloc(U64 n) {  // report memory allocated
    memused+=n;
    if (memused>maxmem) maxmem=memused;
  }
  void free(U64 n) {  // free memory 
  if (memused<n) memused=0;
  else  memused-=n;
  }
  ProgramChecker(): memused(0), maxmem(0) {
    start_time=clock();
    assert(sizeof(U8)==1);
    assert(sizeof(U16)==2);
    assert(sizeof(U32)==4);
    assert(sizeof(U64)==8);
    assert(sizeof(short)==2);
    assert(sizeof(int)==4);  
  }
  void print() const {  // print time and memory used
    printf("Time %1.2f sec, used %0.1f MB (%0.0f bytes) of memory\n",
      double(clock()-start_time)/CLOCKS_PER_SEC, ((maxmem+0.1)/1024)/1024,(maxmem+0.0));
  }
} programChecker;

//////////////////////////// Array ////////////////////////////

// Array<T, ALIGN> a(n); creates n elements of T initialized to 0 bits.
// Constructors for T are not called.
// Indexing is bounds checked if assertions are on.
// a.size() returns n.
// a.resize(n) changes size to n, padding with 0 bits or truncating.
// a.push_back(x) appends x and increases size by 1, reserving up to size*2.
// a.pop_back() decreases size by 1, does not free memory.
// Copy and assignment are not supported.
// Memory is aligned on a ALIGN byte boundary (power of 2), default is none.

template <class T, int ALIGN=0> class Array {
private:
  U32 n;     // user size
  U32 reserved;  // actual size
  char *ptr; // allocated memory, zeroed
  T* data;   // start of n elements of aligned data
  void create(U32 i);  // create with size i
public:
  explicit Array(U32 i=0) {create(i);}
  ~Array();
  T& operator[](U32 i) {
#ifndef NDEBUG
    if (i<0 || i>=n)     fprintf(stderr, "%d out of bounds %d\n", i, n), quit();
#endif
    return data[i];
  }
  const T& operator[](U32 i) const {
#ifndef NDEBUG
    if (i<0 || i>=n)     fprintf(stderr, "%d out of bounds %d\n", i, n), quit();
#endif
    return data[i];
  }
  U32 size() const {return n;}
  void resize(U32 i);  // change size to i
  void pop_back() {if (n>0) --n;}  // decrement size
  void push_back(const T& x);  // increment size, append x
private:
  Array(const Array&);  // no copy or assignment
  Array& operator=(const Array&);
};

template<class T, int ALIGN> void Array<T, ALIGN>::resize(U32 i) {
  if (i<=reserved) {
    n=i;
    return;
  }
  char *saveptr=ptr;
  T *savedata=data;
  int saven=n;
  create(i);
  if (saveptr) {
    if (savedata) {
      memcpy(data, savedata, sizeof(T)*min(i, saven));
      programChecker.free(U64(ALIGN+n*sizeof(T)));
    }
    free(saveptr);
  }
}

template<class T, int ALIGN> void Array<T, ALIGN>::create(U32 i) {
  n=reserved=i;
  if (i<=0) {
    data=0;
    ptr=0;
    return;
  }

  const size_t sz=ALIGN+n*sizeof(T);
  assert(sz>0);
//#ifndef NDEBUG 
//printf("\nAlloc: Try to allocate %0.0f bytes\n",sz+0.0);
//#endif
  ptr = (char*)calloc(sz, 1);
  if (!ptr) printf("\nError: Allocating %0.0f bytes\n",sz+0.0),programChecker.print(), quit("Out of memory");
  programChecker.alloc(U64(sz));
  data = (ALIGN ? (T*)(ptr+ALIGN-(((long long)ptr)&(ALIGN-1))) : (T*)ptr);
  assert((char*)data>=ptr && (char*)data<=ptr+ALIGN);
}

template<class T, int ALIGN> Array<T, ALIGN>::~Array() {
  programChecker.free(U64(ALIGN+n*sizeof(T)));
  free(ptr);
}

template<class T, int ALIGN> void Array<T, ALIGN>::push_back(const T& x) {
  if (n==reserved) {
    int saven=n;
    resize(max(1, n*2));
    n=saven;
  }
  data[n++]=x;
}

/////////////////////////// String /////////////////////////////

// A tiny subset of std::string
// size() includes NUL terminator.

class String: public Array<char> {
public:
  const char* c_str() const {return &(*this)[0];}
  void operator=(const char* s) {
    resize(strlen(s)+1);
    strcpy(&(*this)[0], s);
  }
  void operator+=(const char* s) {
    assert(s);
    pop_back();
    while (*s) push_back(*s++);
    push_back(0);
  }
  String(const char* s=""): Array<char>(1) {
    (*this)+=s;
  }
};


//////////////////////////// rnd ///////////////////////////////

// 32-bit pseudo random number generator
class Random{
  Array<U32> table;
  int i;
public:
  Random(): table(64) {
    table[0]=123456789;
    table[1]=987654321;
    for (int j=0; j<62; j++) table[j+2]=table[j+1]*11+table[j]*23/16;
    i=0;
  }
  U32 operator()() {
    return ++i, table[i&63]=table[(i-24)&63]^table[(i-55)&63];
  }
} ;

////////////////////////////// Buf /////////////////////////////

// Buf(n) buf; creates an array of n bytes (must be a power of 2).
// buf[i] returns a reference to the i'th byte with wrap (no out of bounds).
// buf(i) returns i'th byte back from pos (i > 0)
// buf.size() returns n.


class Buf {
  Array<U8> b;
public:
int pos;  // Number of input bytes in buf (is wrapped)
int poswr; //wrapp
  Buf(U32 i=0): b(i),pos(0) {poswr=i-1;}
  void setsize(int i) {
    if (!i) return;
    assert(i>0 && (i&(i-1))==0);
    poswr=i-1;
    b.resize(i);
  }
  U8& operator[](U32 i) {
    return b[i&(b.size()-1)];
  }
  int operator()(U32 i) const {
    assert(i>0);
    return b[(pos-i)&(b.size()-1)];
  }
  U32 size() const {
    return b.size();
  }
};

// IntBuf(n) is a buffer of n int (must be a power of 2).
// intBuf[i] returns a reference to i'th element with wrap.

class IntBuf {
  Array<int> b;
public:
  IntBuf(U32 i=0): b(i) {}
  int& operator[](U32 i) {
    return b[i&(b.size()-1)];
  }
};

// Buffer for file segment info 
// type size info(if not -1)
class Segment {
  Array<U8> b;
public:
    int pos;  //size of buffer
    U64 hpos; //header pos points to segment info at archive end
    //int count; //count of segments
  Segment(int i=0): b(i),pos(0),hpos(0)/*,count(0)*/ {}
  void setsize(int i) {
    if (!i) return;
    assert(i>0);
    b.resize(i);
  }
  U8& operator[](int i) {
      if (i>=b.size()) setsize(i+1);
    return b[i];
  }
  int operator()(int i) const {
    assert(i>=0);
    assert(i<=b.size());
    return b[i];
  }
  
  // put 8 bytes to segment buffer
  void put8(U64 num){
    if ((pos+8)>=b.size()) setsize(pos+8);
    b[pos++]=(num>>56)&0xff;
    b[pos++]=(num>>48)&0xff;
    b[pos++]=(num>>40)&0xff;
    b[pos++]=(num>>32)&0xff;
    b[pos++]=(num>>24)&0xff;
    b[pos++]=(num>>16)&0xff;
    b[pos++]=(num>>8)&0xff;
    b[pos++]=num&0xff;  
  }
  void put4(U32 num){
    if ((pos+4)>=b.size()) setsize(pos+4);
    b[pos++]=(num>>24)&0xff;
    b[pos++]=(num>>16)&0xff;
    b[pos++]=(num>>8)&0xff;
    b[pos++]=num&0xff;  
  }
  void put1(U8 num){
    if (pos>=b.size()) setsize(pos+1);
    b[pos++]=num;
  }
  int size() const {
    return b.size();
  }
};


/////////////////////// Global context /////////////////////////
bool modeFast=false;
bool modeQuick=false;
U64 level=DEFAULT_OPTION;  // Compression level 0 to 15
U64 MEM(){
     return 0x10000UL<<level;
}

Segment segment; //for file segments type size info(if not -1)
const int streamc=11;
FILE * filestreams[streamc];
const int datatypecount=21;
typedef enum {DEFAULT, JPEG, HDR, IMAGE1,IMAGE4, IMAGE8, IMAGE24, AUDIO, EXE,
              CD, TEXT,TEXT0, TXTUTF8,NESROM, BASE64, BASE85, GIF ,SZDD,MRBR,
              ZLIB,MDF,DICTTXT,BIGTEXT} Filetype;
const char* typenames[datatypecount]={"default", "jpeg", "hdr", "1b-image", "4b-image", "8b-image", "24b-image", "audio",
                                "exe", "cd", "text","text0","utf-8","nes","base64","base85","gif","SZDD","mrb","zlib","mdf"};

U32 WRT_mpw[16]= { 4, 4, 3, 2, 2, 2, 1, 1,  1, 1, 1, 1, 0, 0, 0, 0 };
U32 WRT_mtt[16]= { 0, 0, 1, 2, 3, 4, 5, 5,  6, 6, 6, 6, 7, 7, 7, 7 };
U32 tri[4]={0,4,3,7}, trj[4]={0,6,6,12};

// Contain all global data usable between models
class BlockData {
public: 
    Segment segment; //for file segments type size info(if not -1)
    int y; // Last bit, 0 or 1, set by encoder
    int c0; // Last 0-7 bits of the partial byte with a leading 1 bit (1-255)
    U32 c4; // Last 4 whole bytes, packed.  Last byte is bits 0-7.
    int bpos; // bits in c0 (0 to 7)
    Buf buf;  // Rotating input queue set by Predictor
    int blpos; // Relative position in block
    int rm1;
    Filetype filetype;
    U32 b2,b3,w4;
    U32 w5,f4,tt;
    int col;
    U32 x4,s4;
    int finfo;
    U32 fails, failz, failcount,x5;
    U32 frstchar,spafdo,spaces,spacecount, words,wordcount,wordlen,wordlen1;
    U32 clevel; //level in predictor
BlockData():y(0), c0(1), c4(0),bpos(0),blpos(0),rm1(1),filetype(DEFAULT),
    b2(0),b3(0),w4(0), w5(0),f4(0),tt(0),col(0),x4(0),s4(0),finfo(0),fails(0),failz(0),
    failcount(0),x5(0), frstchar(0),spafdo(0),spaces(0),spacecount(0), words(0),wordcount(0),wordlen(0),wordlen1(0){
        clevel=(U32)level;
        // Set globals according to option
        assert(level>=0 && level<=15);
        if (level>=9) buf.setsize(0x10000000); //limit 256mb
        else buf.setsize(MEM()*8);
        #ifndef NDEBUG 
        printf("\n Buf size %d bytes\n", buf.poswr);
        #endif
        if (modeFast) clevel=0;
    }
    ~BlockData(){ }
};
///////////////////////////// ilog //////////////////////////////

// ilog(x) = round(log2(x) * 16), 0 <= x < 64K
class Ilog {
  Array<U8> t;
public:
  int operator()(U16 x) const {return t[x];}
  Ilog();
} ilog;

// Compute lookup table by numerical integration of 1/x
Ilog::Ilog(): t(65536) {
  U32 x=14155776;
  for (int i=2; i<65536; ++i) {
    x+=774541002/(i*2-1);  // numerator is 2^29/ln 2
    t[i]=x>>24;
  }
}

// llog(x) accepts 32 bits
inline int llog(U32 x) {
  if (x>=0x1000000)
    return 256+ilog(x>>16);
  else if (x>=0x10000)
    return 128+ilog(x>>8);
  else
    return ilog(x);
}

///////////////////////// state table ////////////////////////

// State table:
//   nex(state, 0) = next state if bit y is 0, 0 <= state < 256
//   nex(state, 1) = next state if bit y is 1
//   nex(state, 2) = number of zeros in bit history represented by state
//   nex(state, 3) = number of ones represented
//
// States represent a bit history within some context.
// State 0 is the starting state (no bits seen).
// States 1-30 represent all possible sequences of 1-4 bits.
// States 31-252 represent a pair of counts, (n0,n1), the number
//   of 0 and 1 bits respectively.  If n0+n1 < 16 then there are
//   two states for each pair, depending on if a 0 or 1 was the last
//   bit seen.
// If n0 and n1 are too large, then there is no state to represent this
// pair, so another state with about the same ratio of n0/n1 is substituted.
// Also, when a bit is observed and the count of the opposite bit is large,
// then part of this count is discarded to favor newer data over old.

#if 1 // change to #if 0 to generate this table at run time (4% slower)
static const U8 State_table[256][4]={
  {  1,  2, 0, 0},{  3,  5, 1, 0},{  4,  6, 0, 1},{  7, 10, 2, 0}, // 0-3
  {  8, 12, 1, 1},{  9, 13, 1, 1},{ 11, 14, 0, 2},{ 15, 19, 3, 0}, // 4-7
  { 16, 23, 2, 1},{ 17, 24, 2, 1},{ 18, 25, 2, 1},{ 20, 27, 1, 2}, // 8-11
  { 21, 28, 1, 2},{ 22, 29, 1, 2},{ 26, 30, 0, 3},{ 31, 33, 4, 0}, // 12-15
  { 32, 35, 3, 1},{ 32, 35, 3, 1},{ 32, 35, 3, 1},{ 32, 35, 3, 1}, // 16-19
  { 34, 37, 2, 2},{ 34, 37, 2, 2},{ 34, 37, 2, 2},{ 34, 37, 2, 2}, // 20-23
  { 34, 37, 2, 2},{ 34, 37, 2, 2},{ 36, 39, 1, 3},{ 36, 39, 1, 3}, // 24-27
  { 36, 39, 1, 3},{ 36, 39, 1, 3},{ 38, 40, 0, 4},{ 41, 43, 5, 0}, // 28-31
  { 42, 45, 4, 1},{ 42, 45, 4, 1},{ 44, 47, 3, 2},{ 44, 47, 3, 2}, // 32-35
  { 46, 49, 2, 3},{ 46, 49, 2, 3},{ 48, 51, 1, 4},{ 48, 51, 1, 4}, // 36-39
  { 50, 52, 0, 5},{ 53, 43, 6, 0},{ 54, 57, 5, 1},{ 54, 57, 5, 1}, // 40-43
  { 56, 59, 4, 2},{ 56, 59, 4, 2},{ 58, 61, 3, 3},{ 58, 61, 3, 3}, // 44-47
  { 60, 63, 2, 4},{ 60, 63, 2, 4},{ 62, 65, 1, 5},{ 62, 65, 1, 5}, // 48-51
  { 50, 66, 0, 6},{ 67, 55, 7, 0},{ 68, 57, 6, 1},{ 68, 57, 6, 1}, // 52-55
  { 70, 73, 5, 2},{ 70, 73, 5, 2},{ 72, 75, 4, 3},{ 72, 75, 4, 3}, // 56-59
  { 74, 77, 3, 4},{ 74, 77, 3, 4},{ 76, 79, 2, 5},{ 76, 79, 2, 5}, // 60-63
  { 62, 81, 1, 6},{ 62, 81, 1, 6},{ 64, 82, 0, 7},{ 83, 69, 8, 0}, // 64-67
  { 84, 71, 7, 1},{ 84, 71, 7, 1},{ 86, 73, 6, 2},{ 86, 73, 6, 2}, // 68-71
  { 44, 59, 5, 3},{ 44, 59, 5, 3},{ 58, 61, 4, 4},{ 58, 61, 4, 4}, // 72-75
  { 60, 49, 3, 5},{ 60, 49, 3, 5},{ 76, 89, 2, 6},{ 76, 89, 2, 6}, // 76-79
  { 78, 91, 1, 7},{ 78, 91, 1, 7},{ 80, 92, 0, 8},{ 93, 69, 9, 0}, // 80-83
  { 94, 87, 8, 1},{ 94, 87, 8, 1},{ 96, 45, 7, 2},{ 96, 45, 7, 2}, // 84-87
  { 48, 99, 2, 7},{ 48, 99, 2, 7},{ 88,101, 1, 8},{ 88,101, 1, 8}, // 88-91
  { 80,102, 0, 9},{103, 69,10, 0},{104, 87, 9, 1},{104, 87, 9, 1}, // 92-95
  {106, 57, 8, 2},{106, 57, 8, 2},{ 62,109, 2, 8},{ 62,109, 2, 8}, // 96-99
  { 88,111, 1, 9},{ 88,111, 1, 9},{ 80,112, 0,10},{113, 85,11, 0}, // 100-103
  {114, 87,10, 1},{114, 87,10, 1},{116, 57, 9, 2},{116, 57, 9, 2}, // 104-107
  { 62,119, 2, 9},{ 62,119, 2, 9},{ 88,121, 1,10},{ 88,121, 1,10}, // 108-111
  { 90,122, 0,11},{123, 85,12, 0},{124, 97,11, 1},{124, 97,11, 1}, // 112-115
  {126, 57,10, 2},{126, 57,10, 2},{ 62,129, 2,10},{ 62,129, 2,10}, // 116-119
  { 98,131, 1,11},{ 98,131, 1,11},{ 90,132, 0,12},{133, 85,13, 0}, // 120-123
  {134, 97,12, 1},{134, 97,12, 1},{136, 57,11, 2},{136, 57,11, 2}, // 124-127
  { 62,139, 2,11},{ 62,139, 2,11},{ 98,141, 1,12},{ 98,141, 1,12}, // 128-131
  { 90,142, 0,13},{143, 95,14, 0},{144, 97,13, 1},{144, 97,13, 1}, // 132-135
  { 68, 57,12, 2},{ 68, 57,12, 2},{ 62, 81, 2,12},{ 62, 81, 2,12}, // 136-139
  { 98,147, 1,13},{ 98,147, 1,13},{100,148, 0,14},{149, 95,15, 0}, // 140-143
  {150,107,14, 1},{150,107,14, 1},{108,151, 1,14},{108,151, 1,14}, // 144-147
  {100,152, 0,15},{153, 95,16, 0},{154,107,15, 1},{108,155, 1,15}, // 148-151
  {100,156, 0,16},{157, 95,17, 0},{158,107,16, 1},{108,159, 1,16}, // 152-155
  {100,160, 0,17},{161,105,18, 0},{162,107,17, 1},{108,163, 1,17}, // 156-159
  {110,164, 0,18},{165,105,19, 0},{166,117,18, 1},{118,167, 1,18}, // 160-163
  {110,168, 0,19},{169,105,20, 0},{170,117,19, 1},{118,171, 1,19}, // 164-167
  {110,172, 0,20},{173,105,21, 0},{174,117,20, 1},{118,175, 1,20}, // 168-171
  {110,176, 0,21},{177,105,22, 0},{178,117,21, 1},{118,179, 1,21}, // 172-175
  {110,180, 0,22},{181,115,23, 0},{182,117,22, 1},{118,183, 1,22}, // 176-179
  {120,184, 0,23},{185,115,24, 0},{186,127,23, 1},{128,187, 1,23}, // 180-183
  {120,188, 0,24},{189,115,25, 0},{190,127,24, 1},{128,191, 1,24}, // 184-187
  {120,192, 0,25},{193,115,26, 0},{194,127,25, 1},{128,195, 1,25}, // 188-191
  {120,196, 0,26},{197,115,27, 0},{198,127,26, 1},{128,199, 1,26}, // 192-195
  {120,200, 0,27},{201,115,28, 0},{202,127,27, 1},{128,203, 1,27}, // 196-199
  {120,204, 0,28},{205,115,29, 0},{206,127,28, 1},{128,207, 1,28}, // 200-203
  {120,208, 0,29},{209,125,30, 0},{210,127,29, 1},{128,211, 1,29}, // 204-207
  {130,212, 0,30},{213,125,31, 0},{214,137,30, 1},{138,215, 1,30}, // 208-211
  {130,216, 0,31},{217,125,32, 0},{218,137,31, 1},{138,219, 1,31}, // 212-215
  {130,220, 0,32},{221,125,33, 0},{222,137,32, 1},{138,223, 1,32}, // 216-219
  {130,224, 0,33},{225,125,34, 0},{226,137,33, 1},{138,227, 1,33}, // 220-223
  {130,228, 0,34},{229,125,35, 0},{230,137,34, 1},{138,231, 1,34}, // 224-227
  {130,232, 0,35},{233,125,36, 0},{234,137,35, 1},{138,235, 1,35}, // 228-231
  {130,236, 0,36},{237,125,37, 0},{238,137,36, 1},{138,239, 1,36}, // 232-235
  {130,240, 0,37},{241,125,38, 0},{242,137,37, 1},{138,243, 1,37}, // 236-239
  {130,244, 0,38},{245,135,39, 0},{246,137,38, 1},{138,247, 1,38}, // 240-243
  {140,248, 0,39},{249,135,40, 0},{250, 69,39, 1},{ 80,251, 1,39}, // 244-247
  {140,252, 0,40},{249,135,41, 0},{250, 69,40, 1},{ 80,251, 1,40}, // 248-251
  {140,252, 0,41}};  // 252, 253-255 are reserved

#define nex(state,sel) State_table[state][sel]

// The code used to generate the above table at run time (4% slower).
// To print the table, uncomment the 4 lines of print statements below.
// In this code x,y = n0,n1 is the number of 0,1 bits represented by a state.
#else

class StateTable {
  Array<U8> ns;  // state*4 -> next state if 0, if 1, n0, n1
  enum {B=5, N=64}; // sizes of b, t
  static const int b[B];  // x -> max y, y -> max x
  static U8 t[N][N][2];  // x,y -> state number, number of states
  int num_states(int x, int y);  // compute t[x][y][1]
  void discount(int& x);  // set new value of x after 1 or y after 0
  void next_state(int& x, int& y, int b);  // new (x,y) after bit b
public:
  int operator()(int state, int sel) {return ns[state*4+sel];}
  StateTable();
} nex;

const int StateTable::b[B]={42,41,13,6,5};  // x -> max y, y -> max x
U8 StateTable::t[N][N][2];

int StateTable::num_states(int x, int y) {
  if (x<y) return num_states(y, x);
  if (x<0 || y<0 || x>=N || y>=N || y>=B || x>=b[y]) return 0;

  // States 0-30 are a history of the last 0-4 bits
  if (x+y<=4) {  // x+y choose x = (x+y)!/x!y!
    int r=1;
    for (int i=x+1; i<=x+y; ++i) r*=i;
    for (int i=2; i<=y; ++i) r/=i;
    return r;
  }

  // States 31-255 represent a 0,1 count and possibly the last bit
  // if the state is reachable by either a 0 or 1.
  else
    return 1+(y>0 && x+y<16);
}

// New value of count x if the opposite bit is observed
void StateTable::discount(int& x) {
  if (x>2) x=ilog(x)/6-1;
}

// compute next x,y (0 to N) given input b (0 or 1)
void StateTable::next_state(int& x, int& y, int b) {
  if (x<y)
    next_state(y, x, 1-b);
  else {
    if (b) {
      ++y;
      discount(x);
    }
    else {
      ++x;
      discount(y);
    }
    while (!t[x][y][1]) {
      if (y<2) --x;
      else {
        x=(x*(y-1)+(y/2))/y;
        --y;
      }
    }
  }
}

// Initialize next state table ns[state*4] -> next if 0, next if 1, x, y
StateTable::StateTable(): ns(1024) {

  // Assign states
  int state=0;
  for (int i=0; i<256; ++i) {
    for (int y=0; y<=i; ++y) {
      int x=i-y;
      int n=num_states(x, y);
      if (n) {
        t[x][y][0]=state;
        t[x][y][1]=n;
        state+=n;
      }
    }
  }

  // Print/generate next state table
  state=0;
  for (int i=0; i<N; ++i) {
    for (int y=0; y<=i; ++y) {
      int x=i-y;
      for (int k=0; k<t[x][y][1]; ++k) {
        int x0=x, y0=y, x1=x, y1=y;  // next x,y for input 0,1
        int ns0=0, ns1=0;
        if (state<15) {
          ++x0;
          ++y1;
          ns0=t[x0][y0][0]+state-t[x][y][0];
          ns1=t[x1][y1][0]+state-t[x][y][0];
          if (x>0) ns1+=t[x-1][y+1][1];
          ns[state*4]=ns0;
          ns[state*4+1]=ns1;
          ns[state*4+2]=x;
          ns[state*4+3]=y;
        }
        else if (t[x][y][1]) {
          next_state(x0, y0, 0);
          next_state(x1, y1, 1);
          ns[state*4]=ns0=t[x0][y0][0];
          ns[state*4+1]=ns1=t[x1][y1][0]+(t[x1][y1][1]>1);
          ns[state*4+2]=x;
          ns[state*4+3]=y;
        }
          // uncomment to print table above
//        printf("{%3d,%3d,%2d,%2d},", ns[state*4], ns[state*4+1],
//          ns[state*4+2], ns[state*4+3]);
//        if (state%4==3) printf(" // %d-%d\n  ", state-3, state);
        assert(state>=0 && state<256);
        assert(t[x][y][1]>0);
        assert(t[x][y][0]<=state);
        assert(t[x][y][0]+t[x][y][1]>state);
        assert(t[x][y][1]<=6);
        assert(t[x0][y0][1]>0);
        assert(t[x1][y1][1]>0);
        assert(ns0-t[x0][y0][0]<t[x0][y0][1]);
        assert(ns0-t[x0][y0][0]>=0);
        assert(ns1-t[x1][y1][0]<t[x1][y1][1]);
        assert(ns1-t[x1][y1][0]>=0);
        ++state;
      }
    }
  }
//  printf("%d states\n", state); exit(0);  // uncomment to print table above
}

#endif

///////////////////////////// Squash //////////////////////////////

// return p = 1/(1 + exp(-d)), d scaled by 8 bits, p scaled by 12 bits
class Squash {
  Array<U16> t;
public:
  Squash();
  int operator()(int p) const {
    if (p>2047) return 4095;
  if (p<-2047) return 0;
    return t[p+2048];
  }
} squash;

Squash::Squash(): t(4096) {
int ts[33]={1,2,3,6,10,16,27,45,73,120,194,310,488,747,1101,
    1546,2047,2549,2994,3348,3607,3785,3901,3975,4022,
    4050,4068,4079,4085,4089,4092,4093,4094};
  int w,d;
  for (int i=-2047; i<=2047; ++i){
    w=i&127;
  d=(i>>7)+16;
  t[i+2048]=(ts[d]*(128-w)+ts[(d+1)]*w+64) >> 7;
    }
}
//////////////////////////// Stretch ///////////////////////////////

// Inverse of squash. d = ln(p/(1-p)), d scaled by 8 bits, p by 12 bits.
// d has range -2047 to 2047 representing -8 to 8.  p has range 0 to 4095.

class Stretch {
  Array<short> t;
public:
  Stretch();
  int operator()(int p) const {
    assert(p>=0 && p<4096);
    return t[p];
  }
} stretch;

Stretch::Stretch(): t(4096) {
  int pi=0;
  for (int x=-2047; x<=2047; ++x) {  // invert squash()
    int i=squash(x);
    for (int j=pi; j<=i; ++j)
      t[j]=x;
    pi=i+1;
  }
  t[4095]=2047;
}

//////////////////////////// Mixer /////////////////////////////

// Mixer m(N, M, S=1, w=0) combines models using M neural networks with
//   N inputs each, of which up to S may be selected.  If S > 1 then
//   the outputs of these neural networks are combined using another
//   neural network (with parameters S, 1, 1).  If S = 1 then the
//   output is direct.  The weights are initially w (+-32K).
//   It is used as follows:
// m.update() trains the network where the expected output is the
//   last bit (in the global variable y).
// m.add(stretch(p)) inputs prediction from one of N models.  The
//   prediction should be positive to predict a 1 bit, negative for 0,
//   nominally +-256 to +-2K.  The maximum allowed value is +-32K but
//   using such large values may cause overflow if N is large.
// m.set(cxt, range) selects cxt as one of 'range' neural networks to
//   use.  0 <= cxt < range.  Should be called up to S times such
//   that the total of the ranges is <= M.
// m.p() returns the output prediction that the next bit is 1 as a
//   12 bit number (0 to 4095).

#if !defined(__GNUC__)

#if (2 == _M_IX86_FP) // 2 if /arch:SSE2 was used.
# define __SSE2__
#elif (1 == _M_IX86_FP) // 1 if /arch:SSE was used.
# define __SSE__
#endif

#endif /* __GNUC__ */

#if defined(__AVX2__)
#include <smmintrin.h>
#define OPTIMIZE "AVX2-"
#elif defined(__SSE4_1__)  || defined(__SSSE3__)
#include<immintrin.h>

#elif defined(__SSE2__) 
#include <emmintrin.h>
#define OPTIMIZE "SSE2-"

#elif defined(__SSE__)
#include <xmmintrin.h>
#define OPTIMIZE "SSE-"
#endif
/**
 * Vector product a*b of n signed words, returning signed integer scaled down by 8 bits.
 * n is rounded up to a multiple of 8.
 */
//static int dot_product (const short* const t, const short* const w, int n);

/**
 * Train n neural network weights w[n] on inputs t[n] and err.
 * w[i] += ((t[i]*2*err)+(1<<16))>>17 bounded to +- 32K.
 * n is rounded up to a multiple of 8.
 */
//static void train (const short* const t, short* const w, int n, const int e);



class Mixer {
private: 
	const int N, M, S;   // max inputs, max contexts, max context sets
  Array<short, 32> tx; // N inputs from add()
  Array<short, 32> wx; // N*M weights
  Array<int> cxt;  // S contexts
  int ncxt;        // number of contexts (0 to S)
  int base;        // offset of next context
  int nx;          // Number of inputs in tx, 0 to N
  Array<int> pr;   // last result (scaled 12 bits)
  Mixer* mp;       // points to a Mixer to combine results
  
public:
BlockData& x;
  Mixer(int n, int m,BlockData& bd, int s=1, int w=0);
  
#if defined(__AVX2__)
 int dot_product (const short* const t, const short* const w, int n) {
  assert(n == ((n + 15) & -16));
  __m256i sum = _mm256_setzero_si256 ();
  while ((n -= 16) >= 0) { // Each loop sums 16 products
    __m256i tmp = _mm256_madd_epi16 (*(__m256i *) &t[n], *(__m256i *) &w[n]); // t[n] * w[n] + t[n+1] * w[n+1]
    tmp = _mm256_srai_epi32 (tmp, 8); //                                        (t[n] * w[n] + t[n+1] * w[n+1]) >> 8
    sum = _mm256_add_epi32 (sum, tmp); //                                sum += (t[n] * w[n] + t[n+1] * w[n+1]) >> 8
  } 
 // exctract high and low of sum and adds
  __m128i low = _mm_add_epi32 (_mm256_extracti128_si256(sum,0),_mm256_extracti128_si256(sum,1)); 
  low = _mm_add_epi32 (low, _mm_srli_si128 (low, 8)); // Add 16 sums together ...
  low = _mm_add_epi32 (low, _mm_srli_si128 (low, 4));
  return _mm_cvtsi128_si32 (low); //                     ...  and scale back to integer
}

 void train (const short* const t, short* const w, int n, const int e) {
  assert(n == ((n + 15) & -16));
  if (e) {
    const __m256i one = _mm256_set1_epi16 (1);
    const __m256i err = _mm256_set1_epi16 (short(e));
    while ((n -= 16) >= 0) { // Each iteration adjusts 16 weights
      __m256i tmp = _mm256_adds_epi16 (*(__m256i *) &t[n], *(__m256i *) &t[n]); // t[n] * 2
      tmp = _mm256_mulhi_epi16 (tmp, err); //                                     (t[n] * 2 * err) >> 16
      tmp = _mm256_adds_epi16 (tmp, one); //                                     ((t[n] * 2 * err) >> 16) + 1
      tmp = _mm256_srai_epi16 (tmp, 1); //                                      (((t[n] * 2 * err) >> 16) + 1) >> 1
      tmp = _mm256_adds_epi16 (tmp, *(__m256i *) &w[n]); //                    ((((t[n] * 2 * err) >> 16) + 1) >> 1) + w[n]
      *(__m256i *) &w[n] = tmp; //                                          save the new eight weights, bounded to +- 32K
    }
  }
}

#elif defined(__SSE2__) || defined(__SSSE3__)
 int dot_product (const short* const t, const short* const w, int n) {
  assert(n == ((n + 15) & -16));
  __m128i sum = _mm_setzero_si128 ();
  while ((n -= 8) >= 0) { // Each loop sums eight products
    __m128i tmp = _mm_madd_epi16 (*(__m128i *) &t[n], *(__m128i *) &w[n]); // t[n] * w[n] + t[n+1] * w[n+1]
    tmp = _mm_srai_epi32 (tmp, 8); //                                        (t[n] * w[n] + t[n+1] * w[n+1]) >> 8
    sum = _mm_add_epi32 (sum, tmp); //                                sum += (t[n] * w[n] + t[n+1] * w[n+1]) >> 8
  }
  sum = _mm_add_epi32 (sum, _mm_srli_si128 (sum, 8)); // Add eight sums together ...
  sum = _mm_add_epi32 (sum, _mm_srli_si128 (sum, 4));
  return _mm_cvtsi128_si32 (sum); //                     ...  and scale back to integer
}

 void train (const short* const t, short* const w, int n, const int e) {
  assert(n == ((n + 15) & -16));
  if (e) {
    const __m128i one = _mm_set1_epi16 (1);
    const __m128i err = _mm_set1_epi16 (short(e));
    while ((n -= 8) >= 0) { // Each iteration adjusts eight weights
      __m128i tmp = _mm_adds_epi16 (*(__m128i *) &t[n], *(__m128i *) &t[n]); // t[n] * 2
      tmp = _mm_mulhi_epi16 (tmp, err); //                                     (t[n] * 2 * err) >> 16
      tmp = _mm_adds_epi16 (tmp, one); //                                     ((t[n] * 2 * err) >> 16) + 1
      tmp = _mm_srai_epi16 (tmp, 1); //                                      (((t[n] * 2 * err) >> 16) + 1) >> 1
      tmp = _mm_adds_epi16 (tmp, *(__m128i *) &w[n]); //                    ((((t[n] * 2 * err) >> 16) + 1) >> 1) + w[n]
      *(__m128i *) &w[n] = tmp; //                                          save the new eight weights, bounded to +- 32K
    }
  }
}

#elif defined(__SSE__)
 int dot_product (const short* const t, const short* const w, int n) {
  assert(n == ((n + 15) & -16));
  __m64 sum = _mm_setzero_si64 ();
  while ((n -= 8) >= 0) { // Each loop sums eight products
    __m64 tmp = _mm_madd_pi16 (*(__m64 *) &t[n], *(__m64 *) &w[n]); //   t[n] * w[n] + t[n+1] * w[n+1]
    tmp = _mm_srai_pi32 (tmp, 8); //                                    (t[n] * w[n] + t[n+1] * w[n+1]) >> 8
    sum = _mm_add_pi32 (sum, tmp); //                            sum += (t[n] * w[n] + t[n+1] * w[n+1]) >> 8

    tmp = _mm_madd_pi16 (*(__m64 *) &t[n + 4], *(__m64 *) &w[n + 4]); // t[n+4] * w[n+4] + t[n+5] * w[n+5]
    tmp = _mm_srai_pi32 (tmp, 8); //                                    (t[n+4] * w[n+4] + t[n+5] * w[n+5]) >> 8
    sum = _mm_add_pi32 (sum, tmp); //                            sum += (t[n+4] * w[n+4] + t[n+5] * w[n+5]) >> 8
  }
  sum = _mm_add_pi32 (sum, _mm_srli_si64 (sum, 32)); // Add eight sums together ...
  const int retval = _mm_cvtsi64_si32 (sum); //                     ...  and scale back to integer
  _mm_empty(); // Empty the multimedia state
  return retval;
}

 void train (const short* const t, short* const w, int n, const int e) {
  assert(n == ((n + 15) & -16));
  if (e) {
    const __m64 one = _mm_set1_pi16 (1);
    const __m64 err = _mm_set1_pi16 (short(e));
    while ((n -= 8) >= 0) { // Each iteration adjusts eight weights
      __m64 tmp = _mm_adds_pi16 (*(__m64 *) &t[n], *(__m64 *) &t[n]); //   t[n] * 2
      tmp = _mm_mulhi_pi16 (tmp, err); //                                 (t[n] * 2 * err) >> 16
      tmp = _mm_adds_pi16 (tmp, one); //                                 ((t[n] * 2 * err) >> 16) + 1
      tmp = _mm_srai_pi16 (tmp, 1); //                                  (((t[n] * 2 * err) >> 16) + 1) >> 1
      tmp = _mm_adds_pi16 (tmp, *(__m64 *) &w[n]); //                  ((((t[n] * 2 * err) >> 16) + 1) >> 1) + w[n]
      *(__m64 *) &w[n] = tmp; //                                       save the new four weights, bounded to +- 32K

      tmp = _mm_adds_pi16 (*(__m64 *) &t[n + 4], *(__m64 *) &t[n + 4]); // t[n+4] * 2
      tmp = _mm_mulhi_pi16 (tmp, err); //                                 (t[n+4] * 2 * err) >> 16
      tmp = _mm_adds_pi16 (tmp, one); //                                 ((t[n+4] * 2 * err) >> 16) + 1
      tmp = _mm_srai_pi16 (tmp, 1); //                                  (((t[n+4] * 2 * err) >> 16) + 1) >> 1
      tmp = _mm_adds_pi16 (tmp, *(__m64 *) &w[n + 4]); //              ((((t[n+4] * 2 * err) >> 16) + 1) >> 1) + w[n]
      *(__m64 *) &w[n + 4] = tmp; //                                   save the new four weights, bounded to +- 32K
    }
    _mm_empty(); // Empty the multimedia state
  }
}
#else

// dot_product returns dot product t*w of n elements.  n is rounded
// up to a multiple of 8.  Result is scaled down by 8 bits.
int dot_product(short *t, short *w, int n) {
  int sum=0;
  n=(n+15)&-16;
  for (int i=0; i<n; i+=2)
    sum+=(t[i]*w[i]+t[i+1]*w[i+1]) >> 8;
  return sum;
}

// Train neural network weights w[n] given inputs t[n] and err.
// w[i] += t[i]*err, i=0..n-1.  t, w, err are signed 16 bits (+- 32K).
// err is scaled 16 bits (representing +- 1/2).  w[i] is clamped to +- 32K
// and rounded.  n is rounded up to a multiple of 8.

void train(short *t, short *w, int n, int err) {
  n=(n+15)&-16;
  for (int i=0; i<n; ++i) {
    int wt=w[i]+(((t[i]*err*2>>16)+1)>>1);
    if (wt<-32768) wt=-32768;
    if (wt>32767) wt=32767;
    w[i]=wt;
  }
}
#endif 
  // Adjust weights to minimize coding cost of last prediction
  void update() {
    for (int i=0; i<ncxt; ++i) {
      int err=((x.y<<12)-pr[i])*7;
      assert(err>=-32768 && err<32768);
      train(&tx[0], &wx[cxt[i]*N], nx, err);
    }
    nx=base=ncxt=0;
  }
  
  void update2() {
    if(x.filetype==DICTTXT || x.filetype==BIGTEXT) train(&tx[0], &wx[0], nx, ((x.y<<12)-base)*3/2), nx=base=ncxt=0;
    else update();
  }
  // Input x (call up to N times)
  void add(int x) {
    assert(nx<N);
    tx[nx++]=x;
  }

  // Set a context (call S times, sum of ranges <= M)
  void set(int cx, int range) {
    assert(range>=0);
    assert(ncxt<S);
    assert(cx>=0);
    assert(cx<range);
    assert(base+cx<M);

    cxt[ncxt++]=base+cx;
    base+=range;
  }
  // predict next bit
  int p() {
    while (nx&15) tx[nx++]=0;  // pad
    if (mp) {  // combine outputs
      mp->update2();
      for (int i=0; i<ncxt; ++i) {
         int dp=((dot_product(&tx[0], &wx[cxt[i]*N], nx)));//*7)>>8);
          if(x.filetype==DICTTXT || x.filetype==BIGTEXT) dp=(dp*9)>>9;  else dp=dp>>5;
          pr[i]=squash(dp);
          mp->add(dp);
      }
     if(!(x.filetype==DICTTXT || x.filetype==BIGTEXT))  mp->set(0, 1);
      return mp->p();
    }
    else {  // S=1 context
    if(!(x.filetype==DICTTXT || x.filetype==BIGTEXT))  return pr[0]=squash(dot_product(&tx[0], &wx[0], nx)>>8);
      int z=dot_product(&tx[0], &wx[0], nx);
    base=squash( (z*16) >>13);
    return squash(z>>9);
    }
  }
  ~Mixer();
};


Mixer::~Mixer() {
  delete mp;
}

Mixer::Mixer(int n, int m, BlockData& bd, int s, int w):
    N((n+15)&-16), M(m), S(s), tx(N), wx(N*M),
    cxt(S), ncxt(0), base(0), nx(0), pr(S), mp(0),x(bd) {
  assert(n>0 && N>0 && (N&15)==0 && M>0);
   int i;
  for (i=0; i<S; ++i)
    pr[i]=2048;
  for (i=0; i<N*M; ++i)
    wx[i]=w;
  if (S>1) mp=new Mixer(S, 1,x ,1);
}


//////////////////////////// APM1 //////////////////////////////

// APM1 maps a probability and a context into a new probability
// that bit y will next be 1.  After each guess it updates
// its state to improve future guesses.  Methods:
//
// APM1 a(N) creates with N contexts, uses 66*N bytes memory.
// a.p(pr, cx, rate=7) returned adjusted probability in context cx (0 to
//   N-1).  rate determines the learning rate (smaller = faster, default 7).
//   Probabilities are scaled 12 bits (0-4095).

class APM1 {
  int index;     // last p, context
  const int N;   // number of contexts
  Array<U16> t;  // [N][33]:  p, context -> p
  BlockData& x;
public:
  APM1(int n,BlockData& x);
  int p(int pr=2048, int cxt=0, int rate=7) {
    assert(pr>=0 && pr<4096 && cxt>=0 && cxt<N && rate>0 && rate<32);
    pr=stretch(pr);
    int g=(x.y<<16)+(x.y<<rate)-x.y-x.y;
    t[index] += (g-t[index]) >> rate;
    t[index+1] += (g-t[index+1]) >> rate;
    const int w=pr&127;  // interpolation weight (33 points)
    index=((pr+2048)>>7)+cxt*33;
    return (t[index]*(128-w)+t[index+1]*w) >> 11;
  }
};

// maps p, cxt -> p initially
APM1::APM1(int n,BlockData& bd): index(0), N(n), t(n*33),x(bd) {
  for (int i=0; i<N; ++i)
    for (int j=0; j<33; ++j)
      t[i*33+j] = i==0 ? squash((j-16)*128)*16 : t[j];
}

//////////////////////////// StateMap, APM //////////////////////////

// A StateMap maps a context to a probability.  Methods:
//
// Statemap sm(n) creates a StateMap with n contexts using 4*n bytes memory.
// sm.p(y, cx, limit) converts state cx (0..n-1) to a probability (0..4095).
//     that the next y=1, updating the previous prediction with y (0..1).
//     limit (1..1023, default 1023) is the maximum count for computing a
//     prediction.  Larger values are better for stationary sources.

static int dt[1024];  // i -> 16K/(i+3)

class StateMap {
protected:
  const int N;  // Number of contexts
  int cxt;      // Context of last prediction
  Array<U32> t;       // cxt -> prediction in high 22 bits, count in low 10 bits
  inline void update(int y, int limit) {
    assert(cxt>=0 && cxt<N);
    assert(y==0 || y==1);
    U32 *p=&t[cxt], p0=p[0];
    int n=p0&1023, pr=p0>>10;  // count, prediction
    //if (n<limit) ++p0;
    //else p0=(p0&0xfffffc00)|limit;
    p0+=(n<limit);
    p0+=(((y<<22)-pr)>>3)*dt[n]&0xfffffc00;
    p[0]=p0;
  }

public:
  StateMap(int n=256);

  // update bit y (0..1), predict next bit in context cx
  int p(int cx, int y,int limit=1023) {
    assert(cx>=0 && cx<N);
    assert(limit>0 && limit<1024);
    assert(y==0 || y==1);
    update(y,limit);
    return t[cxt=cx]>>20;
  }
};

StateMap::StateMap(int n): N(n), cxt(0), t(n) {
  for (int i=0; i<N; ++i)
    t[i]=1<<31;
}

// An APM maps a probability and a context to a new probability.  Methods:
//
// APM a(n) creates with n contexts using 96*n bytes memory.
// a.pp(y, pr, cx, limit) updates and returns a new probability (0..4095)
//     like with StateMap.  pr (0..4095) is considered part of the context.
//     The output is computed by interpolating pr into 24 ranges nonlinearly
//     with smaller ranges near the ends.  The initial output is pr.
//     y=(0..1) is the last bit.  cx=(0..n-1) is the other context.
//     limit=(0..1023) defaults to 255.

class APM: public StateMap {
public:
  APM(int n);
  int p(int pr, int cx, int y,int limit=255) {
   // assert(y>>1==0);
    assert(pr>=0 && pr<4096);
    assert(cx>=0 && cx<N/24);
    assert(y==0 || y==1);
    assert(limit>0 && limit<1024);
    update(y,limit);
    pr=(stretch(pr)+2048)*23;
    int wt=pr&0xfff;  // interpolation weight of next element
    cx=cx*24+(pr>>12);
    assert(cx>=0 && cx<N-1);
    cxt=cx+(wt>>11);
    pr=((t[cx]>>13)*(0x1000-wt)+(t[cx+1]>>13)*wt)>>19;
    return pr;
  }
};

APM::APM(int n): StateMap(n*24) {
  for (int i=0; i<N; ++i) {
    int p=((i%24*2+1)*4096)/48-2048;
    t[i]=(U32(squash(p))<<20)+6;
  }
}


//////////////////////////// hash //////////////////////////////

// Hash 2-5 ints.
inline U32 hash(U32 a, U32 b, U32 c=0xffffffff, U32 d=0xffffffff,
    U32 e=0xffffffff) {
  U32 h=a*200002979u+b*30005491u+c*50004239u+d*70004807u+e*110002499u;
  return h^h>>9^a>>2^b>>3^c>>4^d>>5^e>>6;
}

///////////////////////////// BH ////////////////////////////////

// A BH maps a 32 bit hash to an array of B bytes (checksum and B-2 values)
//
// BH bh(N); creates N element table with B bytes each.
//   N must be a power of 2.  The first byte of each element is
//   reserved for a checksum to detect collisions.  The remaining
//   B-1 bytes are values, prioritized by the first value.  This
//   byte is 0 to mark an unused element.
//
// bh[i] returns a pointer to the i'th element, such that
//   bh[i][0] is a checksum of i, bh[i][1] is the priority, and
//   bh[i][2..B-1] are other values (0-255).
//   The low lg(n) bits as an index into the table.
//   If a collision is detected, up to M nearby locations in the same
//   cache line are tested and the first matching checksum or
//   empty element is returned.
//   If no match or empty element is found, then the lowest priority
//   element is replaced.

// 2 byte checksum with LRU replacement (except last 2 by priority)
template <int B> class BH {
  enum {M=8};  // search limit
  Array<U8, 64> t; // elements
  //Array<U8> tmp;
  U8 tmp[B];
  U32 n; // size-1
public:
  BH(int i): t(i*B), n(i-1) {
    assert(B>=2 && i>0 && (i&(i-1))==0); // size a power of 2?
  }
  U8* operator[](U32 i);
};

template <int B>
inline  U8* BH<B>::operator[](U32 i) {
  U16 chk=(i>>16^i)&0xffff;
  i=i*M&n;
  U8 *p;
  U16 *cp;
  int j;
  for (j=0; j<M; ++j) {
    p=&t[(i+j)*B];
    cp=(U16*)p;
    if (p[2]==0) {*cp=chk;break;}
    if (*cp==chk) break;  // found
  }
  if (j==0) return p+1;  // front
  //static U8 tmp[B];  // element to move to front
  if (j==M) {
    --j;
    memset(&tmp, 0, B);
    memmove(&tmp, &chk, 2);
    if (M>2 && t[(i+j)*B+2]>t[(i+j-1)*B+2]) --j;
  }
  else memcpy(&tmp, cp, B);
  memmove(&t[(i+1)*B], &t[i*B], j*B);
  memcpy(&t[i*B], &tmp, B);
  return &t[i*B+1];
}

/////////////////////////// ContextMap /////////////////////////
//
// A ContextMap maps contexts to a bit histories and makes predictions
// to a Mixer.  Methods common to all classes:
//
// ContextMap cm(M, C); creates using about M bytes of memory (a power
//   of 2) for C contexts.
// cm.set(cx);  sets the next context to cx, called up to C times
//   cx is an arbitrary 32 bit value that identifies the context.
//   It should be called before predicting the first bit of each byte.
// cm.mix(m) updates Mixer m with the next prediction.  Returns 1
//   if context cx is found, else 0.  Then it extends all the contexts with
//   global bit y.  It should be called for every bit:
//
//     if (bpos==0)
//       for (int i=0; i<C; ++i) cm.set(cxt[i]);
//     cm.mix(m);
//
// The different types are as follows:
//
// - RunContextMap.  The bit history is a count of 0-255 consecutive
//     zeros or ones.  Uses 4 bytes per whole byte context.  C=1.
//     The context should be a hash.
// - SmallStationaryContextMap.  0 <= cx < M/512.
//     The state is a 16-bit probability that is adjusted after each
//     prediction.  C=1.
// - ContextMap.  For large contexts, C >= 1.  Context need not be hashed.



// A RunContextMap maps a context into the next byte and a repeat
// count up to M.  Size should be a power of 2.  Memory usage is 3M/4.
class RunContextMap {
  BH<4> t;
  U8* cp;
  BlockData& x;
public:
  RunContextMap(int m,BlockData& bd): t(m/4),x(bd) {cp=t[0]+1;}
  void set(U32 cx) {  // update count
    if (cp[0]==0 || cp[1]!=x.buf(1)) cp[0]=1, cp[1]=x.buf(1);
    else if (cp[0]<255) ++cp[0];
    cp=t[cx]+1;
  }
  int p() {  // predict next bit
    if ((cp[1]+256)>>(8-x.bpos)==x.c0)
      return ((cp[1]>>(7-x.bpos)&1)*2-1)*ilog(cp[0]+1)*8;
    else
      return 0;
  }
  int mix(Mixer& m) {  // return run length
    m.add(p());
    return cp[0]!=0;
  }
};

// Context is looked up directly.  m=size is power of 2 in bytes.
// Context should be < m/512.  High bits are discarded.
class SmallStationaryContextMap {
  Array<U16> t;
  int cxt;
  U16 *cp;
  //BlockData& x;
public:
  SmallStationaryContextMap(int m): t(m/2), cxt(0) {
    assert((m/2&m/2-1)==0); // power of 2?
    for (int i=0; i<t.size(); ++i)
      t[i]=32768;
    cp=&t[0];
  }
  void set(U32 cx) {
    cxt=cx*256&(t.size()-256);
  }
  void mix(Mixer& m, int rate=7) {
    *cp += ((m.x.y<<16)-(*cp)+(1<<(rate-1))) >> rate;
    cp=&t[cxt+m.x.c0];
     m.add(stretch((*cp)>>4));
  }
};

 
inline int mix3(Mixer& m, int s, StateMap& sm) {
  int p1=sm.p(s,m.x.y);
  //int n0=-!nex(s,2);
  //int n1=-!nex(s,3);
  int st=stretch(p1)>>2;
  m.add(st);
  p1>>=4;
  int p0=255-p1;
  m.add(p1-p0);
 /// m.add(st*(n1-n0));
 // m.add((p1&n0)-(p0&n1));
  //m.add((p1&n1)-(p0&n0));
  return s>0;
}

// Medium ContextMap
class MContextMap {
  Array<U8, 64> t;
  Array<U8*>  cp;
  Array<U32> cxt;
  int cn;
  StateMap *sm;
public:
  MContextMap(size_t m,int c):t(m), cp(c), cxt(c),cn(0) {
          sm=new StateMap[c];
    for (int i = 0; i < c; ++i) { cp[i] = 0; }
  }
  ~MContextMap(){ delete[] sm;}
  void set(U32 cx) {
    cxt[cn++] = cx * 16;
  }
  int mix(Mixer&m) {
      int result=0;
    for (int i = 0; i < cn; ++i) {
      if (cp[i]) {
      *cp[i] = nex(*cp[i], m.x.y);
      }
    }
    const int c0b = m.x.c0 < 16 ? m.x.c0 : (16 * (1 + ((m.x.c0 >> (m.x.bpos - 4)) & 15))) +
              ((m.x.c0 & ((1 << (m.x.bpos - 4)) - 1)) | (1 << (m.x.bpos - 4)));
    for (int i = 0; i < cn; ++i) {
      cp[i] = &t[(cxt[i] + c0b) & (t.size() - 1)]; // Update bit context pointers
    }
    // Predict from bit context
    for (int i = 0; i < cn; ++i) {
       result+=mix3(m, *cp[i]?*cp[i]:0, sm[i]);
     
    }
  
    if (m.x.bpos == 7) cn = 0;
    return result;
  }
};


// Context map for large contexts.  Most modeling uses this type of context
// map.  It includes a built in RunContextMap to predict the last byte seen
// in the same context, and also bit-level contexts that map to a bit
// history state.
//
// Bit histories are stored in a hash table.  The table is organized into
// 64-byte buckets alinged on cache page boundaries.  Each bucket contains
// a hash chain of 7 elements, plus a 2 element queue (packed into 1 byte)
// of the last 2 elements accessed for LRU replacement.  Each element has
// a 2 byte checksum for detecting collisions, and an array of 7 bit history
// states indexed by the last 0 to 2 bits of context.  The buckets are indexed
// by a context ending after 0, 2, or 5 bits of the current byte.  Thus, each
// byte modeled results in 3 main memory accesses per context, with all other
// accesses to cache.
//
// On bits 0, 2 and 5, the context is updated and a new bucket is selected.
// The most recently accessed element is tried first, by comparing the
// 16 bit checksum, then the 7 elements are searched linearly.  If no match
// is found, then the element with the lowest priority among the 5 elements
// not in the LRU queue is replaced.  After a replacement, the queue is
// emptied (so that consecutive misses favor a LFU replacement policy).
// In all cases, the found/replaced element is put in the front of the queue.
//
// The priority is the state number of the first element (the one with 0
// additional bits of context).  The states are sorted by increasing n0+n1
// (number of bits seen), implementing a LFU replacement policy.
//
// When the context ends on a byte boundary (bit 0), only 3 of the 7 bit
// history states are used.  The remaining 4 bytes implement a run model
// as follows: <count:7,d:1> <b1> <unused> <unused> where <b1> is the last byte
// seen, possibly repeated.  <count:7,d:1> is a 7 bit count and a 1 bit
// flag (represented by count * 2 + d).  If d=0 then <count> = 1..127 is the
// number of repeats of <b1> and no other bytes have been seen.  If d is 1 then
// other byte values have been seen in this context prior to the last <count>
// copies of <b1>.
//
// As an optimization, the last two hash elements of each byte (representing
// contexts with 2-7 bits) are not updated until a context is seen for
// a second time.  This is indicated by <count,d> = <1,0> (2).  After update,
// <count,d> is updated to <2,0> or <1,1> (4 or 3).

inline U64 CMlimit(U64 size){
    if (size>0xFFFFFFBF) return 0xFFFFFFBF; //if (size>0x7FFFFFFF) return 0x7FFFFFFF; //limit to 2GB power of 2
    return (size);
}

class ContextMap {
  const int C;  // max number of contexts
  class E {  // hash element, 64 bytes
    U16 chk[7];  // byte context checksums
    U8 last;     // last 2 accesses (0-6) in low, high nibble
  public:
    U8 bh[7][7]; // byte context, 3-bit context -> bit history state
      // bh[][0] = 1st bit, bh[][1,2] = 2nd bit, bh[][3..6] = 3rd bit
      // bh[][0] is also a replacement priority, 0 = empty
    U8* get(U16 chk);  // Find element (0-6) matching checksum.
      // If not found, insert or replace lowest priority (not last).
  };
  Array<E, 64> t;  // bit histories for bits 0-1, 2-4, 5-7
    // For 0-1, also contains a run count in bh[][4] and value in bh[][5]
    // and pending update count in bh[7]
  Array<U8*> cp;   // C pointers to current bit history
  Array<U8*> cp0;  // First element of 7 element array containing cp[i]
  Array<U32> cxt;  // C whole byte contexts (hashes)
  Array<U8*> runp; // C [0..3] = count, value, unused, unused
  StateMap *sm;    // C maps of state -> p
  int cn;          // Next context to set by set()
  //void update(U32 cx, int c);  // train model that context cx predicts c
  Random rnd;
  int mix1(Mixer& m, int cc, int bp, int c1, int y1);
    // mix() with global context passed as arguments to improve speed.
public:
  ContextMap(U32 m, int c=1);  // m = memory in bytes, a power of 2, C = c
  ~ContextMap();
  void set(U32 cx, int next=-1);   // set next whole byte context to cx
    // if next is 0 then set order does not matter
  int mix(Mixer& m) {return mix1(m, m.x.c0, m.x.bpos, m.x.buf(1), m.x.y);}
};

#if defined(SIMD_GET_SSE) 
typedef __m128i XMM;  
#define xmmbshl(x,n)  _mm_slli_si128(x,n) // xm <<= 8*n  -- BYTE shift left
#define xmmbshr(x,n)  _mm_srli_si128(x,n) // xm >>= 8*n  -- BYTE shift right
#define xmmshl64(x,n) _mm_slli_epi64(x,n) // xm.hi <<= n, xm.lo <<= n
#define xmmshr64(x,n) _mm_srli_epi64(x,n) // xm.hi >>= n, xm.lo >>= n
#define xmmand(a,b)   _mm_and_si128(a,b)
#define xmmor(a,b)    _mm_or_si128(a,b)
#define xmmxor(a,b)   _mm_xor_si128(a,b)
#define xmmzero       _mm_setzero_si128()
#ifdef _MSC_VER
#include <intrin.h>
U32 __inline clz(U32 value){ //asume newer version of vc
    unsigned long leading_zero = 0;
    if (_BitScanReverse( &leading_zero, value)) return 31-leading_zero;
    else return 32;
}
  U32 __inline ctz(U32 x ){
    unsigned long leading_zero = 0;
   if (_BitScanForward(&leading_zero, x )) {
       return  leading_zero;
    }
    else{
         return 0; // Same remarks as above
    }
}

#elif ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
U32 __inline clz(U32 value){ 
    return __builtin_clz(value);
}
U32 __inline ctz(U32 value){ 
    return __builtin_ctz(value);
}
#endif
#endif

// Find or create hash element matching checksum ch
inline U8* ContextMap::E::get(U16 ch) {
    
  if (chk[last&15]==ch) return &bh[last&15][0];
  int b=0xffff, bi=0;
#if defined(SIMD_GET_SSE)   
  const XMM xmmch=_mm_set1_epi16 (short(ch)); //fill 8 ch values
//load 8 values, discard last one as only 7 are needed.
//reverse order and compare 7 chk values to ch
//get mask is set get first index and return value  
  XMM tmp=_mm_load_si128 ((XMM *) &chk[0]); //load 8 values (8th will be discarded)
#if defined(__SSE2__) 
  tmp=_mm_shufflelo_epi16(tmp,0x1B); //swap order for mask  (0,1,2,3)
  tmp=_mm_shufflehi_epi16(tmp,0x1B);                      //(0,1,2,3)
  tmp=_mm_shuffle_epi32(tmp,0x4E);                        //(1,0,3,2)   
#elif defined(__SSSE3__)
#include <immintrin.h>
  const XMM vm=_mm_setr_epi8(14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1);// initialise vector mask 
  tmp=_mm_shuffle_epi8(tmp,vm);        
#endif
  tmp=_mm_cmpeq_epi16 (tmp,xmmch); //compare ch values
  tmp=_mm_packs_epi16(tmp,xmmzero); //pack result
  U32 t=(_mm_movemask_epi8(tmp))>>1; //get mask of comparsion, bit is set if eq, discard 8th bit
  U32 a;    //index into bh or 7 if not found
  if(t){
      a=(clz(t)-1)&7;
      return last=last<<4|a, (U8*)&bh[a][0];
  }

 XMM   lastl=_mm_set1_epi8((last&15));
 XMM   lasth=_mm_set1_epi8((last>>4));
 XMM   one1  =_mm_set1_epi8(1);
 XMM   vm=_mm_setr_epi8(0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7);

 XMM   lastx=_mm_unpacklo_epi64(lastl,lasth); //last&15 last>>4
 XMM   eq0  =_mm_cmpeq_epi8 (lastx,vm); //compare   values

 eq0=_mm_or_si128(eq0,_mm_srli_si128 (eq0, 8));    //or low values with high

 lastx = _mm_and_si128(one1, eq0);                //set to 1 if eq
 XMM sum1 = _mm_sad_epu8(lastx,xmmzero);        //cout values, abs(a0 - b0) + abs(a1 - b1) .... up to b8
 const U32 pcount=_mm_cvtsi128_si32(sum1); //population count
/*for (int i=0; i<7; ++i) {
   bh[i][0]=i+1;
    
  }*/
 U32 t0=(~_mm_movemask_epi8(eq0));
for (int i=pcount; i<7; ++i) {
    int bitt =ctz(t0);     //get index 
//#if ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))    
//asm("btr %1,%0" : "+r"(t0) : "r"(bitt)); // clear bit set and test again https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47769
//#else
    t0 &= ~(1 << bitt); // clear bit set and test again
//#endif 
   int pri=bh[bitt][0];
    if (pri<b  ) b=pri, bi=bitt;


  }  
  /*
  //uncomment above SIMD version and comment out code below to use full SIMD (SSE2) version
  for (int i=0; i<7; ++i) {
    int pri=bh[i][0];
    if (pri<b && (last&15)!=i && (last>>4)!=i) b=pri, bi=i;
  }*/
  return last=0xf0|bi, chk[bi]=ch, (U8*)memset(&bh[bi][0], 0, 7);
#else
  for (int i=0; i<7; ++i) {
    if (chk[i]==ch) return last=last<<4|i, (U8*)&bh[i][0];
    int pri=bh[i][0];
    if (pri<b && (last&15)!=i && last>>4!=i) b=pri, bi=i;
  }
  return last=0xf0|bi, chk[bi]=ch, (U8*)memset(&bh[bi][0], 0, 7);
#endif
}

// Construct using m bytes of memory for c contexts
ContextMap::ContextMap(U32 m, int c): C(c), t(m>>6), cp(c), cp0(c),
    cxt(c), runp(c), cn(0) {
  assert(m>=64 && (m&m-1)==0);  // power of 2?
  assert(sizeof(E)==64);
  sm=new StateMap[C];
  for (int i=0; i<C; ++i) {
    cp0[i]=cp[i]=&t[0].bh[0][0];
    runp[i]=cp[i]+3;
  }
}

ContextMap::~ContextMap() {
  delete[] sm;
}

// Set the i'th context to cx
inline void ContextMap::set(U32 cx, int next) {
  int i=cn++;
  i&=next;
  assert(i>=0 && i<C);

  cx=cx*987654323+i;  // permute (don't hash) cx to spread the distribution
  cx=cx<<16|cx>>16;
  cxt[i]=cx*123456791+i;
}
// Predict to mixer m from bit history state s, using sm to map s to
// a probability.

inline int mix2(Mixer& m, int s, StateMap& sm) {
  int p1=sm.p(s,m.x.y);
  int n0=-!nex(s,2);
  int n1=-!nex(s,3);
  int st=stretch(p1)>>2;
  m.add(st);
  p1>>=4;
  int p0=255-p1;
  if (m.x.rm1)m.add(p1-p0);
  m.add(st*(n1-n0));
  m.add((p1&n0)-(p0&n1));
  m.add((p1&n1)-(p0&n0));
  return s>0;
}
// Update the model with bit y1, and predict next bit to mixer m.
// Context: cc=c0, bp=bpos, c1=buf(1), y1=y.
int ContextMap::mix1(Mixer& m, int cc, int bp, int c1, int y1) {
  // Update model with y
  int result=0;
  for (int i=0; i<cn; ++i) {
    if (cp[i]) {
      assert(cp[i]>=&t[0].bh[0][0] && cp[i]<=&t[t.size()-1].bh[6][6]);
      assert(((long long)(cp[i])&63)>=15);
      int ns=nex(*cp[i], y1);
      if (ns>=204 && rnd() << ((452-ns)>>3)) ns-=4;  // probabilistic increment
      *cp[i]=ns;
    }


    // Update context pointers
    if (m.x.bpos>1 && runp[i][0]==0)
    {
     cp[i]=0;
    }
    else
    {
     switch(m.x.bpos)
     {
      case 1: case 3: case 6: cp[i]=cp0[i]+1+(cc&1); break;
      case 4: case 7: cp[i]=cp0[i]+3+(cc&3); break;
      case 2: case 5: cp0[i]=cp[i]=t[(cxt[i]+cc)&(t.size()-1)].get(cxt[i]>>16); break;
      default:
      {
       cp0[i]=cp[i]=t[(cxt[i]+cc)&(t.size()-1)].get(cxt[i]>>16);
       // Update pending bit histories for bits 2-7
       if (cp0[i][3]==2) {
         const int c=cp0[i][4]+256;
         U8 *p=t[(cxt[i]+(c>>6))&(t.size()-1)].get(cxt[i]>>16);
         p[0]=1+((c>>5)&1);
         p[1+((c>>5)&1)]=1+((c>>4)&1);
         p[3+((c>>4)&3)]=1+((c>>3)&1);
         p=t[(cxt[i]+(c>>3))&(t.size()-1)].get(cxt[i]>>16);
         p[0]=1+((c>>2)&1);
         p[1+((c>>2)&1)]=1+((c>>1)&1);
         p[3+((c>>1)&3)]=1+(c&1);
         cp0[i][6]=0;
       }
       // Update run count of previous context
       if (runp[i][0]==0)  // new context
         runp[i][0]=2, runp[i][1]=c1;
       else if (runp[i][1]!=c1)  // different byte in context
         runp[i][0]=1, runp[i][1]=c1;
       else if (runp[i][0]<254)  // same byte in context
         runp[i][0]+=2;
       else if (runp[i][0]==255)
         runp[i][0]=128;
       runp[i]=cp0[i]+3;
      } break;
     }
    }

    // predict from last byte in context
    if ((runp[i][1]+256)>>(8-bp)==cc) {
      int rc=runp[i][0];  // count*2, +1 if 2 different bytes seen
      int b=(runp[i][1]>>(7-bp)&1)*2-1;  // predicted bit + for 1, - for 0
      int c=ilog(rc+1)<<(2+(~rc&1));
      m.add(b*c);
    }
    else
      m.add(0);

    // predict from bit context
    if (cp[i]) {
      result+=mix2(m, *cp[i], sm[i]);
    } else {
      mix2(m, 0, sm[i]);
    }

  }
  if (bp==7) cn=0;
  return result;
}

//////////////////////////// Models //////////////////////////////

// All of the models below take a Mixer as a parameter and write
// predictions to it.

class Model {
public:
 virtual  int p(Mixer& m,int val1,int val2)=0;
};


//////////////////////////// matchModel ///////////////////////////

// matchModel() finds the longest matching context and returns its length
class matchModel1: public Model {
    BlockData& x;
    Buf& buf;
    const int MAXLEN;  // longest allowed match + 1
    Array<int> t;  // hash table of pointers to contexts
    int h;  // hash of last 7 bytes
    U32 ptr;  // points to next byte of match if any
    int len;  // length of match, or 0 if no match
    int result;

    SmallStationaryContextMap scm1;
public:
    matchModel1( BlockData& bd,U32 val=0): x(bd),buf(bd.buf),  MAXLEN(65534), 
    t(CMlimit(MEM())),h(0), ptr(0),len(0),result(0), scm1(0x20000) {
    }

    int p(Mixer& m,int val1=0,int val2=0)  {
        if (!x.bpos) {
            h=(h*997*8+buf(1)+1)&(t.size()-1);  // update context hash
            if (len) ++len, ++ptr;
            else {  // find match
                ptr=t[h];
                if (ptr && buf.pos-ptr<buf.size())
                while (buf(len+1)==buf[ptr-len-1] && len<MAXLEN) ++len;
            }
            t[h]=buf.pos;  // update hash table
            result=len;
            //    if (result>0 && !(result&0xfff)) printf("pos=%d len=%d ptr=%d\n", pos, len, ptr);
            scm1.set(buf.pos);
        }

        // predict
        if (len)  {
            if (buf(1)==buf[ptr-1] && x.c0==(buf[ptr]+256)>>(8-x.bpos)) {
                if (len>MAXLEN) len=MAXLEN;
                if (buf[ptr]>>(7-x.bpos)&1)  {
                    m.add(ilog(len)<<2);
                    m.add(min(len, 32)<<6);
                }
                else {
                    m.add(-(ilog(len)<<2));
                    m.add(-(min(len, 32)<<6));
                }
            }
            else {
                len=0;
                m.add(0);
                m.add(0);
            }
        }
        else {
            m.add(0);
            m.add(0);
        }

        scm1.mix(m);
        return result;
    }
    ~matchModel1(){ }
};
//////////////////////////// wordModel /////////////////////////


// Model English text (words and columns/end of line)
class wordModel1: public Model {
   BlockData& x;
   Buf& buf;
     U32 word0, word1, word2, word3, word4, word5;  // hashes
     U32 xword0,xword1,xword2,cword0,ccword;
     U32 number0, number1;  // hashes
     U32 text0;  // hash stream of letters
     ContextMap cm;
     int nl1, nl;  // previous, current newline position
     U32 mask;
     Array<int> wpos;  // last position of word
     int w;
public:
  wordModel1( BlockData& bd,U32 val=0): x(bd),buf(bd.buf),word0(0),word1(0),word2(0),
  word3(0),word4(0),word5(0),xword0(0),xword1(0),xword2(0),cword0(0),ccword(0),number0(0),
  number1(0),text0(0),cm(CMlimit(MEM()*32), 45),nl1(-3), nl(-2),mask(0),wpos(0x10000),w(0) {
   }

   int p(Mixer& m,int val1=0,int val2=0)  {
    // Update word hashes
    if (x.bpos==0) {
        int c=x.c4&255,f=0;
        if (x.spaces&0x80000000) --x.spacecount;
        if (x.words&0x80000000) --x.wordcount;
        x.spaces=x.spaces*2;
        x.words=x.words*2;

        if (c>='A' && c<='Z') c+='a'-'A';
        if ((c>='a' && c<='z') || c==1 || c==2 ||(c>=128 &&(x.b2!=3))) {
            ++x.words, ++x.wordcount;
            word0^=hash(word0, c,0);//word0=word0*263*32+c;
            text0=text0*997*16+c;
            x.wordlen++;
            x.wordlen=min(x.wordlen,45);
            f=0;
             w=word0&(wpos.size()-1);
        }
        else {
            if (word0) {
                word5=word4;//*23;
                word4=word3;//*19;
                word3=word2;//*17;
                word2=word1;//*13;
                word1=word0;//*11;
                x.wordlen1=x.wordlen;
                 wpos[w]=x.blpos;
                if (c==':'|| c=='=') cword0=word0;
                if (c==']'&& (x.frstchar!=':' || x.frstchar!='*')) xword0=word0;
                ccword=0;
                word0=x.wordlen=0;
                if((c=='.'||c=='!'||c=='?' ||c=='}' ||c==')') && buf(2)!=10 && x.filetype!=EXE) f=1; 
                
            }
            if ((x.c4&0xFFFF)==0x3D3D) xword1=word1,xword2=word2; // ==
            if ((x.c4&0xFFFF)==0x2727) xword1=word1,xword2=word2; // ''
            if (c==32 || c==10) { ++x.spaces, ++x.spacecount; if (c==10 ) nl1=nl, nl=buf.pos-1;}
            else if (c=='.' || c=='!' || c=='?' || c==',' || c==';' || c==':') x.spafdo=0,ccword=c;//*31; 
            else { ++x.spafdo; x.spafdo=min(63,x.spafdo); }
        }
        if (c>='0' && c<='9') {
            //number0=number0*263*32+c;
            number0^=hash(number0, c,1);
        }
        else if (number0) {
            number1=number0;//*11;
            number0=0,ccword=0;
        }
   
        x.col=min(255, buf.pos-nl);
        int above=buf[nl1+x.col]; // text column context
        if (x.col<=2) x.frstchar=(x.col==2?min(c,96):0);
        if (x.frstchar=='[' && c==32)    {if(buf(3)==']' || buf(4)==']' ) x.frstchar=96,xword0=0;}
    //cm.set(hash(532,spafdo, col));
//256+ hash 513+ none
        cm.set(hash(513,x.spafdo, x.spaces,ccword));
        cm.set(hash(514,x.frstchar, c));
        cm.set(hash(515,x.col, x.frstchar));
        cm.set(hash(516,x.spaces, (x.words&255)));
        
        cm.set(hash(256,number0, word2));
        cm.set(hash(257,number0, word1));
        cm.set(hash(258,number1, c,ccword));
        cm.set(hash(259,number0, number1));
        cm.set(hash(260,word0, number1));

       
//    cm.set(wordlen<<16|c);
        cm.set(hash(518,x.wordlen1,x.col));
        cm.set(hash(519,c,x.spacecount/2));
        U32 h=x.wordcount*64+x.spacecount;
        cm.set(hash(520,c,h,ccword));
         cm.set(hash(517,x.frstchar,h));
//    cm.set(h); // 
        cm.set(hash(521,h,x.spafdo));

        U32 d=x.c4&0xf0ff;
        cm.set(hash(522,d,x.frstchar,ccword));

        h=word0*271;
        //cm.set(hash(261,h, ccword));
        h=h+buf(1);
        cm.set(hash(262,h, 0));
        cm.set(hash(263,word0, 0));
        //cm.set(word1+(c==32));
        cm.set(hash(264,h, word1));
        cm.set(hash(265,word0, word1));
        cm.set(hash(266,h, word1,word2));
        //cm.set(hash(267,text0&0xffffff,0));
        cm.set(hash(268,text0&0xfffff, 0));
          cm.set(hash(269,word0, xword0));
          cm.set(hash(270,word0, xword1));
          cm.set(hash(271,word0, xword2));
          cm.set(hash(272,x.frstchar, xword2));
        
        cm.set(hash(273,word0, cword0));
        cm.set(hash(274,number0, cword0));
        cm.set(hash(275,h, word2));
        cm.set(hash(276,h, word3));
        cm.set(hash(277,h, word4));
        cm.set(hash(278,h, word5));
//        cm.set(buf(1)|buf(3)<<8|buf(5)<<16);
//        cm.set(buf(2)|buf(4)<<8|buf(6)<<16);
        cm.set(hash(279,h, word1,word3));
        cm.set(hash(280,h, word2,word3));
        if (f) {
            word5=word4;//*29;
            word4=word3;//*31;
            word3=word2;//*37;
            word2=word1;//*41;
            word1='.';
        }
        cm.set(hash(523,x.col,buf(1),above));
        cm.set(hash(524,buf(1),above));
        cm.set(hash(525,x.col,buf(1)));
        cm.set(hash(526,x.col,c==32));
        //cm.set(hash(527,col,0));
        cm.set(hash(281, w, llog(x.blpos-wpos[w])>>4));
        cm.set(hash(282,buf(1),llog(x.blpos-wpos[w])>>2));
   
        
   int fl = 0;
    if ((x.c4&0xff) != 0) {
      if (isalpha(x.c4&0xff)) fl = 1;
      else if (ispunct(x.c4&0xff)) fl = 2;
      else if (isspace(x.c4&0xff)) fl = 3;
      else if ((x.c4&0xff) == 0xff) fl = 4;
      else if ((x.c4&0xff) < 16) fl = 5;
      else if ((x.c4&0xff) < 64) fl = 6;
      else fl = 7;
    }
    mask = (mask<<3)|fl;
 
    cm.set(hash(528,mask,0));
    cm.set(hash(529,mask,buf(1)));
    cm.set(hash(530,mask&0xff,x.col));
    cm.set(hash(531,mask,buf(2),buf(3)));
    cm.set(hash(532,mask&0x1ff,x.f4&0x00fff0));
    }
    cm.mix(m);
	return 0;
}
 ~wordModel1(){ }
};

   

//////////////////////////// im1bitModel /////////////////////////////////
// Model for 1-bit image data
class im1bitModel1: public Model {
   BlockData& x;
   Buf& buf;
   U32 r0, r1, r2, r3;  // last 4 rows, bit 8 is over current pixel
   Array<U8> t;  // model: cxt -> state
   const int N;  // number of contexts
   Array<int>  cxt;  // contexts
   StateMap* sm;
   BH<4> t1;
  U8* cp;
public:
  im1bitModel1( BlockData& bd,U32 val=0 ): x(bd),buf(bd.buf),r0(0),r1(0),r2(0),r3(0), 
    t(0x10200),N(8), cxt(N),t1(65536/2) {
   sm=new StateMap[N];
   cp=t1[0]+1;
   }
  
int p(Mixer& m,int w=0,int val2=0)  {
  // update the model
  int i;
  for (i=0; i<N; ++i)
    t[cxt[i]]=nex(t[cxt[i]],x.y);
if (cp[0]==0 || cp[1]!=x.y) cp[0]=1, cp[1]=x.y;
    else if (cp[0]<255) ++cp[0];
   cp=t1[x.c4]+1;
  // update the contexts (pixels surrounding the predicted one)
  r0+=r0+x.y;
  r1+=r1+((x.buf(w-1)>>(7-x.bpos))&1);
  r2+=r2+((x.buf(w+w-1)>>(7-x.bpos))&1);
  r3+=r3+((x.buf(w+w+w-1)>>(7-x.bpos))&1);
  cxt[0]=(r0&0x7)|(r1>>4&0x38)|(r2>>3&0xc0);
  cxt[1]=0x100+((r0&1)|(r1>>4&0x3e)|(r2>>2&0x40)|(r3>>1&0x80));
  cxt[2]=0x200+((r0&0x3f)^(r1&0x3ffe)^(r2<<2&0x7f00)^(r3<<5&0xf800));
  cxt[3]=0x400+((r0&0x3e)^(r1&0x0c0c)^(r2&0xc800));
  cxt[4]=0x800+(((r1&0x30)^(r3&0x0c0c))|(r0&3));
  cxt[5]=0x1000+((!r0&0x444)|(r1&0xC0C)|(r2&0xAE3)|(r3&0x51C));
  cxt[6]=0x2000+((r0&1)|(r1>>4&0x1d)|(r2>>1&0x60)|(r3&0xC0));
  cxt[7]=0x4000+((r0>>4&0x2AC)|(r1&0xA4)|(r2&0x349)|(!r3&0x14D));
 
  // predict
  for (i=0; i<N; ++i) m.add(stretch(sm[i].p(t[cxt[i]],x.y)));
  if (cp[1]==x.y)
      m.add( ((cp[1]&1)*2-1)*ilog(cp[0]+1)*8);
    else
       m.add( 0);
  m.set((r0&0x7)|(r1>>4&0x38)|(r2>>3&0xc0), 256);
  m.set(((r1&0x30)^(r3&0x0c))|(r0&3),256);
  m.set((r0&1)|(r1>>4&0x3e)|(r2>>2&0x40)|(r3>>1&0x80), 256);
  m.set((r0&0x3e)^((r1>>8)&0x0c)^((r2>>8)&0xc8),256);
   m.set( cp[0],256);
    m.set( cp[1],1);
  return 0;
}
 ~im1bitModel1(){  delete[] sm;}
 
};
//////////////////////////// recordModel ///////////////////////

// Model 2-D data with fixed record length.  Also order 1-2 models
// that include the distance to the last match.

class recordModel1: public Model {
   BlockData& x;
   Buf& buf;
   Array<int> cpos1, cpos2, cpos3, cpos4;
   Array<int> wpos1; // buf(1..2) -> last position
   int rlen, rlen1, rlen2, rlen3,rlenl;  // run length and 2 candidates
   int rcount1, rcount2,rcount3;  // candidate counts
   ContextMap cm, cn, co, cp;
public:
  recordModel1( BlockData& bd,U32 msize=CMlimit(MEM()*2) ): x(bd),buf(bd.buf),  cpos1(256) , cpos2(256), cpos3(256), cpos4(256),wpos1(0x10000),
   rlen(2), rlen1(3), rlen2(4), rlen3(5),rlenl(0), rcount1(0), rcount2(0),rcount3(0),
   cm(32768, 3), cn(32768/2, 3), co(32768*2, 3), cp(CMlimit(msize), 3+1) {
   }
  
int p(Mixer& m,int rrlen=0,int val2=0) {
   // Find record length
  if (!x.bpos) {
    int w=x.c4&0xffff, c=w&255, d=w>>8;
    int r=buf.pos-cpos1[c];
    if (r>1 ){
            if( rrlen==0 ) {
      if ((r==cpos1[c]-cpos2[c] || r==cpos2[c]-cpos3[c] || r==cpos3[c]-cpos4[c] 
        )&& (r>10 || ((c==buf(r*5+1)) && c==buf(r*6+1)))) {
      if (r==rlen1) ++rcount1;
      else if (r==rlen2) ++rcount2;
      else if (r==rlen3) ++rcount3;
      else if (rcount1>rcount2) rlen2=r, rcount2=1;
      else if (rcount2>rcount3) rlen3=r, rcount3=1;
      else rlen1=r, rcount1=1;
     }  
     }
     else   rlen=rrlen;
    } 
 
    if (rcount1>12 && rlen!=rlen1 && rlenl*2!=rlen1) rlenl=rlen=rlen1, rcount1=rcount2=rcount3=0;//, printf("R1: %d  \n",rlen);
    if (rcount2>18 && rlen!=rlen2 && rlenl*2!=rlen2) rlenl=rlen,rlen=rlen2, rcount1=rcount2=rcount3=0;//, printf("R2: %d  \n",rlen);
    if (rcount3>24 && rlen!=rlen3 && rlenl*2!=rlen3) rlenl=rlen,rlen=rlen3, rcount1=rcount2=rcount3=0;//, printf("R3: %d  \n",rlen);

    // Set 2 dimensional contexts
    assert(rlen>0);
    cm.set(c<<8| (min(255, buf.pos-cpos1[c])/4));
    cm.set(w<<9| llog(buf.pos-wpos1[w])>>2);
    //cm.set((buf(rlen)>>4)<<12|(buf(rlen2)>>4)<<8|(buf(rlen3)>>4));

    cm.set(rlen|buf(rlen)<<10|buf(rlen*2)<<18);
    cn.set(w|rlen<<8);
    cn.set(d|rlen<<16);
    cn.set(c|rlen<<8);

    co.set(buf(1)<<8|min(255, buf.pos-cpos1[buf(1)]));
    co.set(buf(1)<<17|buf(2)<<9|llog(buf.pos-wpos1[w])>>2);
    int col=buf.pos%rlen;
    co.set(buf(1)<<8|buf(rlen));

    //cp.set(w*16);
    //cp.set(d*32);
    //cp.set(c*64);
    cp.set(rlen|buf(rlen)<<10|col<<18);
    cp.set(rlen|buf(1)<<10|col<<18);
    cp.set(col|rlen<<12);
    cp.set(col|(buf(rlen)-buf(1))<<12);
    // update last context positions
    cpos4[c]=cpos3[c];
    cpos3[c]=cpos2[c];
    cpos2[c]=cpos1[c];
    cpos1[c]=buf.pos;
    wpos1[w]=buf.pos;
  }
  x.rm1=0;
  cm.mix(m);
  x.rm1=1;
  cn.mix(m);
  co.mix(m);
  cp.mix(m);
  return rlen;
  }
  ~recordModel1(){ }
};

class recordModelx: public Model {
  BlockData& x;
  Buf& buf;
  Array<int> cpos1;
  Array<int> wpos1;// buf(1..2) -> last position
  ContextMap cm, cn, co,cp, cq;
public:
  recordModelx(BlockData& bd,U32 val=0):x(bd),buf(bd.buf),cpos1(256),wpos1(0x10000),
  cm(32768, 2), cn(32768/2, 4+1), co(32768*4, 4),cp(32768*2, 3), cq(32768*2, 3) {
  }
  int p(Mixer& m,int val1=0,int val2=0){
   // Find record length
  if (!x.bpos) {
    int w=x.c4&0xffff, c=w&255, d=w&0xf0ff,e=x.c4&0xffffff;
   
    cm.set(c<<8| (min(255, buf.pos-cpos1[c])/4));
    cm.set(w<<9| llog(buf.pos-wpos1[w])>>2);
  
    cn.set(w);
    cn.set(d<<8);
    cn.set(c<<16);
    cn.set((x.f4&0xfffff)); 
    int col=buf.pos&3;
    cn.set(col|2<<12);

    co.set(c    );
    co.set(w<<8 );
    co.set(x.w5&0x3ffff);
    co.set(e<<3);
    
    cp.set(d    );
    cp.set(c<<8 );
    cp.set(w<<16);

    cq.set(w<<3 );
    cq.set(c<<19);
    cq.set(e);
    // update last context positions
    cpos1[c]=buf.pos;
    wpos1[w]=buf.pos;
  }
  x.rm1=0;
  cm.mix(m);
  cn.mix(m);
  co.mix(m);
  cq.mix(m);
  cp.mix(m);
  x.rm1=1;
  return 0;
  }
  ~recordModelx(){ }
};
//////////////////////////// sparseModel ///////////////////////

// Model order 1-2 contexts with gaps.
class sparseModely: public Model {
  BlockData& x;
  Buf& buf;
  ContextMap cm;
public:
  sparseModely(BlockData& bd,U32 val=0):x(bd),buf(bd.buf), cm(CMlimit(MEM()*2), 40+2) {
  }
  int p(Mixer& m,int seenbefore,int howmany){
  if (x.bpos==0) {
    cm.set(seenbefore);
    cm.set(howmany);
    cm.set(buf(1)|buf(5)<<8);
    cm.set(buf(1)|buf(6)<<8);
    cm.set(buf(3)|buf(6)<<8);
    cm.set(buf(4)|buf(8)<<8);
    cm.set(buf(1)|buf(3)<<8|buf(5)<<16);
    cm.set(buf(2)|buf(4)<<8|buf(6)<<16);
    cm.set(x.c4&0x00f0f0ff);
    cm.set(x.c4&0x00ff00ff);
    cm.set(x.c4&0xff0000ff);
    cm.set(x.c4&0x00f8f8f8);
    cm.set(x.c4&0xf8f8f8f8);
    cm.set(x.f4&0x00000fff);
    cm.set(x.f4);
    cm.set(x.c4&0x00e0e0e0);
    cm.set(x.c4&0xe0e0e0e0);
    cm.set(x.c4&0x810000c1);
    cm.set(x.c4&0xC3CCC38C);
    cm.set(x.c4&0x0081CC81);
    cm.set(x.c4&0x00c10081);
    for (int i=1; i<8; ++i) {
      cm.set(seenbefore|buf(i)<<8);
      cm.set((buf(i+2)<<8)|buf(i+1));
      cm.set((buf(i+3)<<8)|buf(i+1));
    }
  }
  cm.mix(m);
  return 0;
}
~sparseModely(){ }
};
//////////////////////////// distanceModel ///////////////////////

// Model for modelling distances between symbols
class distanceModel1: public Model {
  ContextMap cr;
  int pos00,pos20,posnl;
  BlockData& x;
  Buf& buf;
public:
  distanceModel1(BlockData& bd): cr(CMlimit(MEM()), 3), pos00(0),pos20(0),posnl(0), x(bd),buf(bd.buf) {
    }
  int p(Mixer& m,int val1=0,int val2=0){
  if (x.bpos == 0) {
    int c=x.c4&0xff;
    if (c==0x00) pos00=x.buf.pos;
    if (c==0x20) pos20=x.buf.pos;
    if (c==0xff||c=='\r'||c=='\n') posnl=x.buf.pos;
    cr.set(min(buf.pos-pos00,255)|(c<<8));
    cr.set(min(buf.pos-pos20,255)|(c<<8));
    cr.set(min(buf.pos-posnl,255)|((c<<8)+234567));
  }
  cr.mix(m);
  return 0;
}
~distanceModel1(){ }
};

//////////////////////////// im24bitModel /////////////////////////////////
// Model for 24-bit image data
class im24bitModel1: public Model {
 const int SC;
 SmallStationaryContextMap scm1, scm2, scm3, scm4, scm5, scm6, scm7, scm8, scm9, scm10;
 ContextMap cm;
 int col;
 BlockData& x;
 Buf& buf;
public:
  im24bitModel1(BlockData& bd): SC(0x20000),scm1(SC), scm2(SC),
    scm3(SC), scm4(SC), scm5(SC), scm6(SC), scm7(SC), scm8(SC), scm9(SC*2), scm10(512),
    cm(CMlimit(MEM()*8), 15),col(0) ,x(bd),buf(bd.buf) {
    }
  int p(Mixer& m,int w,int val2=0){
  assert(w>3); 
  if (!x.bpos) {
    int color=buf.pos%3;
    int mean=buf(3)+buf(w-3)+buf(w)+buf(w+3);
    const int var=(sqrbuf(3)+sqrbuf(w-3)+sqrbuf(w)+sqrbuf(w+3)-mean*mean/4)>>2;
    mean>>=2;
    const int logvar=ilog(var);
    int i=color<<4;
    cm.set(hash(++i, buf(3)));
    cm.set(hash(++i, buf(3), buf(1)));
    cm.set(hash(++i, buf(3), buf(1), buf(2)));
    cm.set(hash(++i, buf(w)));
    cm.set(hash(++i, buf(w), buf(1)));
    cm.set(hash(++i, buf(w), buf(1), buf(2)));
    cm.set(hash(++i, (buf(3)+buf(w))>>3, buf(1)>>4, buf(2)>>4));
    cm.set(hash(++i, buf(1), buf(2)));
    cm.set(hash(++i, buf(3), buf(1)-buf(4)));
    cm.set(hash(++i, buf(3)+buf(1)-buf(4)));
    cm.set(hash(++i, buf(w), buf(1)-buf(w+1)));
    cm.set(hash(++i, buf(w)+buf(1)-buf(w+1)));
    cm.set(hash(++i, buf(w*3-3), buf(w*3-6)));
    cm.set(hash(++i, buf(w*3+3), buf(w*3+6)));
    
    cm.set(hash(++i, mean, logvar>>4));
    scm1.set(buf(3)+buf(w)-buf(w+3));
    scm2.set(buf(3)+buf(w-3)-buf(w));
    scm3.set(buf(3)*2-buf(6));
    scm4.set(buf(w)*2-buf(w*2));
    scm5.set(buf(w+3)*2-buf(w*2+6));
    scm6.set(buf(w-3)*2-buf(w*2-6));
    scm7.set(buf(w-3)+buf(1)-buf(w-2));
    scm8.set(buf(w)+buf(w-3)-buf(w*2-3));
    scm9.set(mean>>1|(logvar<<1&0x180));
  }

  // Predict next bit
  scm1.mix(m);
  scm2.mix(m);
  scm3.mix(m);
  scm4.mix(m);
  scm5.mix(m);
  scm6.mix(m);
  scm7.mix(m);
  scm8.mix(m);
  scm9.mix(m);
  scm10.mix(m);
  cm.mix(m);
  if (++col>=24) col=0;
  m.set((buf(3)+buf(6))>>6, 8);
  m.set(col, 24);
  m.set((buf(1)>>4)*3+(buf.pos%3), 48);
  m.set(x.c0, 256);
  return 0;//8 24 48 256
  }
  // Square buf(i)
inline int sqrbuf(int i) {
  assert(i>0);
  return buf(i)*buf(i);
}
  ~im24bitModel1(){ }
 
};

//////////////////////////// im8bitModel /////////////////////////////////
// X is predicted pixel
// NNWWW NNWW NNW NN NNE
//  NWWW  NWW  NW  N  NE
//   WWW   WW   W  X
#define pW buf(1)
#define pN buf(w-1)
#define pNW buf(w)
#define pNE buf(w-2)
#define pWW buf(2)
#define pNN buf(w*2-1)
#define pNNE buf(w*2-2)
#define pNWW buf(w+1)
#define pNNW buf(w*3)
#define pNWWW buf(w*2+2)
#define pNNWW buf(w*2+1)
#define pNWNW pN+pW-pNW
#define pGw 2*pN-pNN
#define pGn 2*pW-pWW
#define pDh (abs(pW-pWW)+abs(pN-pNW)+abs(pN-pNE))
#define pDv (abs(pW-pNW)+abs(pN-pNN)+abs(pNE-pNNE))
#define pGv (abs(pNW-pW)+ abs(pNN-pN))
#define pGh (abs(pWW-pW)+ abs(pNW-pN))
#define HBB1 pW+pN-pNW  
#define HBB2 (pNW+pW)>>1
// Model for 8-bit image data

class im8bitModel1: public Model {
 const int SC;
 SmallStationaryContextMap scm1, scm2,
 scm3, scm4, scm5, scm6 ;
 ContextMap cm;
 int ml,ml1;
 int col;
 BlockData& x;
 Buf& buf;
 int itype;
 int id8;
 int id9;
public:
  im8bitModel1( BlockData& bd): SC(0x20000),scm1(SC), scm2(SC),
   scm3(SC), scm4(SC), scm5(SC), scm6(SC*2),cm(CMlimit(MEM()*16), 45+12+8),
   ml(0),ml1(0),col(0),x(bd),buf(bd.buf),itype(0),id8(1),id9(1) {
  }
int p(Mixer& m,int w,int val2=0){
  assert(w>3); 
      if (!x.bpos) { 
    int mean=buf(1)+buf(w-1)+buf(w)+buf(w+1);
    const int var=(sqrbuf(1)+sqrbuf(w-1)+sqrbuf(w)+sqrbuf(w+1)-mean*mean/4)>>2;
    mean>>=2;
    const int logvar=ilog(var);
    int i=0;
  
    const int errr=(pWW+pNWW-pNW);
    if(abs(errr-pNWNW)>255)id8++; 
    else id9++;
    if (x.blpos==0) id8=id9=1,itype=0;    // reset on new block
    if(x.blpos%w==0 && x.blpos>w) itype=(id9/id8)<4; // select model
    
    if (itype==0){ //faster, for smooth images
    
    cm.set(hash(++i, pW,0));
    cm.set(hash(++i, pN,0));
    cm.set(hash(++i, pNE,0));
    cm.set(hash(++i, pWW,0));
    cm.set(hash(++i, pNN,0));
    cm.set(hash(++i, pNWNW,pW));
    cm.set(hash(++i, pNWW,0));
    cm.set(hash(++i, pNNE,0));
    cm.set(hash(++i, HBB1,0));
    cm.set(hash(++i, HBB2,0));
    cm.set(hash(++i, pGw,pW));
    
    cm.set(hash(++i, pGn,pW));
    cm.set(hash(++i, pDh,pW));
    cm.set(hash(++i, pDv,pW));
    cm.set(hash(++i, pGv,pW));
    cm.set(hash(++i, pGh,pW));
    cm.set(hash(++i, pGv,pN));
    cm.set(hash(++i, pGh,pN));
    cm.set(hash(++i, abs(errr-pNWNW),pW));
   
    cm.set(hash(++i,mean,logvar));
     cm.set(hash(++i,pGn,pGw));
     cm.set(hash(++i,pDh, pDv));
      cm.set(hash(++i,pGv, pGh));
      
     cm.set(hash(++i,abs(min(pW,min( pN, pNW)) + max(pW,max(pN,pNW)) -pNW)));
    cm.set(hash(++i, buf(1)>>2, buf(w)>>2));
    cm.set(hash(++i, buf(1)>>2, buf(2)>>2));
    cm.set(hash(++i, buf(w)>>2, buf(w*2)>>2));
    cm.set(hash(++i, buf(1)>>2, buf(w-1)>>2));
    cm.set(hash(++i, buf(w)>>2, buf(w+1)>>2));
    cm.set(hash(++i, buf(w+1)>>2, buf(w+2)>>2));
    cm.set(hash(++i, (buf(w+1)+buf(w*2+2))>>1));
    cm.set(hash(++i, (buf(w-1)+buf(w*2-2))>>1));
    
    cm.set(hash(++i, pGw,pN));
    
    cm.set(hash(++i, pGn,pN));
    cm.set(hash(++i, pNN,pNE,pW));
    cm.set(hash(++i, (buf(1)+buf(w))>>1));
    cm.set(hash(++i, (buf(1)+buf(2))>>1));
    cm.set(hash(++i, (buf(w)+buf(w*2))>>1));
    cm.set(hash(++i, (buf(1)+buf(w-1))>>1));
    cm.set(hash(++i, (buf(w)+buf(w+1))>>1));
    cm.set(hash(++i, (buf(w+1)+buf(w+2))>>1));
    cm.set(hash(++i, (buf(w+1)+buf(w*2+2))>>1));
    cm.set(hash(++i, (buf(w-1)+buf(w*2-2))>>1));
    cm.set(hash(++i, pNNE,pN,pW));
     cm.set(hash(++i, pNWW,pN,pNE,pW));
     cm.set(hash(++i, pGn,pNE,pNNE));
     cm.set(hash(++i, pWW,pNWW,pNW,pN));
     cm.set(hash(++i,pNNW, pNW,pW));
     cm.set(hash(++i, buf(w)>>2, buf(3)>>2, buf(w-1)>>2));
    cm.set(hash(++i, buf(3)>>2, buf(w-2)>>2, buf(w*2-2)>>2));
  
    cm.set(hash(++i, buf(w)>>2, buf(1)>>2, buf(w-1)>>2));
    cm.set(hash(++i, buf(w-1)>>2, buf(w)>>2, buf(w+1)>>2));
    cm.set(hash(++i, buf(1)>>2, buf(w-1)>>2, buf(w*2-1)>>2));
    
    }
    // 2 x
   else
   {
    i=512;
    cm.set(hash(++i, buf(1),0));
    cm.set(hash(++i, buf(2), 0));
    cm.set(hash(++i, buf(w), 0));
    cm.set(hash(++i, buf(w+1), 0));
    cm.set(hash(++i, buf(w-1), 0));
    cm.set(hash(++i, HBB1,0));
    cm.set(hash(++i, HBB2,0));
    cm.set(hash(++i, pGv,pW));
    cm.set(hash(++i, pGh,pW));
     cm.set(hash(++i,pGv, pGh));
     cm.set(hash(++i, pGv,pN));
    cm.set(hash(++i, pGh,pN));
     cm.set(hash(++i,abs(min(pW,min( pN, pNW)) + max(pW,max(pN,pNW)) -pNW)));
    cm.set(hash(++i, (buf(2)+buf(w)-buf(w+1)), 0));
    cm.set(hash(++i,(buf(w)+(buf(2)-buf(w+1))>>1), 0));
    cm.set(hash(++i, (buf(2)+buf(w+1))>>1, 0));
    cm.set(hash(++i, (buf(w-1)-buf(w)), buf(1)>>1));
    cm.set(hash(++i,(buf(w)-buf(w+1)), buf(1)>>1));
    cm.set(hash(++i, (buf(w+1)+buf(2)), buf(1)>>1));

    cm.set(hash(++i, buf(1)>>2, buf(w)>>2));
    cm.set(hash(++i, buf(1)>>2, buf(2)>>2));
    cm.set(hash(++i, buf(w)>>2, buf(w*2)>>2));
    cm.set(hash(++i, buf(1)>>2, buf(w-1)>>2));
    cm.set(hash(++i, buf(w)>>2, buf(w+1)>>2));
    cm.set(hash(++i, buf(w+1)>>2, buf(w+2)>>2));
    cm.set(hash(++i, buf(w+1)>>2, buf(w*2+2)>>2));
    cm.set(hash(++i, buf(w-1)>>2, buf(w*2-2)>>2));
      cm.set(hash(++i,(buf(1)+buf(w))>>1));
      cm.set(hash(++i,(buf(1)+buf(2))>>1));
      cm.set(hash(++i,(buf(w)+buf(w*2))>>1));
      cm.set(hash(++i,(buf(1)+buf(w-1))>>1));
      cm.set(hash(++i,(buf(w)+buf(w+1))>>1));
      cm.set(hash(++i,(buf(w+1)+buf(w+2))>>1));
      cm.set(hash(++i,(buf(w+1)+buf(w*2+2))>>1));
      cm.set(hash(++i,(buf(w-1)+buf(w*2-2))>>1));
    // 3 x
    cm.set(hash(++i, buf(w)>>2, buf(1)>>2, buf(w-1)>>2));
    cm.set(hash(++i, buf(w-1)>>2, buf(w)>>2, buf(w+1)>>2));
    cm.set(hash(++i, buf(1)>>2, buf(w-1)>>2, buf(w*2-1)>>2));
    // mixed
      cm.set(hash(++i,(buf(3)+buf(w))>>1, buf(1)>>2, buf(2)>>2));
      cm.set(hash(++i,(buf(2)+buf(1))>>1,(buf(w)+buf(w*2))>>1,buf(w-1)>>2));
      cm.set(hash(++i,(buf(2)+buf(1))>>2,(buf(w-1)+buf(w))>>2));
      cm.set(hash(++i,(buf(2)+buf(1))>>1,(buf(w)+buf(w*2))>>1));
      cm.set(hash(++i,(buf(2)+buf(1))>>1,(buf(w-1)+buf(w*2-2))>>1));
      cm.set(hash(++i,(buf(2)+buf(1))>>1,(buf(w+1)+buf(w*2+2))>>1));
      cm.set(hash(++i,(buf(w)+buf(w*2))>>1,(buf(w-1)+buf(w*2+2))>>1));
      cm.set(hash(++i,(buf(w-1)+buf(w))>>1,(buf(w)+buf(w+1))>>1));
      cm.set(hash(++i,(buf(1)+buf(w-1))>>1,(buf(w)+buf(w*2))>>1));
      cm.set(hash(++i,(buf(1)+buf(w-1))>>2,(buf(w)+buf(w+1))>>2));
      cm.set(hash(++i,(((buf(1)-buf(w-1))>>1)+buf(w))>>2));
      cm.set(hash(++i,(((buf(w-1)-buf(w))>>1)+buf(1))>>2));
      cm.set(hash(++i,(-buf(1)+buf(w-1)+buf(w))>>2));
    
    cm.set(hash(++i,(buf(1)*2-buf(2))>>1));
    cm.set(hash(++i,mean,logvar));
    cm.set(hash(++i,(buf(w)*2-buf(w*2))>>1));
    cm.set(hash(++i,(buf(1)+buf(w)-buf(w+1))>>1));
    
    cm.set(hash(++i, (buf(4)+buf(3))>>2,(buf(w-1)+buf(w))>>2));
    cm.set(hash(++i, (buf(4)+buf(3))>>1,(buf(w)+buf(w*2))>>1));
    cm.set(hash(++i, (buf(4)+buf(3))>>1,(buf(w-1)+buf(w*2-2))>>1));
    cm.set(hash(++i, (buf(4)+buf(3))>>1,(buf(w+1)+buf(w*2+2))>>1));
    cm.set(hash(++i, (buf(4)+buf(1))>>2,(buf(w-3)+buf(w))>>2));
    cm.set(hash(++i, (buf(4)+buf(1))>>1,(buf(w)+buf(w*2))>>1));
    cm.set(hash(++i, (buf(4)+buf(1))>>1,(buf(w-3)+buf(w*2-3))>>1));
    cm.set(hash(++i, (buf(4)+buf(1))>>1,(buf(w+3)+buf(w*2+3))>>1));
    cm.set(hash(++i, buf(w)>>2, buf(3)>>2, buf(w-1)>>2));
    cm.set(hash(++i, buf(3)>>2, buf(w-2)>>2, buf(w*2-2)>>2));
    }
        
    scm1.set((buf(1)+buf(w))>>1);
    scm2.set((buf(1)+buf(w)-buf(w+1))>>1);
    scm3.set((buf(1)*2-buf(2))>>1);
    scm4.set((buf(w)*2-buf(w*2))>>1);
    scm5.set((buf(1)+buf(w)-buf(w-1))>>1);
    ml1=((abs(errr-pNWNW)>255)<<10);
    ml=mean>>1|(logvar<<1&0x180);
    scm6.set(mean>>1|(logvar<<1&0x180));
  }

  // Predict next bit
  scm1.mix(m);
  scm2.mix(m);
  scm3.mix(m);
  scm4.mix(m);
  scm5.mix(m);
  scm6.mix(m);
  cm.mix(m);
  if (++col>=8) col=0; // reset after every 24 columns?
  m.set(2, 8);
  m.set(col, 8);
  m.set((buf(w)+buf(1))>>4, 32);
  m.set(x.c0, 256);
   m.set(ml, 512);
 
  m.set(pDv|ml1, 1792);
  return 0; //8 8 32 256 512 1792
  }
  // Square buf(i)
inline int sqrbuf(int i) {
  assert(i>0);
  return buf(i)*buf(i);
}
  ~im8bitModel1(){ }
 
};


//////////////////////////// jpegModel /////////////////////////

// Model JPEG. Return 1 if a JPEG file is detected or else 0.
// Only the baseline and 8 bit extended Huffman coded DCT modes are
// supported.  The model partially decodes the JPEG image to provide
// context for the Huffman coded symbols.

// Print a JPEG segment at buf[p...] for debugging
/*
void dump(const char* msg, int p) {
  printf("%s:", msg);
  int len=buf[p+2]*256+buf[p+3];
  for (int i=0; i<len+2; ++i)
    printf(" %02X", buf[p+i]);
  printf("\n");
}
*/

// Detect invalid JPEG data.  The proper response is to silently
// fall back to a non-JPEG model.
#define jassert(x) if (!(x)) { \
  /*printf("JPEG error at %d, line %d: %s\n", buf.pos, __LINE__, #x);*/ \
  jpeg=0; \
  return 0;}

struct HUF {U32 min, max; int val;}; // Huffman decode tables
  // huf[Tc][Th][m] is the minimum, maximum+1, and pointer to codes for
  // coefficient type Tc (0=DC, 1=AC), table Th (0-3), length m+1 (m=0-15)

class jpegModelx: public Model {
   // State of parser
  enum {SOF0=0xc0, SOF1, SOF2, SOF3, DHT, RST0=0xd0, SOI=0xd8, EOI, SOS, DQT,
    DNL, DRI, APP0=0xe0, COM=0xfe, FF};  // Second byte of 2 byte codes
   int jpeg;  // 1 if JPEG is header detected, 2 if image data
  //static int next_jpeg=0;  // updated with jpeg on next byte boundary
   int app;  // Bytes remaining to skip in APPx or COM field
   int sof, sos, data;  // pointers to buf
   Array<int> ht;  // pointers to Huffman table headers
   int htsize;  // number of pointers in ht

  // Huffman decode state
   U32 huffcode;  // Current Huffman code including extra bits
   int huffbits;  // Number of valid bits in huffcode
   int huffsize;  // Number of bits without extra bits
   int rs;  // Decoded huffcode without extra bits.  It represents
    // 2 packed 4-bit numbers, r=run of zeros, s=number of extra bits for
    // first nonzero code.  huffcode is complete when rs >= 0.
    // rs is -1 prior to decoding incomplete huffcode.
   int mcupos;  // position in MCU (0-639).  The low 6 bits mark
    // the coefficient in zigzag scan order (0=DC, 1-63=AC).  The high
    // bits mark the block within the MCU, used to select Huffman tables.

  // Decoding tables
   Array<HUF> huf;  // Tc*64+Th*16+m -> min, max, val
   int mcusize;  // number of coefficients in an MCU
   int linesize; // width of image in MCU
   int hufsel[2][10];  // DC/AC, mcupos/64 -> huf decode table
   Array<U8> hbuf;  // Tc*1024+Th*256+hufcode -> RS

  // Image state
   Array<int> color;  // block -> component (0-3)
   Array<int> pred;  // component -> last DC value
   int dc;  // DC value of the current block
   int width;  // Image width in MCU
   int row, column;  // in MCU (column 0 to width-1)
   Buf cbuf; // Rotating buffer of coefficients, coded as:
    // DC: level shifted absolute value, low 4 bits discarded, i.e.
    //   [-1023...1024] -> [0...255].
    // AC: as an RS code: a run of R (0-15) zeros followed by an S (0-15)
    //   bit number, or 00 for end of block (in zigzag order).
    //   However if R=0, then the format is ssss11xx where ssss is S,
    //   xx is the first 2 extra bits, and the last 2 bits are 1 (since
    //   this never occurs in a valid RS code).
   int cpos;  // position in cbuf
  //static U32 huff1=0, huff2=0, huff3=0, huff4=0;  // hashes of last codes
   int rs1;//, rs2, rs3, rs4;  // last 4 RS codes
   int ssum, ssum1, ssum2, ssum3;
    // sum of S in RS codes in block and sum of S in first component

   IntBuf cbuf2;
   Array<int> adv_pred, sumu, sumv;
   Array<int> ls;  // block -> distance to previous block
   Array<int> lcp, zpos;

    //for parsing Quantization tables
   int dqt_state , dqt_end , qnum;
   Array<U8> qtab; // table
   Array<int> qmap; // block -> table number

   // Context model
  const int N; // size of t, number of contexts
   BH<9> t;  // context hash -> bit history
    // As a cache optimization, the context does not include the last 1-2
    // bits of huffcode if the length (huffbits) is not a multiple of 3.
    // The 7 mapped values are for context+{"", 0, 00, 01, 1, 10, 11}.
   Array<U32> cxt;  // context hashes
   Array<U8*> cp;  // context pointers
   StateMap *sm;  
   Mixer m1;
   APM a1, a2;
   BlockData& x;
   Buf& buf;
    int hbcount;
public:
  jpegModelx(BlockData& bd): jpeg(0),app(0),sof(0),sos(0),data(0),ht(8),htsize(0),huffcode(0),
  huffbits(0),huffsize(0),rs(-1), mcupos(0), huf(128), mcusize(0),linesize(0),
  hbuf(2048),color(10), pred(4), dc(0),width(0), row(0),column(0),cbuf(0x20000),
  cpos(0), rs1(0), ssum(0), ssum1(0), ssum2(0), ssum3(0),cbuf2(0x20000),adv_pred(7),
  sumu(8), sumv(8), ls(10),lcp(4), zpos(64), dqt_state(-1),dqt_end(0),qnum(0),
  qtab(256),qmap(10),N(28),t(level>11?0x8000000:CMlimit(MEM())),cxt(N),cp(N),m1(32, 770,bd, 3),
  a1(0x8000),a2(0x10000),x(bd),buf(bd.buf),hbcount(2) {
  sm=new StateMap[N];
  }
  int p(Mixer& m,int val1=0,int val2=0){
  const  U8 zzu[64]={  // zigzag coef -> u,v
    0,1,0,0,1,2,3,2,1,0,0,1,2,3,4,5,4,3,2,1,0,0,1,2,3,4,5,6,7,6,5,4,
    3,2,1,0,1,2,3,4,5,6,7,7,6,5,4,3,2,3,4,5,6,7,7,6,5,4,5,6,7,7,6,7};
  const  U8 zzv[64]={
    0,0,1,2,1,0,0,1,2,3,4,3,2,1,0,0,1,2,3,4,5,6,5,4,3,2,1,0,0,1,2,3,
    4,5,6,7,7,6,5,4,3,2,1,2,3,4,5,6,7,7,6,5,4,3,4,5,6,7,7,6,5,6,7,7};

  if (!x.bpos && !x.blpos) jpeg=0;
  // Be sure to quit on a byte boundary
  if (x.bpos && !jpeg) return 0;
  if (!x.bpos && app>=0) --app;
  if (app>0) return 0;
  if (!x.bpos) {


    // Parse.  Baseline DCT-Huffman JPEG syntax is:
    // SOI APPx... misc... SOF0 DHT... SOS data EOI
    // SOI (= FF D8) start of image.
    // APPx (= FF Ex) len ... where len is always a 2 byte big-endian length
    //   including the length itself but not the 2 byte preceding code.
    //   Application data is ignored.  There may be more than one APPx.
    // misc codes are DQT, DNL, DRI, COM (ignored).
    // SOF0 (= FF C0) len 08 height width Nf [C HV Tq]...
    //   where len, height, width (in pixels) are 2 bytes, Nf is the repeat
    //   count (1 byte) of [C HV Tq], where C is a component identifier
    //   (color, 0-3), HV is the horizontal and vertical dimensions
    //   of the MCU (high, low bits, packed), and Tq is the quantization
    //   table ID (not used).  An MCU (minimum compression unit) consists
    //   of 64*H*V DCT coefficients for each color.
    // DHT (= FF C4) len [TcTh L1...L16 V1,1..V1,L1 ... V16,1..V16,L16]...
    //   defines Huffman table Th (1-4) for Tc (0=DC (first coefficient)
    //   1=AC (next 63 coefficients)).  L1..L16 are the number of codes
    //   of length 1-16 (in ascending order) and Vx,y are the 8-bit values.
    //   A V code of RS means a run of R (0-15) zeros followed by S (0-15)
    //   additional bits to specify the next nonzero value, negative if
    //   the first additional bit is 0 (e.g. code x63 followed by the
    //   3 bits 1,0,1 specify 7 coefficients: 0, 0, 0, 0, 0, 0, 5.
    //   Code 00 means end of block (remainder of 63 AC coefficients is 0).
    // SOS (= FF DA) len Ns [Cs TdTa]... 0 3F 00
    //   Start of scan.  TdTa specifies DC/AC Huffman tables (0-3, packed
    //   into one byte) for component Cs matching C in SOF0, repeated
    //   Ns (1-4) times.
    // EOI (= FF D9) is end of image.
    // Huffman coded data is between SOI and EOI.  Codes may be embedded:
    // RST0-RST7 (= FF D0 to FF D7) mark the start of an independently
    //   compressed region.
    // DNL (= FF DC) 04 00 height
    //   might appear at the end of the scan (ignored).
    // FF 00 is interpreted as FF (to distinguish from RSTx, DNL, EOI).

    // Detect JPEG (SOI, APPx)
    if (!jpeg && buf(4)==FF && buf(3)==SOI && buf(2)==FF && (buf(1)>>4==0xe || buf(1)==0xdb)) {
      jpeg=1;
      sos=sof=htsize=data=mcusize=linesize=0, app=2;
      huffcode=huffbits=huffsize=mcupos=cpos=0, rs=-1;
      memset(&huf[0], 0, huf.size()*sizeof(HUF));
      memset(&pred[0], 0, pred.size()*sizeof(int));
    }

    // Detect end of JPEG when data contains a marker other than RSTx
    // or byte stuff (00).
    if (jpeg && data && buf(2)==FF && buf(1) && (buf(1)&0xf8)!=RST0) {
      jassert(buf(1)==EOI);
      jpeg=0;
    }
    if (!jpeg) return 0;

    // Detect APPx or COM field
    if (!data && !app && buf(4)==FF && (buf(3)>>4==0xe || buf(3)==COM))
      app=buf(2)*256+buf(1)+2;

    // Save pointers to sof, ht, sos, data,
    if (buf(5)==FF && buf(4)==SOS) {
      int len=buf(3)*256+buf(2);
      if (len==6+2*buf(1) && buf(1) && buf(1)<=4)  // buf(1) is Ns
        sos=buf.pos-5, data=sos+len+2, jpeg=2;
    }
    if (buf(4)==FF && buf(3)==DHT && htsize<8) ht[htsize++]=buf.pos-4;
    if (buf(4)==FF && buf(3)==SOF0) sof=buf.pos-4;

    // Parse Quantizazion tables
    if (buf(4)==FF && buf(3)==DQT)
      dqt_end=buf.pos+buf(2)*256+buf(1)-1, dqt_state=0;
    else if (dqt_state>=0) {
      if (buf.pos>=dqt_end)
        dqt_state = -1;
      else {
        if (dqt_state%65==0)
          qnum = buf(1);
        else {
          jassert(buf(1)>0);
          jassert(qnum>=0 && qnum<4);
          qtab[qnum*64+((dqt_state%65)-1)]=buf(1)-1;
        }
        dqt_state++;
      }
    }

    // Restart
    if (buf(2)==FF && (buf(1)&0xf8)==RST0) {
      huffcode=huffbits=huffsize=mcupos=0, rs=-1;
      memset(&pred[0], 0, pred.size()*sizeof(int));
    }
  }

  {
    // Build Huffman tables
    // huf[Tc][Th][m] = min, max+1 codes of length m, pointer to byte values
    if (buf.pos==data && x.bpos==1) {
      jassert(htsize>0);
      int i;
      for (i=0; i<htsize; ++i) {
        int p=ht[i]+4;  // pointer to current table after length field
        int end=p+buf[p-2]*256+buf[p-1]-2;  // end of Huffman table
        int count=0;  // sanity check
        while (p<end && end<buf.pos && end<p+2100 && ++count<10) {
          int tc=buf[p]>>4, th=buf[p]&15;
          if (tc>=2 || th>=4) break;
          jassert(tc>=0 && tc<2 && th>=0 && th<4);
          HUF* h=&huf[tc*64+th*16]; // [tc][th][0];
          int val=p+17;  // pointer to values
          int hval=tc*1024+th*256;  // pointer to RS values in hbuf
          int j;
          for (j=0; j<256; ++j) // copy RS codes
            hbuf[hval+j]=buf[val+j];
          int code=0;
          for (j=0; j<16; ++j) {
            h[j].min=code;
            h[j].max=code+=buf[p+j+1];
            h[j].val=hval;
            val+=buf[p+j+1];
            hval+=buf[p+j+1];
            code*=2;
          }
          p=val;
          jassert(hval>=0 && hval<2048);
        }
        jassert(p==end);
      }
      huffcode=huffbits=huffsize=0, rs=-1;

      // Build Huffman table selection table (indexed by mcupos).
      // Get image width.
      if (!sof && sos) return 0;
      int ns=buf[sos+4];
      int nf=buf[sof+9];
      jassert(ns<=4 && nf<=4);
      mcusize=0;  // blocks per MCU
      int hmax=0;  // MCU horizontal dimension
      for (i=0; i<ns; ++i) {
        for (int j=0; j<nf; ++j) {
          if (buf[sos+2*i+5]==buf[sof+3*j+10]) { // Cs == C ?
            int hv=buf[sof+3*j+11];  // packed dimensions H x V
            if (hv>>4>hmax) hmax=hv>>4;
            hv=(hv&15)*(hv>>4);  // number of blocks in component C
            jassert(hv>=1 && hv+mcusize<=10);
            while (hv) {
              jassert(mcusize<10);
              hufsel[0][mcusize]=buf[sos+2*i+6]>>4&15;
              hufsel[1][mcusize]=buf[sos+2*i+6]&15;
              jassert (hufsel[0][mcusize]<4 && hufsel[1][mcusize]<4);
              color[mcusize]=i;
              int tq=buf[sof+3*j+12];  // quantization table index (0..3)
              jassert(tq>=0 && tq<4);
              qmap[mcusize]=tq; // quantizazion table mapping
              --hv;
              ++mcusize;
            }
          }
        }
      }
      jassert(hmax>=1 && hmax<=10);
      int j;
      for (j=0; j<mcusize; ++j) {
        ls[j]=0;
        for (int i=1; i<mcusize; ++i) if (color[(j+i)%mcusize]==color[j]) ls[j]=i;
        ls[j]=(mcusize-ls[j])<<6;
      }
      for (j=0; j<64; ++j) zpos[zzu[j]+8*zzv[j]]=j;
      width=buf[sof+7]*256+buf[sof+8];  // in pixels
      width=(width-1)/(hmax*8)+1;  // in MCU
      jassert(width>0);
      mcusize*=64;  // coefficients per MCU
      row=column=0;
    }
  }


  // Decode Huffman
  {
    if (mcusize && buf(1+(!x.bpos))!=FF) {  // skip stuffed byte
      jassert(huffbits<=32);
      huffcode+=huffcode+x.y;
      ++huffbits;
      if (rs<0) {
        jassert(huffbits>=1 && huffbits<=16);
        const int ac=(mcupos&63)>0;
        jassert(mcupos>=0 && (mcupos>>6)<10);
        jassert(ac==0 || ac==1);
        const int sel=hufsel[ac][mcupos>>6];
        jassert(sel>=0 && sel<4);
        const int i=huffbits-1;
        jassert(i>=0 && i<16);
        const HUF *h=&huf[ac*64+sel*16]; // [ac][sel];
        jassert(h[i].min<=h[i].max && h[i].val<2048 && huffbits>0);
        if (huffcode<h[i].max) {
          jassert(huffcode>=h[i].min);
          int k=h[i].val+huffcode-h[i].min;
          jassert(k>=0 && k<2048);
          rs=hbuf[k];
          huffsize=huffbits;
        }
      }
      if (rs>=0) {
        if (huffsize+(rs&15)==huffbits) { // done decoding
         // huff4=huff3;
         // huff3=huff2;
         // huff2=huff1;
         // huff1=hash(huffcode, huffbits);
         // rs4=rs3;
         // rs3=rs2;
         // rs2=rs1;
          rs1=rs;
          int xe=0;  // decoded extra bits
          if (mcupos&63) {  // AC
            if (rs==0) { // EOB
              mcupos=(mcupos+63)&-64;
              jassert(mcupos>=0 && mcupos<=mcusize && mcupos<=640);
              while (cpos&63) {
                cbuf2[cpos]=0;
                cbuf[cpos++]=0;
              }
            }
            else {  // rs = r zeros + s extra bits for the next nonzero value
                    // If first extra bit is 0 then value is negative.
              jassert((rs&15)<=10);
              const int r=rs>>4;
              const int s=rs&15;
              jassert(mcupos>>6==(mcupos+r)>>6);
              mcupos+=r+1;
              xe=huffcode&((1<<s)-1);
              if (s && !(xe>>(s-1))) xe-=(1<<s)-1;
              for (int i=r; i>=1; --i) {
                cbuf2[cpos]=0;
                cbuf[cpos++]=i<<4|s;
              }
              cbuf2[cpos]=xe;
              cbuf[cpos++]=(s<<4)|(huffcode<<2>>s&3)|12;
              ssum+=s;
            }
          }
          else {  // DC: rs = 0S, s<12
            jassert(rs<12);
            ++mcupos;
            xe=huffcode&((1<<rs)-1);
            if (rs && !(xe>>(rs-1))) xe-=(1<<rs)-1;
            jassert(mcupos>=0 && mcupos>>6<10);
            const int comp=color[mcupos>>6];
            jassert(comp>=0 && comp<4);
            dc=pred[comp]+=xe;
            jassert((cpos&63)==0);
            cbuf2[cpos]=dc;
            cbuf[cpos++]=(dc+1023)>>3;
            if ((mcupos>>6)==0) {
              ssum1=0;
              ssum2=ssum3;
            } else {
              if (color[(mcupos>>6)-1]==color[0]) ssum1+=(ssum3=ssum);
              ssum2=ssum1;
            }
            ssum=rs;
          }
          jassert(mcupos>=0 && mcupos<=mcusize);
          if (mcupos>=mcusize) {
            mcupos=0;
            if (++column==width) column=0, ++row;
          }
          huffcode=huffsize=huffbits=0, rs=-1;

          // UPDATE_ADV_PRED !!!!
          {
            const int acomp=mcupos>>6, q=64*qmap[acomp];
            const int zz=mcupos&63, cpos_dc=cpos-zz;
            if (zz==0) {
              for (int i=0; i<8; ++i) sumu[i]=sumv[i]=0;
              int cpos_dc_ls_acomp = cpos_dc-ls[acomp];
              int cpos_dc_mcusize_width = cpos_dc-mcusize*width;
              for (int i=0; i<64; ++i) {
                sumu[zzu[i]]+=(zzv[i]&1?-1:1)*(zzv[i]?16*(16+zzv[i]):181)*(qtab[q+i]+1)*cbuf2[cpos_dc_mcusize_width+i];
                sumv[zzv[i]]+=(zzu[i]&1?-1:1)*(zzu[i]?16*(16+zzu[i]):181)*(qtab[q+i]+1)*cbuf2[cpos_dc_ls_acomp+i];
              }
            }
            else {
              sumu[zzu[zz-1]]-=(zzv[zz-1]?16*(16+zzv[zz-1]):181)*(qtab[q+zz-1]+1)*cbuf2[cpos-1];
              sumv[zzv[zz-1]]-=(zzu[zz-1]?16*(16+zzu[zz-1]):181)*(qtab[q+zz-1]+1)*cbuf2[cpos-1];
            }

            for (int i=0; i<3; ++i)
              for (int st=0; st<8; ++st) {
                const int zz2=min(zz+st, 63);
                int p=(sumu[zzu[zz2]]*i+sumv[zzv[zz2]]*(2-i))/2;
                p/=(qtab[q+zz2]+1)*181*(16+zzv[zz2])*(16+zzu[zz2])/256;
                if (zz2==0) p-=cbuf2[cpos_dc-ls[acomp]];
                p=(p<0?-1:+1)*ilog(10*abs(p)+1)/10;
                if (st==0) {
                  adv_pred[i]=p;
                  adv_pred[i+4]=p/4;
                }
                else if (abs(p)>abs(adv_pred[i])+1) {
                  adv_pred[i]+=(st*2+(p>0))<<6;
                  if (abs(p/4)>abs(adv_pred[i+4])+1) adv_pred[i+4]+=(st*2+(p>0))<<6;
                  break;
                }
              }
            xe=2*sumu[zzu[zz]]+2*sumv[zzv[zz]];
            for (int i=0; i<8; ++i) xe-=(zzu[zz]<i)*sumu[i]+(zzv[zz]<i)*sumv[i];
            xe/=(qtab[q+zz]+1)*181;
            if (zz==0) xe-=cbuf2[cpos_dc-ls[acomp]];
            adv_pred[3]=(xe<0?-1:+1)*ilog(10*abs(xe)+1)/10;

            for (int i=0; i<4; ++i) {
              const int a=(i&1?zzv[zz]:zzu[zz]), b=(i&2?2:1);
              if (a<b) xe=255;
              else {
                const int zz2=zpos[zzu[zz]+8*zzv[zz]-(i&1?8:1)*b];
                xe=(qtab[q+zz2]+1)*cbuf2[cpos_dc+zz2]/(qtab[q+zz]+1);
                xe=(xe<0?-1:+1)*ilog(10*abs(xe)+1)/10;
              }
              lcp[i]=xe;
            }
            if (column==0) adv_pred[1]=adv_pred[2], adv_pred[0]=1;
            if (row==0) adv_pred[1]=adv_pred[0], adv_pred[2]=1;
          } // !!!!

        }
      }
    }
  }

  // Estimate next bit probability
  if (!jpeg || !data) return 0;
  if (buf(1+(!x.bpos))==FF) {
    m.add(128);
    m.set(1, 8);
    m.set(0, 257);
    m.set(buf(1), 256);
    return 1;
  }

  // Update model
  if (cp[N-1]) {
    for (int i=0; i<N; ++i)
      *cp[i]=nex(*cp[i],x.y);
  }
  m1.update();

  // Update context
  const int comp=color[mcupos>>6];
  const int coef=(mcupos&63)|comp<<6;
  const int hc=(huffcode*4+((mcupos&63)==0)*2+(comp==0))|1<<(huffbits+2);
  
  if (++hbcount>2 || huffbits==0) hbcount=0;
  jassert(coef>=0 && coef<256);
  const int zu=zzu[mcupos&63], zv=zzv[mcupos&63];
  if (hbcount==0) {
    int n=0;
    cxt[0]=hash(++n, hc, coef, adv_pred[2], ssum2>>6);
    cxt[1]=hash(++n, hc, coef, adv_pred[0], ssum2>>6);
    cxt[2]=hash(++n, hc, coef, adv_pred[1], ssum2>>6);
    cxt[3]=hash(++n, hc, rs1, adv_pred[2]);
    cxt[4]=hash(++n, hc, rs1, adv_pred[0]);
    cxt[5]=hash(++n, hc, rs1, adv_pred[1]);
    cxt[6]=hash(++n, hc, adv_pred[2], adv_pred[0]);
    cxt[7]=hash(++n, hc, cbuf[cpos-width*mcusize], adv_pred[3]);
    cxt[8]=hash(++n, hc, cbuf[cpos-ls[mcupos>>6]], adv_pred[3]);
    cxt[9]=hash(++n, hc, lcp[0], lcp[1], adv_pred[1]);
    cxt[10]=hash(++n, hc, lcp[0], lcp[1], mcupos&63);
    cxt[11]=hash(++n, hc, zu, lcp[0], lcp[2]/3);
    cxt[12]=hash(++n, hc, zv, lcp[1], lcp[3]/3);
    cxt[13]=hash(++n, hc, mcupos>>1);
    cxt[14]=hash(++n, hc, mcupos&63, column>>1);
    cxt[15]=hash(++n, hc, column>>3, lcp[0]+256*(lcp[2]/4), lcp[1]+256*(lcp[3]/4));
    cxt[16]=hash(++n, hc, ssum>>3, mcupos&63);
    cxt[17]=hash(++n, hc, rs1, mcupos&63);
    cxt[18]=hash(++n, hc, mcupos>>3, ssum2>>5, adv_pred[3]);
    cxt[19]=hash(++n, hc, lcp[0]/4, lcp[1]/4, adv_pred[5]);
    cxt[20]=hash(++n, hc, cbuf[cpos-width*mcusize], adv_pred[6]);
    cxt[21]=hash(++n, hc, cbuf[cpos-ls[mcupos>>6]], adv_pred[4]);
    cxt[22]=hash(++n, hc, adv_pred[2]);
    cxt[23]=hash(n, hc, adv_pred[0]);
    cxt[24]=hash(n, hc, adv_pred[1]);
    cxt[25]=hash(++n, hc, zv, lcp[1], adv_pred[6]);
    cxt[26]=hash(++n, hc, zu, lcp[0], adv_pred[4]);
    cxt[27]=hash(++n, hc, lcp[0], lcp[1], adv_pred[3]);
    
  }

  // Predict next bit
  m1.add(128);
  assert(hbcount<=2);
 switch(hbcount)
  {
   case 0: for (int i=0; i<N; ++i) cp[i]=t[cxt[i]]+1, m1.add(stretch(sm[i].p(*cp[i],x.y))); break;
   case 1: { int hc=1+(huffcode&1)*3; for (int i=0; i<N; ++i) cp[i]+=hc, m1.add(stretch(sm[i].p(*cp[i],x.y))); } break;
   default: { int hc=1+(huffcode&1); for (int i=0; i<N; ++i) cp[i]+=hc, m1.add(stretch(sm[i].p(*cp[i],x.y))); } break;
  }

  m1.set(column==0, 2);
  m1.set(coef, 256);
  m1.set(hc&511, 512);
  int pr=m1.p();
  m.add(stretch(pr));
  pr=a1.p(pr, (hc&511)|((adv_pred[1]==0?0:(abs(adv_pred[1])-4)&63)<<9), x.y,1023);
  pr=a2.p(pr, (hc&255)|(coef<<8),x.y, 255);
  m.add(stretch(pr));
  m.set(1, 8);
  m.set(1+(hc&255), 257);
  m.set(buf(1), 256);
  return 1;
  }
  ~jpegModelx(){
  delete[] sm;
   }
 
};

//////////////////////////// exeModel /////////////////////////

// Model x86 code.  The contexts are sparse containing only those
// bits relevant to parsing (2 prefixes, opcode, and mod and r/m fields
// of modR/M byte).
class exeModel1: public Model {
  BlockData& x;
  Buf& buf;
  const int N;
 ContextMap cm;
public:
  exeModel1(BlockData& bd,U32 val=0):x(bd),buf(bd.buf),N(14), cm(CMlimit(MEM()), N) {
  }
int p(Mixer& m,int val1=0,int val2=0){
  if (!x.bpos) {
    for (int i=0; i<N; ++i) cm.set(execxt(i+1, x.buf(1)*(i>6)));
  }
  cm.mix(m);
  return 0;
}

inline int pref(int i) { return (buf(i)==0x0f)+2*(buf(i)==0x66)+3*(buf(i)==0x67); }

// Get context at buf(i) relevant to parsing 32-bit x86 code
U32 execxt( int i, int xb=0) {
  int prefix=0, opcode=0, modrm=0;
  if (i) prefix+=4*pref(i--);
  if (i) prefix+=pref(i--);
  if (i) opcode+=buf(i--);
  if (i) modrm+=buf(i)&0xc7;
  return prefix|opcode<<4|modrm<<12|xb<<20;
}
~exeModel1(){ }
};

//////////////////////////// indirectModel /////////////////////

// The context is a byte string history that occurs within a
// 1 or 2 byte context.
class indirectModel1: public Model {
  BlockData& x;
  Buf& buf;
  ContextMap cm;
  Array<U32> t1;
  Array<U16> t2;
  Array<U16> t3;
  Array<U16> t4;
public:
  indirectModel1(BlockData& bd,U32 val=0):x(bd),buf(bd.buf),cm(CMlimit(MEM()),9+3),t1(256),
   t2(0x10000), t3(0x8000),t4(0x8000) {
  }
int p(Mixer& m,int val1=0,int val2=0){
  if (!x.bpos) {
      
    U32 d=x.c4&0xffff, c=d&255, d2=(x.buf(1)&31)+32*(x.buf(2)&31)+1024*(x.buf(3)&31);
    U32 d3=(x.buf(1)>>3&31)+32*(x.buf(3)>>3&31)+1024*(x.buf(4)>>3&31);
    U32& r1=t1[d>>8];
    r1=r1<<8|c;
    U16& r2=t2[x.c4>>8&0xffff];
    r2=r2<<8|c;
    U16& r3=t3[(x.buf(2)&31)+32*(x.buf(3)&31)+1024*(x.buf(4)&31)];
    r3=r3<<8|c;
    U16& r4=t4[(x.buf(2)>>3&31)+32*(x.buf(4)>>3&31)+1024*(x.buf(5)>>3&31)];
    r4=r4<<8|c;
    const U32 t=c|t1[c]<<8;
    const U32 t0=d|t2[d]<<16;
    const U32 ta=d2|t3[d2]<<16;
    const U32 tc=d3|t4[d3]<<16;
    cm.set(t);
    cm.set(t0);
    cm.set(ta);
    cm.set(tc);
    cm.set(t&0xff00);
    cm.set(t0&0xff0000);
    cm.set(ta&0xff0000);
    cm.set(tc&0xff0000);
    cm.set(t&0xffff);
    cm.set(t0&0xffffff);
    cm.set(ta&0xffffff);
    cm.set(tc&0xffffff);
  }
  cm.mix(m);
  return 0;
}
~indirectModel1(){ }
};


//////////////////////////// dmcModel //////////////////////////

// Model using DMC.  The bitwise context is represented by a state graph,
// initilaized to a bytewise order 1 model as in
// http://plg.uwaterloo.ca/~ftp/dmc/dmc.c but with the following difference:
// - It uses integer arithmetic.
// - The threshold for cloning a state increases as memory is used up.
// - Each state maintains both a 0,1 count and a bit history (as in a
//   context model).  The 0,1 count is best for stationary data, and the
//   bit history for nonstationary data.  The bit history is mapped to
//   a probability adaptively using a StateMap.  The two computed probabilities
//   are combined.
// - When memory is used up the state graph is reinitialized to a bytewise
//   order 1 context as in the original DMC.  However, the bit histories
//   are not cleared.

class dmcModel1: public Model {
  struct DMCNode {  // 12 bytes
  unsigned int nx[2];  // next pointers
  U8 state;  // bit history
  unsigned int c0:12, c1:12;  // counts * 256
  };
  BlockData& x;
  Buf& buf;
  int top, curr;  // allocated, current node
  Array<DMCNode> t;  // state graph
  StateMap sm;
  int threshold; 
public:
  dmcModel1(BlockData& bd,U32 val=0):x(bd),buf(bd.buf), top(0), curr(0),
   t(CMlimit(MEM()*sizeof(DMCNode))/sizeof(DMCNode)), threshold(256)  {
  }
int p(Mixer& m,int val1=0,int val2=0){
  // clone next state
  if (top>0 && top<t.size()) {
    int next=t[curr].nx[x.y];
    int n=x.y?t[curr].c1:t[curr].c0;
    int nn=t[next].c0+t[next].c1;
    if (n>=threshold*2 && nn-n>=threshold*3) {
      int r=n*4096/nn;
      assert(r>=0 && r<=4096);
      t[next].c0 -= t[top].c0 = t[next].c0*r>>12;
      t[next].c1 -= t[top].c1 = t[next].c1*r>>12;
      t[top].nx[0]=t[next].nx[0];
      t[top].nx[1]=t[next].nx[1];
      t[top].state=t[next].state;
      t[curr].nx[x.y]=top;
      ++top;
      if (top==(t.size()*4)/8) { //        5/8, 4/8, 3/8
        threshold=512;
      } else if (top==(t.size()*6)/8) { // 6/8, 6/8, 7/8
        threshold=768;
      }
    }
  }

  // Initialize to a bytewise order 1 model at startup or when flushing memory
  if (top==t.size() && x.bpos==1) top=0;
  if (top==0) {
    assert(t.size()>=65536);
    for (int i=0; i<256; ++i) {
      for (int j=0; j<256; ++j) {
        if (i<127) {
          t[j*256+i].nx[0]=j*256+i*2+1;
          t[j*256+i].nx[1]=j*256+i*2+2;
        }
        else {
          t[j*256+i].nx[0]=(i-127)*256;
          t[j*256+i].nx[1]=(i+1)*256;
        }
        t[j*256+i].c0=128;
        t[j*256+i].c1=128;
      }
    }
    top=65536;
    curr=0;
    threshold=256;
  }

  // update count, state
   if (x.y) {
    if (t[curr].c1<=3840) t[curr].c1+=256;
   } else  if (t[curr].c0<=3840)   t[curr].c0+=256;
  t[curr].state=nex(t[curr].state, x.y);
  curr=t[curr].nx[x.y];

  // predict
  const int pr1=sm.p(t[curr].state,x.y);
  const int n1=t[curr].c1;
  const int n0=t[curr].c0;
  const int pr2=(n1+5)*4096/(n0+n1+10);
  m.add(stretch(pr1));
  m.add(stretch(pr2));
  return 0;
}
~dmcModel1(){ }
};

class nestModel1: public Model {
  BlockData& x;
  Buf& buf;
  int ic, bc, pc,vc, qc, lvc, wc;
  ContextMap cm;
public:
  nestModel1(BlockData& bd,U32 val=0):x(bd),buf(bd.buf), ic(0), bc(0),
   pc(0),vc(0), qc(0), lvc(0), wc(0), cm(CMlimit(MEM()/2), 14-4)  {
  }
int p(Mixer& m,int val1=0,int val2=0){
  if (x.bpos==0) {
    int c=x.c4&255, matched=1, vv;
    const int lc = (c >= 'A' && c <= 'Z'?c+'a'-'A':c);
    if (lc == 'a' || lc == 'e' || lc == 'i' || lc == 'o' || lc == 'u') vv = 1; else
    if (lc >= 'a' && lc <= 'z') vv = 2; else
    if (lc == ' ' || lc == '.' || lc == ',' || lc == '!' || lc == '?' || lc == '\n') vv = 3; else
    if (lc >= '0' && lc <= '9') vv = 4; else
    if (lc == 'y') vv = 5; else
    if (lc == '\'') vv = 6; else vv=(c&32)?7:0;
    vc = (vc << 3) | vv;
    if (vv != lvc) {
      wc = (wc << 3) | vv;
      lvc = vv;
    }
    switch(c) {
      case ' ': qc = 0; break;
      case '(': ic += 513; break;
      case ')': ic -= 513; break;
      case '[': ic += 17; break;
      case ']': ic -= 17; break;
      case '<': ic += 23; qc += 34; break;
      case '>': ic -= 23; qc /= 5; break;
      case ':': pc = 20; break;
      case '{': ic += 22; break;
      case '}': ic -= 22; break;
      case '|': pc += 223; break;
      case '"': pc += 0x40; break;
      case '\'': pc += 0x42; break;
      case '\n': pc = qc = 0; break;
      case '.': pc = 0; break;
      case '!': pc = 0; break;
      case '?': pc = 0; break;
      case '#': pc += 0x08; break;
      case '%': pc += 0x76; break;
      case '$': pc += 0x45; break;
      case '*': pc += 0x35; break;
      case '-': pc += 0x3; break;
      case '@': pc += 0x72; break;
      case '&': qc += 0x12; break;
      case ';': qc /= 3; break;
      case '\\': pc += 0x29; break;
      case '/': pc += 0x11;
                if (buf.size() > 1 && buf(1) == '<') qc += 74;
                break;
      case '=': pc += 87; break;
      default: matched = 0;
    }
    if (matched) bc = 0; else bc += 1;
    if (bc > 300) bc = ic = pc = qc = 0;

    cm.set((3*vc+77*pc+373*ic+qc)&0xffff);
    cm.set((31*vc+27*pc+281*qc)&0xffff);
    cm.set((13*vc+271*ic+qc+bc)&0xffff);
    cm.set((17*pc+7*ic)&0xffff);
    cm.set((13*vc+ic)&0xffff);
    cm.set((vc/3+pc)&0xffff);
    cm.set((7*wc+qc)&0xffff);
    cm.set((vc&0xffff)|((x.f4&0xf)<<16));
    cm.set(((3*pc)&0xffff)|((x.f4&0xf)<<16));
    cm.set((ic&0xffff)|((x.f4&0xf)<<16));
  }
  cm.mix(m);
  return 0;
}
~nestModel1(){ }
};


class sparseModelx: public Model {
   ContextMap cm;
   SmallStationaryContextMap scm1, scm2, scm3,
   scm4, scm5,scm6, scma;
   BlockData& x;
   Buf& buf;
public:
  sparseModelx(BlockData& bd): cm(CMlimit(MEM()*4), 31),scm1(0x10000), scm2(0x20000), scm3(0x2000),
     scm4(0x8000), scm5(0x2000),scm6(0x2000), scma(0x10000),x(bd),buf(bd.buf) {
    }
  int p(Mixer& m, int seenbefore, int howmany){
  if (x.bpos==0) {
    scm5.set(seenbefore);
    scm6.set(howmany);
  /*  cm.set(x4&0x00ff00ff);
    cm.set(x4&0xff0000ff);
    cm.set(x4&0x00ffff00);
    cm.set((x4&0xf8f8f8f8));
    cm.set((x4&0x80f0f0ff));*/
  U32  h=x.x4<<6;
    cm.set(buf(1)+(h&0xffffff00));
    cm.set(buf(1)+(h&0x00ffff00));
    cm.set(buf(1)+(h&0x0000ff00));
      U32 d=x.c4&0xffff;
     h<<=6;
    cm.set(d+(h&0xffff0000));
    cm.set(d+(h&0x00ff0000));
     h<<=6, d=x.c4&0xffffff;
    cm.set(d+(h&0xff000000));

    for (int i=1; i<5; ++i) { 
      cm.set(seenbefore|buf(i)<<8);
      cm.set((x.buf(i+3)<<8)|buf(i+1));
    }
    cm.set(x.spaces&0x7fff);
    cm.set(x.spaces&0xff);
    cm.set(x.words&0x1ffff);
    cm.set(x.f4&0x000fffff);
    cm.set(x.tt&0x00000fff);
      h=x.w4<<6;
    cm.set(buf(1)+(h&0xffffff00));
    cm.set(buf(1)+(h&0x00ffff00));
    cm.set(buf(1)+(h&0x0000ff00));
      d=x.c4&0xffff;
     h<<=6;
    cm.set(d+(h&0xffff0000));
    cm.set(d+(h&0x00ff0000));
     h<<=6, d=x.c4&0xffffff;
    cm.set(d+(h&0xff000000));
    cm.set(x.w4&0xf0f0f0ff);
    
    //cm.set(f4);
    cm.set((x.w4&63)*128+(5<<17));
    cm.set((x.f4&0xffff)<<11|x.frstchar);
    cm.set(x.spafdo*8*((x.w4&3)==1));
    
      scm1.set(x.words&127);
      scm2.set((x.words&12)*16+(x.w4&12)*4+(x.f4&0xf));
      scm3.set(x.w4&15);
      scm4.set(x.spafdo*((x.w4&3)==1));
      scma.set(x.frstchar);
  }
  x.rm1=0;
  cm.mix(m);
  x.rm1=1;
  scm1.mix(m);
  scm2.mix(m);
  scm3.mix(m);
  scm4.mix(m);
  scm5.mix(m);
  scm6.mix(m);
  scma.mix(m);
  return 0;
  }
  ~sparseModelx(){ }
};


class normalModel1: public Model {
  BlockData& x;
  Buf& buf;
  ContextMap cm;
  RunContextMap rcm7, rcm9, rcm10;
  Array<U32> cxt; // order 0-11 contexts
public:
  normalModel1(BlockData& bd,U32 val=0):x(bd),buf(bd.buf), cm(CMlimit(MEM()*32), 9),rcm7(CMlimit(MEM()/4),bd),
  rcm9(CMlimit(MEM()/4),bd), rcm10(CMlimit(MEM()/2),bd), cxt(14){
 }
int p(Mixer& m,int val1=0,int val2=0){
  
  int primes[]={ 0, 257,251,241,239,233,229,227,223,211,199,197,193,191,181,179,173};   

  if (x.bpos==0) {
    int i;
    if((buf(2)=='.'||buf(2)=='!'||buf(2)=='?' ||buf(2)=='}') && buf(3)!=10 && 
    (x.filetype==DICTTXT || x.filetype==BIGTEXT ||x.filetype==TXTUTF8 ||x.filetype==TEXT)) for (i=13; i>0; --i)  // update order 0-11 context hashes
      cxt[i]=cxt[i-1]*primes[i];
      
    for (i=13; i>0; --i)  // update order 0-11 context hashes
      cxt[i]=cxt[i-1]*primes[i]+(x.c4&255);
    for (i=0; i<7; ++i)
      cm.set(cxt[i]);
    rcm7.set(cxt[7]);
    cm.set(cxt[8]);
    rcm9.set(cxt[10]);
    
    rcm10.set(cxt[12]);
    cm.set(cxt[13]);
  }
  rcm7.mix(m);
  rcm9.mix(m);
  rcm10.mix(m);
  return cm.mix(m);
}
  ~normalModel1(){ }
};


#include "mod_ppmd.inc"
class ppmdModel1: public Model {
  BlockData& x;
  Buf& buf;
   ppmd_Model ppmd_12_256_1;
  ppmd_Model ppmd_6_32_1;
public:
  ppmdModel1(BlockData& bd,U32 val=0):x(bd),buf(bd.buf){
  	int ppmdmem=(210<<(x.clevel>8))<<(x.clevel>13);
  	ppmd_12_256_1.Init(12+(x.clevel>8),ppmdmem,1,0);
    ppmd_6_32_1.Init(3<<(x.clevel>8),16<<(x.clevel>8),1,0);
 }
int p(Mixer& m,int val1=0,int val2=0){
  
m.add( stretch(4096-ppmd_12_256_1.ppmd_Predict(4096,x.y)) );
  m.add( stretch(4096-ppmd_6_32_1.ppmd_Predict(4096,x.y)) );

}
  ~ppmdModel1(){ }
};

//Name - add per context
//CM      5+1  (context)
//SmallCM 2    (smallcontext)
//RCM     1    (run)
//SCM     1    (small)
//M set add    (mixer)
//
//recordModel1    M 0 0 CM 3 3 3 3                =3*6+3*6+3*6+3*6=     72 inputs
//im8bitModel1    M 6 0 SCM 6 SmallCM 45          =6*1+45*2=            96 inputs  6 sets +24inputs
//im24bitModel1   M 4 0 SCM 10 CM 15              =10*1+15*6=           100 inputs 4 sets
//recordModelx    M 0 0 CM 2 5 4 3 3              =2*6+5*6+4*6+3*6+3*6= 102 inputs
//sparseModelx    M 0 0 SCM 7 CM 31               =7*1+31*6=            193 inputs
//jpegModelx      M 3 2                           =                     2 inputs   3 sets
//matchModel1     M 0 2 SCM 1                     =2+1*1=               3 inputs
//distanceModel1  M 0 0 CM 3                      =3*6=                 18 inputs
//sparseModely    M 0 0 CM 42                     =42*6=                252 inputs
//wordModel1      M 0 0 CM 45                     =45*6=                270 264 inputs
//exeModel1       M 0 0 CM 14                     =14*6=                84  inputs
//indirectModel1  M 0 0 CM 12                     =12*6=                72 inputs
//dmcModel1       M 0 2                           =                     2 inputs
//nestModel1      M 0 0 CM 10                     =10*6=                60 inputs
//normalModel1    M 0 0 CM 9 RCM 1 1 1            =9*6+1*1+1*1+1*1=     57 inputs
//im1bitModel1    M 4 8                           =                     8 inputs   4 sets

//////////////////////////// Predictor /////////////////////////

// A Predictor estimates the probability that the next bit of
// uncompressed data is 1.  Methods:
// p() returns P(1) as a 12 bit number (0-4095).
// update(y) trains the predictor with the actual bit (0 or 1).

//base class
class Predictors {
public:
  BlockData x; //maintains current global data block between models
  //list of all models
  recordModel1* recordModel;
  im8bitModel1* im8bitModel;
  im24bitModel1* im24bitModel;
  recordModelx* recordModelw;
  sparseModelx* sparseModel1;
  jpegModelx* jpegModel;
  matchModel1* matchModel;
  distanceModel1* distanceModel;
  sparseModely* sparseModel;
  wordModel1* wordModel;
  exeModel1* exeModel;
  indirectModel1* indirectModel;
  dmcModel1* dmcModel;
  nestModel1* nestModel;
  normalModel1* normalModel;
  im1bitModel1* im1bitModel;
ppmdModel1* ppmdModel;
virtual ~Predictors(){
  if (jpegModel!=0) delete jpegModel;
  if (sparseModel1!=0) delete sparseModel1;
  if (im8bitModel!=0) delete im8bitModel;
  if (im24bitModel!=0) delete im24bitModel;
  if (recordModelw!=0) delete recordModelw;
  if (exeModel!=0) delete exeModel;
  if (recordModel!=0) delete recordModel; 
  if (distanceModel!=0) delete distanceModel; 
  if (sparseModel!=0) delete sparseModel; 
  if (wordModel!=0) delete wordModel; 
  if (indirectModel!=0) delete indirectModel; 
  if (dmcModel!=0) delete dmcModel; 
  if (nestModel!=0) delete nestModel; 
  if (matchModel!=0) delete matchModel;
  if (normalModel!=0) delete normalModel;
  if (im1bitModel!=0) delete im1bitModel; 
  if (ppmdModel!=0) delete ppmdModel; 
   };
Predictors(){
  recordModel=0;
  im8bitModel=0;
  im24bitModel=0;
  recordModelw=0;
  recordModel=0;
  sparseModel1=0;
  jpegModel=0;
  matchModel=0;
  distanceModel=0;
  sparseModel=0;
  wordModel=0;
  exeModel=0;
  indirectModel=0;
  dmcModel=0;
  nestModel=0;
  normalModel=0;
  im1bitModel=0;
  ppmdModel=0;
}
  virtual int p() const =0;
  virtual void update()=0;
  void update0(){
    // Update global context: pos, bpos, c0, c4, buf
    x.c0+=x.c0+x.y;
    if (x.c0>=256) {
        x.buf[x.buf.pos++]=x.c0;
        x.c0=1;
        ++x.blpos;
      x.buf.pos=x.buf.pos&x.buf.poswr; //wrap
    }
    x.bpos=(x.bpos+1)&7;
  }
};

//general predicor class
class Predictor: public Predictors {
  int pr;  // next prediction
  int pr0;
  int order;
  int ismatch;
  Mixer m;
  APM1 a, a1, a2, a3, a4, a5, a6;
  void setmixer();
public:
  Predictor();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~Predictor(){
  }
};

Predictor::Predictor(): pr(2048),pr0(pr),order(0),ismatch(0), m(809+6+1+2+6+2, 7432,x, 7),
 a(256,x), a1(0x10000,x), a2(0x10000,x), a3(0x10000,x), a4(0x10000,x), a5(0x10000,x), a6(0x10000,x) {
  //=1+72+18+252+264+72+2+60+3+57+8=809 
  if (x.clevel>=4 && recordModel==0) recordModel=new recordModel1(x); 
  if (x.clevel>=4 && distanceModel==0) distanceModel=new distanceModel1(x); 
  if (x.clevel>=4 && sparseModel==0) sparseModel=new sparseModely(x); 
  if (x.clevel>=4 && wordModel==0) wordModel=new wordModel1(x); 
  if (x.clevel>=4 && indirectModel==0) indirectModel=new indirectModel1(x); 
  if (x.clevel>=4 && dmcModel==0) dmcModel=new dmcModel1(x);
  if (x.clevel>=4 && nestModel==0) nestModel=new nestModel1(x); 
  if (x.clevel>=4 && ppmdModel==0) ppmdModel=new ppmdModel1(x);
  matchModel=new matchModel1(x);
  normalModel=new normalModel1(x);
  im1bitModel=new im1bitModel1(x);
}
//264+256+1024+2048+2048+256+1536=7431 
inline void Predictor::setmixer(){
  U32 c1=x.buf(1), c2=x.buf(2), c3=x.buf(3), c;
  m.set(c1+8, 264); 
  m.set(x.c0, 256);
  m.set(c2, 1024);
  U8 d=x.c0<<(8-x.bpos);
  m.set(order*256+(x.w4&240)+(x.b3>>4),2048);
  m.set(x.bpos*256+((x.words<<x.bpos&255)>>x.bpos|(d&255)),2048);
  m.set(ismatch, 256);
  if (x.bpos) {
    c=d; if (x.bpos==1)c+=c3/2;
    c=(min(x.bpos,5))*256+c1/32+8*(c2/32)+(c&192);
  }
  else c=c3/128+(x.c4>>31)*2+4*(c2/64)+(c1&240); 
  m.set(c, 1536);
  pr0=m.p();
}

void Predictor::update()  {
    update0();
    if (x.bpos==0) {
        int b1=x.buf(1);
        x.c4=(x.c4<<8)+b1;
        int i=WRT_mpw[b1>>4];
        x.w4=x.w4*4+i;
        if (x.b2==3) i=2;
        x.w5=x.w5*4+i;
        x.b3=x.b2;
        x.b2=b1;   
        x.x4=x.x4*256+b1,x.x5=(x.x5<<8)+b1;
        if(b1=='.' || b1=='!' || b1=='?' || b1=='/'|| b1==')'|| b1=='}') {
            x.w5=(x.w5<<8)|0x3ff,x.f4=(x.f4&0xfffffff0)+2,x.x5=(x.x5<<8)+b1,x.x4=x.x4*256+b1;
            if(b1!='!') x.w4|=12, x.tt=(x.tt&0xfffffff8)+1,x.b3='.';
        }
        if (b1==32) --b1;
        x.tt=x.tt*8+WRT_mtt[b1>>4];
        x.f4=x.f4*16+(b1>>4);
    }

    m.update();
    m.add(256);
    ismatch=ilog(matchModel->p(m));  // Length of longest matching context
    order=normalModel->p(m);
    order=order-2; if(order<0) order=0;
    if (x.clevel>=4){        
        int rlen=0;
        rlen=recordModel->p(m);
        //if (rlen==216) { //PIC
        //    im1bitModel->p(m, rlen);  //
       //     return;
            //pr=m.p();
       // }
       // else{
            wordModel->p(m);
            sparseModel->p(m,ismatch,order);
            distanceModel->p(m);
            indirectModel->p(m);
            nestModel->p(m);
            dmcModel->p(m);
            ppmdModel->p(m);
              
       // }
    } 
    setmixer(); 
    ///////////////////////////////
    // Filter the context model with APMs

    if (x.fails&0x00000080) --x.failcount;
    x.fails=x.fails*2;
    x.failz=x.failz*2;
    if (x.y) pr^=4095;
    if (pr>=1820) ++x.fails, ++x.failcount;
    if (pr>= 848) ++x.failz;
    int pv, pu,pz,pt;
    pu=(a.p(pr0, x.c0, 3)+7*pr0+4)>>3, pz=x.failcount+1;
    pz+=tri[(x.fails>>5)&3];
    pz+=trj[(x.fails>>3)&3];
    pz+=trj[(x.fails>>1)&3];
    if (x.fails&1) pz+=8;
    pz=pz/2;      

    int r=7;
    pu=a4.p(pu,   ( (x.c0*2)^hash(x.buf(1), (x.x5>>8)&255, (x.x5>>16)&0x80ff))&0xffff,r);
    pv=a2.p(pr0,  ( (x.c0*8)^hash(29,x.failz&2047))&0xffff,1+r);
    pv=a5.p(pv,           hash(x.c0,x.w5&0xfffff)&0xffff,r);
    pt=a3.p(pr0, ( (x.c0*32)^hash(19,     x.x5&0x80ffff))&0xffff,r);
    pz=a6.p(pu,   ( (x.c0*4)^hash(min(9,pz),x.x5&0x80ff))&0xffff,r);
    if (x.fails&255)  pr =(pt*6+pu  +pv*11+pz*14 +16)>>5;
    else              pr =(pt*4+pu*5+pv*12+pz*11 +16)>>5;

}

//JPEG predicor class
class PredictorJPEG: public Predictors {
  int pr;  // next prediction
  Mixer m;
public:
  PredictorJPEG();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
  ~PredictorJPEG(){
  
 }
};

PredictorJPEG::PredictorJPEG(): pr(2048), m(6, 2568,x, 5) {
  //1+3+2=5
  matchModel=new matchModel1(x); 
  jpegModel=new jpegModelx(x); 
}
//264+256+256+256+1536=2568
void PredictorJPEG::update()  {
    update0();
    m.update();
    m.add(256);
    int ismatch=ilog(matchModel->p(m));  // Length of longest matching context
    if (jpegModel->p(m)) { 
        pr= m.p();
    }
    else{
        
        U32 c1=x.buf(1), c2=x.buf(2), c3=x.buf(3), c;
        m.set(c1+8, 264); 
        m.set(x.c0, 256);
        m.set(c2, 256);
        m.set(ismatch, 256);
        U8 d=x.c0<<(8-x.bpos);
        if (x.bpos) {
            c=d; if (x.bpos==1)c+=c3/2;
            c=(min(x.bpos,5))*256+c1/32+8*(c2/32)+(c&192);
        }
        else c=c3/128+(x.c4>>31)*2+4*(c2/64)+(c1&240); 
        m.set(c, 1536);
        pr=m.p();
    }
}

//EXE predicor class
class PredictorEXE: public Predictors {
  int pr;  // next prediction
  Mixer m;
  APM1 a, a1, a2, a3, a4, a5, a6;
  void setmixer();
public:
  PredictorEXE();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
    ~PredictorEXE(){ }
};

PredictorEXE::PredictorEXE(): pr(2048), m(825+6+6, 3080,x, 7),
 a(256,x), a1(0x10000,x), a2(0x10000,x), a3(0x10000,x), a4(0x10000,x), a5(0x10000,x), a6(0x10000,x) {
  //   1+ 72+ 18+ 252+ 264+6+ 84 +72 + 2+ 3+ 57=825+6
  if (x.clevel>=4){
    if (recordModel==0) recordModel=new recordModel1(x); 
    if (distanceModel==0) distanceModel=new distanceModel1(x); 
    if (sparseModel==0) sparseModel=new sparseModely(x); 
    if (wordModel==0) wordModel=new wordModel1(x); 
    if (exeModel==0) exeModel=new exeModel1(x); 
    if (indirectModel==0) indirectModel=new indirectModel1(x);
    if (dmcModel==0) dmcModel=new dmcModel1(x);  
  }
  matchModel=new matchModel1(x); 
  normalModel=new normalModel1(x);
}
//264 +256+ 256+ 256 +256 +256+ 1536=3080
void PredictorEXE::update()  {
    update0();
    if (x.bpos==0) {
        int b1=x.buf(1);
        x.c4=(x.c4<<8)+b1;
        int i=WRT_mpw[b1>>4];
        x.w4=x.w4*4+i;
        if (x.b2==3) i=2;
        x.w5=x.w5*4+i;
        x.b3=x.b2;
        x.b2=b1;   
        x.x4=x.x4*256+b1,x.x5=(x.x5<<8)+b1;
        if(b1=='.' || b1=='!' || b1=='?' || b1=='/'|| b1==')'|| b1=='}') {
            x.w5=(x.w5<<8)|0x3ff,x.f4=(x.f4&0xfffffff0)+2,x.x5=(x.x5<<8)+b1,x.x4=x.x4*256+b1;
            if(b1!='!') x.w4|=12, x.tt=(x.tt&0xfffffff8)+1,x.b3='.';
        }
        if (b1==32) --b1;
        x.tt=x.tt*8+WRT_mtt[b1>>4];
        x.f4=x.f4*16+(b1>>4);
    }
    m.update();
    m.add(256);
    int ismatch=ilog(matchModel->p(m));  // Length of longest matching context
    int order=normalModel->p(m);
    order=order-2; if(order<0) order=0;
    if (x.clevel>=4 ){
        recordModel->p(m);
        wordModel->p(m);
        sparseModel->p(m,ismatch,order);
        distanceModel->p(m);
        indirectModel->p(m);
        dmcModel->p(m);
        exeModel->p(m);
    }
    U32 c1=x.buf(1), c2=x.buf(2), c3=x.buf(3), c;
    m.set(c1+8, 264); 
    m.set(x.c0, 256);
    m.set(c2, 256);
    U8 d=x.c0<<(8-x.bpos);
    m.set(order+8*(x.c4>>6&3)+32*(x.bpos==0)+64*(c1==c2)+(c2==0x8b || c2==0x89)*128, 256); //8b 89 for mov
    m.set(c3, 256);
    m.set(ismatch, 256);
    if (x.bpos) {
        c=d; if (x.bpos==1)c+=c3/2;
        c=(min(x.bpos,5))*256+c1/32+8*(c2/32)+(c&192);
    }
    else c=c3/128+(x.c4>>31)*2+4*(c2/64)+(c1&240); 
    m.set(c, 1536);
    int pr0=m.p();
    ///////////////////////////////
    // Filter the context model with APMs

    if (x.fails&0x00000080) --x.failcount;
    x.fails=x.fails*2;
    x.failz=x.failz*2;
    if (x.y) pr^=4095;
    if (pr>=1820) ++x.fails, ++x.failcount;
    if (pr>= 848) ++x.failz;
    int pv, pu,pz,pt;
    pu=(a.p(pr0, x.c0, 3)+7*pr0+4)>>3, pz=x.failcount+1;
    pz+=tri[(x.fails>>5)&3];
    pz+=trj[(x.fails>>3)&3];
    pz+=trj[(x.fails>>1)&3];
    if (x.fails&1) pz+=8;
    pz=pz/2;      

    int r=6;
    pu=a4.p(pu,   ( (x.c0*2)^hash(x.buf(1), (x.x5>>8)&255, (x.x5>>16)&0x80ff))&0xffff,r);
    pv=a2.p(pr0,  ( (x.c0*8)^hash(29,x.failz&2047))&0xffff,1+r);
    pv=a5.p(pv,           hash(x.c0,x.w5&0xfffff)&0xffff,r);
    pt=a3.p(pr0, ( (x.c0*32)^hash(19,     x.x5&0x80ffff))&0xffff,r);
    pz=a6.p(pu,   ( (x.c0*4)^hash(min(9,pz),x.x5&0x80ff))&0xffff,r);
    if (x.fails&255)  pr =(pt*6+pu  +pv*11+pz*14 +16)>>5;
    else              pr =(pt*4+pu*5+pv*12+pz*11 +16)>>5;
}

//IMG4 predicor class
class PredictorIMG4: public Predictors {
  int pr;  // next prediction
  Mixer m;
  APM1 a, a1, a2, a3, a4, a5, a6;
public:
  PredictorIMG4();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~PredictorIMG4(){
  }
};

PredictorIMG4::PredictorIMG4(): pr(2048), m(78+6, 2568,x, 5),
 a(256,x), a1(0x10000,x), a2(0x10000,x), a3(0x10000,x), a4(0x10000,x), a5(0x10000,x), a6(0x10000,x) {
   //1 +3 +72+ 2=  78               
  matchModel=new matchModel1(x);   
  recordModel=new recordModel1(x);
  dmcModel=new dmcModel1(x); 
}
//264 +256+ 256+ 1536+  256 =2568
void PredictorIMG4::update()  {
  update0();
  if (x.bpos==0) {
    int b1=x.buf(1);
    x.c4=(x.c4<<8)+b1;
}
  m.update();
  m.add(256);
  int ismatch=ilog(matchModel->p(m));  // Length of longest matching context
  dmcModel->p(m);
  recordModel->p(m, x.finfo);
  U32 c1=x.buf(1), c2=x.buf(2), c3=x.buf(3), c;
  m.set(c1+8, 264); 
  m.set(x.c0, 256);
  m.set(c2, 256);
  U8 d=x.c0<<(8-x.bpos);
  m.set(ismatch, 256);
  if (x.bpos) {
    c=d; if (x.bpos==1)c+=c3/2;
    c=(min(x.bpos,5))*256+c1/32+8*(c2/32)+(c&192);
  }
  else c=c3/128+(x.c4>>31)*2+4*(c2/64)+(c1&240); 
  m.set(c, 1536);
  int pr0=m.p();
  
  ///////////////////////////////
  // Filter the context model with APMs
      if (x.fails&0x00000080) --x.failcount;
        x.fails=x.fails*2;
        x.failz=x.failz*2;
        if (x.y) pr^=4095;
        if (pr>=1820) ++x.fails, ++x.failcount;
        if (pr>= 848) ++x.failz;
      int pv, pu,pz,pt;
        pu=(a.p(pr0, x.c0, 3)+7*pr0+4)>>3, pz=x.failcount+1;
        pz+=tri[(x.fails>>5)&3];
        pz+=trj[(x.fails>>3)&3];
        pz+=trj[(x.fails>>1)&3];
        if (x.fails&1) pz+=8;
        pz=pz/2;      
 
     int r=7;
   pu=a4.p(pu,  ( (x.c0*2)^hash(x.buf(1), 0, 0))&0xffff,r);
  pv=a2.p(pr0,  ( (x.c0*8)^hash(29,x.failz&2047))&0xffff,1+r);
  pv=a5.p(pv,           hash(x.c0,0)&0xffff,r);
  pt=a3.p(pr0,  ( (x.c0*32)^hash(19,    0))&0xffff,r);
  pz=a6.p(pu,   ( (x.c0*4)^hash(min(9,pz),0))&0xffff,r);
  if (x.fails&255)  pr =(pt*6+pu  +pv*11+pz*14 +16)>>5;
  else              pr =(pt*4+pu*5+pv*12+pz*11 +16)>>5;

}

//IMG8 predicor class
class PredictorIMG8: public Predictors {
  int pr;  // next prediction
  Mixer m;
  APM1 a, a1, a2, a3, a4, a5, a6;
public:
  PredictorIMG8();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~PredictorIMG8(){
  }
};

PredictorIMG8::PredictorIMG8(): pr(2048), m((100 +24+8*6)*3 , 2864,x, 7),
 a(256,x), a1(0x10000,x), a2(0x10000,x), a3(0x10000,x), a4(0x10000,x), a5(0x10000,x), a6(0x10000,x) {
  //1 +3 +96=  100                
  matchModel=new matchModel1(x);   
  im8bitModel=new im8bitModel1(x); 
  //recordModel=new recordModel1(x);
}
//  256 +8 +8 +32 +256 +512+ 1792=2864

void PredictorIMG8::update()  {
  update0();
   int b1=0;
  if (x.bpos==0) {
     b1=x.buf(1);
    x.c4=(x.c4<<8)+x.buf(1);
}
   
  m.update();
  m.add(256);
  int ismatch=ilog(matchModel->p(m));  // Length of longest matching context
  m.set(ismatch, 256);
  im8bitModel->p(m,x.finfo);
  //recordModel->p(m, x.finfo);
  int pr0=m.p();
  
  ///////////////////////////////
  // Filter the context model with APMs
 
  pr=a.p(pr0, x.c0);

  int pr1=a1.p(pr0, x.c0+256*x.buf(1));
  int pr2=a2.p(pr0, (x.c0^hash(x.buf(1), x.buf(2)))&0xffff);
  int pr3=a3.p(pr0, (x.c0^hash(x.buf(1), x.buf(2), x.buf(3)))&0xffff);
  pr0=(pr0+pr1+pr2+pr3+2)>>2;

  pr1=a4.p(pr, x.c0+256*x.buf(1));
  pr2=a5.p(pr, (x.c0^hash(x.buf(1), x.buf(2)>>1))&0xffff);
  pr3=a6.p(pr, (x.c0^hash(x.buf(1), x.buf(2)>>1, x.buf(3)>>2))&0xffff);
  pr=(pr+pr1+pr2+pr3+2)>>2;

  pr=(pr+pr0+1)>>1;

}

//IMG24 predicor class
class PredictorIMG24: public Predictors {
  int pr;  // next prediction
  Mixer m;
  APM1 a, a1, a2, a3, a4, a5, a6;
public:
  PredictorIMG24();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~PredictorIMG24(){
  }
};

PredictorIMG24::PredictorIMG24(): pr(2048), m(104, 336+256,x, 4+1),
 a(256,x), a1(0x10000,x), a2(0x10000,x), a3(0x10000,x), a4(0x10000,x), a5(0x10000,x), a6(0x10000,x) {
  //   1+ 3 +100=104
  matchModel=new matchModel1(x); 
  im24bitModel=new im24bitModel1(x);
}
//8+ 24+ 48 +256
void PredictorIMG24::update()  {
  update0();
  if (x.bpos==0) {
    int b1=x.buf(1);
    x.c4=(x.c4<<8)+b1;
        int i=WRT_mpw[b1>>4];
        x.w5=x.w5*4+i;
        x.x5=(x.x5<<8)+b1;
}
   
  m.update();
  m.add(256);
  int ismatch=ilog(matchModel->p(m));  // Length of longest matching context
  m.set(ismatch,256);
  im24bitModel->p(m,x.finfo);
  int pr0=m.p();
  
  ///////////////////////////////
  // Filter the context model with APMs
 {
     if (x.fails&0x00000080) --x.failcount;
        x.fails=x.fails*2;
        x.failz=x.failz*2;
        if (x.y) pr^=4095;
        if (pr>=1820) ++x.fails, ++x.failcount;
        if (pr>= 848) ++x.failz;
      int pv, pu,pz,pt;
        pu=(a.p(pr0, x.c0, 3)+7*pr0+4)>>3, pz=x.failcount+1;
        pz+=tri[(x.fails>>5)&3];
        pz+=trj[(x.fails>>3)&3];
        pz+=trj[(x.fails>>1)&3];
        if (x.fails&1) pz+=8;
        pz=pz/2;      
 
     int r=7;
   pu=a4.p(pu,   ( (x.c0*2)^hash(x.buf(1), (x.x5>>8)&255, (x.x5>>16)&0x80ff))&0xffff,r);
  pv=a2.p(pr0,  ( (x.c0*8)^hash(29,x.failz&2047))&0xffff,1+r);
  pv=a5.p(pv,           hash(x.c0,x.w5&0xfffff)&0xffff,r);
  pt=a3.p(pr0, ( (x.c0*32)^hash(19,     x.x5&0x80ffff))&0xffff,r);
  pz=a6.p(pu,   ( (x.c0*4)^hash(min(9,pz),x.x5&0x80ff))&0xffff,r);
  if (x.fails&255)  pr =(pt*6+pu  +pv*11+pz*14 +16)>>5;
  else              pr =(pt*4+pu*5+pv*12+pz*11 +16)>>5;
}
}
//TEXT predicor class
class PredictorTXTWRT: public Predictors {
  int pr;  // next prediction
  int pr0;
  int order;
  int ismatch;
  Mixer m;
  APM1 a, a1, a2, a3, a4, a5, a6;
  void setmixer();
public:
  PredictorTXTWRT();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~PredictorTXTWRT(){
  }
};

PredictorTXTWRT::PredictorTXTWRT(): pr(2048),pr0(pr),order(0),ismatch(0),
 m(925+6+2, 10240-256*2,x, 7),a(256,x),a1(0x10000,x),a2(0x10000,x),a3(0x10000,x),
 a4(0x10000,x),a5(0x10000,x),a6(0x10000,x) {
  if (x.clevel>=4){
    if (recordModelw==0) recordModelw=new recordModelx(x);
    if (sparseModel1==0) sparseModel1=new sparseModelx(x);
    if (wordModel==0) wordModel=new wordModel1(x);
    if (indirectModel==0) indirectModel=new indirectModel1(x);
    if (dmcModel==0) dmcModel=new dmcModel1(x);
    if (nestModel==0) nestModel=new nestModel1(x);
    if (ppmdModel==0) ppmdModel=new ppmdModel1(x);
  }
  matchModel=new matchModel1(x);
  normalModel=new normalModel1(x);
}
//264+256+1024+2048+2048+256+1536=7432 
//256*8 +256 +256 +256*8+ 256*8 +1536 +2048=10240
inline void PredictorTXTWRT::setmixer(){
  U32 c1=x.buf(1), c2=x.buf(2), c3=x.buf(3), c;
  m.set(c1+8, 264); 
  m.set(x.c0, 256);
  c=(c2&0x1F)|((c3&0x1F)<<5);
  m.set(c, 1024);
  U8 d=x.c0<<(8-x.bpos);
  m.set(order*256+(x.w4&240)+(x.b3>>4),256*7);
  m.set(x.bpos*256+((x.words<<x.bpos&255)>>x.bpos|(d&255)),2048);
  m.set(ismatch, 256);
  if (x.bpos) {
    c=d; if (x.bpos==1)c+=c3/2;
    c=(min(x.bpos,5))*256+c1/32+8*(c2/32)+(c&192);
  }
  else c=c3/128+(x.c4>>31)*2+4*(c2/64)+(c1&240); 
  m.set(c, 1536);
  pr0=m.p();
}

void PredictorTXTWRT::update()  {
    update0();
    if (x.bpos==0) {
        int b1=x.buf(1);
        x.c4=(x.c4<<8)+b1;
        int i=WRT_mpw[b1>>4];
        x.w4=x.w4*4+i;
        if (x.b2==3) i=2;
        x.w5=x.w5*4+i;
        x.b3=x.b2;
        x.b2=b1;   
        x.x4=x.x4*256+b1,x.x5=(x.x5<<8)+b1;
        if(b1=='.' || b1=='!' || b1=='?' || b1=='/'|| b1==')'|| b1=='}') {
            x.w5=(x.w5<<8)|0x3ff,x.f4=(x.f4&0xfffffff0)+2,x.x5=(x.x5<<8)+b1,x.x4=x.x4*256+b1;
            if(b1!='!') x.w4|=12, x.tt=(x.tt&0xfffffff8)+1,x.b3='.';
        }
        if (b1==32) --b1;
        x.tt=x.tt*8+WRT_mtt[b1>>4];
        x.f4=x.f4*16+(b1>>4);
    }

    m.update();
    m.add(256);
    ismatch=ilog(matchModel->p(m));  // Length of longest matching context
    order=normalModel->p(m);
    order=order-3; if(order<0) order=0;
    if (x.clevel>=4){        
        wordModel->p(m);
        sparseModel1->p(m,ismatch,order);
        nestModel->p(m);
        indirectModel->p(m);
        dmcModel->p(m);
        recordModelw->p(m);
        ppmdModel->p(m);
        //256*8 +256 +256 +256*8+ 256*8 +1536 +2048=10240
        U32 c3=x.buf(3), c;
        c=(x.words>>1)&63;
        m.set((x.w4&3)*64+c+order*256, 256*7); 
        m.set(x.c0, 256);
        m.set(ismatch, 256);
        m.set(256*order + (x.w4&240) + (x.b3>>4), 256*7);
        c=(x.w4&255)+256*x.bpos;
        m.set(c, 256*8);
        if (x.bpos){
            c=x.c0<<(8-x.bpos); if (x.bpos==1)c+=c3/2;
            c=(min(x.bpos,5))*256+(x.tt&63)+(c&192);
        }
        else c=(x.words&12)*16+(x.tt&63);
        m.set(c, 1536);
        c=x.bpos*256+((x.c0<<(8-x.bpos))&255);
        c3 = (x.words<<x.bpos) & 255;
        m.set(c+(c3>>x.bpos), 2048);
        pr0=m.p();}
    else{
        setmixer();
    }
    ///////////////////////////////
    // Filter the context model with APMs

    if (x.fails&0x00000080) --x.failcount;
    x.fails=x.fails*2;
    x.failz=x.failz*2;
    if (x.y) pr^=4095;
    if (pr>=1820) ++x.fails, ++x.failcount;
    if (pr>= 848) ++x.failz;
    int pv, pu,pz,pt;
    pu=(a.p(pr0, x.c0, 3)+7*pr0+4)>>3, pz=x.failcount+1;
    pz+=tri[(x.fails>>5)&3];
    pz+=trj[(x.fails>>3)&3];
    pz+=trj[(x.fails>>1)&3];
    if (x.fails&1) pz+=8;
    pz=pz/2;      

    int r=7;
    pu=a4.p(pu,   ( (x.c0*2)^hash(x.buf(1), (x.x5>>8)&255, (x.x5>>16)&0x80ff))&0xffff,r);
    pv=a2.p(pr0,  ( (x.c0*8)^hash(29,x.failz&2047))&0xffff,1+r);
    pv=a5.p(pv,           hash(x.c0,x.w5&0xfffff)&0xffff,r);
    pt=a3.p(pr0, ( (x.c0*32)^hash(19,     x.x5&0x80ffff))&0xffff,r);
    pz=a6.p(pu,   ( (x.c0*4)^hash(min(9,pz),x.x5&0x80ff))&0xffff,r);
    if (x.fails&255)  pr =(pt*6+pu  +pv*11+pz*14 +16)>>5;
    else              pr =(pt*4+pu*5+pv*12+pz*11 +16)>>5;
}

//IMG1 predicor class
class PredictorIMG1: public Predictors {
  int pr;  // next prediction
  Mixer m;
public:
  PredictorIMG1();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~PredictorIMG1(){
  }
};

PredictorIMG1::PredictorIMG1(): pr(2048), m(12, 12801+256,x, 7) {
    //8+3+1=12
  matchModel=new matchModel1(x); 
  im1bitModel=new im1bitModel1(x); 
}
//256*4+256=1280
void PredictorIMG1::update()  {
  // Update global context: pos, bpos, c0, c4, buf
  update0();
  m.update();
  m.add(256);
  int ismatch=ilog(matchModel->p(m));  // Length of longest matching context
  m.set(ismatch,256);
  im1bitModel->p(m, x.finfo);
  pr=m.p(); 
}

//AUDIO predicor class
class PredictorAUDIO: public Predictors {
  int pr,col;  // next prediction
  Mixer m;
  SmallStationaryContextMap scm1,scm2;
public:
  PredictorAUDIO();
  int p()  const {assert(pr>=0 && pr<4096); return pr;} 
  void update() ;
   ~PredictorAUDIO(){
  }
};

PredictorAUDIO::PredictorAUDIO(): pr(2048),col(0), m(80, 4*256+32,x, 3+2),
scm1(0x100000), scm2(0x1000) {
    //1+2+3+2+ 72=80
 dmcModel=new dmcModel1(x); 
 matchModel=new matchModel1(x);
 if (x.clevel>=4 && indirectModel==0) indirectModel=new indirectModel1(x);
}

void PredictorAUDIO::update()  {
  update0();
   if (x.bpos==0) {
    int b1=x.buf(1);
    x.c4=(x.c4<<8)+b1;
}
  m.update();
  m.add(256);
  int ismatch=ilog(matchModel->p(m)); 
  
  if (x.bpos==0){
     scm1.set(hash(x.c4& 0xfffff,x.blpos%4)& 0xfffff);
     scm2.set(hash(x.c4& 0xff,x.blpos%4)& 0xfff);
  }
  scm1.mix(m);
  scm2.mix(m);
  if (x.clevel>=4)indirectModel->p(m);
  dmcModel->p(m);
  U32 c1=x.buf(1), c2=x.buf(2);
  m.set(x.c0, 256);
  m.set(c1, 256);
  m.set(c2, 256);
  m.set(ismatch, 256);
  if (++col>=32) col=0;
  m.set(col, 32);
  pr= m.p();
}

class PredictorFast: public Predictors {
  int cxt[4];
  int cxt1,cxt2,cxt3,cxt4;
  int pr0;
  StateMap *sm;
  Array<U8> t; 
  APM1 a;
  const int MAXLEN;  // longest allowed match + 1
    Array<int> tm;  // hash table of pointers to contexts
    int h;  // hash of last 7 bytes
    U32 ptr;  // points to next byte of match if any
    int len;  // length of match, or 0 if no match
    int mm1,mm2;
public:
  PredictorFast(): cxt1(0),cxt2(0),cxt3(0),cxt4(0),pr0(2048),t(0x80000),a(256,x),MAXLEN(65534), 
    tm(CMlimit(MEM()/4)),h(0), ptr(0),len(0),mm1(2048),mm2(2048){
      sm=new StateMap[4];
      cxt[0]=cxt[1]=cxt[2]=cxt[3]=0;     
  }

  int p() const {
    return pr0;
  }

  void update() {
    update0();
    if (x.bpos==0){
        cxt4=cxt3;
        cxt3=cxt2;
        cxt2=cxt1*256;
        cxt1=x.buf(1);
        //update match

        h=(h*997*8+cxt1+1)&(tm.size()-1);  // update context hash
            if (len) ++len, ++ptr;
            else {  // find match
                ptr=tm[h];
                if (ptr && x.buf.pos-ptr<x.buf.size())
                while (x.buf(len+1)==x.buf[ptr-len-1] && len<MAXLEN) ++len;
            }
            tm[h]=x.buf.pos;  // update hash table
            //    if (result>0 && !(result&0xfff)) printf("pos=%d len=%d ptr=%d\n", pos, len, ptr);
    }
    for (int i=0; i<4; ++i){
        t[cxt[i]]=nex(t[cxt[i]],x.y);
    }
    cxt[0]=cxt1*256+x.c0;
    cxt[1]=cxt2+x.c0+0x10000;
    cxt[2]=cxt3+x.c0+0x20000;
    cxt[3]=cxt4+x.c0+0x40000;

    pr0=(sm[0].p(t[cxt[0]],x.y)+sm[1].p(t[cxt[1]],x.y)+sm[2].p(t[cxt[2]],x.y)+sm[3].p(t[cxt[3]],x.y))>>2;
        
        // predict match
            if (len>0 && x.buf(1)==x.buf[ptr-1] && x.c0==(x.buf[ptr]+256)>>(8-x.bpos)) {
                if (len>MAXLEN) len=MAXLEN;
                if(x.buf[ptr]>>(7-x.bpos)&1)  {
                    mm1=squash(ilog(len)<<2);
                    mm2=squash(min(len, 64)<<5);
                }
                else {
                    mm1=squash(-(ilog(len)<<2));
                    mm2=squash(-(min(len, 64)<<5));
                }
            }
            else {
                len=0;
                mm1=mm2=2047;
            }
    pr0=(((mm1+mm2)>>1)+pr0)>>1;
    pr0=a.p(pr0, x.c0);
  }
};
  
//////////////////////////// Encoder ////////////////////////////

// An Encoder does arithmetic encoding.  Methods:
// Encoder(COMPRESS, f) creates encoder for compression to archive f, which
//   must be open past any header for writing in binary mode.
// Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
//   which must be open past any header for reading in binary mode.
// code(i) in COMPRESS mode compresses bit i (0 or 1) to file f.
// code() in DECOMPRESS mode returns the next decompressed bit from file f.
//   Global y is set to the last bit coded or decoded by code().
// compress(c) in COMPRESS mode compresses one byte.
// decompress() in DECOMPRESS mode decompresses and returns one byte.
// flush() should be called exactly once after compression is done and
//   before closing f.  It does nothing in DECOMPRESS mode.
// size() returns current length of archive
// setFile(f) sets alternate source to FILE* f for decompress() in COMPRESS
//   mode (for testing transforms).
// If level (global) is 0, then data is stored without arithmetic coding.

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder {
private:
  const Mode mode;       // Compress or decompress?
  FILE* archive;         // Compressed data file
  U32 x1, x2;            // Range, initially [0, 1), scaled by 2^32
  U32 x;                 // Decompress mode: last 4 input bytes of archive
  FILE *alt;             // decompress() source in COMPRESS mode

  // Compress bit y or return decompressed bit
  void code(int i=0) {
    int p=predictor.p();
    assert(p>=0 && p<4096);
    p+=p<2048;
    U32 xmid=x1 + ((x2-x1)>>12)*p + (((x2-x1)&0xfff)*p>>12);
    assert(xmid>=x1 && xmid<x2);
    predictor.x.y=i;
    i ? (x2=xmid) : (x1=xmid+1);
    predictor.update();
    while (((x1^x2)&0xff000000)==0) {  // pass equal leading bytes of range
      putc(x2>>24, archive);
      x1<<=8;
      x2=(x2<<8)+255;
    }
  }
  int decode() {
    int p=predictor.p();
    assert(p>=0 && p<4096);
    p+=p<2048;
    U32 xmid=x1 + ((x2-x1)>>12)*p + (((x2-x1)&0xfff)*p>>12);
    assert(xmid>=x1 && xmid<x2);
    x<=xmid ? (x2=xmid,predictor.x.y=1) : (x1=xmid+1,predictor.x.y=0);
    predictor.update();
    while (((x1^x2)&0xff000000)==0) {  // pass equal leading bytes of range
      x1<<=8;
      x2=(x2<<8)+255;
      x=(x<<8)+(getc(archive)&255);  // EOF is OK
    }
    return predictor.x.y;
  }
 
public:
 // BlockData blockdata;  
 Predictors& predictor;
  Encoder(Mode m, FILE* f,Predictors& predict);
  Mode getMode() const {return mode;}
  U64 size() const {return ftello(archive);}  // length of archive so far
  void flush();  // call this when compression is finished
  void setFile(FILE* f) {alt=f;}

  // Compress one byte
  void compress(int c) {
    assert(mode==COMPRESS);
    if (level==0)
      putc(c, archive);
    else {
      for (int i=7; i>=0; --i)
        code((c>>i)&1);
    }
  }

  // Decompress and return one byte
  int decompress() {
    if (mode==COMPRESS) {
      assert(alt);
      return getc(alt);
    }
    else if (level==0){
     int a;
     a=getc(archive);
      return a ;}
    else {
      int c=0;
      for (int i=0; i<8; ++i)
        c+=c+decode();
      
      return c;
    }
  }
  ~Encoder(){
  
   }
};

Encoder::Encoder(Mode m, FILE* f,Predictors& predict):
    mode(m), archive(f), x1(0), x2(0xffffffff), x(0), alt(0),predictor(predict) {
        
  if (level>0 && mode==DECOMPRESS) {  // x = first 4 bytes of archive
    for (int i=0; i<4; ++i)
      x=(x<<8)+(getc(archive)&255);
  }
  for (int i=0; i<1024; ++i)
    dt[i]=16384/(i+i+3);

}

void Encoder::flush() {
  if (mode==COMPRESS && level>0)
    putc(x1>>24, archive),putc(x1>>16, archive),putc(x1>>8, archive),putc(x1, archive);  // Flush first unequal byte of range
}
 
/////////////////////////// Filters /////////////////////////////////
//
// Before compression, data is encoded in blocks with the following format:
//
//   <type> <size> <encoded-data>
//
// Type is 1 byte (type Filetype): DEFAULT=0, JPEG, EXE, ...
// Size is 4 bytes in big-endian format.
// Encoded-data decodes to <size> bytes.  The encoded size might be
// different.  Encoded data is designed to be more compressible.
//
//   void encode(FILE* in, FILE* out, int n);
//
// Reads n bytes of in (open in "rb" mode) and encodes one or
// more blocks to temporary file out (open in "wb+" mode).
// The file pointer of in is advanced n bytes.  The file pointer of
// out is positioned after the last byte written.
//
//   en.setFile(FILE* out);
//   int decode(Encoder& en);
//
// Decodes and returns one byte.  Input is from en.decompress(), which
// reads from out if in COMPRESS mode.  During compression, n calls
// to decode() must exactly match n bytes of in, or else it is compressed
// as type 0 without encoding.
//
//   Filetype detect(FILE* in, int n, Filetype type);
//
// Reads n bytes of in, and detects when the type changes to
// something else.  If it does, then the file pointer is repositioned
// to the start of the change and the new type is returned.  If the type
// does not change, then it repositions the file pointer n bytes ahead
// and returns the old type.
//
// For each type X there are the following 2 functions:
//
//   void encode_X(FILE* in, FILE* out, int n, ...);
//
// encodes n bytes from in to out.
//
//   int decode_X(Encoder& en);
//
// decodes one byte from en and returns it.  decode() and decode_X()
// maintain state information using static variables.


FILE* tmpfile2(void){
    FILE *f;
#if defined(WINDOWS)    
    int i;
    char temppath[MAX_PATH]; 
    char filename[MAX_PATH];
    
    i=GetTempPath(MAX_PATH,temppath);
    if ((i==0) || (i>MAX_PATH)) return NULL;
    if (GetTempFileName(temppath,"tmp",0,filename)==0) return NULL;
    f=fopen(filename,"w+bTD");
    if (f==NULL) unlink(filename);
    return f;
#else
    f=tmpfile();  // temporary file
    if (!f) return NULL;
    return f;
#endif
}
#define bswap(x) \
+   ((((x) & 0xff000000) >> 24) | \
+    (((x) & 0x00ff0000) >>  8) | \
+    (((x) & 0x0000ff00) <<  8) | \
+    (((x) & 0x000000ff) << 24))

#define IMG_DET(type,start_pos,header_len,width,height) return dett=(type),\
deth=int(header_len),detd=int((width)*(height)),info=int(width),\
fseeko(in, start+(start_pos), SEEK_SET),HDR

#define AUD_DET(type,start_pos,header_len,data_len,wmode) return dett=(type),\
deth=int(header_len),detd=(data_len),info=(wmode),\
fseeko(in, start+(start_pos), SEEK_SET),HDR

//Return only base64 data. No HDR.
#define B64_DET(type,start_pos,header_len,base64len) return dett=(type),\
deth=(-1),detd=int(base64len),\
fseeko(in, start+start_pos, SEEK_SET),DEFAULT

#define B85_DET(type,start_pos,header_len,base85len) return dett=(type),\
deth=(-1),detd=int(base85len),\
fseeko(in, start+start_pos, SEEK_SET),DEFAULT

#define SZ_DET(type,start_pos,header_len,base64len,unsize) return dett=(type),\
deth=(-1),detd=int(base64len),info=(unsize),\
fseeko(in, start+start_pos, SEEK_SET),DEFAULT

#define MRBRLE_DET(start_pos,header_len,data_len,width,height) return dett=(MRBR),\
deth=(header_len),detd=(data_len),info=(((width+3)/4)*4),info2=(height),\
fseeko(in, start+(start_pos), SEEK_SET),HDR

#define TIFFJPEG_DET(start_pos,header_len,data_len) return dett=(JPEG),\
deth=(header_len),detd=(data_len),info=(-1),info2=(-1),\
fseeko(in, start+(start_pos), SEEK_SET),HDR

#define NES_DET(type,start_pos,header_len,base64len) return dett=(type),\
deth=(-1),detd=int(base64len),\
fseeko(in, start+start_pos, SEEK_SET),DEFAULT

inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c=='+') || (c=='/')|| (c==10) || (c==13));
}

inline bool is_base85(unsigned char c) {
    return (isalnum(c) || (c==13) || (c==10) || (c=='y') || (c=='z') || (c>='!' && c<='u'));
}
// Function ecc_compute(), edc_compute() and eccedc_init() taken from 
// ** UNECM - Decoder for ECM (Error Code Modeler) format.
// ** Version 1.0
// ** Copyright (C) 2002 Neill Corlett

/* LUTs used for computing ECC/EDC */
static U8 ecc_f_lut[256];
static U8 ecc_b_lut[256];
static U32 edc_lut[256];
static int luts_init=0;

void eccedc_init(void) {
  if (luts_init) return;
  U32 i, j, edc;
  for(i = 0; i < 256; i++) {
    j = (i << 1) ^ (i & 0x80 ? 0x11D : 0);
    ecc_f_lut[i] = j;
    ecc_b_lut[i ^ j] = i;
    edc = i;
    for(j = 0; j < 8; j++) edc = (edc >> 1) ^ (edc & 1 ? 0xD8018001 : 0);
    edc_lut[i] = edc;
  }
  luts_init=1;
}

void ecc_compute(U8 *src, U32 major_count, U32 minor_count, U32 major_mult, U32 minor_inc, U8 *dest) {
  U32 size = major_count * minor_count;
  U32 major, minor;
  for(major = 0; major < major_count; major++) {
    U32 index = (major >> 1) * major_mult + (major & 1);
    U8 ecc_a = 0;
    U8 ecc_b = 0;
    for(minor = 0; minor < minor_count; minor++) {
      U8 temp = src[index];
      index += minor_inc;
      if(index >= size) index -= size;
      ecc_a ^= temp;
      ecc_b ^= temp;
      ecc_a = ecc_f_lut[ecc_a];
    }
    ecc_a = ecc_b_lut[ecc_f_lut[ecc_a] ^ ecc_b];
    dest[major              ] = ecc_a;
    dest[major + major_count] = ecc_a ^ ecc_b;
  }
}

U32 edc_compute(const U8  *src, int size) {
  U32 edc = 0;
  while(size--) edc = (edc >> 8) ^ edc_lut[(edc ^ (*src++)) & 0xFF];
  return edc;
}

int expand_cd_sector(U8 *data, int a, int test) {
  U8 d2[2352];
  eccedc_init();
  d2[0]=d2[11]=0;
  for (int i=1; i<11; i++) d2[i]=255;
  int mode=(data[15]!=1?2:1);
  int form=(data[15]==3?2:1);
  if (a==-1) for (int i=12; i<15; i++) d2[i]=data[i]; else {
    int c1=(a&15)+((a>>4)&15)*10;
    int c2=((a>>8)&15)+((a>>12)&15)*10;
    int c3=((a>>16)&15)+((a>>20)&15)*10;
    c1=(c1+1)%75;
    if (c1==0) {
      c2=(c2+1)%60;
      if (c2==0) c3++;
    }
    d2[12]=(c3%10)+16*(c3/10);
    d2[13]=(c2%10)+16*(c2/10);
    d2[14]=(c1%10)+16*(c1/10);
  }
  d2[15]=mode;
  if (mode==2) for (int i=16; i<24; i++) d2[i]=data[i-4*(i>=20)];
  if (form==1) {
    if (mode==2) {
      d2[1]=d2[12],d2[2]=d2[13],d2[3]=d2[14];
      d2[12]=d2[13]=d2[14]=d2[15]=0;
    } else {
      for(int i=2068; i<2076; i++) d2[i]=0;
    }
    for (int i=16+8*(mode==2); i<2064+8*(mode==2); i++) d2[i]=data[i];
    U32 edc=edc_compute(d2+16*(mode==2), 2064-8*(mode==2));
    for (int i=0; i<4; i++) d2[2064+8*(mode==2)+i]=(edc>>(8*i))&0xff;
    ecc_compute(d2+12, 86, 24,  2, 86, d2+2076);
    ecc_compute(d2+12, 52, 43, 86, 88, d2+2248);
    if (mode==2) {
      d2[12]=d2[1],d2[13]=d2[2],d2[14]=d2[3],d2[15]=2;
      d2[1]=d2[2]=d2[3]=255;
    }
  }
  for (int i=0; i<2352; i++) if (d2[i]!=data[i] && test) form=2;
  if (form==2) {
    for (int i=24; i<2348; i++) d2[i]=data[i];
    U32 edc=edc_compute(d2+16, 2332);
    for (int i=0; i<4; i++) d2[2348+i]=(edc>>(8*i))&0xff;
  }
  for (int i=0; i<2352; i++) if (d2[i]!=data[i] && test) return 0; else data[i]=d2[i];
  return mode+form-1;
}

//LZSS compressor/decompressor class
//http://my.execpc.com/~geezer/code/lzss.c
class LZSS {
    private:
    const U32 N;                // size of ring buffer
    const U32 F;                // upper limit for g_match_len.
                                // 16 for compatibility with Microsoft COMPRESS.EXE and EXPAND.EXE
    const U32 THRESHOLD;        // encode string into position and length if match_length is greater than this
    U32  NIL;                   // index for root of binary search trees
    Array<U8> LZringbuffer;     // ring buffer of size N, with extra F-1 bytes
                                // to facilitate string comparison
    U32 matchpos;               // position and length of longest match; set by insert_node()
    U32 matchlen;
    Array<U32> LZ_lchild;       // left & right children & parent -- these constitute binary search tree
    Array<U32> LZ_rchild;
    Array<U32> LZ_parent;
    FILE *g_infile, *g_outfile; //input and output file to be compressed
    U32 filesizez;

    // Inserts string of length F, LZringbuffer[r..r+F-1], into one of the
    // trees (LZringbuffer[r]'th tree) and returns the longest-match position
    // and length via the global variables matchpos and matchlen.
    // If matchlen = F, then removes the old node in favour of the new
    // one, because the old one will be deleted sooner.
    // Note r plays double role, as tree node and position in buffer.
void insert_node(int r){
    U8 *key;
    U32 i, p;
    int cmp;
    cmp = 1;
    key = &LZringbuffer[r];
    p=N+1+key[0];
    LZ_rchild[r]=LZ_lchild[r]=NIL;
    matchlen = 0;
    while(1){
        if(cmp>= 0){
            if(LZ_rchild[p]!=NIL) p=LZ_rchild[p];
            else{
                LZ_rchild[p]=r;
                LZ_parent[r]=p;
                return;
            }
        }
        else{
            if(LZ_lchild[p]!=NIL) p=LZ_lchild[p];
            else{
                LZ_lchild[p]=r;
                LZ_parent[r]=p;
                return;
            }
        }
        for(i=1;i<F;i++){
            cmp=key[i]-LZringbuffer[p+i];
            if(cmp != 0) break;
        }
        if(i>matchlen){
            matchpos=p;
            matchlen=i;
            if(matchlen>=F) break;
        }
    }
    LZ_parent[r]=LZ_parent[p];
    LZ_lchild[r]=LZ_lchild[p];
    LZ_rchild[r]=LZ_rchild[p];
    LZ_parent[LZ_lchild[p]]=r;
    LZ_parent[LZ_rchild[p]]=r;
    if(LZ_rchild[LZ_parent[p]]==p) LZ_rchild[LZ_parent[p]]=r;
    else LZ_lchild[LZ_parent[p]]=r;
    LZ_parent[p]=NIL;                   // remove p
}

//deletes node p from tree
void delete_node(unsigned p){
    U32 q;
    if(LZ_parent[p]==NIL) return;       // not in tree
    if(LZ_rchild[p]==NIL) q=LZ_lchild[p];
    else if(LZ_lchild[p]==NIL) q=LZ_rchild[p];
    else{
        q=LZ_lchild[p];
        if(LZ_rchild[q]!=NIL){
            do q=LZ_rchild[q];
            while(LZ_rchild[q]!=NIL);
            LZ_rchild[LZ_parent[q]]=LZ_lchild[q];
            LZ_parent[LZ_lchild[q]]=LZ_parent[q];
            LZ_lchild[q]=LZ_lchild[p];
            LZ_parent[LZ_lchild[p]]=q;
        }
        LZ_rchild[q]=LZ_rchild[p];
        LZ_parent[LZ_rchild[p]]=q;
    }
    LZ_parent[q] = LZ_parent[p];
    if(LZ_rchild[LZ_parent[p]]==p) LZ_rchild[LZ_parent[p]]=q;
    else LZ_lchild[LZ_parent[p]] = q;
    LZ_parent[p]=NIL;
}
public:
    U32 usize;
    LZSS(FILE *in, FILE* out,U32 fsize,U32 qn);

//may fail when compressed size is larger the input (uncompressible data)
U32 compress(){
    U32 i, len, r, s, last_match_length, code_buf_ptr;
    U8 code_buf[17], mask;
    U32 c,ocount;
    // code_buf[1..16] saves eight units of code, and code_buf[0] works as
    // eight flags, "1" representing that the unit is an unencoded letter (1 byte),
    // "0" a position-and-length pair (2 bytes). Thus, eight units require at most
    // 16 bytes of code.
    ocount=0;
    code_buf[0]=0;
    code_buf_ptr=mask=1;
    s=0;
    r=N-F;
    // Clear the buffer with any character that will appear often.
    memset(&LZringbuffer[0]+s,' ',r-s);
    // Read F bytes into the last F bytes of the buffer
    for(len=0;len<F;len++){
        c=getc(g_infile);
        if(c==EOF)break;
        LZringbuffer[r+len]=c;
    }
    if(len==0) return 0; //text of size zero
    // Insert the F strings, each of which begins with one or more 'space'
    // characters. Note the order in which these strings are inserted.
    // This way, degenerate trees will be less likely to occur.
    for(i=1; i<=F;i++) insert_node(r-i);
    // Finally, insert the whole string just read. The global variables
    // matchlen and matchpos are set.
    insert_node(r);
    do{
        // matchlen may be spuriously long near the end of text.
        if(matchlen>len) matchlen=len;
        if(matchlen<=THRESHOLD){            // Not long enough match. Send one byte.
            matchlen=1;
            code_buf[0]|=mask;              // 'send one byte' flag 
            code_buf[code_buf_ptr]=LZringbuffer[r];  // Send uncoded.
            code_buf_ptr++;
        }
        else{                               // Send position and length pair. Note matchlen > THRESHOLD.
            code_buf[code_buf_ptr]=(U8)matchpos;
            code_buf_ptr++;
            code_buf[code_buf_ptr]=(U8)(((matchpos>>4)&0xF0)|(matchlen-(THRESHOLD+1)));
            code_buf_ptr++;
        }
        mask<<=1;                           // Shift mask left one bit.
        if(mask==0){                        // Send at most 8 units of code together
            for(i=0;i<code_buf_ptr;i++){
                putc(code_buf[i], g_outfile),ocount++;
                if(ocount>=filesizez) return ocount;
            }
            code_buf[0]=0;
            code_buf_ptr=mask=1;
        }
        last_match_length=matchlen;
        for(i=0;i<last_match_length;i++){
            c=getc(g_infile);
            if(c==EOF) break;
            delete_node(s);                 // Delete old strings and read new bytes
            LZringbuffer[s] = c;
            // If the position is near the end of buffer, extend the buffer
            // to make string comparison easier.
            // Since this is a ring buffer, increment the position modulo N.
            // Register the string in LZringbuffer[r..r+F-1] 
            if(s<F-1) LZringbuffer[s+N]=c;
            s=(s+1)&(N-1);
            r=(r+1)&(N-1);
            insert_node(r);
        }
        while(i++<last_match_length){       // After the end of text,
            delete_node(s);                 // no need to read, but
            s=(s+1)&(N-1);
            r=(r+1)&(N-1);
            len--;
            if(len) insert_node(r);         // buffer may not be empty.
        }
    } while(len>0);                         //until length of string to be processed is zero
    if(code_buf_ptr>1){                     // Send remaining code.
        for(i=0;i<code_buf_ptr;i++){
            putc(code_buf[i], g_outfile),ocount++;
            if(ocount>=filesizez) return ocount;
        }
    }
    return ocount;    //return compressed size
}

U32 decompress(){
    U32 r, flags;
    U32 c, i, j, k,icount,incount;
    icount=incount=0;
    memset(&LZringbuffer[0],' ',N-F);
    r = N - F;
    for(flags=0;;flags>>=1){
    // Get a byte. For each bit of this byte:
    // 1=copy one byte literally, from input to output
    // 0=get two more bytes describing length and position of previously-seen
    // data, and copy that data from the ring buffer to output
        if((flags&0x100)==0){
            c=getc(g_infile),incount++;
            if(c==EOF||icount>=filesizez) break;
            flags=c|0xFF00;
        }
        if(flags & 1){
            c=getc(g_infile),incount++;
            if(c==EOF||icount>=filesizez) break;
            putc(c,g_outfile),icount++;
            LZringbuffer[r]=c;
            r=(r+1)&(N-1);
        }
        // 0=get two more bytes describing length and position of previously-
        // seen data, and copy that data from the ring buffer to output
        else{
            i=getc(g_infile),incount++;
            if(i==EOF||icount>=filesizez) break;
            j=getc(g_infile),incount++;
            if(j==EOF ||icount>=filesizez) break;
            i|=((j&0xF0)<< 4);
            j=(j&0x0F)+THRESHOLD;
            for(k=0;k<=j;k++){
                c=LZringbuffer[(i+k)&(N-1)];
                putc(c,g_outfile),icount++;
                LZringbuffer[r]=c;
                r=(r+1)&(N-1);
            }
        }
    }
    usize=icount;       //decompressed size
    return incount-1;   //return compressed size
}
};
LZSS::LZSS(FILE *in, FILE* out,U32 fsize,U32 qn=0): N(4096),F(16+qn),THRESHOLD(2),NIL(N),
LZringbuffer(N+F-1), LZ_lchild(N+1), LZ_rchild(N+257), LZ_parent(N+1),filesizez(fsize),usize(0){
    g_infile=in, g_outfile=out;
    // initialize trees
    // For i = 0 to N - 1, LZ_rchild[i] and LZ_lchild[i] will be the right and
    // left children of node i. These nodes need not be initialized.
    // Also, LZ_parent[i] is the parent of node i. These are initialized to
    // NIL (= N), which stands for 'not used.'
    // For i = 0 to 255, LZ_rchild[N + i + 1] is the root of the tree
    // for strings that begin with character i. These are initialized
    // to NIL.  Note there are 256 trees.
    for(U32 i=N+1;i<=N+256;i++)
        LZ_rchild[i]=NIL;
    for(U32 i=0;i<N;i++)
        LZ_parent[i]=NIL;
}

//read compressed word,dword
U32 GetCDWord(FILE *f)
{
    U16 w = getc(f);
    w=w | (getc(f)<<8);
    if(w&1){
        U16 w1 = getc(f);
        w1=w1 | (getc(f)<<8);
        return ((w1<<16)|w)>>1;
    }
    return w>>1;
}
U8 GetCWord(FILE *f)
{
    U8 b=getc(f);
    if(b&1) return ((getc(f)<<8)|b)>>1;
    return b>>1;
}

int parse_zlib_header(int header) {
    switch (header) {
        case 0x2815 : return 0;  case 0x2853 : return 1;  case 0x2891 : return 2;  case 0x28cf : return 3;
        case 0x3811 : return 4;  case 0x384f : return 5;  case 0x388d : return 6;  case 0x38cb : return 7;
        case 0x480d : return 8;  case 0x484b : return 9;  case 0x4889 : return 10; case 0x48c7 : return 11;
        case 0x5809 : return 12; case 0x5847 : return 13; case 0x5885 : return 14; case 0x58c3 : return 15;
        case 0x6805 : return 16; case 0x6843 : return 17; case 0x6881 : return 18; case 0x68de : return 19;
        case 0x7801 : return 20; case 0x785e : return 21; case 0x789c : return 22; case 0x78da : return 23;
    }
    return -1;
}
int zlib_inflateInit(z_streamp strm, int zh) {
    if (zh==-1) return inflateInit2(strm, -MAX_WBITS); else return inflateInit(strm);
}
//int wrtn=0; //=1 if more numbers then text
#define base64max 0x8000000 //128M limit
#define base85max 0x8000000 //128M limit

// Detect EXE or JPEG data
Filetype detect(FILE* in, U64 n, Filetype type, int &info, int &info2, int it=0,int s1=0) {
  U32 buf3=0, buf2=0, buf1=0, buf0=0;  // last 8 bytes
  U64 start=ftello(in);

  // For EXE detection
  Array<U64> abspos(256),  // CALL/JMP abs. addr. low byte -> last offset
    relpos(256);    // CALL/JMP relative addr. low byte -> last offset
  int e8e9count=0;  // number of consecutive CALL/JMPs
  U64 e8e9pos=0;    // offset of first CALL or JMP instruction
  U64 e8e9last=0;   // offset of most recent CALL or JMP

  U64 soi=0, sof=0, sos=0, app=0;  // For JPEG detection - position where found
  U64 wavi=0;
  int wavsize=0,wavch=0,wavbps=0,wavm=0,wavsr=0,wavt=0;  // For WAVE detection
  U64 aiff=0;
  int aiffm=0,aiffs=0;  // For AIFF detection
  U64 s3mi=0;
  int s3mno=0,s3mni=0;  // For S3M detection
  U64 bmp=0;
  int imgbpp=0,bmpx=0,bmpy=0,bmpof=0;  // For BMP detection
  U64 rgbi=0;
  int rgbx=0,rgby=0;  // For RGB detection
  U64 tga=0;
  U64 tgax=0;
  int tgay=0,tgaz=0,tgat=0;  // For TGA detection
  U64 pgm=0;
  int pgmcomment=0,pgmw=0,pgmh=0,pgm_ptr=0,pgmc=0,pgmn=0;  // For PBM, PGM, PPM detection
  char pgm_buf[32];
  U64 cdi=0;
  U64 mdfa=0;
  int cda=0,cdm=0;  // For CD sectors detection
  U32 cdf=0;
 // For TEXT
  U64 txtStart=0,txtLen=0,txtOff=0,txtbinc=0,txtbinp=0,txta=0,txt0=0;
  int utfc=0,utfb=0,txtIsUTF8=0; //utf count 2-6, current byte
  const int txtMinLen=1024*64;//2;
  //int teolu=0,teolw=0,teoll=0;
  //wrtn=0;

  //base64
  U64 b64s=0,b64s1=0,b64p=0,b64slen=0,b64h=0,b64i=0;
  U64 base64start=0,base64end=0,b64line=0,b64nl=0,b64lcount=0;
  
   U64 b85s=0,b85s1=0,b85p=0,b85slen=0,b85h=0;
  U64 base85start=0,base85end=0,b85line=0,b85nl=0,b85lcount=0;
  //int b64f=0,b64fstart=0,b64flen=0,b64send=0,b64fline=0; //force base64 detection
  int gif=0,gifa=0,gifi=0,gifw=0,gifc=0,gifb=0; // For GIF detection

  U64 fSZDD=0;
  U64 fKWAJ=0;
  LZSS* lz77;
  unsigned char zbuf[32], zin[1<<16], zout[1<<16]; // For ZLIB stream detection
  int zbufpos=0,zzippos=-1;
  int pdfim=0,pdfimw=0,pdfimh=0,pdfimb=0,pdfimp=0;
U64 mrb=0,mrb0=0,mrb1=0,mrbsize=0,mrbcsize=0,mrbPictureType=0,mrbPackingMethod=0,mrbTell=0,mrbTell1=0,mrbw=0,mrbh=0; // For MRB detection
  //
  U64 nesh=0,nesp=0;
  // For image detection
  static int deth=0,detd=0;  // detected header/data size in bytes
  static Filetype dett;  // detected block type
  if (deth >1) return fseeko(in, start+deth, SEEK_SET),deth=0,dett;
  else if (deth ==-1) return fseeko(in, start, SEEK_SET),deth=0,dett;
  else if (detd) return fseeko(in, start+detd, SEEK_SET),detd=0,DEFAULT;

  for (U64 i=0; i<n; ++i) {
    int c=getc(in);
    if (c==EOF) return (Filetype)(-1);
    buf3=buf3<<8|buf2>>24;
    buf2=buf2<<8|buf1>>24;
    buf1=buf1<<8|buf0>>24;
    buf0=buf0<<8|c;
    
    // ZLIB stream detection
    zbuf[zbufpos]=c;
    zbufpos=(zbufpos+1)%32;
    if(!cdi && !gif && !soi && !pgm && !rgbi && !bmp && !wavi && !tga && !b64s1 && !b64s && !b85s1 && !b85s && !mdfa && !nesh && !mrb)  {

    int zh=parse_zlib_header(((int)zbuf[zbufpos])*256+(int)zbuf[(zbufpos+1)%32]);
    if ((i>=31 && zh!=-1) || zzippos==i) {
      int streamLength=0, ret=0;

      // Quick check possible stream by decompressing first 32 bytes
      z_stream strm;
      strm.zalloc=Z_NULL; strm.zfree=Z_NULL; strm.opaque=Z_NULL;
      strm.next_in=Z_NULL; strm.avail_in=0;
      if (zlib_inflateInit(&strm,zh)==Z_OK) {
        unsigned char tmp[32];
        for (int j=0; j<32; j++) tmp[j]=zbuf[(zbufpos+j)%32];
        strm.next_in=tmp; strm.avail_in=32;
        strm.next_out=zout; strm.avail_out=1<<16;
        ret=inflate(&strm, Z_FINISH);
        ret=(inflateEnd(&strm)==Z_OK && (ret==Z_STREAM_END || ret==Z_BUF_ERROR) && strm.total_in>=16);
      }
      if (ret) {
        // Verify valid stream and determine stream length
        long savedpos=ftell(in);
        strm.zalloc=Z_NULL; strm.zfree=Z_NULL; strm.opaque=Z_NULL;
        strm.next_in=Z_NULL; strm.avail_in=0; strm.total_in=strm.total_out=0;
        if (zlib_inflateInit(&strm,zh)==Z_OK) {
          for (int j=i-31; j<n; j+=1<<16) {
            unsigned int blsize=min(n-j,1<<16);
            fseek(in, start+j, SEEK_SET);
            if (fread(zin, 1, blsize, in)!=blsize) break;
            strm.next_in=zin; strm.avail_in=blsize;
            do {
              strm.next_out=zout; strm.avail_out=1<<16;
              ret=inflate(&strm, Z_FINISH);
            } while (strm.avail_out==0 && ret==Z_BUF_ERROR);
            if (ret==Z_STREAM_END) streamLength=strm.total_in;
            if (ret!=Z_BUF_ERROR) break;
          }
          if (inflateEnd(&strm)!=Z_OK) streamLength=0;
        }
        fseek(in, savedpos, SEEK_SET);
      }
      if (streamLength>5) {
        info=0;
        if (pdfimw>0 && pdfimh>0) {
          if (pdfimb==8 && (int)strm.total_out==pdfimw*pdfimh) info=(8<<24)+pdfimw;
          if (pdfimb==8 && (int)strm.total_out==pdfimw*pdfimh*3) info=(24<<24)+pdfimw*3;
          if (pdfimb==4 && (int)strm.total_out==((pdfimw+1)/2)*pdfimh) info=(4<<24)+((pdfimw+1));
        }
        return fseek(in, start+i-31, SEEK_SET),detd=streamLength,ZLIB;
      }
    }
    if (zh==-1 && zbuf[zbufpos]=='P' && zbuf[(zbufpos+1)%32]=='K' && zbuf[(zbufpos+2)%32]=='\x3'
      && zbuf[(zbufpos+3)%32]=='\x4' && zbuf[(zbufpos+8)%32]=='\x8' && zbuf[(zbufpos+9)%32]=='\0') {
        int nlen=(int)zbuf[(zbufpos+26)%32]+((int)zbuf[(zbufpos+27)%32])*256
                +(int)zbuf[(zbufpos+28)%32]+((int)zbuf[(zbufpos+29)%32])*256;
        if (nlen<256 && i+30+nlen<n) zzippos=i+30+nlen;
    }
    if (i-pdfimp>1024) pdfim=pdfimw=pdfimh=pdfimb=0;
    if (pdfim>1 && !(isspace(c) || isdigit(c))) pdfim=1;
    if (pdfim==2 && isdigit(c)) pdfimw=pdfimw*10+(c-'0');
    if (pdfim==3 && isdigit(c)) pdfimh=pdfimh*10+(c-'0');
    if (pdfim==4 && isdigit(c)) pdfimb=pdfimb*10+(c-'0');
    if ((buf0&0xffff)==0x3c3c) pdfimp=i,pdfim=1; // <<
    if (pdfim && (buf1&0xffff)==0x2f57 && buf0==0x69647468) pdfim=2,pdfimw=0; // /Width
    if (pdfim && (buf1&0xffffff)==0x2f4865 && buf0==0x69676874) pdfim=3,pdfimh=0; // /Height
    if (pdfim && buf3==0x42697473 && buf2==0x50657243 && buf1==0x6f6d706f
       && buf0==0x6e656e74 && zbuf[(zbufpos+15)%32]=='/') pdfim=4,pdfimb=0; // /BitsPerComponent
}
    // NES rom 
    if (buf0==0x4E45531A) nesh=i,nesp=0;
    if (nesh) {
      const int p=int(i-nesh);
      if (p==1) nesp=buf0&0xff; //count of pages*0x3FFF
      else if (p==6 && ((buf0&0xfe)!=0) )nesh=0;
      else if (p==11 && (buf0!=0) )nesh=0;
      else if (p==12) {
        if (nesp>1 && nesp<129) NES_DET(NESROM,nesh-3,15,nesp*0x3FFF);
        nesh=0;
      }
    }
    
      //detect LZSS compressed data in compress.exe generated archives
   /* if ((buf0==0x88F027D1 && buf1==0x4B57414A && !cdi ) ||(buf1==0x535A2088 && buf0==0xF02733D1)) fKWAJ=i;
    if (fKWAJ && (i-fKWAJ ==6) && (buf1&0xffff)==0x0200 && (buf0==0x0e000000 )){
        U32 fsizez=bswap(buf0); //uncompressed file size FIX ME
        if (fsizez<0x1ffffff){
            FILE* outf=tmpfile2();          // try to decompress file
            lz77=new LZSS(in,outf,fsizez);
            int64_t savedpos=ftello(in);
            int u=lz77->decompress(); //compressed size
            int uf= lz77->usize; //uncompressed size
            delete lz77;
            int csize=ftello(in)-savedpos-(!feof(in)?1:0);
            if (u!=csize || u>fsizez) fKWAJ=0;          // reset if not same size or compressed size > uncompressed size
            else{
                fseeko(outf, 0, SEEK_SET);  // try to compress decompressed file
                FILE* out2=tmpfile2();
                lz77=new LZSS(outf,out2,u);
                int r=lz77->compress();
                delete lz77;
                fclose(out2);
                fclose(outf);
                if (r!=(csize)) fKWAJ=0;    // reset if not same size
                else{
                    fseeko(in, savedpos, SEEK_SET); //all good
                    SZ_DET(SZDD,fSZDD+7,14,r,uf);
                }
            }
            fclose(outf);
        }
        else fKWAJ=0;
    }*/
    //detect LZSS compressed data in compress.exe generated archives
    if ((buf0==0x88F02733 && buf1==0x535A4444 && !cdi ) ||(buf1==0x535A2088 && buf0==0xF02733D1)) fSZDD=i;
    if (fSZDD && (((i-fSZDD ==6) && (buf1&0xff00)==0x4100 && ((buf1&0xff)==0 ||(buf1&0xff)>'a')&&(buf1&0xff)<'z') || (buf1!=0x88F02733 && !cdi  && (i-fSZDD)==4))){
	   int lz2=0;
		if (buf1!=0x88F02733 && (i-fSZDD)==4) lz2=2;  //+2 treshold
        U32 fsizez=bswap(buf0); //uncompressed file size
        if (fsizez<0x1ffffff){
            FILE* outf=tmpfile2();          // try to decompress file
            lz77=new LZSS(in,outf,fsizez,lz2);
            int64_t savedpos=ftello(in);
            int u=lz77->decompress(); //compressed size
            int uf= lz77->usize; //uncompressed size
            delete lz77;
            int csize=ftello(in)-savedpos-(!feof(in)?1:0);
            if (u!=csize || u>fsizez) fSZDD=0;          // reset if not same size or compressed size > uncompressed size
            else{
                fseeko(outf, 0, SEEK_SET);  // try to compress decompressed file
                FILE* out2=tmpfile2();
                lz77=new LZSS(outf,out2,u,lz2);
                int r=lz77->compress();
                delete lz77;
                fclose(out2);
                fclose(outf);
                if (r!=(csize)) fSZDD=0;    // reset if not same size
                else{
                    fseeko(in, savedpos, SEEK_SET); //all good
                    //flag for +2 treshold, set bit 25
                    SZ_DET(SZDD,fSZDD+7-lz2,14-lz2,r,uf+(lz2?(1<<25):0)); 
                }
            }
            fclose(outf);
        }
        else fSZDD=0;
    } 
    
    //detect rle encoded mrb files inside windows hlp files
    if (((buf0&0xFFFF)==0x6c70 || (buf0&0xFFFF)==0x6C50) && !b64s1 && !b64s && !b85s1 && !b85s )
		mrb=i; 
	if (mrb){
	    const int p=int(i-mrb);
        if (p==1 && !c==1)	mrb=0;    // if not 1 image per/file            
	    if (p==7 && (c==5 || c==6 ))  // 5=DDB   6=DIB   8=metafile
		    mrbPictureType=c;
	    if (p==8 && (c==1 ))          // 0=uncomp 1=RunLen 2=LZ77 3=both
		  mrbPackingMethod=c;	
	    if (p==10){
		  if (mrbPictureType==6 && (mrbPackingMethod==1 || mrbPackingMethod==2)){
		//save ftell
		mrbTell=ftell(in)-2;
		fseek(in,mrbTell,SEEK_SET);
		U32 Xdpi=GetCDWord(in);
		U32 Ydpi=GetCDWord(in);
        U32 Planes=GetCWord(in);
		U32 BitCount=GetCWord(in);
        mrbw=GetCDWord(in);
		mrbh=GetCDWord(in);
        U32 ColorsUsed=GetCDWord(in);
		U32 ColorsImportant=GetCDWord(in);
        mrbcsize=GetCDWord(in);
		U32 HotspotSize=GetCDWord(in);
		int CompressedOffset=getc(in)<<24;
            CompressedOffset|=getc(in)<<16;
            CompressedOffset|=getc(in)<<8;
            CompressedOffset|=getc(in);
	    int HotspotOffset=getc(in)<<24;
            HotspotOffset|=getc(in)<<16;
            HotspotOffset|=getc(in)<<8;
            HotspotOffset|=getc(in);
	    CompressedOffset=bswap(CompressedOffset);
		HotspotOffset=bswap(HotspotOffset);
		mrbsize=mrbcsize+ftell(in)-mrbTell+10+(1<<BitCount)*4; // ignore HotspotSize
		if (BitCount!=8 || mrbw<4 || mrbw>1024) {
			mrbPictureType=mrb=mrbsize=0;
		    mrbTell=mrbTell+2;
		    fseek(in,mrbTell,SEEK_SET);
		}
	   }
	   }
	   if (type==MRBR &&   (mrbPictureType==6 || mrbPictureType==8) && mrbsize){
		return fseek(in, start+mrbsize, SEEK_SET),DEFAULT;
	   }
	   if ( (mrbPictureType==6) && mrbsize){
		MRBRLE_DET(mrb-1, mrbsize-mrbcsize, mrbcsize, mrbw, mrbh);
	   }
    }
    
     // MDF (Alcohol 120%) CD (mode 1 and mode 2 form 1+2 - 2352 bytes+96 channel data)
    if ( !cdi && mdfa && type!=MDF)  return  fseeko(in, start+mdfa-7, SEEK_SET), MDF;
    if (buf1==0x00ffffff && buf0==0xffffffff   && !mdfa  && type==MDF) mdfa=i;
    if (mdfa && i>mdfa) {
      	const int p=(i-mdfa)%2448;
      	if (p==8 && (buf1!=0xffffff00 || ((buf0&0xff)!=1 && (buf0&0xff)!=2))) mdfa=0;
        if (!mdfa && type==MDF)  return fseeko(in, start+i-p-7, SEEK_SET), DEFAULT;
    }
    if (type==MDF) continue;
    
    // CD sectors detection (mode 1 and mode 2 form 1+2 - 2352 bytes)
    if (buf1==0x00ffffff && buf0==0xffffffff && !cdi && !mdfa) cdi=i,cda=-1,cdm=0;
    if (cdi && i>cdi) {
      const int p=(i-cdi)%2352;
      if (p==8 && (buf1!=0xffffff00 || ((buf0&0xff)!=1 && (buf0&0xff)!=2))) cdi=0; // FIX it ?
      else if (p==16 && i+2336<n) {
        U8 data[2352];
        int64_t savedpos=ftello(in);
        fseeko(in, start+i-23, SEEK_SET);
        fread(data, 1, 2352, in);
        int t=expand_cd_sector(data, cda, 1);
        if (t!=cdm) cdm=t*(i-cdi<2352);
        if (cdm && cda!=10 && (cdm==1 || buf0==buf1) && type!=CD) {
        	//skip possible 96 byte channel data and test if another frame
        	fseeko(in, ftello(in)+96, SEEK_SET);
        	U32 mdf=(getc(in)<<24)+(getc(in)<<16)+(getc(in)<<8)+getc(in);
    		U32 mdf1= (getc(in)<<24)+(getc(in)<<16)+(getc(in)<<8)+getc(in);
	 		if (mdf==0x00ffffff && mdf1==0xffffffff ) mdfa=cdi,cdi=cdm=0; //drop to mdf mode?
 		}
 		fseeko(in, savedpos, SEEK_SET); // seek back if no mdf
        if (cdm && cda!=10 && (cdm==1 || buf0==buf1)) {
          if (type!=CD) return info=cdm,fseeko(in, start+cdi-7, SEEK_SET), CD;
          cda=(data[12]<<16)+(data[13]<<8)+data[14];
          if (cdm!=1 && i-cdi>2352 && buf0!=cdf) cda=10;
          if (cdm!=1) cdf=buf0;
        } else cdi=0;
      }
      if (!cdi && type==CD) return fseeko(in, start+i-p-7, SEEK_SET), DEFAULT;
    }
    if (type==CD) continue;
    
    // Detect JPEG by code SOI APPx (FF D8 FF Ex) followed by
    // SOF0 (FF C0 xx xx 08) and SOS (FF DA) within a reasonable distance.
    // Detect end by any code other than RST0-RST7 (FF D9-D7) or
    // a byte stuff (FF 00).

    if (!soi && i>=3 && ((buf0&0xfffffff0)==0xffd8ffe0 /*|| buf0==0xffd8ffdb*/)) soi=i, app=i+2, sos=sof=0;
    if (soi) {
      if (app==i && (buf0>>24)==0xff &&
         ((buf0>>16)&0xff)>0xc0 && ((buf0>>16)&0xff)<0xff) app=i+(buf0&0xffff)+2;
      if (app<i && (buf1&0xff)==0xff && (buf0&0xff0000ff)==0xc0000008) sof=i;
      if (sof && sof>soi && i-sof<0x1000 && (buf0&0xffff)==0xffda) {
        sos=i;
        if (type!=JPEG) return fseeko(in, start+soi-3, SEEK_SET), JPEG;
      }
      if (i-soi>0x40000 && !sos) soi=0;
    }
    if (type==JPEG && sos && i>sos && (buf0&0xff00)==0xff00
        && (buf0&0xff)!=0 && (buf0&0xf8)!=0xd0) return DEFAULT;
    
    // Detect .wav file header
    if (buf0==0x52494646) wavi=i,wavm=0;
    if (wavi) {
      const int p=int(i-wavi);
      if (p==4) wavsize=bswap(buf0);
      else if (p==8 && buf0!=0x57415645) wavi=0;
      else if (p==16 && (buf1!=0x666d7420 || bswap(buf0)!=16)) wavi=0;
      else if (p==20) wavt=bswap(buf0)&0xffff;
      else if (p==22) wavch=bswap(buf0)&0xffff;
      else if (p==24) wavsr=bswap(buf0) ;
      else if (p==34) wavbps=bswap(buf0)&0xffff;
      else if (p==40+wavm && buf1!=0x64617461) wavm+=bswap(buf0)+8,wavi=(wavm>0xfffff?0:wavi);
      else if (p==40+wavm) {
        int wavd=bswap(buf0);
        info2=wavsr;
        if ((wavch==1 || wavch==2) && (wavbps==8 || wavbps==16) && wavt==1 && wavd>0 && wavsize>=wavd+36
           && wavd%((wavbps/8)*wavch)==0 && wavsr>=0) AUD_DET(AUDIO,wavi-3,44+wavm,wavd,wavch+wavbps/4-3);
           //32bit IEEE
        //if ((wavch==1 || wavch==2) && wavbps==32 && wavt==3 && wavd>0 && wavsize>=wavd+36
        //   && wavd%((wavbps/8)*wavch)==0 && wsr>=0) AUD_DET(AUDIO,wavi-3,44+wavm,wavd,wavch+wavbps/8-3+(wsr<<5)+(1<<7));
        wavi=0;
      }
    }

    // Detect .aiff file header
    if (buf0==0x464f524d) aiff=i,aiffs=0; // FORM
    if (aiff) {
      const int p=int(i-aiff);
      if (p==12 && (buf1!=0x41494646 || buf0!=0x434f4d4d)) aiff=0; // AIFF COMM
      else if (p==24) {
        const int bits=buf0&0xffff, chn=buf1>>16;
        if ((bits==8 || bits==16) && (chn==1 || chn==2)) aiffm=chn+bits/4+1; else aiff=0;
      } else if (p==42+aiffs && buf1!=0x53534e44) aiffs+=(buf0+8)+(buf0&1),aiff=(aiffs>0x400?0:aiff);
      else if (p==42+aiffs) AUD_DET(AUDIO,aiff-3,54+aiffs,buf0-8,aiffm);
    }

    // Detect .mod file header 
    if ((buf0==0x4d2e4b2e || buf0==0x3643484e || buf0==0x3843484e  // M.K. 6CHN 8CHN
       || buf0==0x464c5434 || buf0==0x464c5438) && (buf1&0xc0c0c0c0)==0 && i>=1083) {
      int64_t savedpos=ftello(in);
      const int chn=((buf0>>24)==0x36?6:(((buf0>>24)==0x38 || (buf0&0xff)==0x38)?8:4));
      int len=0; // total length of samples
      int numpat=1; // number of patterns
      for (int j=0; j<31; j++) {
        fseeko(in, start+i-1083+42+j*30, SEEK_SET);
        const int i1=getc(in);
        const int i2=getc(in); 
        len+=i1*512+i2*2;
      }
      fseeko(in, start+i-131, SEEK_SET);
      for (int j=0; j<128; j++) {
        int x=getc(in);
        if (x+1>numpat) numpat=x+1;
      }
      if (numpat<65) AUD_DET(AUDIO,i-1083,1084+numpat*256*chn,len,4);
      fseeko(in, savedpos, SEEK_SET);
    }
    
    // Detect .s3m file header 
    if (buf0==0x1a100000) s3mi=i,s3mno=s3mni=0;
    if (s3mi) {
      const int p=int(i-s3mi);
      if (p==4) s3mno=bswap(buf0)&0xffff,s3mni=(bswap(buf0)>>16);
      else if (p==16 && (((buf1>>16)&0xff)!=0x13 || buf0!=0x5343524d)) s3mi=0;
      else if (p==16) {
        int64_t savedpos=ftello(in);
        int b[31],sam_start=(1<<16),sam_end=0,ok=1;
        for (int j=0;j<s3mni;j++) {
          fseeko(in, start+s3mi-31+0x60+s3mno+j*2, SEEK_SET);
          int i1=getc(in);
          i1+=getc(in)*256;
          fseeko(in, start+s3mi-31+i1*16, SEEK_SET);
          i1=getc(in);
          if (i1==1) { // type: sample
            for (int k=0;k<31;k++) b[k]=fgetc(in);
            int len=b[15]+(b[16]<<8);
            int ofs=b[13]+(b[14]<<8);
            if (b[30]>1) ok=0;
            if (ofs*16<sam_start) sam_start=ofs*16;
            if (ofs*16+len>sam_end) sam_end=ofs*16+len;
          }
        }
        if (ok && sam_start<(1<<16)) AUD_DET(AUDIO,s3mi-31,sam_start,sam_end-sam_start,0);
        s3mi=0;
        fseeko(in, savedpos, SEEK_SET);
      }
    }
    
    // Detect .bmp image
    if ((buf0&0xffff)==16973) imgbpp=bmpx=bmpy=bmpof=0,bmp=i;  //possible 'BM'
    if (bmp) {
      const int p=int(i-bmp);
      if (p==12) bmpof=bswap(buf0);
      else if (p==16 && buf0!=0x28000000) bmp=0; //windows bmp?
      else if (p==20) bmpx=bswap(buf0),bmp=((bmpx==0||bmpx>0x40000)?0:bmp); //width
      else if (p==24) bmpy=abs((int)bswap(buf0)),bmp=((bmpy==0||bmpy>0x20000)?0:bmp); //height
      else if (p==27) imgbpp=c,bmp=((imgbpp!=1 && imgbpp!=4 && imgbpp!=8 && imgbpp!=24)?0:bmp);
      else if (p==31) {
        if (imgbpp!=0 && buf0==0 && bmpx>1) {
          if (imgbpp==1) IMG_DET(IMAGE1,bmp-1,bmpof,(((bmpx-1)>>5)+1)*4,bmpy);
          else if (imgbpp==4) IMG_DET(IMAGE4,bmp-1,bmpof,((bmpx>>1)+3)&-4,bmpy);
          else if (imgbpp==8) IMG_DET(IMAGE8,bmp-1,bmpof,(bmpx+3)&-4,bmpy);
          else if (imgbpp==24) IMG_DET(IMAGE24,bmp-1,bmpof,((bmpx*3)+3)&-4,bmpy);
        }
        bmp=0;
      }
    }
    
    // Detect .pbm .pgm .ppm image 
    if ((buf0&0xfff0ff)==0x50300a && txtLen<txtMinLen ) { //see if text, only single files
      pgmn=(buf0&0xf00)>>8;
      if (pgmn>=4 && pgmn <=6) pgm=i,pgm_ptr=pgmw=pgmh=pgmc=pgmcomment=0;
    }
    if (pgm) {
      if (i-pgm==1 && c==0x23) pgmcomment=1; //pgm comment
      if (!pgmcomment && pgm_ptr) {
        int s=0;
        if ((c==0x20|| c==0x0a) && !pgmw) s=1;
        else if (c==0x0a && !pgmh) s=2;
        else if (c==0x0a && !pgmc && pgmn!=4) s=3;
        if (s) {
          if (pgm_ptr>=32) pgm_ptr=31;
          pgm_buf[pgm_ptr++]=0;
          int v=atoi(&pgm_buf[0]);
          if (v<5 || v>20000) v=0;
          if (s==1) pgmw=v; else if (s==2) pgmh=v; else if (s==3) pgmc=v;
          if (v==0 || (s==3 && v>255)) pgm=0; else pgm_ptr=0;
        }
      }
      if (!pgmcomment) pgm_buf[pgm_ptr++]=((c>='0' && c<='9') || ' ')?c:0;
      if (pgm_ptr>=32) pgm=pgm_ptr=0;
      if (i-pgm>255) pgm=pgm_ptr=0;
      if (pgmcomment && c==0x0a) pgmcomment=0;
      if (pgmw && pgmh && !pgmc && pgmn==4) IMG_DET(IMAGE1,pgm-2,i-pgm+3,(pgmw+7)/8,pgmh);
      if (pgmw && pgmh && pgmc && pgmn==5) IMG_DET(IMAGE8,pgm-2,i-pgm+3,pgmw,pgmh);
      if (pgmw && pgmh && pgmc && pgmn==6) IMG_DET(IMAGE24,pgm-2,i-pgm+3,pgmw*3,pgmh);
    }
    
    // Detect .rgb image
    if ((buf0&0xffff)==0x01da) rgbi=i,rgbx=rgby=0;
    if (rgbi) {
      const int p=int(i-rgbi);
      if (p==1 && c!=0) rgbi=0;
      else if (p==2 && c!=1) rgbi=0;
      else if (p==4 && (buf0&0xffff)!=1 && (buf0&0xffff)!=2 && (buf0&0xffff)!=3) rgbi=0;
      else if (p==6) rgbx=buf0&0xffff,rgbi=(rgbx==0?0:rgbi);
      else if (p==8) rgby=buf0&0xffff,rgbi=(rgby==0?0:rgbi);
      else if (p==10) {
        int z=buf0&0xffff;
        if (rgbx && rgby && (z==1 || z==3 || z==4)) IMG_DET(IMAGE8,rgbi-1,512,rgbx,rgby*z);
        rgbi=0;
      }
    }
    
    // Detect .tiff file header (2/8/24 bit color, not compressed).
    if (buf1==0x49492a00 && n>i+(int)bswap(buf0)) {
      U64 savedpos=ftello(in);
      fseeko(in, start+i+(int)bswap(buf0)-7, SEEK_SET);

      // read directory
      int dirsize=getc(in);
      int tifx=0,tify=0,tifz=0,tifzb=0,tifc=0,tifofs=0,tifofval=0,b[12],tifsiz=0;
      if (getc(in)==0) {
        for (int i=0; i<dirsize; i++) {
          for (int j=0; j<12; j++) b[j]=getc(in);
          if (b[11]==EOF) break;
          int tag=b[0]+(b[1]<<8);
          int tagfmt=b[2]+(b[3]<<8);
          int taglen=b[4]+(b[5]<<8)+(b[6]<<16)+(b[7]<<24);
          int tagval=b[8]+(b[9]<<8)+(b[10]<<16)+(b[11]<<24);
          //printf("Tag %d  val %d\n",tag, tagval);
          if (tagfmt==3||tagfmt==4) {
            if (tag==256) tifx=tagval;
            else if (tag==257) tify=tagval;
            else if (tag==258) tifzb=taglen==1?tagval:8; // bits per component
            else if (tag==259) tifc=tagval; // 1 = no compression
            else if (tag==273 && tagfmt==4) tifofs=tagval,tifofval=(taglen<=1);
            else if (tag==277) tifz=tagval; // components per pixel
            else if (tag==279) tifsiz=tagval;
          }
        }
      }
      if (tifx>1 && tify && tifzb && (tifz==1 || tifz==3) && (tifc==1) && (tifofs && tifofs+i<n)) {
        if (!tifofval) {
          fseeko(in, start+i+tifofs-7, SEEK_SET);
          for (int j=0; j<4; j++) b[j]=getc(in);
          tifofs=b[0]+(b[1]<<8)+(b[2]<<16)+(b[3]<<24);
        }
        if (tifofs && tifofs<(1<<18) && tifofs+i<n && tifx>1) {
          if (tifz==1 && tifzb==1) IMG_DET(IMAGE1,i-7,tifofs,((tifx-1)>>3)+1,tify);
          else if (tifz==1 && tifzb==8 && tifx<30000) IMG_DET(IMAGE8,i-7,tifofs,tifx,tify);
          else if (tifz==3 && tifzb==8 && tifx<30000) IMG_DET(IMAGE24,i-7,tifofs,tifx*3,tify);
        }
      }
      else if  (tifx>1 && tify && tifzb &&  (tifc==6) && (tifofs && tifofs+i<n)) {
          TIFFJPEG_DET(i-7,tifofs,tifsiz);
      }
      fseeko(in, savedpos, SEEK_SET);
    }
    
    // Detect .tga image (8-bit 256 colors or 24-bit uncompressed)
    if (buf1==0x00010100 && buf0==0x00000118) tga=i,tgax=tgay,tgaz=8,tgat=1;
    else if (buf1==0x00000200 && buf0==0x00000000) tga=i,tgax=tgay,tgaz=24,tgat=2;
    else if (buf1==0x00000300 && buf0==0x00000000) tga=i,tgax=tgay,tgaz=8,tgat=3;
    if (tga) {
      if (i-tga==8) tga=(buf1==0?tga:0),tgax=(bswap(buf0)&0xffff),tgay=(bswap(buf0)>>16);
      else if (i-tga==10) {
        if (tgaz==(int)((buf0&0xffff)>>8) && tgax<30000 && tgax>1 && tgay>1 && tgay<30000){
          if (tgat==1) IMG_DET(IMAGE8,tga-7,18+256*3,tgax,tgay);
          else if (tgat==2) IMG_DET(IMAGE24,tga-7,18,tgax*3,tgay);
          else if (tgat==3) IMG_DET(IMAGE8,tga-7,18,tgax,tgay);
        }
        tga=0;
      }
    }
    // Detect .gif
    if (type==DEFAULT && dett==GIF && i==0) {
      dett=DEFAULT;
      if (c==0x2c || c==0x21) gif=2,gifi=2;
    }
    if (!gif && (buf1&0xffff)==0x4749 && (buf0==0x46383961 || buf0==0x46383761)) gif=1,gifi=i+5;
    if (gif) {
      if (gif==1 && i==gifi) gif=2,gifi=i+5+((c&128)?(3*(2<<(c&7))):0);
      if (gif==2 && i==gifi) {
        if ((buf0&0xff0000)==0x210000) gif=5,gifi=i;
        else if ((buf0&0xff0000)==0x2c0000) gif=3,gifi=i;
        else gif=0;
      }
      if (gif==3 && i==gifi+6) gifw=(bswap(buf0)&0xffff);
      if (gif==3 && i==gifi+7) gif=4,gifc=gifb=0,gifa=gifi=i+2+((c&128)?(3*(2<<(c&7))):0);
      if (gif==4 && i==gifi) {
        if (c>0 && gifb && gifc!=gifb) gifw=0;
        if (c>0) gifb=gifc,gifc=c,gifi+=c+1;
        else if (!gifw) gif=2,gifi=i+3;
        else if (i-gifa+2<10) gif=0;
        else return fseek(in, start+gifa-1, SEEK_SET),detd=i-gifa+2,info=gifw,dett=GIF;
      }
      if (gif==5 && i==gifi) {
        if (c>0) gifi+=c+1; else gif=2,gifi=i+3;
      }
    }
    
    // Detect EXE if the low order byte (little-endian) XX is more
    // recently seen (and within 4K) if a relative to absolute address
    // conversion is done in the context CALL/JMP (E8/E9) XX xx xx 00/FF
    // 4 times in a row.  Detect end of EXE at the last
    // place this happens when it does not happen for 64KB.

    if (((buf1&0xfe)==0xe8 || (buf1&0xfff0)==0x0f80) && ((buf0+1)&0xfe)==0) {
      int r=buf0>>24;  // relative address low 8 bits
      int a=((buf0>>24)+i)&0xff;  // absolute address low 8 bits
      int rdist=int(i-relpos[r]);
      int adist=int(i-abspos[a]);
      if (adist<rdist && adist<0x800 && abspos[a]>5) {
        e8e9last=i;
        ++e8e9count;
        if (e8e9pos==0 || e8e9pos>abspos[a]) e8e9pos=abspos[a];
      }
      else e8e9count=0;
      if (type==DEFAULT && e8e9count>=4 && e8e9pos>5)
        return fseeko(in, start+e8e9pos-5, SEEK_SET), EXE;
      abspos[a]=i;
      relpos[r]=i;
    }
    if (i-e8e9last>0x4000) {
      if (type==EXE) return fseeko(in, start+e8e9last, SEEK_SET), DEFAULT;
      e8e9count=0,e8e9pos=0;
    }
  
    // base64 encoded data detection
    // detect base64 in html/xml container, single stream
    // ';base64,' or '![CDATA['
    if (b64s1==0 &&   ((buf1==0x3b626173 && buf0==0x6536342c)||(buf1==0x215b4344 && buf0==0x4154415b))) b64s1=1,b64h=i+1,base64start=i+1; //' base64'
    else if (b64s1==1 && (isalnum(c) || (c == '+') || (c == '/')||(c == '=')) ) {
        continue;
        }  
    else if (b64s1==1) {
         base64end=i,b64s1=0;
         if (base64end -base64start>128) B64_DET(BASE64,b64h, 8,base64end -base64start);
    }
   
   // detect base64 in eml, etc. multiline
   if (b64s==0 && buf0==0x73653634 && ((buf1&0xffffff)==0x206261 || (buf1&0xffffff)==0x204261)) b64s=1,b64p=i-6,b64h=0,b64slen=0,b64lcount=0; //' base64' ' Base64'
    else if (b64s==1 && buf0==0x0D0A0D0A ) {
        b64s=2,b64h=i+1,b64slen=b64h-b64p;
        base64start=i+1;
        if (b64slen>192) b64s=0; //drop if header is larger 
        }
    else if (b64s==2 && (buf0&0xffff)==0x0D0A && b64line==0) {
         b64line=i-base64start,b64nl=i+2;//capture line lenght
         if (b64line<=4 || b64line>255) b64s=0;
         else continue;
    }
    else if (b64s==2 && (buf0&0xffff)==0x0D0A  && b64line!=0 && (buf0&0xffffff)!=0x3D0D0A && buf0!=0x3D3D0D0A ){
         if (i-b64nl+1<b64line && buf0!=0x0D0A0D0A) { // if smaller and not padding
            base64end=i-1;
            if (((base64end-base64start)>512) && ((base64end-base64start)<base64max)){
             b64s=0;
             B64_DET(BASE64,b64h,b64slen,base64end -base64start);
            }
         }
         else if (buf0==0x0D0A0D0A) { // if smaller and not padding
           base64end=i-1-2;
           if (((base64end-base64start)>512) && ((base64end-base64start)<base64max))
               B64_DET(BASE64,b64h,b64slen,base64end -base64start);
           b64s=0;
         }
         b64nl=i+2; //update 0x0D0A pos
         b64lcount++;
         continue;
         }
    else if (b64s==2 && ((buf0&0xffffff)==0x3D0D0A ||buf0==0x3D3D0D0A)) { //if padding '=' or '=='
        base64end=i-1;
        b64s=0;
        if (((base64end-base64start)>512) && ((base64end-base64start)<base64max))
            B64_DET(BASE64,b64h,b64slen,base64end -base64start);
    }
    else if (b64s==2 && (is_base64(c) || c=='='))   continue;
    else if (b64s==2)   b64s=0;
   /* // this version probably breaks on vm.dll
     // Detect base64 encoded data
    if (b64s==0 && buf0==0x73653634 && ((buf1&0xffffff)==0x206261 || (buf1&0xffffff)==0x204261)) b64s=1,b64i=i-6; //' base64' ' Base64'
    if (b64s==0 && ((buf1==0x3b626173 && buf0==0x6536342c) || (buf1==0x215b4344 && buf0==0x4154415b))) b64s=3,b64i=i+1; // ';base64,' '![CDATA['
    if (b64s>0) {
      if (b64s==1 && buf0==0x0d0a0d0a) b64s=((i-b64i>=128)?0:2),b64i=i+1,b64line=0;
      else if (b64s==2 && (buf0&0xffff)==0x0d0a && b64line==0) b64line=i+1-b64i,b64nl=i;
      else if (b64s==2 && (buf0&0xffff)==0x0d0a && b64line>0 && (buf0&0xffffff)!=0x3d0d0a) {
         if (i-b64nl<b64line && buf0!=0x0d0a0d0a) i-=1,b64s=5;
         else if (buf0==0x0d0a0d0a) i-=3,b64s=5;
         else if (i-b64nl==b64line) b64nl=i;
         else b64s=0;
      }
      else if (b64s==2 && (buf0&0xffffff)==0x3d0d0a) i-=1,b64s=5; // '=' or '=='
      else if (b64s==2 && !(isalnum(c) || c=='+' || c=='/' || c==10 || c==13 || c=='=')) b64s=0;
      if (b64line>0 && (b64line<=4 || b64line>255)) b64s=0;
      if (b64s==3 && i>=b64i && !(isalnum(c) || c=='+' || c=='/' || c=='=')) b64s=4;
      if ((b64s==4 && i-b64i>128) || (b64s==5 && i-b64i>512 && i-b64i<(1<<27))) return fseek(in, start+b64i, SEEK_SET),detd=i-b64i,BASE64;
      if (b64s>3) b64s=0;
    }*/
    
    //detect ascii85 encoded data
    //headers: stream\n stream\r\n oNimage\n utimage\n \nimage\n
    if (b85s==0 && ((buf0==0x65616D0A && (buf1&0xffffff)==0x737472)|| (buf0==0x616D0D0A && buf1==0x73747265)|| (buf0==0x6167650A && buf1==0x6F4E696D) || (buf0==0x6167650A && buf1==0x7574696D) || (buf0==0x6167650A && (buf1&0xffffff)==0x0A696D))){
	    b85s=1,b85p=i-6,b85h=0,b85slen=0,b85lcount=0; // 
         //else if (b85s==1 && buf0==0x0D0A0D0A ) 
        b85s=2,b85h=i+1,b85slen=b85h-b85p;
        base85start=i+1;
        if (b85slen>128) b85s=0; //drop if header is larger 
        }
    else if (b85s==2){
        if  ((buf0&0xff)==0x0A && b85line==0) {
            b85line=i-base85start,b85nl=i+2;//capture line lenght
            if (b85line<=4 || b85line>255) b85s=0;
            else continue;
        }
        else if ( ((buf0&0xff)==0x7E|| (buf0&0xffff)==0x0a7E)) { //if padding '=' or '=='
            base85end=i-((buf0&0xff00)==0x0a00);
            b85s=0;
            if (((base85end-base85start)>60) && ((base85end-base85start)<base85max))
            B85_DET(BASE85,b85h,b85slen,base85end -base85start);
        }
        else if ( (is_base85(c)))  	    continue;
        else     b85s=0;   
    }

 
     //Detect text and utf-8   teolc=0,teoll=0;
    if (txtStart==0 && !gif && !soi && !pgm && !rgbi && !bmp && !wavi && !tga && !b64s1 && !b64s && !b85s1 && !b85s &&
        ((c<128 && c>=32) || c==10 || c==13 || c==0x12 || c==9 )) txtStart=1,txtOff=i,txt0=0,txta=0;
    if (txtStart   ) {
       //if ((buf0&0xffff)==0x0D0A ){
       //    if (i-teoll<90) teolw++;
       ///    teoll=i;
       //} 
       //if ((buf0&0xff)==0x0A && ((buf0&0xff00)!=0x0d00) ){
       //    if (i-teoll<90) teolu++;
       //    teoll=i;
      // }  
        if ((c<128 && c>=32) || c==10 || c==13 || c==0x12 || c==9) {
            ++txtLen;
        if ((c>='a' && c<='z') ||  (c>='A' && c<='Z')) txta++;
        if (c>='0' && c<='9') txt0++;
            if (i-txtbinp>512) txtbinc=txtbinc>>2;
       }
       else if ((c&0xE0)==0xc0 && utfc==0){ //if possible UTF8 2 byte
            utfc=2,utfb=1;
            }
       else if ((c&0xF0)==0xE0 && utfc==0){//if possible UTF8 3 byte
            utfc=3,utfb=1;
            }
       else if ((c&0xF8)==0xF0 && utfc==0){//if possible UTF8 4 byte
            utfc=4,utfb=1;
            }
       else if ((c&0xFC)==0xF8 && utfc==0){ //if possible UTF8 5 byte
            utfc=5,utfb=1;
            }
       else if ((c&0xFE)==0xFC && utfc==0){//if possible UTF8 6 byte
            utfc=6,utfb=1;
            }

       else if (utfc>=2 && utfb>=1 && utfb<=6 && (c&0xC0)==0x80){
            utfb=utfb+1; //inc byte count
            //overlong UTF-8 sequence, UTF-16 surrogates as well as U+FFFE and U+FFFF test needed!!!
            switch (utfc){ //switch by detected type
            case 2:
                 if (utfb==2) txtLen=txtLen+2,utfc=0,utfb=0,txtIsUTF8=1;
                  break;
            case 3:
                 if (utfb==3) txtLen=txtLen+3,utfc=0,utfb=0,txtIsUTF8=1;
                  break;
            case 4:
                 if (utfb==4) txtLen=txtLen+4,utfc=0,utfb=0,txtIsUTF8=1;
                 break;
            case 5:
                 if (utfb==5) txtLen=txtLen+5,utfc=0,utfb=0,txtIsUTF8=1;
                 break;
            case 6:
                 if (utfb==6) txtLen=txtLen+6,utfc=0,utfb=0,txtIsUTF8=1;
                 break;
            }
       }
       else if (utfc>=2 && utfb>=1){ // wrong utf
             txtbinc=txtbinc+1;
             txtbinp=i;
             txtLen=txtLen+utfb;
             ++txtLen;
            utfc=utfb=txtIsUTF8=0;
            if (txtbinc>=128){
            if ( txtLen<txtMinLen){
               txtStart=txtOff=txtLen=txtbinc=0;
                   txtIsUTF8=utfc=utfb=0;
            }
            else{
                if (txta<txt0) info=1; else info=0; //use num0-9
               // if ((teolw==0 && teolu>100) || (teolu==0 && teolw>100)) info=info+2;
                if (type==TEXT|| type==TEXT0 ||type== TXTUTF8 ) return fseeko(in, start+txtLen, SEEK_SET),DEFAULT;
                if (info==1)  return fseeko(in, start+txtOff, SEEK_SET),TEXT0;
                if (txtIsUTF8==1)
                    return fseeko(in, start+txtOff, SEEK_SET),TXTUTF8;
                else
                    return fseeko(in, start+txtOff, SEEK_SET),TEXT;
            }
          
        }
       }
       else if (txtLen<txtMinLen){
            txtbinc=txtbinc+1;
            txtbinp=i;
            ++txtLen;
            if (txtbinc>=128)
            txtStart=txtLen=txtOff=txtbinc=0;
            utfc=utfb=0;
       }
       else if (txtLen>=txtMinLen){
            txtbinc=txtbinc+1;
            txtbinp=i;
            ++txtLen;
            if (txtbinc>=128){
                if (txta<txt0)   info=1; else info=0 ;
                //if ((teolw==0 && teolu>100) || (teolu==0 && teolw>100)) info=info+2;
                if (type==TEXT|| type==TEXT0 ||type== TXTUTF8 ) return fseeko(in, start+txtLen, SEEK_SET),DEFAULT;
                if (info==1)  return fseeko(in, start+txtOff, SEEK_SET),TEXT0;
                if (txtIsUTF8==1)
                    return fseeko(in, start+txtOff, SEEK_SET),TXTUTF8;
                 else
                    return fseeko(in, start+txtOff, SEEK_SET),TEXT;
            }
      
       } 
    }
  }
  if (txtStart) {
        if ( txtLen>=txtMinLen || (/*it==0 &&*/ s1==0 && txtLen==n)) { //ignore minimum lenght if whole file(s1==0) is text
            if (txta<txt0)   info=1; else info=0;
            //if ((teolw==0 && teolu>100) || (teolu==0 && teolw>100)) info=info+2;

            if (type==TEXT || type==TEXT0 ||type== TXTUTF8 ) return fseeko(in, start+txtLen, SEEK_SET),DEFAULT;
            if (info==1)  return fseeko(in, start+txtOff, SEEK_SET),TEXT0;
            if (txtIsUTF8==1)
                return fseeko(in, start+txtOff, SEEK_SET),TXTUTF8;
            else
                return fseeko(in, start+txtOff, SEEK_SET),TEXT;
        }
 }      
  return type;


}

typedef enum {FDECOMPRESS, FCOMPARE, FDISCARD} FMode;

// Print progress: n is the number of bytes compressed or decompressed
void printStatus(U64 n, U64 size,int tid=-1) {
if (level>0 && tid>=0)  printf("%2d %6.2f%%\b\b\b\b\b\b\b\b\b\b",tid, float(100)*n/(size+1)), fflush(stdout);
else if (level>0)  printf("%6.2f%%\b\b\b\b\b\b\b", float(100)*n/(size+1)), fflush(stdout);
}

void encode_cd(FILE* in, FILE* out, int len, int info) {
  const int BLOCK=2352;
  U8 blk[BLOCK];
  fputc((len%BLOCK)>>8,out);
  fputc(len%BLOCK,out);
  for (int offset=0; offset<len; offset+=BLOCK) {
    if (offset+BLOCK > len) {
      fread(&blk[0], 1, len-offset, in);
      fwrite(&blk[0], 1, len-offset, out);
    } else {
      fread(&blk[0], 1, BLOCK, in);
      if (info==3) blk[15]=3;
      if (offset==0) fwrite(&blk[12], 1, 4+4*(blk[15]!=1), out);
      fwrite(&blk[16+8*(blk[15]!=1)], 1, 2048+276*(info==3), out);
      if (offset+BLOCK*2 > len && blk[15]!=1) fwrite(&blk[16], 1, 4, out);
    }
  }
}

int decode_cd(FILE *in, int size, FILE *out, FMode mode, U64 &diffFound) {
  const int BLOCK=2352;
  U8 blk[BLOCK];
  long i=0, i2=0;
  int a=-1, bsize=0, q=fgetc(in);
  q=(q<<8)+fgetc(in);
  size-=2;
  while (i<size) {
    if (size-i==q) {
      fread(blk, q, 1, in);
      fwrite(blk, q, 1, out);
      i+=q;
      i2+=q;
    } else if (i==0) {
      fread(blk+12, 4, 1, in);
      if (blk[15]!=1) fread(blk+16, 4, 1, in);
      bsize=2048+(blk[15]==3)*276;
      i+=4*(blk[15]!=1)+4;
    } else {
      a=(blk[12]<<16)+(blk[13]<<8)+blk[14];
    }
    fread(blk+16+(blk[15]!=1)*8, bsize, 1, in);
    i+=bsize;
    if (bsize>2048) blk[15]=3;
    if (blk[15]!=1 && size-q-i==4) {
      fread(blk+16, 4, 1, in);
      i+=4;
    }
    expand_cd_sector(blk, a, 0);
    if (mode==FDECOMPRESS) fwrite(blk, BLOCK, 1, out);
    else if (mode==FCOMPARE) for (int j=0; j<BLOCK; ++j) if (blk[j]!=getc(out) && !diffFound) diffFound=i2+j+1;
    i2+=BLOCK;
  }
  return i2;
}

int encode_zlib(FILE* in, FILE* out, int len) {
  const int BLOCK=1<<16, LIMIT=128;
  U8 zin[BLOCK*2],zout[BLOCK],zrec[BLOCK*2], diffByte[81*LIMIT];
  int diffPos[81*LIMIT];
  
  // Step 1 - parse offset type form zlib stream header
  long pos=ftell(in);
  unsigned int h1=fgetc(in), h2=fgetc(in);
  fseek(in, pos, SEEK_SET);
  int zh=parse_zlib_header(h1*256+h2);
  int memlevel,clevel,window=zh==-1?0:MAX_WBITS+10+zh/4,ctype=zh%4;
  int minclevel=window==0?1:ctype==3?7:ctype==2?6:ctype==1?2:1;
  int maxclevel=window==0?9:ctype==3?9:ctype==2?6:ctype==1?5:1;

  // Step 2 - check recompressiblitiy, determine parameters and save differences
  z_stream main_strm, rec_strm[81];
  int diffCount[81], recpos[81], main_ret=Z_STREAM_END;
  main_strm.zalloc=Z_NULL; main_strm.zfree=Z_NULL; main_strm.opaque=Z_NULL;
  main_strm.next_in=Z_NULL; main_strm.avail_in=0;
  if (zlib_inflateInit(&main_strm,zh)!=Z_OK) return false;
  for (int i=0; i<81; i++) {
    memlevel=(i%9)+1;
    clevel=(i/9)+1;
    rec_strm[i].zalloc=Z_NULL; rec_strm[i].zfree=Z_NULL; rec_strm[i].opaque=Z_NULL;
    rec_strm[i].next_in=Z_NULL; rec_strm[i].avail_in=0;
    int ret=deflateInit2(&rec_strm[i], clevel, Z_DEFLATED, window-MAX_WBITS, memlevel, Z_DEFAULT_STRATEGY);
    diffCount[i]=(clevel>=minclevel && clevel<=maxclevel && ret==Z_OK)?0:LIMIT;
    recpos[i]=BLOCK*2;
    diffPos[i*LIMIT]=-1;
    diffByte[i*LIMIT]=0;
  }
  for (int i=0; i<len; i+=BLOCK) {
    unsigned int blsize=min(len-i,BLOCK);
    for (int j=0; j<81; j++) {
      if (diffCount[j]>=LIMIT) continue;
      memmove(&zrec[0], &zrec[BLOCK], BLOCK);
      recpos[j]-=BLOCK;
    }
    memmove(&zin[0], &zin[BLOCK], BLOCK);
    fread(&zin[BLOCK], 1, blsize, in); // Read block from input file
    
    // Decompress/inflate block
    main_strm.next_in=&zin[BLOCK]; main_strm.avail_in=blsize;
    do {
      main_strm.next_out=&zout[0]; main_strm.avail_out=BLOCK;
      main_ret=inflate(&main_strm, Z_FINISH);

      // Recompress/deflate block with all possible parameters
      for (int j=0; j<81; j++) {
        if (diffCount[j]>=LIMIT) continue;
        rec_strm[j].next_in=&zout[0];  rec_strm[j].avail_in=BLOCK-main_strm.avail_out;
        rec_strm[j].next_out=&zrec[recpos[j]]; rec_strm[j].avail_out=BLOCK*2-recpos[j];
        int ret=deflate(&rec_strm[j], (int)main_strm.total_in == len ? Z_FINISH : Z_NO_FLUSH);
        if (ret!=Z_BUF_ERROR && ret!=Z_STREAM_END && ret!=Z_OK) { diffCount[j]=LIMIT; continue; }

        // Compare
        int end=2*BLOCK-(int)rec_strm[j].avail_out;
        int tail=max(main_ret==Z_STREAM_END ? len-(int)rec_strm[j].total_out : 0,0);
        for (int k=recpos[j]; k<end+tail; k++) {
          if ((k<end && i+k-BLOCK<len && zrec[k]!=zin[k]) || k>=end) {
            if (++diffCount[j]<LIMIT) {
              const int p=j*LIMIT+diffCount[j];
              diffPos[p]=i+k-BLOCK;
              diffByte[p]=zin[k];
            }
          }
        }
        recpos[j]=2*BLOCK-rec_strm[j].avail_out;
      }
    } while (main_strm.avail_out==0 && main_ret==Z_BUF_ERROR);
    if (main_ret!=Z_BUF_ERROR && main_ret!=Z_STREAM_END) break;
  }
  int minCount=LIMIT, index;
  for (int i=80; i>=0; i--) {
    deflateEnd(&rec_strm[i]);
    if (diffCount[i]<minCount) {
      minCount=diffCount[i];
      memlevel=(i%9)+1;
      clevel=(i/9)+1;
      index=i;
    }
  }
  inflateEnd(&main_strm);
  if (minCount==LIMIT) return false;
  
  // Step 3 - write parameters, differences and precompressed (inflated) data
  fputc(diffCount[index], out);
  fputc(window, out);
  fputc(index, out);
  for (int i=0; i<=diffCount[index]; i++) {
    const int v=i==diffCount[index] ? len-diffPos[index*LIMIT+i]
                                    : diffPos[index*LIMIT+i+1]-diffPos[index*LIMIT+i]-1;
    fputc(v>>24, out); fputc(v>>16, out); fputc(v>>8, out); fputc(v, out);
  }
  for (int i=0; i<diffCount[index]; i++) fputc(diffByte[index*LIMIT+i+1], out);
  
  fseek(in, pos, SEEK_SET);
  main_strm.zalloc=Z_NULL; main_strm.zfree=Z_NULL; main_strm.opaque=Z_NULL;
  main_strm.next_in=Z_NULL; main_strm.avail_in=0;
  if (zlib_inflateInit(&main_strm,zh)!=Z_OK) return false;
  for (int i=0; i<len; i+=BLOCK) {
    unsigned int blsize=min(len-i,BLOCK);
    fread(&zin[0], 1, blsize, in);
    main_strm.next_in=&zin[0]; main_strm.avail_in=blsize;
    do {
      main_strm.next_out=&zout[0]; main_strm.avail_out=BLOCK;
      main_ret=inflate(&main_strm, Z_FINISH);
      fwrite(&zout[0], 1, BLOCK-main_strm.avail_out, out);
    } while (main_strm.avail_out==0 && main_ret==Z_BUF_ERROR);
    if (main_ret!=Z_BUF_ERROR && main_ret!=Z_STREAM_END) break;
  }
  return main_ret==Z_STREAM_END;
}

int decode_zlib(FILE* in, int size, FILE *out, FMode mode, U64 &diffFound) {
  const int BLOCK=1<<16, LIMIT=128;
  U8 zin[BLOCK],zout[BLOCK];
  int diffCount=min(fgetc(in),LIMIT-1);
  int window=fgetc(in)-MAX_WBITS;
  int index=fgetc(in);
  int memlevel=(index%9)+1;
  int clevel=(index/9)+1;  
  int len=0;
  int diffPos[LIMIT];
  diffPos[0]=-1;
  for (int i=0; i<=diffCount; i++) {
    int v=fgetc(in)<<24; v|=fgetc(in)<<16; v|=fgetc(in)<<8; v|=fgetc(in);
    if (i==diffCount) len=v+diffPos[i]; else diffPos[i+1]=v+diffPos[i]+1;
  }
  U8 diffByte[LIMIT];
  diffByte[0]=0;
  for (int i=0; i<diffCount; i++) diffByte[i+1]=fgetc(in);
  size-=7+5*diffCount;
  
  z_stream rec_strm;
  int diffIndex=1,recpos=0;
  rec_strm.zalloc=Z_NULL; rec_strm.zfree=Z_NULL; rec_strm.opaque=Z_NULL;
  rec_strm.next_in=Z_NULL; rec_strm.avail_in=0;
  int ret=deflateInit2(&rec_strm, clevel, Z_DEFLATED, window, memlevel, Z_DEFAULT_STRATEGY);
  if (ret!=Z_OK) return 0;
  for (int i=0; i<size; i+=BLOCK) {
    int blsize=min(size-i,BLOCK);
    fread(&zin[0], 1, blsize, in);
    rec_strm.next_in=&zin[0];  rec_strm.avail_in=blsize;
    do {
      rec_strm.next_out=&zout[0]; rec_strm.avail_out=BLOCK;
      ret=deflate(&rec_strm, i+blsize==size ? Z_FINISH : Z_NO_FLUSH);
      if (ret!=Z_BUF_ERROR && ret!=Z_STREAM_END && ret!=Z_OK) break;
      const int have=min(BLOCK-rec_strm.avail_out,len-recpos);
      while (diffIndex<=diffCount && diffPos[diffIndex]>=recpos && diffPos[diffIndex]<recpos+have) {
        zout[diffPos[diffIndex]-recpos]=diffByte[diffIndex];
        diffIndex++;
      }
      if (mode==FDECOMPRESS) fwrite(&zout[0], 1, have, out);
      else if (mode==FCOMPARE) for (int j=0; j<have; j++) if (zout[j]!=getc(out) && !diffFound) diffFound=recpos+j+1;
      recpos+=have;
      
    } while (rec_strm.avail_out==0);
  }
  while (diffIndex<=diffCount) {
    if (mode==FDECOMPRESS) fputc(diffByte[diffIndex], out);
    else if (mode==FCOMPARE) if (diffByte[diffIndex]!=getc(out) && !diffFound) diffFound=recpos+1;
    diffIndex++;
    recpos++;
  }  
  deflateEnd(&rec_strm);
  return recpos==len ? len : 0;
}

// 24-bit image data transform:
// simple color transform (b, g, r) -> (g, g-r, g-b)

void encode_bmp(FILE* in, FILE* out, int len, int width) {
  int r,g,b;
  for (int i=0; i<len/width; i++) {
    for (int j=0; j<width/3; j++) {
      b=fgetc(in), g=fgetc(in), r=fgetc(in);
      fputc(g, out);
      fputc(g-r, out);
      fputc(g-b, out);
    }
    for (int j=0; j<width%3; j++) fputc(fgetc(in), out);
  }
}

int decode_bmp(Encoder& en, int size, int width, FILE *out, FMode mode, U64 &diffFound) {
  int r,g,b,p;
  for (int i=0; i<size/width; i++) {
    p=i*width;
    for (int j=0; j<width/3; j++) {
      b=en.decompress(), g=en.decompress(), r=en.decompress();
      if (mode==FDECOMPRESS) {
        fputc(b-r, out);
        fputc(b, out);
        fputc(b-g, out);
      }
      else if (mode==FCOMPARE) {
        if (((b-r)&255)!=getc(out) && !diffFound) diffFound=p+1;
        if (b!=getc(out) && !diffFound) diffFound=p+2;
        if (((b-g)&255)!=getc(out) && !diffFound) diffFound=p+3;
        p+=3;
      }
    }
    for (int j=0; j<width%3; j++) {
      if (mode==FDECOMPRESS) {
        fputc(en.decompress(), out);
      }
      else if (mode==FCOMPARE) {
        if (en.decompress()!=getc(out) && !diffFound) diffFound=p+j+1;
      }
    }
  }
  return size;
}

// EXE transform: <encoded-size> <begin> <block>...
// Encoded-size is 4 bytes, MSB first.
// begin is the offset of the start of the input file, 4 bytes, MSB first.
// Each block applies the e8e9 transform to strings falling entirely
// within the block starting from the end and working backwards.
// The 5 byte pattern is E8/E9 xx xx xx 00/FF (x86 CALL/JMP xxxxxxxx)
// where xxxxxxxx is a relative address LSB first.  The address is
// converted to an absolute address by adding the offset mod 2^25
// (in range +-2^24).

void encode_exe(FILE* in, FILE* out, int len, int begin) {
  const int BLOCK=0x10000;
  Array<U8> blk(BLOCK);
  fprintf(out, "%c%c%c%c", begin>>24, begin>>16, begin>>8, begin);

  // Transform
  for (int offset=0; offset<len; offset+=BLOCK) {
    int size=min(int(len-offset), BLOCK);
    int bytesRead=fread(&blk[0], 1, size, in);
    if (bytesRead!=size) quit("encode_exe read error");
    for (int i=bytesRead-1; i>=5; --i) {
      if ((blk[i-4]==0xe8 || blk[i-4]==0xe9 || (blk[i-5]==0x0f && (blk[i-4]&0xf0)==0x80))
         && (blk[i]==0||blk[i]==0xff)) {
        int a=(blk[i-3]|blk[i-2]<<8|blk[i-1]<<16|blk[i]<<24)+offset+begin+i+1;
        a<<=7;
        a>>=7;
        blk[i]=a>>24;
        blk[i-1]=a^176;
        blk[i-2]=(a>>8)^176;
        blk[i-3]=(a>>16)^176;
      }
    }
    fwrite(&blk[0], 1, bytesRead, out);
  }
}

U64 decode_exe(Encoder& en, int size, FILE *out, FMode mode, U64 &diffFound, int s1=0, int s2=0) {
  const int BLOCK=0x10000;  // block size
  int begin, offset=6, a, showstatus=(s2!=0);
  U8 c[6];
  begin=en.decompress()<<24;
  begin|=en.decompress()<<16;
  begin|=en.decompress()<<8;
  begin|=en.decompress();
  size-=4;
  for (int i=4; i>=0; i--) c[i]=en.decompress();  // Fill queue

  while (offset<size+6) {
    memmove(c+1, c, 5);
    if (offset<=size) c[0]=en.decompress();
    // E8E9 transform: E8/E9 xx xx xx 00/FF -> subtract location from x
    if ((c[0]==0x00 || c[0]==0xFF) && (c[4]==0xE8 || c[4]==0xE9 || (c[5]==0x0F && (c[4]&0xF0)==0x80))
     && (((offset-1)^(offset-6))&-BLOCK)==0 && offset<=size) { // not crossing block boundary
      a=((c[1]^176)|(c[2]^176)<<8|(c[3]^176)<<16|c[0]<<24)-offset-begin;
      a<<=7;
      a>>=7;
      c[3]=a;
      c[2]=a>>8;
      c[1]=a>>16;
      c[0]=a>>24;
    }
    if (mode==FDECOMPRESS) putc(c[5], out);
    else if (mode==FCOMPARE && c[5]!=getc(out) && !diffFound) diffFound=offset-6+1;
    if (showstatus && !(offset&0xfff)) printStatus(s1+offset-6, s2);
    offset++;
  }
  return size;
}


//Based on XWRT 3.2 (29.10.2007) - XML compressor by P.Skibinski, inikep@gmail.com
#include "wrtpre.cpp"

void encode_txt(FILE* in, FILE* out, int len,int wrtn) {
    assert(wrtn<2);
   XWRT_Encoder* wrt;
   wrt=new XWRT_Encoder();
   wrt->defaultSettings(wrtn);
   wrt->WRT_start_encoding(in,out,len,false);
   delete wrt;
}

//called only when encode_txt output was smaller then input
int decode_txt(Encoder& en, int size, FILE *out, FMode mode, U64 &diffFound) {
    XWRT_Decoder* wrt;
    wrt=new XWRT_Decoder();
    FILE* dtmp;
    char c;
    int b=0;
    int bb=0;
    dtmp=tmpfile2();
    if (!dtmp) quit("ERR WRT tmpfile");
    //decompress file
    for (int i=0; i<size; i++) {
        c=en.decompress(); 
        putc(c,dtmp);    
    }
    fseeko(dtmp,0, SEEK_SET);
    wrt->defaultSettings(0);
    bb=wrt->WRT_start_decoding(dtmp);
    for ( int i=0; i<bb; i++) {
        b=wrt->WRT_decode();    
        if (mode==FDECOMPRESS) {
            fputc(b, out);
        }
        else if (mode==FCOMPARE) {
            if (b!=fgetc(out) && !diffFound) diffFound=i;
        }
    }
    fclose(dtmp);
    delete wrt;
    return bb; 
}

#include "ttafilter.cpp" //v2.0
void encode_audio(FILE* in, FILE* out, int len,int info, int info2) {
  compress(in,out,len,info,info2);
}

//called only when encode_txt output was smaller then input
int decode_audio(Encoder& en, int size, FILE *out, int info, int info2,int smr, FMode mode, U64 &diffFound) {
    FILE* dtmp;
    FILE* dtmp1;
    char c;
    int b=0;
    dtmp=tmpfile2();
    dtmp1=tmpfile2();
    //decompress file
    for (int i=0; i<size; i++) {
        c=en.decompress(); 
        putc(c,dtmp);    
    }
    //U64 dfile=ftello(dtmp);
    fseeko(dtmp,0, SEEK_SET);
    decompress (dtmp, dtmp1,info2,   info, info2,smr);
    fseeko(dtmp1,0, SEEK_SET);
    fclose(dtmp);
    for ( int i=0; i<info2; i++) {
        b=getc(dtmp1);
        if (mode==FDECOMPRESS) {
            fputc(b, out);
        }
        else if (mode==FCOMPARE) {
            if (b!=fgetc(out) && !diffFound) diffFound=i;
        }
    }
    fclose(dtmp1);
    return info2; 
}

// decode/encode base64 
static const char  table1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
bool isbase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/')|| (c == 10) || (c == 13));
}

int decode_base64(FILE *in, int size, FILE *out, FMode mode, U64 &diffFound){
    U8 inn[3];
    int i,len1=0, len=0, blocksout = 0;
    int fle=0;
    int linesize=0; 
    int outlen=0;
    int tlf=0,g=0;
    linesize=getc(in);
    outlen=getc(in);
    outlen+=(getc(in)<<8);
    outlen+=(getc(in)<<16);
    tlf=(getc(in));
    outlen+=((tlf&63)<<24);
    U8 *ptr,*fptr;
    ptr = (U8*)calloc((outlen>>2)*4+20, 1);
    if (!ptr) quit("Out of memory (decode_B64)");
    programChecker.alloc((outlen>>2)*4+10); //report used memory
    fptr=&ptr[0];
    tlf=(tlf&192);
    if (tlf==128)       tlf=10;        // LF: 10
    else if (tlf==64)   tlf=13;        // LF: 13
    else                tlf=0;
 
    while(fle<outlen){
        len=0;
        for(i=0;i<3;i++){
            inn[i] = getc( in );
            if(!feof(in)){
                len++;
                len1++;
            }
            else {
                inn[i] = 0,g=1;
            }
        }
        if(len){
            U8 in0,in1,in2;
            in0=inn[0],in1=inn[1],in2=inn[2];
            fptr[fle++]=(table1[in0>>2]);
            fptr[fle++]=(table1[((in0&0x03)<<4)|((in1&0xf0)>>4)]);
            fptr[fle++]=((len>1?table1[((in1&0x0f)<<2)|((in2&0xc0)>>6)]:'='));
            fptr[fle++]=((len>2?table1[in2&0x3f]:'='));
            blocksout++;
        }
        if(blocksout>=(linesize/4) && linesize!=0){ //no lf if linesize==0
            if( blocksout &&  !feof(in) && fle<=outlen) { //no lf if eof
                if (tlf) fptr[fle++]=(tlf);
                else fptr[fle++]=13,fptr[fle++]=10;
            }
            blocksout = 0;
        }
        if (g) break; //if past eof, break
    }
    //Write out or compare
    if (mode==FDECOMPRESS){
            fwrite(&ptr[0], 1, outlen, out);
        }
    else if (mode==FCOMPARE){
    for(i=0;i<outlen;i++){
        U8 b=fptr[i];
            if (b!=fgetc(out) && !diffFound) diffFound=ftello(out);
        }
    }
    free(ptr);
    programChecker.free(outlen+10);
    return outlen;
}
   
inline char valueb(char c){
       const char *p = strchr(table1, c);
       if(p) {
          return p-table1;
       } else {
          return 0;
       }
}

void encode_base64(FILE* in, FILE* out, int len) {
  int in_len = 0;
  int i = 0;
  int j = 0;
  int b=0;
  int lfp=0;
  int tlf=0;
  char src[4];
  U8 *ptr,*fptr;
  int b64mem=(len>>2)*3+10;
    ptr = (U8*)calloc(b64mem, 1);
    if (!ptr) quit("Out of memory (encode_B64)");
    programChecker.alloc(b64mem);
    fptr=&ptr[0];
    int olen=5;

  while (b=fgetc(in),in_len++ , ( b != '=') && is_base64(b) && in_len<=len) {
    if (b==13 || b==10) {
       if (lfp==0) lfp=in_len ,tlf=b;
       if (tlf!=b) tlf=0;
       continue;
    }
    src[i++] = b; 
    if (i ==4){
          for (j = 0; j <4; j++) src[j] = valueb(src[j]);
          src[0] = (src[0] << 2) + ((src[1] & 0x30) >> 4);
          src[1] = ((src[1] & 0xf) << 4) + ((src[2] & 0x3c) >> 2);
          src[2] = ((src[2] & 0x3) << 6) + src[3];
    
          fptr[olen++]=src[0];
          fptr[olen++]=src[1];
          fptr[olen++]=src[2];
      i = 0;
    }
  }

  if (i){
    for (j=i;j<4;j++)
      src[j] = 0;

    for (j=0;j<4;j++)
      src[j] = valueb(src[j]);

    src[0] = (src[0] << 2) + ((src[1] & 0x30) >> 4);
    src[1] = ((src[1] & 0xf) << 4) + ((src[2] & 0x3c) >> 2);
    src[2] = ((src[2] & 0x3) << 6) + src[3];

    for (j=0;(j<i-1);j++) {
        fptr[olen++]=src[j];
    }
  }
  fptr[0]=lfp&255; //nl lenght
  fptr[1]=len&255;
  fptr[2]=len>>8&255;
  fptr[3]=len>>16&255;
  if (tlf!=0) {
    if (tlf==10) fptr[4]=128;
    else fptr[4]=64;
  }
  else
      fptr[4]=len>>24&63; //1100 0000
  fwrite(&ptr[0], 1, olen, out);
  free(ptr);
  programChecker.free(b64mem);
}

//base85
int powers[5] = {85*85*85*85, 85*85*85, 85*85, 85, 1};

int decode_ascii85(FILE *in, int size, FILE *out, FMode mode, U64 &diffFound){
    int i;
    int fle=0;
    int nlsize=0; 
    int outlen=0;
    int tlf=0;
    nlsize=getc(in);
    outlen=getc(in);
    outlen+=(getc(in)<<8);
    outlen+=(getc(in)<<16);
    tlf=(getc(in));
    outlen+=((tlf&63)<<24);
    U8 *ptr,*fptr;
    ptr = (U8*)calloc((outlen>>2)*4+10, 1);
    if (!ptr) quit("Out of memory (decode_A85)");
    programChecker.alloc((outlen>>2)*4+10); //report used memory
    fptr=&ptr[0];
    tlf=(tlf&192);
    if (tlf==128)      tlf=10;        // LF: 10
    else if (tlf==64)  tlf=13;        // LF: 13
    else               tlf=0;
    int c, count = 0, lenlf = 0;
    uint32_t tuple = 0;

    while(fle<outlen){ 
        c = getc(in);
        if (c != EOF) {
            tuple |= ((U32)c) << ((3 - count++) * 8);
            if (count < 4) continue;
        }
        else if (count == 0) break;
        int i, lim;
        char out[5];
        if (tuple == 0 && count == 4) { // for 0x00000000
            if (nlsize && lenlf >= nlsize) {
                if (tlf) fptr[fle++]=(tlf);
                else fptr[fle++]=13,fptr[fle++]=10;
                lenlf = 0;
            }
            fptr[fle++]='z';
        }
        /*    else if (tuple == 0x20202020 && count == 4 ) {
            if (nlsize && lenlf >= nlsize) {
                if (tlf) fptr[fle++]=(tlf);
                else fptr[fle++]=13,fptr[fle++]=10;
                lenlf = 0;
            }
            fptr[fle++]='y',lenlf++;
        }*/
        else {
            for (i = 0; i < 5; i++) {
                out[i] = tuple % 85 + '!';
                tuple /= 85;
            }
            lim = 4 - count;
            for (i = 4; i >= lim; i--) {
                if (nlsize && lenlf >= nlsize && ((outlen-fle)>=5)) {//    skip nl if only 5 bytes left
                    if (tlf) fptr[fle++]=(tlf);
                    else fptr[fle++]=13,fptr[fle++]=10;
                    lenlf = 0;}
                fptr[fle++]=out[i],lenlf++;
            }
        }
        if (c == EOF) break;
        tuple = 0;
        count = 0;
    }
    if (mode==FDECOMPRESS){
        fwrite(&ptr[0], 1, outlen, out);
    }
    else if (mode==FCOMPARE){
        for(i=0;i<outlen;i++){
            U8 b=fptr[i];
            if (b!=fgetc(out) && !diffFound) diffFound=ftello(out);
        }
    }
    free(ptr);
    programChecker.free(outlen+10);
    return outlen;
}

void encode_ascii85(FILE* in, FILE* out, int len) {
    int lfp=0;
    int tlf=0;
    U8 *ptr,*fptr;
    int b64mem=(len>>2)*4+10;
    ptr = (U8*)calloc(b64mem, 1);
    if (!ptr) quit("Out of memory (encode_A85)");
    programChecker.alloc(b64mem);
    fptr=&ptr[0];
    int olen=5;
    int c, count = 0;
    uint32_t tuple = 0;
    for (int f=0;f<len;f++) {
        c = getc(in);
        if (c==13 || c==10) {
            if (lfp==0) lfp=f ,tlf=c;
            if (tlf!=c) tlf=0;
            continue;
        }
        if (c == 'z' && count == 0) {
            for (int i = 1; i < 5; i++) fptr[olen++]=0;
            continue;
        }
        /*    if (c == 'y' && count == 0) {
            for (int i = 1; i < 5; i++) fptr[olen++]=0x20;
            continue;
        }*/
        if (c == EOF) {        
            if (count > 0) {
                tuple += powers[count-1];
                for (int i = 1; i < count; i++) fptr[olen++]=tuple >> ((4 - i) * 8);
            }
            break;
        }
        tuple += (c - '!') * powers[count++];
        if (count == 5) {
            for (int i = 1; i < count; i++) fptr[olen++]=tuple >> ((4 - i) * 8);
            tuple = 0;
            count = 0;
        }
    }
    if (count > 0) {
        tuple += powers[count-1];
        for (int i = 1; i < count; i++) fptr[olen++]=tuple >> ((4 - i) * 8);
    }
    fptr[0]=lfp&255; //nl lenght
    fptr[1]=len&255;
    fptr[2]=len>>8&255;
    fptr[3]=len>>16&255;
    if (tlf!=0) {
        if (tlf==10) fptr[4]=128;
        else fptr[4]=64;
    }
    else
    fptr[4]=len>>24&63; //1100 0000
    fwrite(&ptr[0], 1, olen, out);
    free(ptr);
    programChecker.free(b64mem);
}

//SZDD
int decode_szdd(FILE *in, int size, int info, FILE *out, FMode mode, U64 &diffFound){
    LZSS* lz77;
    int r=0;
    //Write out or compare
    if (mode==FDECOMPRESS){
            lz77=new LZSS(in,out,size,(info>>25)*2);
             r=lz77->compress();
            delete lz77;
        }
    else if (mode==FCOMPARE){
        FILE* out1=tmpfile2();
        lz77=new LZSS(in,out1,size,(info>>25)*2);
        r=lz77->compress();
        delete lz77;
        fseeko(out1,0,SEEK_SET);
        for(int i=0;i<r;i++){
            U8 b=getc(out1);
            if (b!=fgetc(out) && !diffFound) diffFound=ftello(out);
        }
        fclose(out1);
    }
    return r;
}

void encode_szdd(FILE* in, FILE* out, int len) {
    LZSS* lz77;
    lz77=new LZSS(in,out,len&0x1ffffff,(len>>25)*2);
    lz77->decompress();
    delete lz77;
}

//mdf 
int decode_mdf(FILE *in, int size,  FILE *out, FMode mode, U64 &diffFound){
    int q=fgetc(in);
    q=(q<<8)+fgetc(in);
    q=(q<<8)+fgetc(in);
    const int BLOCK=2352;
    const int CHAN=96;
    U8 blk[BLOCK];
    U8 blk1[CHAN];
    U8 *ptr ; 
    ptr = (U8*)calloc(CHAN*q, 1);
    if (!ptr) quit("Out of memory (e_mdf)");
    //Write out or compare
    if (mode==FDECOMPRESS){
  		fread(&ptr[0], 1, CHAN*q, in);
   		for (int offset=0; offset<q; offset++) { 
     		fread(&blk[0], 1,  BLOCK, in);
      		fwrite(&blk[0], 1, BLOCK, out);
      		fwrite(&ptr[offset*CHAN], 1, CHAN, out);
  		}
    }
    else if (mode==FCOMPARE){
  		fread(&ptr[0], 1, CHAN*q, in);
		int offset=0;
        for( int i=3;i<size;){
        	fread(&blk[0], 1,  BLOCK, in);
        	for(int j=0;j<BLOCK;j++,i++){
            	U8 b=blk[j];
            	if (b!=fgetc(out) && !diffFound) diffFound=ftello(out);
            } 
            for(int j=0;j<CHAN;j++,i++){
            	U8 b=ptr[offset*CHAN+j];
            	if (b!=fgetc(out) && !diffFound) diffFound=ftello(out);
            }
            offset++;
        }
    }
    return size;
}

void encode_mdf(FILE* in, FILE* out, int len) {
    const int BLOCK=2352;
    const int CHAN=96;
  	U8 blk[BLOCK];
   	U8 blk1[CHAN];
   	int ql=len/(BLOCK+CHAN);
   	fputc(ql>>16,out); 
  	fputc(ql>>8,out);
  	fputc(ql,out);
  	int beginin=ftell(in);
  	//channel out
  	for (int offset=0; offset<ql; offset++) { 
      	fread(&blk[0], 1,  BLOCK, in); 
      	fread(&blk1[0], 1, CHAN, in);
	  	fwrite(&blk1[0], 1, CHAN, out);
  	}
  	fseek(in, beginin, SEEK_SET);
   	for (int offset=0; offset<ql; offset++) { 
      	fread(&blk[0], 1,  BLOCK, in);
	  	fread(&blk1[0], 1, CHAN, in);
      	fwrite(&blk[0], 1, BLOCK, out);
  }
}

int encode_gif(FILE* in, FILE* out, int len) {
  int codesize=fgetc(in),diffpos=0,hdrsize=6,clearpos=0,bsize=0;
  int beginin=ftell(in),beginout=ftell(out);
  U8 output[4096];
  fputc(hdrsize>>8, out);
  fputc(hdrsize&255, out);
  fputc(bsize, out);
  fputc(clearpos>>8, out);
  fputc(clearpos&255, out);
  fputc(codesize, out);
  for (int phase=0; phase<2; phase++) {
    fseek(in, beginin, SEEK_SET);
    int bits=codesize+1,shift=0,buf=0;
    int blocksize=0,maxcode=(1<<codesize)+1,last=-1,dict[4096];
    bool end=false;
    while ((blocksize=fgetc(in))>0 && ftell(in)-beginin<len && !end) {
      for (int i=0; i<blocksize; i++) {
        buf|=fgetc(in)<<shift;
        shift+=8;
        while (shift>=bits && !end) {
          int code=buf&((1<<bits)-1);
          buf>>=bits;
          shift-=bits;
          if (!bsize && code!=(1<<codesize)) {
            hdrsize+=4; fputc(0, out); fputc(0, out); fputc(0, out); fputc(0, out);
          }
          if (!bsize) bsize=blocksize;
          if (code==(1<<codesize)) {
            if (maxcode>(1<<codesize)+1) {
              if (clearpos && clearpos!=69631-maxcode) return 0;
              clearpos=69631-maxcode;
            }
            bits=codesize+1, maxcode=(1<<codesize)+1, last=-1;
          }
          else if (code==(1<<codesize)+1) end=true;
          else if (code>maxcode+1) return 0;
          else {
            int j=(code<=maxcode?code:last),size=1;
            while (j>=(1<<codesize)) {
              output[4096-(size++)]=dict[j]&255;
              j=dict[j]>>8;
            }
            output[4096-size]=j;
            if (phase==1) fwrite(&output[4096-size], 1, size, out); else diffpos+=size;
            if (code==maxcode+1) { if (phase==1) fputc(j, out); else diffpos++; }
            if (last!=-1) {
              if (++maxcode>=8191) return 0;
              if (maxcode<=4095)
              {
                dict[maxcode]=(last<<8)+j;
                if (phase==0) {
                  bool diff=false;
                  for (int m=(1<<codesize)+2;m<min(maxcode,4095);m++) if (dict[maxcode]==dict[m]) { diff=true; break; }
                  if (diff) {
                    hdrsize+=4;
                    j=diffpos-size-(code==maxcode);
                    fputc((j>>24)&255, out); fputc((j>>16)&255, out); fputc((j>>8)&255, out); fputc(j&255, out);
                    diffpos=size+(code==maxcode);
                  }
                }
              }
              if (maxcode>=((1<<bits)-1) && bits<12) bits++;
            }
            last=code;
          }
        }
      }
    }
  }
  diffpos=ftell(out);
  fseek(out, beginout, SEEK_SET);
  fputc(hdrsize>>8, out);
  fputc(hdrsize&255, out);
  fputc(255-bsize, out);
  fputc((clearpos>>8)&255, out);
  fputc(clearpos&255, out);
  fseek(out, diffpos, SEEK_SET);
  return ftell(in)-beginin==len-1;
}

#define gif_write_block(count) { output[0]=(count);\
if (mode==FDECOMPRESS) fwrite(&output[0], 1, (count)+1, out);\
else if (mode==FCOMPARE) for (int j=0; j<(count)+1; j++) if (output[j]!=getc(out) && !diffFound) diffFound=outsize+j+1;\
outsize+=(count)+1; blocksize=0; }

#define gif_write_code(c) { buf+=(c)<<shift; shift+=bits;\
while (shift>=8) { output[++blocksize]=buf&255; buf>>=8;shift-=8;\
if (blocksize==bsize) gif_write_block(bsize); }}

int decode_gif(FILE* in, int size, FILE *out, FMode mode, U64 &diffFound) {
  int diffcount=fgetc(in), curdiff=0, diffpos[4096];
  diffcount=((diffcount<<8)+fgetc(in)-6)/4;
  int bsize=255-fgetc(in);
  int clearpos=fgetc(in); clearpos=(clearpos<<8)+fgetc(in);
  clearpos=(69631-clearpos)&0xffff;
  int codesize=fgetc(in),bits=codesize+1,shift=0,buf=0,blocksize=0;
  if (diffcount>4096 || clearpos<=(1<<codesize)+2) return 1;
  int maxcode=(1<<codesize)+1,dict[4096],input;
  for (int i=0; i<diffcount; i++) {
    diffpos[i]=fgetc(in);
    diffpos[i]=(diffpos[i]<<8)+fgetc(in);
    diffpos[i]=(diffpos[i]<<8)+fgetc(in);
    diffpos[i]=(diffpos[i]<<8)+fgetc(in);
    if (i>0) diffpos[i]+=diffpos[i-1];
  }
  U8 output[256];
  size-=6+diffcount*4;
  int last=fgetc(in),total=size+1,outsize=1;
  if (mode==FDECOMPRESS) fputc(codesize, out);
  else if (mode==FCOMPARE) if (codesize!=getc(out) && !diffFound) diffFound=1;
  if (diffcount==0 || diffpos[0]!=0) gif_write_code(1<<codesize) else curdiff++;
  while (size-->=0 && (input=fgetc(in))>=0) {
    int code=-1, key=(last<<8)+input;
    for (int i=(1<<codesize)+2; i<=min(maxcode,4095); i++) if (dict[i]==key) code=i;
    if (curdiff<diffcount && total-size>diffpos[curdiff]) curdiff++,code=-1;
    if (code==-1) {
      gif_write_code(last);
      if (maxcode==clearpos) { gif_write_code(1<<codesize); bits=codesize+1, maxcode=(1<<codesize)+1; }
      else
      {
        ++maxcode;
        if (maxcode<=4095) dict[maxcode]=key;
        if (maxcode>=(1<<bits) && bits<12) bits++;
      }
      code=input;
    }
    last=code;
  }
  gif_write_code(last);
  gif_write_code((1<<codesize)+1);
  if (shift>0) {
    output[++blocksize]=buf&255;
    if (blocksize==bsize) gif_write_block(bsize);
  }
  if (blocksize>0) gif_write_block(blocksize);
  if (mode==FDECOMPRESS) fputc(0, out);
  else if (mode==FCOMPARE) if (0!=getc(out) && !diffFound) diffFound=outsize+1;
  return outsize+1;
}

//mrb
void encode_mrb(FILE* in, FILE* out, int len, int width, int height) {
	int totalSize=(width)*height;
		U32 count=0;
		U8 value=0;
		for(int i=0;i<totalSize; ++i){
			if((count&0x7F)==0)	{
				count=getc(in);
				value=getc(in);
			}
		    else if(count&0x80)	{
				value=getc(in);
			}
			count--;
			putc(value,out);
		}
}

int encodeRLE(U8 *dst, U8 *ptr, int src_end){
    int i=0;
    int ind=0;
    for(ind=0;ind<src_end; ){
        if (ptr[ind+0]!=ptr[ind+1] || ptr[ind+1]!=ptr[ind+2]) {
            // Guess how many non repeating bytes we have
            int j=0;
            for( j=ind+1;j<(src_end);j++)
            if (ptr[j+0]==ptr[j+1] && ptr[j+2]==ptr[j+0] || ((j-ind)>=127)) break;
            int pixels=j-ind;
            if (j+1==src_end && pixels<8)pixels++;
            dst[i++]=0x80 |pixels;
            for(int cnt=0;cnt<pixels;cnt++) { 
                dst[i++]=ptr[ind+cnt];               
            }
            ind=ind+pixels;
        }
        else {
            // Get the number of repeating bytes
            int j=0;
            for(  j=ind+1;j<(src_end);j++)
            if (ptr[j+0]!=ptr[j+1]) break;
            int pixels=j-ind+1;          
            if (j==src_end && pixels<4){
                pixels--;              
                dst[i]=U8(0x80 |pixels);
                i++ ;
                for(int cnt=0;cnt<pixels;cnt++) { 
                    dst[i]=ptr[ind+cnt]; 
                    i++;
                }
                ind=ind+pixels;
            }
            else{ 
                j=pixels;  
                while (pixels>127) {
                    dst[i++]=127;                
                    dst[i++]=ptr[ind];                       
                    pixels=pixels-127;
                }
                if (pixels>0) { 
                    if (j==src_end) pixels--;
                    dst[i++]=pixels;         
                    dst[i++]=ptr[ind];
                }
                ind=ind+j;
            }
        }
    }
    return i;
}
int decode_mrb(FILE* in, int size, int width, FILE *out1, FMode mode, uint64_t &diffFound) {
 
	U32 outlen=0;
	//FILE *out;
	//out=tmpfile2();
    U8 *ptr,*fptr,*fptre;
    ptr = (U8*)calloc(size+4, 1);
    if (!ptr) quit("Out of memory (encode_MBR)");
	fptr = (U8*)calloc(size+4, 1);
	if (!fptr) quit("Out of memory (encode_MBR)");
	fread(&fptr[0], 1, size, in);
	int aaa=encodeRLE(ptr,fptr,size);
    programChecker.alloc(size*2);
	//fwrite(&ptr[0], 1, aaa, out);
	//fclose(out);
	//Write out or compare
    if (mode==FDECOMPRESS){
            fwrite(&ptr[0], 1, aaa, out1);
        }
    else if (mode==FCOMPARE){
    for(int i=0;i<aaa;i++){
        U8 b=ptr[i];
            if (b!=fgetc(out1) && !diffFound) diffFound=ftello(out1);
        }
    }
	free(ptr);
	free(fptr);
	programChecker.alloc(-size*2);
	assert(aaa<size);
	return aaa;
}
//////////////////// Compress, Decompress ////////////////////////////

//for block statistics, levels 0-5
U64 typenamess[datatypecount][5]={0}; //total type size for levels 0-5
U32 typenamesc[datatypecount][5]={0}; //total type count for levels 0-5
int itcount=0;               //level count

int getstreamid(Filetype type){
    if (type==DEFAULT || type==HDR || type==NESROM) return 0;
    else if (type==JPEG ) return  1;
    else if (type==IMAGE1) return 2;
    else if (type==IMAGE4) return 3;
    else if (type==IMAGE8 ) return 4;
    else if (type==IMAGE24) return 5;
    else if (type==AUDIO) return 6;
    else if (type==EXE) return 7;
    else if (type==TEXT0) return 8; //text stream with lots of 0-9
    else if (type==TXTUTF8  || type== DICTTXT || type==TEXT) return 9;
    else if (type==BIGTEXT) return 10;
    //else if (type==NESROM) return 11;
    return -1;
}

bool isstreamtype(Filetype type,int streamid){
    assert(streamid<streamc);
    if ((type==DEFAULT || type==HDR || type==NESROM) && streamid==0 ) return true;
    else if (type==JPEG && streamid==1) return  true;
    else if (type==IMAGE1 && streamid==2) return true;
    else if (type==IMAGE4 && streamid==3) return true;
    else if (type==IMAGE8 && streamid==4 ) return true;
    else if (type==IMAGE24 && streamid==5) return true;
    else if (type==AUDIO && streamid==6) return true;
    else if (type==EXE && streamid==7) return true;
    else if (type==TEXT0 && streamid==8) return true; //text stream with lots of 0-9
    else if ((type==TXTUTF8  || type== DICTTXT || type==TEXT) && streamid==9) return true;
    else if ((type==BIGTEXT) && streamid==10) return true;
    //else if ((type==NESROM) && streamid==11) return true;
    return false;
}

void direct_encode_blockstream(Filetype type, FILE *in, U64 len, Encoder &en, U64 s1, U64 s2, int info=-1) {
  segment[segment.pos++]=type&0xff;
  segment.put8(len);
  if (info!=-1) {
    segment.put4(info);
  }
  FILE *out;
  int srid=getstreamid(type);
  if (srid>=0) out=filestreams[srid];
  else quit("Stream ID type wrong.");
  for (U64 j=s1; j<s1+len; ++j)  putc(getc(in),out);
}

void DetectRecursive(FILE *in, U64 n, Encoder &en, char *blstr, int it, U64 s1, U64 s2);

void transform_encode_block(Filetype type, FILE *in, int len, Encoder &en, int info, int info2, char *blstr, int it, U64 s1, U64 s2, U64 begin) {
    if (type==EXE || type==CD || type==MDF || type==IMAGE24 ||type==MRBR || ((type==TEXT || type==TXTUTF8) && len>0xA00000)  || type==BASE64 || type==BASE85 ||type==SZDD|| type==AUDIO||type==ZLIB|| type==GIF) {
        U64 diffFound=0;
        FILE* tmp=tmpfile2();  // temporary encoded file
        if (!tmp) quit("compressRecursive tmpfile");
        if (type==IMAGE24) encode_bmp(in, tmp, int(len), info);
        else if (type==MRBR) encode_mrb(in, tmp, int(len), info,info2);
        else if (type==AUDIO) encode_audio(in, tmp, int(len), info,info2);
        else if (type==EXE) encode_exe(in, tmp, int(len), int(begin));
        else if ((type==TEXT || type==TXTUTF8) && len>0xA00000) encode_txt(in, tmp, int(len),info&1);
        else if (type==BASE64) encode_base64(in, tmp, int(len));
        else if (type==BASE85) encode_ascii85(in, tmp, int(len));
        else if (type==SZDD) encode_szdd(in, tmp, info);
        else if (type==ZLIB) diffFound=encode_zlib(in, tmp, len)?0:1;//encode_zlib(in, tmp, len);
        else if (type==CD) encode_cd(in, tmp, int(len), info);
        else if (type==MDF) encode_mdf(in, tmp, int(len));
        else if (type==GIF) diffFound=encode_gif(in, tmp, len)?0:1;
        const U64 tmpsize=ftello(tmp);
        if ((type==TEXT || type==TXTUTF8 ) && len>0xA00000)  printf("(wt: %d)\n", int(tmpsize)); 
        
        int tfail=0;
        //if (!(type==TEXT || type==TXTUTF8 )){
        
        rewind(tmp);
        en.setFile(tmp);
        fseeko(in, begin, SEEK_SET);
        if (type==IMAGE24) decode_bmp(en, int(tmpsize), info, in, FCOMPARE, diffFound);
        else if (type==MRBR) decode_mrb(tmp, int(tmpsize), info, in, FCOMPARE, diffFound);
        else if (type==AUDIO) decode_audio(en, int(tmpsize), in,info,int(len),info2,  FCOMPARE, diffFound);
        else if (type==EXE) decode_exe(en, int(tmpsize), in, FCOMPARE, diffFound);
        else if ((type==TEXT || type==TXTUTF8 ) && len>0xA00000) decode_txt(en, int(tmpsize), in, FCOMPARE, diffFound);
        else if (type==BASE64 ) decode_base64(tmp, int(tmpsize), in, FCOMPARE, diffFound);
        else if (type==BASE85 ) decode_ascii85(tmp, int(tmpsize), in, FCOMPARE, diffFound);
        else if (type==SZDD ) decode_szdd(tmp, int(tmpsize),info, in, FCOMPARE, diffFound);
        else if (type==CD) decode_cd(tmp, int(tmpsize), in, FCOMPARE, diffFound);
        else if (type==MDF ) decode_mdf(tmp, int(tmpsize), in, FCOMPARE, diffFound);
        else if (type==ZLIB && !diffFound) decode_zlib(tmp, int(tmpsize), in, FCOMPARE, diffFound);
        else if (type==GIF && !diffFound) decode_gif(tmp, tmpsize, in, FCOMPARE, diffFound);
        tfail=(diffFound || fgetc(tmp)!=EOF);
        //}
        // Test fails, compress without transform
        if (tfail) {
            printf("Transform fails at %0.0f, skipping...\n", diffFound-1+0.0);
            fseeko(in, begin, SEEK_SET);
            direct_encode_blockstream(DEFAULT, in, len, en, s1, s2);
            typenamess[type][it]-=len,  typenamesc[type][it]--;       // if type fails set
            typenamess[DEFAULT][it]+=len,  typenamesc[DEFAULT][it]++; // default info
        } else {
            rewind(tmp);
            if (type==EXE) {
                printf("\n");
                direct_encode_blockstream(type, tmp, tmpsize, en, s1, s2);
            } else if (type==IMAGE24) {
                printf("\n");
                direct_encode_blockstream(type, tmp, tmpsize, en, s1, s2, info);
            } else if (type==MRBR) {
                printf("\n");
                segment.put1(type&0xff);
                segment.put8(tmpsize);
                segment.put4(info); 
                if (it==itcount)    itcount=it+1;
                typenamess[IMAGE8][it+1]+=tmpsize,  typenamesc[IMAGE8][it+1]++;
                direct_encode_blockstream(IMAGE8, tmp, tmpsize, en, s1, s2, info);
            }else if (type==GIF) {
                printf(" (width: %d)", info);
				printf("\n");
                segment.put1(type&0xff);
                segment.put8(tmpsize);
                int hdrsize=fgetc(tmp);
                hdrsize=(hdrsize<<8)+fgetc(tmp);
                rewind(tmp);
                typenamess[HDR][it+1]+=hdrsize,  typenamesc[HDR][it+1]++; 
                direct_encode_blockstream(HDR, tmp, hdrsize, en,0, s2);
                typenamess[IMAGE8][it+1]+=tmpsize-hdrsize,  typenamesc[IMAGE8][it+1]++;
                direct_encode_blockstream(IMAGE8, tmp, tmpsize-hdrsize, en, s1, s2,info);
            } else if (type==AUDIO) {
                segment.put1(type&0xff);
                segment.put8(len); //original lenght
                segment.put4(info2); 
                direct_encode_blockstream(type, tmp, tmpsize, en, s1, s2, info);
            } else if ((type==TEXT || type==TXTUTF8) && len>0xA00000 ) {
                // if (tmpsize<(len-256)) //if WRT is smaller then original block
                direct_encode_blockstream(BIGTEXT, tmp, tmpsize, en, s1, s2);
                //  else {// encode as text
                //      fseeko(in, begin, SEEK_SET);
                //      direct_encode_blockstream(type, in, len, en, s1, s2);
            } else if ((type==BASE64 || type==BASE85 || type==SZDD ||  type==CD  ||  type==MDF ||  type==ZLIB) ) {
                printf("\n");
                segment.put1(type&0xff);
                segment.put8(tmpsize);
                if (type==SZDD ||  type==ZLIB) segment.put4(info);
                if (type==ZLIB && info>0) {// PDF image
                    Filetype type2=(info>>24)==24?IMAGE24:(info>>24)==8?IMAGE8:IMAGE4;
                    int hdrsize=7+5*fgetc(tmp);
                    rewind(tmp);
                    typenamess[HDR][it+1]+=hdrsize,  typenamesc[HDR][it+1]++; 
                    direct_encode_blockstream(HDR, tmp, hdrsize, en,0, s2);
                    typenamess[type2][it+1]+=tmpsize-hdrsize,  typenamesc[type2][it+1]++;
                    //transform_encode_block(type2, tmp, tmpsize-hdrsize, en, info&0xffffff,info2, blstr, it, s1, s2, hdrsize);
                    direct_encode_blockstream(type2, tmp, tmpsize-hdrsize, en, s1, s2,info&0xffffff);
                } else {                        
                    DetectRecursive(tmp, tmpsize, en, blstr, it+1, 0, tmpsize);
                }    
            }
        }
        fclose(tmp);  // deletes
    } else {
        const int i1=(type==IMAGE1 || type==IMAGE8 || type==IMAGE4 )?info:-1;
        printf("\n");
        direct_encode_blockstream(type, in, len, en, s1, s2, i1);
    }
}

void DetectRecursive(FILE *in, U64 n, Encoder &en, char *blstr, int it=0, U64 s1=0, U64 s2=0) {
  static const char* audiotypes[6]={"8b mono","8b stereo","16b mono","16b stereo","32b mono","32b stereo"};
  Filetype type=DEFAULT;
  int blnum=0, info,info2;  // image width or audio type
  U64 begin=ftello(in), end0=begin+n;
  char b2[32];
  strcpy(b2, blstr);
  if (b2[0]) strcat(b2, "-");
  if (it==5) {
    direct_encode_blockstream(DEFAULT, in, n, en, s1, s2);
    return;
  }
  s2+=n;

  // Transform and test in blocks
  while (n>0) {
    Filetype nextType=detect(in, n, type, info,info2,it,s1);
    U64 end=ftello(in);
    fseeko(in, begin, SEEK_SET);
    if (end>end0) {  // if some detection reports longer then actual size file is
      end=begin+1;
      type=DEFAULT;
    }
    U64 len=U64(end-begin);
    
    if (len>0) {
    if (it>itcount)    itcount=it;
    typenamess[type][it]+=len,  typenamesc[type][it]++; 
      s2-=len;
      sprintf(blstr,"%s%d",b2,blnum++);
      
      printf(" %-11s | %-9s |%10.0f b [%0.0f - %0.0f]",blstr,typenames[type],len+0.0,begin+0.0,end-1+0.0);
      if (type==AUDIO) printf(" (%s)\n", audiotypes[(info&31)%4+(info>>7)*2]);
      else if (type==IMAGE1 || type==IMAGE4 || type==IMAGE8 || type==IMAGE24 || type==MRBR) printf(" (width: %d)", info);
      else if (type==CD) printf(" (m%d/f%d)", info==1?1:2, info!=3?1:2);
      else if (type==ZLIB && info>0) printf(" (%db-img w: %d)",info>>24,info&0xffffff);
      transform_encode_block(type, in, len, en, info,info2, blstr, it, s1, s2, begin);
      
      s1+=len;
    }
    n-=len;
    type=nextType;
    begin=end;
  }
}

// Compress a file. Split filesize bytes into blocks by type.
// For each block, output
// <type> <size> and call encode_X to convert to type X.
// Test transform and compress.
void DetectStreams(const char* filename, U64 filesize) {
FILE *tmp=tmpfile2();  // temporary encoded file
Predictors *t;
t=0;
  Encoder en(COMPRESS, tmp,*t);
  assert(en.getMode()==COMPRESS);
  assert(filename && filename[0]);
  FILE *in=fopen(filename, "rb");
  if (!in) perror(filename), quit();
  //U64 start=en.size();
  printf("Block segmentation:\n");
  char blstr[32]="";
  DetectRecursive(in, filesize, en, blstr);
  if (in) fclose(in);
  if (tmp) fclose(tmp);
  //printf("Compressed from %0.0f to %0.0f bytes.\n",filesize+0.0,en.size()-start+0.0);
}

// Try to make a directory, return true if successful
bool makedir(const char* dir) {
#ifdef WINDOWS
  return CreateDirectory(dir, 0)==TRUE;
#else
#ifdef UNIX
  return mkdir(dir, 0777)==0;
#else
  return false;
#endif
#endif
}

U64 decompressStreamRecursive(FILE *out, U64 size, Encoder& en, FMode mode, int it=0, U64 s1=0, U64 s2=0) {
    Filetype type;
    U64 len=0L, i=0L;
    U64 diffFound=0L;
    int info=-1;
    int64_t info2=-1;
    FILE *tmp;
    s2+=size;
    while (i<size) {
        type=(Filetype)segment(segment.pos++);
        info=-1,info2=-1;
        int smr=0;
        for (int k=0; k<8; k++) len=len<<8,len+=segment(segment.pos++);
        if (type==IMAGE1 || type==IMAGE8 || type==IMAGE4 || type==IMAGE24||type==MRBR|| type==AUDIO || type==SZDD|| type==ZLIB) {
            if (type==AUDIO ) {
                info2=len; 
                for (int k=smr=0; k<4; ++k) smr=(smr<<8)+segment(segment.pos++); //sample rate
                segment.pos++; //skip type
                for (int k=len=0; k<8; ++k) len=(len<<8)+segment(segment.pos++);
                for (int k=info=0; k<4; ++k) info=(info<<8)+segment(segment.pos++);
                
            }else
            for (int k=info=0; k<4; ++k) info=(info<<8)+segment(segment.pos++);
        }
        int srid=getstreamid(type);
        if (srid>=0) en.setFile(filestreams[srid]);
        #ifndef NDEBUG 
         printf(" %d  %-9s |%10.0f b [%0.0f]  \n",it, typenames[type],len+0.0,i+0.0 );
        #endif
        if (type==IMAGE24)      len=decode_bmp(en, int(len), info, out, mode, diffFound);
        
        else if (type==AUDIO)   len=decode_audio(en, int(len), out,info,info2,smr,mode, diffFound);
        else if (type==EXE)     len=decode_exe(en, int(len), out, mode, diffFound, int(s1), int(s2));
        else if (type==BIGTEXT) len=decode_txt(en, int(len), out, mode, diffFound);
        else if (type==BASE85 ||type==BASE64 || type==SZDD || type==ZLIB || type==CD || type==MDF  || type==GIF || type==MRBR) {
            tmp=tmpfile2();
            decompressStreamRecursive(tmp, len, en, FDECOMPRESS, it+1, s1+i, s2-len);
            if (mode!=FDISCARD) {
                rewind(tmp);
                if (type==BASE64) len=decode_base64(tmp, int(len), out, mode, diffFound);
                else if (type==BASE85) len=decode_ascii85(tmp, int(len), out, mode, diffFound);
                else if (type==SZDD)   len=decode_szdd(tmp,info,info ,out, mode, diffFound);
                else if (type==ZLIB)   len=decode_zlib(tmp,int(len),out, mode, diffFound);
                else if (type==CD)     len=decode_cd(tmp, int(len), out, mode, diffFound);
                else if (type==MDF)    len=decode_mdf(tmp, int(len), out, mode, diffFound);
                else if (type==GIF)    len=decode_gif(tmp, len, out, mode, diffFound);
                else if (type==MRBR)   len=decode_mrb(tmp, int(len), info, out, mode, diffFound);
            }
            fclose(tmp);
        }
        else {
            for (U64 j=i+s1; j<i+s1+len; ++j) {
                if (!(j&0xfff)) printStatus(j, s2);
                if (mode==FDECOMPRESS) putc(en.decompress(), out);
                else if (mode==FCOMPARE) {
                    int a=fgetc(out);
                    int b=en.decompress();
                    if (a!=b && !diffFound) {
                        mode=FDISCARD;
                        diffFound=j+1;
                    }
                } else en.decompress();
            }
        }
        i+=len;
    }
    return diffFound;
}

// Decompress a file from datastream
void DecodeStreams(const char* filename, U64 filesize) {
  FMode mode=FDECOMPRESS;
  assert(filename && filename[0]);
  FILE *tmp=tmpfile2();  // temporary encoded file
  Predictors *t; //dummy
  t=0;
  Encoder en(COMPRESS, tmp,*t);
  // Test if output file exists.  If so, then compare.
  FILE* f=fopen(filename, "rb");
  if (f) mode=FCOMPARE,printf("Comparing");
  else {
    // Create file
    f=fopen(filename, "wb");
    if (!f) {  // Try creating directories in path and try again
      String path(filename);
      for (int i=0; path[i]; ++i) {
        if (path[i]=='/' || path[i]=='\\') {
          char savechar=path[i];
          path[i]=0;
          if (makedir(path.c_str()))
            printf("Created directory %s\n", path.c_str());
          path[i]=savechar;
        }
      }
      f=fopen(filename, "wb");
    }
    if (!f) mode=FDISCARD,printf("Skipping"); else printf("Extracting");
  }
  printf(" %s %0.0f -> \n", filename, filesize+0.0);

  // Decompress/Compare
  U64 r=decompressStreamRecursive(f, filesize, en, mode);
  if (mode==FCOMPARE && !r && getc(f)!=EOF) printf("file is longer\n");
  else if (mode==FCOMPARE && r) printf("differ at %0.0f\n",r-1+0.0);
  else if (mode==FCOMPARE) printf("identical\n");
  else printf("done   \n");
  if (f) fclose(f);
  if (tmp) fclose(tmp);
}

//////////////////////////// User Interface ////////////////////////////


// int expand(String& archive, String& s, const char* fname, int base) {
// Given file name fname, print its length and base name (beginning
// at fname+base) to archive in format "%ld\t%s\r\n" and append the
// full name (including path) to String s in format "%s\n".  If fname
// is a directory then substitute all of its regular files and recursively
// expand any subdirectories.  Base initially points to the first
// character after the last / in fname, but in subdirectories includes
// the path from the topmost directory.  Return the number of files
// whose names are appended to s and archive.

// Same as expand() except fname is an ordinary file
int putsize(String& archive, String& s, const char* fname, int base) {
  int result=0;
  FILE *f=fopen(fname, "rb");
  if (f) {
    fseeko(f, 0, SEEK_END);
    U64 len=ftello(f);
    if (len>=0) {
      static char blk[24];
      sprintf(blk, "%0.0f\t", len+0.0);
      archive+=blk;
      archive+=(fname+base);
      archive+="\n";
      s+=fname;
      s+="\n";
      ++result;
    }
    fclose(f);
  }
  return result;
}

#ifdef WINDOWS

int expand(String& archive, String& s, const char* fname, int base) {
  int result=0;
  DWORD attr=GetFileAttributes(fname);
  if ((attr != 0xFFFFFFFF) && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
    WIN32_FIND_DATA ffd;
    String fdir(fname);
    fdir+="/*";
    HANDLE h=FindFirstFile(fdir.c_str(), &ffd);
    while (h!=INVALID_HANDLE_VALUE) {
      if (!equals(ffd.cFileName, ".") && !equals(ffd.cFileName, "..")) {
        String d(fname);
        d+="/";
        d+=ffd.cFileName;
        result+=expand(archive, s, d.c_str(), base);
      }
      if (FindNextFile(h, &ffd)!=TRUE) break;
    }
    FindClose(h);
  }
  else // ordinary file
    result=putsize(archive, s, fname, base);
  return result;
}

#else
#ifdef UNIX

int expand(String& archive, String& s, const char* fname, int base) {
  int result=0;
  struct stat sb;
  if (stat(fname, &sb)<0) return 0;

  // If a regular file and readable, get file size
  if (sb.st_mode & S_IFREG && sb.st_mode & 0400)
    result+=putsize(archive, s, fname, base);

  // If a directory with read and execute permission, traverse it
  else if (sb.st_mode & S_IFDIR && sb.st_mode & 0400 && sb.st_mode & 0100) {
    DIR *dirp=opendir(fname);
    if (!dirp) {
      perror("opendir");
      return result;
    }
    dirent *dp;
    while(errno=0, (dp=readdir(dirp))!=0) {
      if (!equals(dp->d_name, ".") && !equals(dp->d_name, "..")) {
        String d(fname);
        d+="/";
        d+=dp->d_name;
        result+=expand(archive, s, d.c_str(), base);
      }
    }
    if (errno) perror("readdir");
    closedir(dirp);
  }
  else printf("%s is not a readable file or directory\n", fname);
  return result;
}

#else  // Not WINDOWS or UNIX, ignore directories

int expand(String& archive, String& s, const char* fname, int base) {
  return putsize(archive, s, fname, base);
}

#endif
#endif


U64 filestreamsize[streamc];

void compressStream(int streamid,U64 size, FILE* in, FILE* out) {
    Encoder* threadencode;
    Predictors* threadpredict;
    U64 datasegmentsize;
    U64 datasegmentlen;
    int datasegmentpos;
    int datasegmentinfo;
    Filetype datasegmenttype;
    int i; //stream
                i=streamid;
                datasegmentsize=size;
                    U64 total=size;
                    datasegmentpos=0;
                    datasegmentinfo=0;
                    datasegmentlen=0;
                    // datastreams
                    // DEFAULT HDR 0
                    // JPEG        1
                    // IMAGE1      2
                    // IMAGE4      3
                    // IMAGE8      4
                    // IMAGE24     5
                    // AUDIO       6
                    // EXE         7
                    // TEXT0       8
                    // TXTUTF8 DICTTXT TEXT 9
                    // bigtext 10
                    switch(i) {
                        case 0: {
      	                    printf("Compressing default stream(0).  Total %0.0f  \n",datasegmentsize +0.0); 
                            if (modeQuick) threadpredict=new PredictorFast();
                            else threadpredict=new Predictor();
                            break;}
                        case 1: {
      	                    printf("Compressing jpeg    stream(1).  Total %0.0f  \n",datasegmentsize +0.0); 
                            threadpredict=new PredictorJPEG();
                            break;}        
                        case 2: {
      	                    printf("Compressing image1  stream(2).  Total %0.0f  \n",datasegmentsize +0.0); 
                            threadpredict=new PredictorIMG1();
                            break;}
                        case 3: {
      	                    printf("Compressing image4  stream(3).  Total %0.0f  \n",datasegmentsize +0.0); 
                            threadpredict=new PredictorIMG4();
                            break;}    
                        case 4: {
      	                    printf("Compressing image8  stream(4).  Total %0.0f  \n",datasegmentsize +0.0); 
                            threadpredict=new PredictorIMG8();
                            break;}
                        case 5: {
      	                    printf("Compressing image24 stream(5).  Total %0.0f  \n",datasegmentsize +0.0); 
                            threadpredict=new PredictorIMG24();
                            break;}        
                        case 6: {
      	                    printf("Compressing audio   stream(6).  Total %0.0f  \n",datasegmentsize +0.0); 
                            threadpredict=new PredictorAUDIO();
                            break;}
                        case 7: {
      	                    printf("Compressing exe     stream(7).  Total %0.0f  \n",datasegmentsize +0.0); 
      	                    if (modeQuick) threadpredict=new PredictorFast();
                            else threadpredict=new PredictorEXE();
                            break;}
                        case 8: {
      	                    printf("Compressing text0 wrt stream(8). Total %0.0f  \n",datasegmentsize +0.0); 
      	                    if (modeQuick) threadpredict=new PredictorFast();
                            else threadpredict=new PredictorTXTWRT();
                            break;}
                        case 9: 
                        case 10:{
      	                    printf("Compressing %stext wrt stream(%d). Total %0.0f  \n",i==10?"big":"",i,datasegmentsize +0.0); 
                            if (modeQuick) threadpredict=new PredictorFast();
                            else threadpredict=new PredictorTXTWRT();
                            break;}   
                    }
                    threadencode=new Encoder (COMPRESS, out ,*threadpredict); 
                     if ((i>=0 && i<=7) || i==10 ){
                        while (datasegmentsize>0) {
                            while (datasegmentlen==0){
                                datasegmenttype=(Filetype)segment(datasegmentpos++);
                                for (int ii=0; ii<8; ii++) datasegmentlen=datasegmentlen<<8,datasegmentlen+=segment(datasegmentpos++);
                                if (datasegmenttype==IMAGE1 || datasegmenttype==IMAGE8 || datasegmenttype==IMAGE4 || datasegmenttype==IMAGE24 ||
                                        datasegmenttype==AUDIO || datasegmenttype==SZDD|| datasegmenttype==MRBR|| datasegmenttype==ZLIB) {
                                            datasegmentinfo=-1; 
                                    if (datasegmenttype==AUDIO ) {
                                       for (int ii=0; ii<4; ++ii)    (datasegmentpos++);
                                        datasegmentpos++; // skip type
                                        for (int ii=0; ii<8; ii++) datasegmentlen=datasegmentlen<<8,datasegmentlen+=segment(datasegmentpos++);
                                    }
                                    for (int ii=0; ii<4; ++ii)  datasegmentinfo=(datasegmentinfo<<8)+segment(datasegmentpos++);
                                    
                                }
                                if (!(isstreamtype(datasegmenttype,i) ))datasegmentlen=0;
                                threadencode->predictor.x.filetype=datasegmenttype;
                                threadencode->predictor.x.blpos=0;
                                threadencode->predictor.x.finfo=datasegmentinfo;
                            }
                            for (U64 k=0; k<datasegmentlen; ++k) {
                                //#ifndef MT
                                if (!(datasegmentsize&0xfff)) printStatus(total-datasegmentsize, total,i);
                                //#endif
                                threadencode->compress(getc(in));
                                datasegmentsize--;
                            }
                            datasegmentlen=0;
                        }
                        threadencode->flush();
                    }
                    if (i==8 || i==9){
                        while (datasegmentsize>0) {
                            FILE *tm=tmpfile2();
                            encode_txt(in,tm,datasegmentsize,i==8);
                            datasegmentlen=ftello(tm);
                            filestreamsize[i]=datasegmentlen;
                            printf(" Total %0.0f wrt: %0.0f\n",datasegmentsize+0.0,datasegmentlen+0.0); 
                            fseeko(tm, 0, SEEK_SET);
                            threadencode->predictor.x.filetype=DICTTXT;
                            threadencode->predictor.x.blpos=0;
                            threadencode->predictor.x.finfo=-1;
                            
                            for (U64 k=0; k<datasegmentlen; ++k) {
                                //#ifndef MT
                                if (!(datasegmentsize&0xfff)) printStatus(total-datasegmentsize, total,i);
                                //#endif
                                threadencode->compress(getc(tm));
                                datasegmentsize--;
                            }
                            datasegmentlen=datasegmentsize=0;
                            fclose(tm);
                        }
                        threadencode->flush();
                    }
            delete threadpredict;
            delete threadencode;
}

#ifdef MT
//multithreading code from pzpaq.cpp v0.05
#ifdef PTHREAD
pthread_cond_t cv=PTHREAD_COND_INITIALIZER;  // to signal FINISHED
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER; // protects cv
typedef pthread_t pthread_tx;
#else
HANDLE mutex;  // protects Job::state
typedef HANDLE pthread_tx;
#endif


FILE * filesmt[streamc];
typedef enum {READY, RUNNING, FINISHED_ERR, FINISHED, ERR, OK} State;
// Instructions to thread to compress or decompress one block.
struct Job {
  State state;        // job state, protected by mutex
  int id;             
  int streamid;
  U64 datasegmentsize;
  int command;
  FILE *in;
  FILE *out;
  pthread_tx tid;      // thread ID (for scheduler)
  Job();
  void print(int i) const;
};

// Initialize
Job::Job(): state(READY),id(0),streamid(-1),datasegmentsize(0),command(-1) {
  // tid is not initialized until state==RUNNING
}

// Print contents
void Job::print(int i=0) const {
  fprintf(stderr,
      "Job %d: state=%d stream=%d\n", i, state,streamid);
}
bool append(FILE* out, FILE* in) {
  if (!in) {
    quit("append in error\n");
    return false;
  }
  if (!out) {
    quit("append out error\n");
    return false;
  }
  const int BUFSIZE=4096;
  char buf[BUFSIZE];
  int n;
  while ((n=fread(buf, 1, BUFSIZE, in))>0)
    fwrite(buf, 1, n, out);
  return true;
}

void decompress(const Job& job) {
}        

#define check(f) { \
  int rc=f; \
  if (rc) fprintf(stderr, "Line %d: %s: error %d\n", __LINE__, #f, rc); \
}
// Worker thread
#ifdef PTHREAD
void*
#else
DWORD
#endif
thread(void *arg) {

  // Do the work and receive status in msg
  Job* job=(Job*)arg;
  const char* result=0;  // error message unless OK
  try {
    if (job->command==0) 
      compressStream(job->streamid,job->datasegmentsize,job->in,job->out);
    else if (job->command==1)
      decompress(*job); 
  }
  catch (const char* msg) {
    result=msg;
  }
// Call f and check that the return code is 0

  // Let controlling thread know we're done and the result
#ifdef PTHREAD
  check(pthread_mutex_lock(&mutex));
  job->state=result?FINISHED_ERR:FINISHED;
  check(pthread_cond_signal(&cv));
  check(pthread_mutex_unlock(&mutex));
#else
  WaitForSingleObject(mutex, INFINITE);
  job->state=result?FINISHED_ERR:FINISHED;
  ReleaseMutex(mutex);
#endif
  return 0;
}
#endif

// To compress to file1.paq8pxd18: paq8pxd_v18 [-n] file1 [file2...]
// To decompress: paq8pxd_v18 file1.paq8pxd18 [output_dir]
int main(int argc, char** argv) {
    bool pause=argc<=2;  // Pause when done?
    try {

        // Get option
        bool doExtract=false;  // -d option
        bool doList=false;  // -l option
        char* aopt;
        aopt=&argv[1][0];
        
#ifdef MT 
        int topt=1;
        if (argc>1 && aopt[0]=='-' && aopt[1]  && strlen(aopt)<=6) {
#else
        if (argc>1 && aopt[0]=='-' && aopt[1]  && strlen(aopt)<=4) {    
#endif
            if (aopt[1]=='d' && !aopt[2])
                doExtract=true;
            else if (aopt[1]=='l' && !aopt[2])
                doList=true;
            else if (aopt[2]>='0' && aopt[2]<='9' && strlen(aopt)==3 && 
            (aopt[1]=='f' || aopt[1]=='s'|| (aopt[1]=='q') )){
                if (aopt[1]=='f') modeFast=true;
                else if (aopt[1]=='q') modeQuick=true;
                level=aopt[2]-'0'; 
                
            }
            else if (aopt[2]=='1' && aopt[3]>='0' && aopt[3]<='5' && strlen(aopt)==4 && 
            (aopt[1]=='f' || aopt[1]=='s'|| (aopt[1]=='q') )){
                if (aopt[1]=='f') modeFast=true;
                else if (aopt[1]=='q') modeQuick=true;
                aopt[1]='-', aopt[0]=' ';
                level=((~atol(aopt))+1); 
                
                }
#ifdef MT 
            else if (aopt[2]>='0' && aopt[2]<='9'&& (aopt[4]<='9' && aopt[4]>'0') && strlen(aopt)==5 && 
            (aopt[1]=='f' || aopt[1]=='s'|| (aopt[1]=='q') )){
               if (aopt[1]=='f') modeFast=true;
               else if (aopt[1]=='q') modeQuick=true;
                topt=aopt[4]-'0';
                level=aopt[2]-'0';}
            else if (aopt[2]=='1' && aopt[3]>='0' && aopt[3]<='5' && 
            (aopt[5]<='9' && aopt[5]>'0')&& strlen(aopt)==6 && 
            (aopt[1]=='f' || aopt[1]=='s'|| (aopt[1]=='q') )){
                 if (aopt[1]=='f') modeFast=true;
                 else if (aopt[1]=='q') modeQuick=true;
                topt=aopt[5]-'0';
                aopt[4]=0;
                aopt[1]='-';
                 aopt[0]=' ';
                level=((~atol(aopt))+1); 
            }
#endif
            else
                quit("Valid options are -M0 through -M15, -d, -l where M is mode\n");
            --argc;
            ++argv;
            pause=false;
        }

        // Print help message quick 
        if (argc<2) {
            printf(PROGNAME " archiver (C) 2016, Matt Mahoney et al.\n"
            "Free under GPL, http://www.gnu.org/licenses/gpl.txt\n\n"
#ifdef WINDOWS
            "To compress or extract, drop a file or folder on the "
            PROGNAME " icon.\n"
            "The output will be put in the same folder as the input.\n"
            "\n"
            "Or from a command window: "
#endif
            "To compress:\n"
            "  " PROGNAME " -Mlevel file               (compresses to file." PROGNAME ")\n"
            "  " PROGNAME " -Mlevel archive files...   (creates archive." PROGNAME ")\n"
            "  " PROGNAME " file                       (level -%d slow mode, pause when done)\n"
            "M: select mode q, f or s                  (q - quick, f - fast, s - slow)\n"
            "level: -[f|s]0 = store,\n"
            "  -[f|s]1...-[f|s]3 = faster (uses 35, 48, 59 MB)\n"
            "  -s4...-s8  = smaller       (uses 133, 233, 435, 837, 1643 MB)\n"
            "  -s9...-s15 = experimental  (uses 3.1, 6.0, 7.8, 9.3, 12.0, 17.8, 25.3 GB)\n"
            "  -f4...-f8  = faster        (uses 81, 126, 216, 396, 756 MB)\n"
            "  -f9...-f15 = exp. faster   (uses 1476, ...... MB)\n"
#ifdef MT 
            "  to use multithreading -Mlevel:threads (1-9, compression only)\n"
            "  " PROGNAME " -s4:2 file (use slow mode level 4 threads 2)\n\n"
#endif            
#if defined(WINDOWS) || defined (UNIX)
            "You may also compress directories.\n"
#endif
            "\n"
            "To extract or compare:\n"
            "  " PROGNAME " -d dir1/archive." PROGNAME "      (extract to dir1)\n"
            "  " PROGNAME " -d dir1/archive." PROGNAME " dir2 (extract to dir2)\n"
            "  " PROGNAME " archive." PROGNAME "              (extract, pause when done)\n"
            "\n"
            "To view contents: " PROGNAME " -l archive." PROGNAME "\n"
            "\n",
            DEFAULT_OPTION);
            quit();
        }
        
        FILE* archive=0;  // compressed file
        int files=0;  // number of files to compress/decompress
        Array<const char*> fname(1);  // file names (resized to files)
        Array<U64> fsize(1);   // file lengths (resized to files)

        // Compress or decompress?  Get archive name
        Mode mode=COMPRESS;
        String archiveName(argv[1]);
        {
            const int prognamesize=strlen(PROGNAME);
            const int arg1size=strlen(argv[1]);
            if (arg1size>prognamesize+1 && argv[1][arg1size-prognamesize-1]=='.'
                    && equals(PROGNAME, argv[1]+arg1size-prognamesize)) {
                mode=DECOMPRESS;
            }
            else if (doExtract || doList)
            mode=DECOMPRESS;
            else {
                archiveName+=".";
                archiveName+=PROGNAME;
            }
        }

        // Compress: write archive header, get file names and sizes
        String header_string;
        String filenames;
        
        if (mode==COMPRESS) {
            segment.setsize(48); //inital segment buffer size (about 277 blocks)
            // Expand filenames to read later.  Write their base names and sizes
            // to archive.
            int i;
            for (i=1; i<argc; ++i) {
                String name(argv[i]);
                int len=name.size()-1;
                for (int j=0; j<=len; ++j)  // change \ to /
                if (name[j]=='\\') name[j]='/';
                while (len>0 && name[len-1]=='/')  // remove trailing /
                name[--len]=0;
                int base=len-1;
                while (base>=0 && name[base]!='/') --base;  // find last /
                ++base;
                if (base==0 && len>=2 && name[1]==':') base=2;  // chop "C:"
                int expanded=expand(header_string, filenames, name.c_str(), base);
                if (!expanded && (i>1||argc==2))
                printf("%s: not found, skipping...\n", name.c_str());
                files+=expanded;
            }

            // If there is at least one file to compress
            // then create the archive header.
            if (files<1) quit("Nothing to compress\n");
            archive=fopen(archiveName.c_str(), "wb+");
            if (!archive) perror(archiveName.c_str()), quit();
            fprintf(archive, PROGNAME "%c", 0);
            putc(level+(modeFast==true?16:0)+(modeQuick==true?32:0),archive);
            segment.hpos=ftello(archive);
            
            for (int i=0; i<12; i++) putc(0,archive);
            
            printf("Creating archive %s with %d file(s)...\n",
            archiveName.c_str(), files);
        }

        // Decompress: open archive for reading and store file names and sizes
        if (mode==DECOMPRESS) {
            archive=fopen(archiveName.c_str(), "rb+");
            if (!archive) perror(archiveName.c_str()), quit();

            // Check for proper format and get option
            String header;
            int len=strlen(PROGNAME)+1, c, i=0;
            header.resize(len+1);
            while (i<len && (c=getc(archive))!=EOF) {
                header[i]=c;
                i++;
            }
            header[i]=0;
            if (strncmp(header.c_str(), PROGNAME "\0", strlen(PROGNAME)+1))
            printf("%s: not a %s file\n", archiveName.c_str(), PROGNAME), quit();
            level=getc(archive);
            if ((level>>4)&1) modeFast=true;
            if ((level>>5)&1) modeQuick=true;
            level=level&0xf;
            if (level<0||level>15) level=DEFAULT_OPTION;
            
            // Read segment data from archive end
            U64 currentpos,datapos=0L;
            for (int i=0; i<8; i++) datapos=datapos<<8,datapos+=getc(archive);
            segment.hpos=datapos;
            segment.pos=getc(archive)<<24; //read segment data size
            segment.pos+=getc(archive)<<16;
            segment.pos+=getc(archive)<<8;
            segment.pos+=getc(archive);
            if (segment.hpos==0 || segment.pos==0) quit("Segment data not found.");
            segment.setsize(segment.pos);
            currentpos=ftello(archive);
            fseeko(archive, segment.hpos, SEEK_SET); 
            if (fread( &segment[0], 1, segment.pos, archive)<segment.pos) quit("Segment data corrupted.");
            if (fread( &filestreamsize[0], 1,8*streamc, archive)<8*streamc) quit("Segment data corrupted (stream size).");
            
            fseeko(archive, currentpos, SEEK_SET); 
            segment.pos=0; //reset to offset 0
        }
        Encoder* en;
        Predictors* predictord;
        if (modeQuick) predictord=new PredictorFast();
        else predictord=new Predictor();
        en=new Encoder(mode, archive,*predictord);
        
        // Compress header
        if (mode==COMPRESS) {
            int len=header_string.size();
            printf("\nFile list (%d bytes)\n", len);
            assert(en->getMode()==COMPRESS);
            U64 start=en->size();
            en->compress(0); // block type 0
            en->compress(len>>24); en->compress(len>>16); en->compress(len>>8); en->compress(len); // block length
            for (int i=0; i<len; i++) en->compress(header_string[i]);
            printf("Compressed from %d to %0.0f bytes.\n",len,en->size()-start+0.0);
        }

        // Deompress header
        if (mode==DECOMPRESS) {
            if (en->decompress()!=0) printf("%s: header corrupted\n", archiveName.c_str()), quit();
            int len=0;
            len+=en->decompress()<<24;
            len+=en->decompress()<<16;
            len+=en->decompress()<<8;
            len+=en->decompress();
            header_string.resize(len);
            for (int i=0; i<len; i++) {
                header_string[i]=en->decompress();
                if (header_string[i]=='\n') files++;
            }
            if (doList) printf("File list of %s archive:\n%s", archiveName.c_str(), header_string.c_str());
        }
        
        // Fill fname[files], fsize[files] with input filenames and sizes
        fname.resize(files);
        fsize.resize(files);
        char *p=&header_string[0];
        char* q=&filenames[0];
        for (int i=0; i<files; ++i) {
            assert(p);
            fsize[i]=atoll(p);
            assert(fsize[i]>=0);
            while (*p!='\t') ++p; *(p++)='\0';
            fname[i]=mode==COMPRESS?q:p;
            while (*p!='\n') ++p; *(p++)='\0';
            if (mode==COMPRESS) { while (*q!='\n') ++q; *(q++)='\0'; }
        }
        // Compress or decompress files
        assert(fname.size()==files);
        assert(fsize.size()==files);
        U64 total_size=0;  // sum of file sizes
        for (int i=0; i<files; ++i) total_size+=fsize[i];
        if (mode==COMPRESS) {
            en->flush();
            delete en;
            delete predictord;
            for (int i=0; i<streamc; ++i) {
                filestreams[i]=tmpfile2();
            }
            for (int i=0; i<files; ++i) {
                printf("\n%d/%d  Filename: %s (%0.0f bytes)\n", i+1, files, fname[i], fsize[i]+0.0);
                DetectStreams(fname[i], fsize[i]); //compress(fname[i], fsize[i], en);
            }
            segment.put1(0xff); //end marker
            printf("\n Segment data size: %d bytes\n",segment.pos);
            //Display Level statistics
            U32 ttc;
            U64 tts;
            for (int j=0; j<=itcount; ++j) {
                printf("\n %-2s |%-9s |%-10s |%-10s\n","TN","Type name", "Count","Total size");
                printf("-----------------------------------------\n");
                ttc=0,tts=0;
                for (int i=0; i<datatypecount; ++i)   if (typenamess[i][j]) printf(" %2d |%-9s |%10d |%11.0f\n",i,typenames[i], typenamesc[i][j],typenamess[i][j]+0.0),ttc+=typenamesc[i][j],tts+=typenamess[i][j];
                printf("-----------------------------------------\n");
                printf("%-13s%1d |%10d |%11.0f\n\n","Total level",j, ttc,tts+0.0);
            }
            
#ifdef MT
            std::vector<Job> jobs;
#endif
            for (int i=0; i<streamc; ++i) {
                U64 datasegmentsize;
                datasegmentsize=ftello(filestreams[i]); //get segment data offset
                filestreamsize[i]=datasegmentsize;
                fseeko(filestreams[i], 0, SEEK_SET);
                if (datasegmentsize>0){ //if segment contains data
#ifdef MT
                    // add streams to job list
                    filesmt[i]=tmpfile2(); //open tmp file for stream output
                    Job job;
                    job.out=filesmt[i];
                    job.in=filestreams[i];
                    job.streamid=i;
                    job.command=0; //0 compress
                    job.datasegmentsize=datasegmentsize;
                    jobs.push_back(job);
#else
                    compressStream(i,datasegmentsize,filestreams[i],archive);
#endif
                }
            }

#ifdef MT
  // Loop until all jobs return OK or ERR: start a job whenever one
  // is eligible. If none is eligible then wait for one to finish and
  // try again. If none are eligible and none are running then it is
  // an error.
  int thread_count=0;  // number RUNNING, not to exceed topt
  int job_count=0;     // number of jobs with state OK or ERR

  // Aquire lock on jobs[i].state.
  // Threads can access only while waiting on a FINISHED signal.
#ifdef PTHREAD
  pthread_attr_t attr; // thread joinable attribute
  check(pthread_attr_init(&attr));
  check(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));
  check(pthread_mutex_lock(&mutex));  // locked
#else
  mutex=CreateMutex(NULL, FALSE, NULL);  // not locked
#endif

  while(job_count<jobs.size()) {

    // If there is more than 1 thread then run the biggest jobs first
    // that satisfies the memory bound. If 1 then take the next ready job
    // that satisfies the bound. If no threads are running, then ignore
    // the memory bound.
    int bi=-1;  // find a job to start
    if (thread_count<topt) {
      for (int i=0; i<jobs.size(); ++i) {
        if (jobs[i].state==READY  && bi<0 ) {
          bi=i;
          if (topt==1) break;
        }
      }
    }

    // If found then run it
    if (bi>=0) {
      jobs[bi].state=RUNNING;
      ++thread_count;
#ifdef PTHREAD
      check(pthread_create(&jobs[bi].tid, &attr, thread, &jobs[bi]));
#else
      jobs[bi].tid=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread,
          &jobs[bi], 0, NULL);
#endif
    }

    // If no jobs can start then wait for one to finish
    else {
#ifdef PTHREAD
      check(pthread_cond_wait(&cv, &mutex));  // wait on cv

      // Join any finished threads. Usually that is the one
      // that signaled it, but there may be others.
      for (int i=0; i<jobs.size(); ++i) {
        if (jobs[i].state==FINISHED || jobs[i].state==FINISHED_ERR) {
          void* status=0;
          check(pthread_join(jobs[i].tid, &status));
          if (jobs[i].state==FINISHED) jobs[i].state=OK;
          if (jobs[i].state==FINISHED_ERR) quit(); //exit program on thread error 
          ++job_count;
          --thread_count;
        }
      }
#else
      // Make a list of running jobs and wait on one to finish
      HANDLE joblist[MAXIMUM_WAIT_OBJECTS];
      int jobptr[MAXIMUM_WAIT_OBJECTS];
      DWORD njobs=0;
      WaitForSingleObject(mutex, INFINITE);
      for (int i=0; i<jobs.size() && njobs<MAXIMUM_WAIT_OBJECTS; ++i) {
        if (jobs[i].state==RUNNING || jobs[i].state==FINISHED
            || jobs[i].state==FINISHED_ERR) {
          jobptr[njobs]=i;
          joblist[njobs++]=jobs[i].tid;
        }
      }
      ReleaseMutex(mutex);
      DWORD id=WaitForMultipleObjects(njobs, joblist, FALSE, INFINITE);
      if (id>=WAIT_OBJECT_0 && id<WAIT_OBJECT_0+njobs) {
        id-=WAIT_OBJECT_0;
        id=jobptr[id];
        if (jobs[id].state==FINISHED) jobs[id].state=OK;
        if (jobs[id].state==FINISHED_ERR) quit(); //exit program on thread error 
        ++job_count;
        --thread_count;
      }
#endif
    }
  }
#ifdef PTHREAD
  check(pthread_mutex_unlock(&mutex));
#endif

  // Append temporary files to archive if OK.
  for (int i=0; i<jobs.size(); ++i) {
      if (jobs[i].state==OK) {
        //printf("streamid:%d \n", jobs[i].streamid);
        fseeko(filesmt[jobs[i].streamid], 0, SEEK_SET);
        append(archive,filesmt[jobs[i].streamid]);
        fclose(filesmt[jobs[i].streamid]);
      }
  }

             #endif
            for (int i=0; i<streamc; ++i) {
                fclose(filestreams[i]);
            }
            
            // Write out segment data
            U64 segmentpos;
            segmentpos=ftello(archive); //get segment data offset
            fseeko(archive, segment.hpos, SEEK_SET);
            putc(segmentpos>>56&0xff,archive); //write segment data offset
            putc(segmentpos>>48&0xff,archive);
            putc(segmentpos>>40&0xff,archive);
            putc(segmentpos>>32&0xff,archive);
            putc(segmentpos>>24&0xff,archive); 
            putc(segmentpos>>16&0xff,archive);
            putc(segmentpos>>8&0xff,archive);
            putc(segmentpos&0xff,archive);

            putc(segment.pos>>24&0xff,archive); //write segment data size
            putc(segment.pos>>16&0xff,archive);
            putc(segment.pos>>8&0xff,archive);
            putc(segment.pos&0xff,archive);
            fseeko(archive, segmentpos, SEEK_SET); 
            fwrite(&segment[0], 1, segment.pos, archive); //write out segment data
            fwrite(&filestreamsize[0], 1, 8*streamc, archive);

            printf("Total %0.0f bytes compressed to %0.0f bytes.\n", total_size+0.0, ftello(archive)+0.0); 
            
        }
        // Decompress files to dir2: paq8pxd_v18 -d dir1/archive.paq8pxd18 dir2
        // If there is no dir2, then extract to dir1
        // If there is no dir1, then extract to .
        else if (!doList) {
            assert(argc>=2);
            String dir(argc>2?argv[2]:argv[1]);
            if (argc==2) {  // chop "/archive.paq8pxd18"
                int i;
                for (i=dir.size()-2; i>=0; --i) {
                    if (dir[i]=='/' || dir[i]=='\\') {
                        dir[i]=0;
                        break;
                    }
                    if (i==1 && dir[i]==':') {  // leave "C:"
                        dir[i+1]=0;
                        break;
                    }
                }
                if (i==-1) dir=".";  // "/" not found
            }
            dir=dir.c_str();
            if (dir[0] && (dir.size()!=3 || dir[1]!=':')) dir+="/";
            /////
            
            delete en;
            delete predictord;
            for (int i=0; i<streamc; ++i) {
                filestreams[i]=tmpfile2();
            }
            
            U64 datasegmentsize;
            U64 datasegmentlen;
            int datasegmentpos;
            int datasegmentinfo;
            Filetype datasegmenttype;
           predictord=0;
           Encoder *defaultencoder;
           defaultencoder=0;
            for (int i=0; i<streamc; ++i) {
                datasegmentsize=(filestreamsize[i]); //get segment data offset
                if (datasegmentsize>0){ //if segment contains data
                    fseeko(filestreams[i], 0, SEEK_SET);
                    U64 total=datasegmentsize;
                    datasegmentpos=0;
                    datasegmentinfo=0;
                    datasegmentlen=0;
                    if (predictord>0) delete predictord,predictord=0;
                    if (defaultencoder>0) delete defaultencoder,defaultencoder=0;
                    switch(i) {
                        case 0: {
      	                    printf("DeCompressing default stream.\n"); 
      	                    if (modeQuick) predictord=new PredictorFast();
                            else predictord=new Predictor();
                            break;}
                        case 1: {
      	                    printf("DeCompressing jpeg stream.\n"); 
                            predictord=new PredictorJPEG();
                            break;}        
                        case 2: {
      	                    printf("DeCompressing image1 stream.\n"); 
                            predictord=new PredictorIMG1();
                            break;}
                        case 3: {
      	                    printf("DeCompressing image4 stream.\n"); 
                            predictord=new PredictorIMG4();
                            break;}    
                        case 4: {
      	                    printf("DeCompressing image8 stream.\n"); 
                            predictord=new PredictorIMG8();
                            break;}
                        case 5: {
      	                    printf("DeCompressing image24 stream.\n"); 
                            predictord=new PredictorIMG24();
                            break;}        
                        case 6: {
      	                    printf("DeCompressing audio stream.\n"); 
                            predictord=new PredictorAUDIO();
                            break;}
                        case 7: {
      	                    printf("DeCompressing exe stream.\n"); 
                            if (modeQuick) predictord=new PredictorFast();
                            else predictord=new PredictorEXE();
                            break;}    
                        case 8: {
      	                    printf("DeCompressing text0 wrt stream.\n"); 
                            if (modeQuick) predictord=new PredictorFast();
                            else predictord=new PredictorTXTWRT();
                            break;}
                        case 9:
                        case 10: {
      	                   // printf("DeCompressing text wrt stream.\n"); 
      	                    printf("DeCompressing %stext wrt stream.\n",i==10?"big":""); 
                            if (modeQuick) predictord=new PredictorFast();
                            else predictord=new PredictorTXTWRT();
                            break;}   
                    }
                     defaultencoder=new Encoder (mode, archive,*predictord); 
                     if ((i>=0 && i<=7)||i==10){
                        while (  datasegmentsize>0) {
                            while (datasegmentlen==0){
                                datasegmenttype=(Filetype)segment(datasegmentpos++);
                                for (int ii=0; ii<8; ii++) datasegmentlen=datasegmentlen<<8,datasegmentlen+=segment(datasegmentpos++);
                                if (datasegmenttype==IMAGE1 || datasegmenttype==IMAGE8 || datasegmenttype==IMAGE4 || datasegmenttype==IMAGE24 ||
                                        datasegmenttype==AUDIO || datasegmenttype==SZDD|| datasegmenttype==MRBR|| datasegmenttype==ZLIB) {
                                    if (datasegmenttype==AUDIO ) {
                                    for (int ii=0; ii<4; ++ii)  (datasegmentpos++);
                                    datasegmentpos++; // skip type
                                    for (int ii=0; ii<8; ii++) datasegmentlen=datasegmentlen<<8,datasegmentlen+=segment(datasegmentpos++);
                                     }
                                    datasegmentinfo=0; for (int ii=0; ii<4; ++ii) datasegmentinfo=(datasegmentinfo<<8)+segment(datasegmentpos++);
                                }
                                if (!(isstreamtype(datasegmenttype,i) ))datasegmentlen=0;
                                defaultencoder->predictor.x.filetype=datasegmenttype;
                                defaultencoder->predictor.x.blpos=0;
                                defaultencoder->predictor.x.finfo=datasegmentinfo;
                            }
                            for (U64 k=0; k<datasegmentlen; ++k) {
                                if (!(datasegmentsize&0xfff)) printStatus(total-datasegmentsize, total,i);
                                putc(defaultencoder->decompress(),filestreams[i]);
                                datasegmentsize--;
                            }
                            datasegmentlen=0;
                        }
                    }
                    if (i==8 || i==9){
                        while (  datasegmentsize>0) {
                            datasegmentlen=datasegmentsize;
                            defaultencoder->predictor.x.filetype=DICTTXT;
                            defaultencoder->predictor.x.blpos=0;
                            defaultencoder->predictor.x.finfo=-1;
                            for (U64 k=0; k<datasegmentlen; ++k) {
                                if (!(datasegmentsize&0xfff)) printStatus(total-datasegmentsize, total,i);
                                putc(defaultencoder->decompress(),filestreams[i]);
                                datasegmentsize--;
                            }
                            FILE *tm=tmpfile2();
                            XWRT_Decoder* wrt;
                            wrt=new XWRT_Decoder();
                            int b=0;
                            wrt->defaultSettings(0);
                            fseeko(filestreams[i], 0, SEEK_SET);
                            int bb=wrt->WRT_start_decoding(filestreams[i]);
                            for ( int ii=0; ii<bb; ii++) {
                                b=wrt->WRT_decode();    
                                fputc(b, tm);
                            }
                            fclose(filestreams[i]);
                            filestreams[i]=tm;
                            fseeko(filestreams[i], 0, SEEK_SET);
                            delete wrt;
                            datasegmentlen=datasegmentsize=0;
                        }
                    }
                }
            } 
            // set datastream file pointers to beginning
            for (int i=0; i<streamc; ++i)         
            fseeko(filestreams[i], 0, SEEK_SET);
            /////
            segment.pos=0;
            for (int i=0; i<files; ++i) {
                String out(dir.c_str());
                out+=fname[i];
                DecodeStreams(out.c_str(), fsize[i]);
            } 
            int d=segment(segment.pos++);
            if (d!=0xff) printf("Segmend end marker not found\n");
            for (int i=0; i<streamc; ++i) {
                fclose(filestreams[i]);
            }
        }
        fclose(archive);
        if (!doList) programChecker.print();
    }
    catch(const char* s) {
        if (s) printf("%s\n", s);
    }
    if (pause) {
        printf("\nClose this window or press ENTER to continue...\n");
        getchar();
    }
    return 0;
}
