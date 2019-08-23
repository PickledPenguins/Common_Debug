#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"
#include "../include/memory_management.h"


int main(void)
{
	int* test_ptr[MAX_NUM_MEM_BLOCKS];
	int i;
	MEM_init();

	for(i=0; i<(MAX_NUM_MEM_BLOCKS); i++)
	{
		LOG(LOG_ID, "i:%i \n", i);
		test_ptr[i] = (int*)MEM_allocate( sizeof(int)*10 );
		CEHCK_PTR( test_ptr[i] );
	}


	for(i=0; i<(MAX_NUM_MEM_BLOCKS); i++)
	{
		LOG(LOG_ID, "i:%i \n", i);
		MEM_free( test_ptr[i] );
		CEHCK_PTR( test_ptr[i] );
	}

	return 0;
}
