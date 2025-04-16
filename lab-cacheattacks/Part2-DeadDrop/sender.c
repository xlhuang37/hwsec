
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>

// TODO: define your own buffer size
#define BUFF_SIZE (1<<21)
//#define BUFF_SIZE [TODO]

int main(int argc, char **argv)
{
  // Allocate a buffer using huge page
  // See the handout for details about hugepage management
  void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }
  // The first access to a page triggers overhead associated with
  // page allocation, TLB insertion, etc.
  // Thus, we use a dummy write here to trigger page allocation
  // so later access will not suffer from such overhead.
  //*((char *)buf) = 1; // dummy write to trigger page allocation


  // TODO:
  // Put your covert channel setup code here
    // [1.2] TODO: Uncomment the following line to allocate a buffer of a size
    // of your chosing. This will help you measure the latencies at L2 and L3.
    int num_l1_set = 64;
    int num_l1_assoc = 8;
    int num_l2_set = 1024;
    int num_l2_assoc = 16;
    int line_size = 64;
    int int64_per_line = line_size / sizeof(uint64_t);
    uint64_t *eviction_buffer = (uint64_t *) buf;
    eviction_buffer[0] = 0;
    uint64_t tmp = 0;

    bool sending = true;
    

    
  
  int64_t before, after;
  int64_t RUNTIME = (1ll << 33);
  while (sending) {
    printf("Please type a message.\n");
    char text_buf[128];
    fgets(text_buf, sizeof(text_buf), stdin);
    int message = string_to_int(text_buf);
      // for(int j = 0; j < num_l2_assoc; j++){
      //   tmp = eviction_buffer[j * int64_per_line];
      // }
      // tmp = eviction_buffer[0 * int64_per_line];
      // tmp = eviction_buffer[8];
      // tmp = eviction_buffer[16];
      // tmp = eviction_buffer[24];
      // for(int i = 0; i < num_l2_assoc; i++) {
      //   for(int j = 0; j < message; j++){
      //       tmp = eviction_buffer[j * 8 + i * num_l2_set * 8];
      //   }
      // }
      before = rdtsc64();
      while (1) {
        int outer_index = 0;
        for (int i = 0; i < 8; i++) {
            int index = outer_index;
            for (int j = 0; j < message; j++) {

                tmp = eviction_buffer[index];
                index += 8;
            }
            outer_index += num_l2_set * 8;
        }

        int64_t current_time = rdtsc64();
        if (current_time - before > RUNTIME) {
            break;
        }
    }
  }

  printf("Sender finished.\n");
  return 0;
}



