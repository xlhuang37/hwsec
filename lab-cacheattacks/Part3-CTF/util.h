
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>
#include <stdbool.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);

void clflush(ADDR_PTR addr);

static inline uint32_t rdtsc32(void) {
    uint32_t lo;
    // Execute the RDTSC instruction and output only the lower 32 bits (in EAX).
    asm volatile ("rdtsc" : "=a"(lo) : : "edx");
    return lo;
}

int find_mode(int arr[], int n);

#endif

