all:
	mkdir -p build
	g++ -o build/huffman frontend/*.cpp backend/*.cpp -O2
