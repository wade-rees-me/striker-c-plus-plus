#include <iostream>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include "memory.hpp"

// Callback function for libcurl to write data
size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	MemoryStruct *mem = static_cast<MemoryStruct *>(userp);

	// Reallocate memory for the new content
	char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
	if (ptr == nullptr) {
		std::cerr << "Not enough memory (realloc returned NULL)" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	mem->memory = ptr;
	std::memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;  // Null-terminate the memory buffer

	return realsize;
}

