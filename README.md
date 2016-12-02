# Huffman
Huffman - just some implementations of Huffman coding algorithm and the tool to use these implementations.


# Navigation
1. [Installation](#installation)
2. [Usage](#usage)

# Installation <div name="installation"></div>
1. Install CMake 2.8+
You can download the package from [this link](https://cmake.org/download/).

2. Clone the code

        $ git clone https://github.com/volkfm98/Huffman

3. Build (assuming you are now in repo directory)

        $ mkdir build
        $ cd build
        $ cmake ..
        $ make

4. The executable is in the build directory

# Usage <div name="usage"></div>
        ./huffman [-e|--engine <engine name>] [-i|--input <input file>] [-o|--output <output file>] [-v|--verbose]

Currently available engines are:
* `huffman-old` - [volkfm](https://github.com/volkfm98)'s implementation
* `huffman-2` - [ionagamed](https://github.com/ionagamed)'s implementation (may be slower, because of extensive STL usage and explicit tree serialization)

Might have renamed those accordingly, but nah.