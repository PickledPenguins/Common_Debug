#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"
#include "../include/memory_management.h"



int main(void)
{
	int* test_ptr;

	MEM_init();

	test_ptr = (int*)MEM_allocate( sizeof(int) );
	CEHCK_PTR( test_ptr, "Check test_ptr \n" );
	MEM_free( test_ptr );

	test_ptr = (int*)MEM_allocate( sizeof(int) );
	CEHCK_PTR( test_ptr, "Check test_ptr \n" );
	MEM_free( test_ptr );

	test_ptr = (int*)MEM_allocate( sizeof(int) );
	CEHCK_PTR( test_ptr, "Check test_ptr \n" );
	MEM_free( test_ptr );

	return 0;
}
