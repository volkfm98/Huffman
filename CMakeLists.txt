cmake_minimum_required(VERSION 2.8)
project(huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")

set(SOURCE_FILES
    engines/huffman-2/tree-build.cpp
    engines/huffman-2/tree-serialize.cpp
    engines/huffman-2/tree.cpp
    engines/huffman-2/util.cpp
    engines/huffman-old/HuffmanArchive.cpp
    engines/huffman-old/HuffmanDearchive.cpp
    src/main.cpp
        engines/huffman-2/deflate.cpp engines/huffman-2/tree-deserialize.cpp engines/huffman-2/inflate.cpp engines/huffman-2/tree-sym.cpp src/log.cpp)

include_directories(src)

add_executable(huffman ${SOURCE_FILES})
