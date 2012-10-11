# LZW encoder/decoder #

This is an implementation of the LZW (Lempel-Ziv-Welch) compression scheme, written for CS323 @ Yale. It includes an encoder and decoder which can be used to compress and decompress files.

Briefly, the LZW compression algorithm involves keeping a string table of all byte sequences seen in the input, and representing each of these sequences with a code, such that repeated sequences can be represented by the same code, resulting in space savings. In this implementation, the string table is stored in a data structure which can be indexed by a hash table or an array, depending on the type of lookup being performed.

## Build Instructions ##

(Note: this process has been minimally tested on Linux and Mac OS X, YMMV)

To build the binaries `encode` and `decode`:
`$ git clone https://github.com/geoffreylitt/lzw.git`
`$ cd lzw/src`
`$ make`

You will then find `encode` and `decode` in `lzw/bin`. You may want to add them to your PATH for convenience.

## Usage Instructions ##

`encode` reads in a byte stream from stdin and outputs a compressed version of the byte stream to stdout. To compress a file and save the compressed version, it can be used like this:
`$ encode < file.raw > file.compressed`

`decode` reads in a compressed bytestream from stdin and outputs the uncompressed version of the byte stream to stdout. To decompress a file and save the decompressed version, it can be used like this:
`$ decode < file.compressed > file.raw`

In addition, `encode` has several flags which can be used to change the parameters of the program, which can be used in any combination. They all affect the compression ratio in various ways, depending on the nature of the file.

- `$ encode -m MAXBITS` specifies the maximum number of bits which will be used to store codes in the string table used in the LZW algorithm. MAXBITS should be between 9 and 24.
- `$ encode -p WINDOW` enables "pruning" of the string table. This means that when the string table runs out of space it will be pruned so that only the last WINDOW codes that were sent remain in the table. WINDOW values should be less than the maximum value of an int type on your system -- typical values should be under 1,000,000. Generally, enabling pruning will increase compression, especially for large files.
- `$ encode -e` enables sending escape codes. By default, the string table is initialized with all one-byte sequences, but when the `-e` flag is enabled, it is not initialized with these sequences, and a special escape code is sent any time a one-byte sequence is seen in the input file for the first time.

For example, one could use `encode` as follows:

`$ encode -e -p 8000 -m 18 < file.raw > file.compressed`

`decode` will automatically detect parameters for a file created by `encode`, so it does not accept any parameters.
