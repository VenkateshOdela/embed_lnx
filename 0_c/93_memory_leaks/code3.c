/* https://dev.to/namantam1/a-simple-program-to-detect-memory-leak-in-our-c-program-5c5i*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_ALLOCATIONS 50
//#define WARN(msg, line) (printf("Warning %d: %s\n", line, msg))

/*
Data Structure to keep track of memory allocations
*/
typedef struct {
    size_t address;
    size_t size;
    uint32_t line;
} Mem;

struct {
    Mem mem[MAX_ALLOCATIONS];
    size_t total_allocated_size;
    size_t total_free_size;
} data_s;

/**
 * Find a memory by its address
 *
 * @return: Pointer to memory
*/
Mem *find_by_address(size_t address) {
    for (uint32_t i=0; i<MAX_ALLOCATIONS; i++) {
        if (data_s.mem[i].address == address)
        {
        	printf("%d \n",i);
        	return &data_s.mem[i]; // as soon as find return
        }

    }

    // otherwise return null
    return NULL;
}


/**
 * insert memory allocated with size
*/
void insert(size_t address, size_t size, uint32_t line) {
    // check for null
    if (address == 0) {
        printf("Warning %d : Memory allocation failed \n", line);
        return;
    }

    Mem *mem = find_by_address(0);
    // if the return value is null we need to increase the MAX_ALLOCATIONS value
    if (mem == NULL) {
        printf("Warning %d :Max allocations reached \n", line);
        return;
    }

    // save all the allocation info
    mem->address = address;
    mem->size = size;
    mem->line = line;

    data_s.total_allocated_size += size;
}

/**
 * Remove the memory allocation
 *
 * @return: -1 on failure else 0
*/
int erase(size_t address, uint32_t line) {
    if (address == 0) {
        printf("Warning %d : Tried to free a null ptr \n", line);
        return -1;
    }

    Mem *mem = find_by_address(address);
    // if the address is not found we assume it is already deleted
    if (mem == NULL) {
        printf("Warning %d : Double free detected \n", line);
        return -1;
    }

    // set address to null and update info
    mem->address = 0;
    data_s.total_free_size += mem->size;
    return 0;
}

void print_report() {
    printf("\nLeak Summary\n");
    printf("Total Memory allocated %lu bytes\n", data_s.total_allocated_size);
    printf("Total Memory freed     %lu bytes\n", data_s.total_free_size);
    printf("Memory Leaked          %lu bytes\n\n",
        data_s.total_allocated_size - data_s.total_free_size);

    if (data_s.total_free_size != data_s.total_allocated_size) {
        printf("Detailed Report\n");
        for (int i=0; i<MAX_ALLOCATIONS; i++) {
            if (data_s.mem[i].address != 0) {
                printf("Memory leak at line %d: (%lu bytes)\n",
                    data_s.mem[i].line,
                    data_s.mem[i].size);
            }
        }
    }
}

// Override allocation functions
void *my_malloc(size_t size, uint32_t line) {
    void *ptr = malloc(size);

    // insert to memory data_s
    insert((size_t)ptr, size, line);

    return ptr;
}

void my_free(void *ptr, uint32_t line) {
    // erase memory data_s
    if (erase((size_t)ptr, line) == 0)
        free(ptr);
}

// redefine allocator functions
//#define malloc(size) _malloc(size, __LINE__)
//#define free(ptr) _free(ptr, __LINE__)

int main() {
    int *n1 = my_malloc(sizeof(int),__LINE__);
    //my_free(n1,__LINE__);
   // my_free(n1,__LINE__);

    //int *n2 = NULL;
    //my_free(n2,__LINE__);
    //my_free(n2,__LINE__);


    int *n3 = my_malloc(sizeof(int),__LINE__);
    //my_free(n3,__LINE__);
    //my_free(n3,__LINE__);


    int *n4 = my_malloc(sizeof(int),__LINE__);

    int *n5 = my_malloc(sizeof(int),__LINE__);


    int *n6 = my_malloc(sizeof(int),__LINE__);

    print_report();
    return 0;
}
