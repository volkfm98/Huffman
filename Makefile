all:
	mkdir -p build
	g++ -std=c++11 -o build/huffman src/main.cpp engines/*/*.cpp -O2
