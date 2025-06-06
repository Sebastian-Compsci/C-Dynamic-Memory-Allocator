#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include "mem.h"

extern void* memset(void* ptr, int value, size_t num);

typedef struct Block{
	size_t size;
	struct Block* next;
} Block;

#define ALIGNMENT 8  //mem blocks aligned to 8 bits
#define BLOCK_SIZE sizeof(Block)

static Block* free_list = NULL;

//
//malloc function
//
void* new_malloc(size_t size){
	if (size == 0) {
		return NULL;
	}

	size = (size + ALIGNMENT-1) & ~(ALIGNMENT - 1);
	// round up to next alignment multiple
	// ALIGNMENT-1 = 7, so the llower bits of the not are zero, keeps higher bits
	
	Block* prev = NULL;
	Block* curr = free_list;

	//finding a big enough block
	while(curr){
		if(curr->size >= size){ 
		//true if enough space is found
			if(curr->size >= size + BLOCK_SIZE+ALIGNMENT) { 
				Block* next = (Block*)((char*)curr + BLOCK_SIZE + size);
				next->size = curr->size - size - BLOCK_SIZE;
				next->next = curr->next;
				curr->size = size;

				if(prev){
					prev->next = next;
				}
				else{
					free_list = next;
				}
			}
			else{
				if(prev){
					prev->next = curr->next;
				}
				else{
					free_list = curr->next;
				}
			}
			return(void*)(curr+1);
			//returns a pointer past the block's header
		}
		prev = curr;
		curr = curr->next;
	}
	// handling no found block
	Block* new_block = sbrk(size + BLOCK_SIZE);
	if (new_block == (void*)-1){
		return NULL;
	}

	new_block->size = size;
	new_block->next = NULL;
	return (void*)(new_block+1);

}

void* new_calloc(size_t num, size_t size){
	if(num == 0 || size == 0){  //overflow check
		return NULL;
	}

	size_t total_size = num*size;
	if(size != 0 && total_size/size != num){
		return NULL;   //overflow found
	}

	void* ptr = new_malloc(total_size);
	if(!ptr){
		return NULL;
	}
	memset(ptr, 0, total_size);  //initializes all as zero
	return ptr;
}

void new_free(void* ptr){
    if (!ptr){
        return;
    }

    Block* block = (Block*)ptr - 1; //block header

    if(!free_list || block < free_list){
        block->next = free_list;
        free_list = block;
    }
    else{
        Block* curr = free_list;
        while(curr->next && curr->next < block){
            curr = curr->next;
        }
        block->next = curr->next;
        curr->next = block;
    }
    //merge blocks
    Block* curr = free_list;
    while(curr && curr->next){
        char* end_curr = (char*)curr + BLOCK_SIZE + curr->size;
        if(end_curr == (char*)curr->next){
            //mergeing adjacent blocks
            curr->size += BLOCK_SIZE + curr->next->size;
            curr->next = curr->next->next;
        }
        else{
            curr = curr->next;
        }
    }
}
