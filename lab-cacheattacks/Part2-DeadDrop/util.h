
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
#include <string.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);

// You Should Not Use clflush in your final submission
// It is only used for debug
void clflush(ADDR_PTR addr);

char *string_to_binary(char *s);
char *binary_to_string(char *data);

int string_to_int(char* s);

static inline uint32_t rdtsc32(void) {
    uint32_t lo;
    // Execute the RDTSC instruction and output only the lower 32 bits (in EAX).
    asm volatile ("rdtsc" : "=a"(lo) : : "edx");
    return lo;
}

static inline uint64_t rdtsc64(void) {
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    // Combine the high and low bits into a 64-bit value.
    return ((uint64_t)hi << 32) | lo;
}

#endif

