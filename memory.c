#include <stdio.h>

#define MEM_SIZE 1024

char ram[MEM_SIZE];

struct node 
{
	struct node* next;
	int freespace;
};

struct node memory_list;

struct node* head = &memory_list;

void memory_init();
{
	for (int i = 0; i < MEM_SIZE; i++)
	{
		ram[i] = 0;
	}
	
}

void* mem_alloc(size_t byte_count)
{
	




int main(void)
{
	




