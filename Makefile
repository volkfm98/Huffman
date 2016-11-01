all:
	mkdir -p build
	g++ -std=c++11 -I backend -o build/huffman frontend/*.cpp backend/*.cpp -O2
