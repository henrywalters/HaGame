#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#include <cstdint>
#include <stdio.h>

struct MemoryUsageStatistics {
	uint32_t totalAllocated;
	uint32_t totalFreed;

	uint32_t currentUsage() {
		return totalAllocated - totalFreed;
	}
};

static MemoryUsageStatistics s_MemoryUsageStatistics;

void PrintCurrentUsage() {
	std::cout << s_MemoryUsageStatistics.currentUsage() << std::endl;
}

void* operator new(size_t size) {
	s_MemoryUsageStatistics.totalAllocated += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size) {
	s_MemoryUsageStatistics.totalFreed += size;
	free(memory);
}

#endif
