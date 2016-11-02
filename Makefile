all:
	mkdir -p build
	g++ -std=c++11 -o build/huffman main.cpp engines/*/*.cpp -O2
