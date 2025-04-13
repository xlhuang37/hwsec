
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define BUFF_SIZE (1<<21)
#define SIZE 1024
int main(int argc, char **argv)
{
	// Put your covert channel setup code here
    int num_l1_set = 64;
    int num_l1_assoc = 8;
    int num_l2_set = 1024;
    int num_l2_assoc = 16;
    int line_size = 64;
    int int64_per_line = line_size / sizeof(uint64_t);
	void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
	uint64_t *target_buffer = (uint64_t *) buf;
	target_buffer[0] = 0;
	uint64_t access_time = 0;
	uint64_t tmp;
    int arr[SIZE];

    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }

    srand((unsigned int)time(NULL));

    for (int i = SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap arr[i] and arr[j].
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }


	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	int count = 0;
	while (listening) {
		int success = 0;
		// for(int j = 0; j < num_l2_set * num_l2_assoc; j++){
		// 	tmp = target_buffer[j * 8];
		// }
		int affected_set = 0;
        int count = 0;
		for(int j = 0; j < num_l2_set; j++){
            int before = rdtsc32();
            asm volatile("lfence\n\t");
            int index = j*8;
            for(int repeat = 0; repeat < 10000; repeat++){
                int inner_index = index;
                for(int k = 0; k < num_l2_assoc; k++){
                    tmp = target_buffer[inner_index];
                    inner_index += num_l2_set * 8;
                }
            }

            asm volatile("lfence\n\t");
            int after = rdtsc32();
            count = (after - before);
            printf("Count: %d\n", count);
		}
			// printf("Receiver: %d\n", affected_set);
		
			// if(count == 128) {
			// 	success++;
			// }
		// if(success > 8) {
		// 	break;
		// }
	}

	printf("Receiver finished.\n");

	return 0;
}



