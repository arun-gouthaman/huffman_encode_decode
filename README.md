# huffman_encode_decode

Encoding:

Obtains the frequency of each character to build the huffman tree and extract bits leading to each character.

Writes the bits to binary file with encoding.

Writes the frequency file as text file for decoding (needs work on this to reduce the size as the file size is large due to being text file.)


Decoding:

Reads from binary file and frequency file.

Build Huffman tree from frequency data.

Navigate the tree suing bits read from binary file and decode characters replacing set of bits with right character. 

to build:

install cmake (https://cmake.org/download/)

navigate to cloned folder

```mkdir build; cd build```

```cmake .. -G "Visual Studio 16 2019"```

compiling code

```msbuild .\<solutuion_file>.sln /p:Configuration=Debug```