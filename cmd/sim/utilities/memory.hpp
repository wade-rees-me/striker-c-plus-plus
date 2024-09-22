#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstddef>

// Structure to store memory data
struct MemoryStruct {
	char *memory;
	size_t size;
};

// Function declarations
size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif // MEMORY_HPP
