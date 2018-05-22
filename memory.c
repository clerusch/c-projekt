#include <stdio.h>

#define MEM_SIZE 1024

char ram[MEM_SIZE];

struct memlist 
{
	struct memlist* next;
	int freespace;
};

//struct memlist memory_list;


void memory_init()
{

	struct memlist* pmb = (void*)&ram[0];

	pmb -> freespace = 100;

	
}

/*
void* mem_alloc(size_t byte_count)
{

}
*/
	




int main(void)
{
	memory_init();
	struct memlist* firstblock = (void*)&ram[0];
	printf("%d\n", firstblock -> freespace);

	if (byte_count < currentblock -> freespace)
		return firstblock;
	else 


	return 0;
}
	




