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
		"%s [-x|--extract] [-p|--pack] [-h|--help] [-v|--verbose] [-t|--tree] [-e|--engine <engine>] [-i|--input <input file>] [-o|--output <output file>]\n"
		"Single-letter args without additional info can be packed into one, like '-xvt'\n"
		"Engine can be any of [ ", 
		name);
	for (auto it : engines) {
		printf("%s ", it.first.c_str());
	}
	printf("]\n");
	exit(0);
}

int main(int argc, char** argv) {
	char* name = argv[0];

	char action = 'n';
	std::string inputFilename;
	std::string outputFilename;

	std::string engine = defaultEngine;

	argc--; argv++;
	while (argc) {
		std::string arg = argv[0];
		if (arg.size() <= 1) {
			usage(name);
		}
		if (arg == "--verbose") {
			verbose = true;
		} else if (arg == "-o" || arg == "--output") {
			outputFilename = argv[1];
			argc--; argv++;
		} else if (arg == "-i" || arg == "--input") {
			inputFilename = argv[1];
			argc--; argv++;
		} else if (arg == "--tree") {
			outputTree = true;
		} else if (arg == "-e" || arg == "--engine") {
			engine = argv[1];
			argc--; argv++;
		} else if (arg == "--help") {
			usage(name);
		} else {
			/**
			 * Single-letter and without additional info
			 */
			if (arg[0] == '-' && arg[1] != '-') {
				for (int i = 1; i < arg.size(); i++) {
					if (arg[i] == 'v') {
						verbose = true;
					} else if (arg[i] == 't') {
						outputTree = true;
					} else if (arg[i] == 'h') {
						usage(name);
					} else if (arg[i] == 'x' || arg[i] == 'p') {
						action = arg[i];
					} else {
						printf("Unknown command line arg: '%c'\n", arg[i]);
						usage(name);
					}
				}
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

	/**
	 * TODO: implement additional command line args to engines
	 */
	eprintf("Using engine '%s'\n", engine.c_str());
	if (action == 'p') {
		eprintf("Starting deflate\n");
		return engines[engine].first(input, output, std::unordered_map<std::string, std::string>());
	} else if (action == 'x') {
		eprintf("Starting inflate\n");
		return engines[engine].second(input, output, std::unordered_map<std::string, std::string>());
	}
}
