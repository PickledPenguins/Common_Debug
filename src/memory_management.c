
#include <stdlib.h>
#include <stdint.h>
#include "../include/common.h"
#include "../include/memory_management.h"

/* array of memory blocks */
TYPE_MEM_BLOCKS g_mem_manager;


static TYPE_MEM_BLOCK*
f_GetNextAvaliableBlock( void );

static TYPE_MEM_BLOCK*
f_FindBlockFromPayload
( void* ptr_payload );

void
MEM_init
( void )
{
  /* Initialize Memory Manager */
  g_mem_manager.num_mem_blocks_total     = MAX_NUM_MEM_BLOCKS;
  g_mem_manager.num_mem_blocks_allocated = 0;

  /* Initialize memory blocks */
  CHECK_TRUE( MAX_NUM_MEM_BLOCKS<INT16_MAX );
  memset( (void*)&g_mem_manager.mem_blocks[0], (int)0, MAX_NUM_MEM_BLOCKS*sizeof(TYPE_MEM_BLOCK));

	/* Return */
	return;
}

void*
MEM_allocate
( size_t p_size )
{
	/* pointer to new block */
	TYPE_MEM_BLOCK* ptr_mem_block;

	/* Check number of free blocks */
	if( g_mem_manager.num_mem_blocks_allocated >= MAX_NUM_MEM_BLOCKS )
	{
		LOG(LOG_ERR_ID, "Out of memory space. All memory blocks are in use! \n");
		return( (void*)NULL );
	}

	/* Find available block */
	ptr_mem_block = f_GetNextAvaliableBlock();
	CEHCK_PTR( ptr_mem_block );

	/* Allocate memory for block
	** Optionally clear memory for block */
	ptr_mem_block->ptr_allocated = (void*)malloc( p_size );
	CEHCK_PTR( ptr_mem_block->ptr_allocated );

	/* Set Common Header */
	ptr_mem_block->common_header.start_byte = 1;
	ptr_mem_block->common_header.type       = MEM_BLOCK_TYPE;
	ptr_mem_block->common_header.subtype    = BARE_MEM_BLOCK_SUBTYPE;

	/* Set memory block meta */
  ptr_mem_block->allocated_flag       = 1;
  ptr_mem_block->allocated_size_bytes = p_size;

	/* Update memory manager meta
	** Track memory allocated, number of calls, etc */
	g_mem_manager.num_mem_blocks_allocated++;

	/* Final checks */
	return( ptr_mem_block->ptr_allocated );
}

void
MEM_free
( void* ptr_allocated )
{
	/* pointer block */
	TYPE_MEM_BLOCK* ptr_mem_block;

  /* Check block exists */
  CEHCK_PTR( ptr_allocated );

  /* Find pointer block */
  ptr_mem_block = f_FindBlockFromPayload( ptr_allocated );
  CEHCK_PTR( ptr_mem_block );

  /* Free data */
  free( ptr_allocated );

  /* Clear meta */
  memset( (void*)ptr_mem_block, (int)0, sizeof(TYPE_MEM_BLOCK) );

  /* Update manager */
	g_mem_manager.num_mem_blocks_allocated--;

	return;
}

static TYPE_MEM_BLOCK*
f_GetNextAvaliableBlock
( void )
{
	/* pointer to new block */
	TYPE_MEM_BLOCK* ptr_mem_block;

	/* Find first available block */
	ptr_mem_block = &g_mem_manager.mem_blocks[0];
	while( (ptr_mem_block < &g_mem_manager.mem_blocks[MAX_NUM_MEM_BLOCKS-1]) &&
				 (ptr_mem_block->allocated_flag == TRUE) )
	{
		ptr_mem_block++;
	}

	/* Check */
	if( ptr_mem_block->allocated_flag != FALSE)
	{
		ptr_mem_block = NULL;
	}

	/* Return */
	return( ptr_mem_block );
}

static TYPE_MEM_BLOCK*
f_FindBlockFromPayload
( void* ptr_allocated )
{
	int16_t i;
	TYPE_MEM_BLOCK* ptr_mem_block;

	/* Find block by matching payload address */
	ptr_mem_block = &g_mem_manager.mem_blocks[MAX_NUM_MEM_BLOCKS-1];
	while( (ptr_allocated > &g_mem_manager.mem_blocks[0]) &&
				 (ptr_mem_block->ptr_allocated != ptr_allocated) )
	{
		ptr_mem_block--;
	}

	/* Check */
	if( ptr_mem_block->ptr_allocated != ptr_allocated )
	{
		ptr_mem_block = NULL;
	}

	return( ptr_mem_block );
}
