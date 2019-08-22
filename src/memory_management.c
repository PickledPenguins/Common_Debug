


void* MEM_allocate( size_t p_size )
{
	/* pointer to new block */
	TYPE_MEM_BLOCK* ptr_mem_block;

	/*
	** Checks
	*/
	/* Check number of free blocks */

	/* Find available block */

	/* Allocate memory for block
	** Optionally clear memory for block */

	/* Set memory block meta */

	/* Update memory manager meta
	** Track memory allocated, number of calls, etc */

	/* Final checks */

	/* Set return pointer (pointer to data) */

	return( (void*)prt_block )
}
