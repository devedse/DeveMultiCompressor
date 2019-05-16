------------------------------------------------------------------------------

RAZOR archiver - Copyright (C) Christian Martelock 
contact: christian.martelock@web.de

This DEMO/TEST version is for non-commercial use only. DO NOT directly or 
indirectly modify, decompile or otherwise reverse engineer this software.
Use this demo at your OWN risk.

See RZ's console help for more information. 

RAZOR's key properties are:
	-strong  compression ratio & highly asymmetrical 
	-fast  decompression speed & low  memory footprint (1.66N)
	-slow    compression speed & high memory footprint
	-rolz/lz compression engine
	-unicode support & solid archiving
	-block-based deduplication (BLAKE2b)
	-ensures integrity for compressed data & archived files (CRC32)
	-special processing for x86/x64, structures, some image-/audio-types

This package contains the following files:
	-rz.exe
	-readme.txt
	
----------------------------------------------------------------------- FUTURE

RAZOR 1.x
	-RZ 1.x is a technology demo.
	-There'll be bug fixes only.
	-There won't be any compression-related changes.
	-An archive created with RZ 1.x can be extracted by any 1.x version.

RAZOR 2.x
	-RZ 2.0 will support massive multi-threading.
	-RZ 2.1 will have APIs for external integration.
	-RZ 2.2 will have ZIP- and JPEG-recompression.
	-RZ 2.x will have a GUI.
	-RZ 2.x won't be compatible with 1.x.

If you want to license RZ's technology, please wait for 2.x. I'm really sorry. 
Compression is a hobby. I can only spend very little time on it. 

=> 2.x is done, when it's done.

If you do have other ideas or serious proposals, feel free to contact me.

-------------------------------------------------------------------- CHANGELOG

RZ 1.00 (08.09.2017)
	-initial release

RZ 1.01 (14.09.2017)
	-cosmetic bug-fix for console-title
	-PGO-compile

RZ 1.03.6 (11.03.2018)
	-changed output of list-command for better external integration
	-added option to force  oem-codepage for output
	-added option to force utf8-codepage for output
	-added @list support for extraction
	-fixed @list bug
	-added option to force header-scanning