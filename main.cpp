#include <initializer_list>
#include <cstdio>
#include <string>
#include <stdarg.h>

#include "engines.h"

int test_deflate(FILE* input, FILE* output) {
	printf("deflate\n");
	return 0;
}
int test_inflate(FILE* input, FILE* output) {
	printf("inflate\n");
	return 0;
}

#define BACKEND_DEFLATE test_deflate
#define BACKEND_INFLATE test_inflate

bool verbose = false;
size_t eprintf(const char* fmt, ...) {
	if (verbose) {
		va_list vp;
		va_start(vp, fmt);
		vprintf(fmt, vp);
	}
}

bool outputTree = false;

void usage(char* name) {
	printf("Usage:\n"
		"%s <x|p> [-v|--verbose] [-t|--tree] [-i <input file>] [-o <output file>]\n"
		"First argument without a leading dash must be x (extract) or p (pack)\n", 
		name);
	exit(0);
}

int main(int argc, char** argv) {
	char* name = argv[0];

	char action = 'n';
	std::string inputFilename;
	std::string outputFilename;

	argc--; argv++;
	while (argc) {
		std::string arg = argv[0];
		eprintf("Got commandline arg '%s'\n", argv[0]);
		if (arg == "-v" || arg == "--verbose") {
			verbose = true;
		} else if (arg == "-o" || arg == "--output") {
			outputFilename = argv[1];
			argc--; argv++;
		} else if (arg == "-i" || arg == "--input") {
			inputFilename = argv[1];
			argc--; argv++;
		} else if (arg == "-t" || arg == "--tree") {
			outputTree = true;
		} else {
			if (action == 'n' && arg != "x" && arg != "p") {
				usage(name);
			} else if (action == 'n' && (arg == "x" || arg == "p")) {
				action = arg[0];
			}
		}
		argc--; argv++;
	}

	eprintf("Input from: ");
	FILE* input;
	if (inputFilename.empty()) {
		eprintf("stdin\n");
		input = stdin;
	} else {
		eprintf("file '%s'\n", inputFilename.c_str());
		input = fopen(inputFilename.c_str(), "rb");
	}

	eprintf("Output to: ");
	FILE* output;
	if (outputFilename.empty()) {
		eprintf("stdout\n");
		output = stdout;
	} else {
		eprintf("file '%s'\n", outputFilename.c_str());
		output = fopen(outputFilename.c_str(), "wb");
	}

	if (action == 'p') {
		eprintf("Beginning deflate\n");
		return BACKEND_DEFLATE(input, output);
	} else if (action == 'x') {
		eprintf("Beginning inflate\n");
		return BACKEND_INFLATE(input, output);
	}
}
