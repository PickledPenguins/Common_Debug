/*****************************************************
* FILE NAME *
* *
* PURPOSE *
* *
*****************************************************/

#include <stdlib.h>
#include <stdint.h>
#include "../include/common.h"
#include "../include/memory_management.h"

/*****************************************************/

/* array of memory blocks */
TYPE_MEM_BLOCKS g_mem_manager;

/*****************************************************/

static void*
f_SecureMemset_v1
(void* ptr, int val, size_t len);

static void*
f_SecureMemset_v2
(void* ptr, int val, size_t len);

static void*
f_allocate_calloc
( size_t p_size );

static TYPE_MEM_BLOCK*
f_GetNextAvaliableBlock
( void );

static TYPE_MEM_BLOCK*
f_FindBlockFromPayload
( void* ptr_allocated );

static void
f_DumpGlobalBlock_Long
( void );

static void
f_DumpGlobalBlock_Short
( void );



/*****************************************************/

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
void
MEM_ZeroMemory
( void* ptr, size_t p_size )
{
	if( USE_SECURE_MEMSET )
	{
		/* Prevent zeroing from being optimized out */
		//ptr = f_SecureMemset_v1( ptr, (int)0, p_size );
		ptr = f_SecureMemset_v2( ptr, (int)0, p_size );
	}
	else
	{
    memset( ptr, (int)0, p_size );
	}
	return;
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
void
MEM_init
( void )
{
  /* Memory Blocking Info */
  g_mem_manager.num_mem_blocks_defined    = MAX_NUM_MEM_BLOCKS;
  g_mem_manager.num_mem_blocks_allocated = 0;
  g_mem_manager.block_id_next            = 0;

  /* Memory Tracking Info */
  g_mem_manager.num_bytes_allocated_now        = 0;
  g_mem_manager.num_bytes_allocated_count      = 0;
  g_mem_manager.num_mem_blocks_allocated_count = 0;

  /* Initialize memory blocks */
  CHECK_TRUE( MAX_NUM_MEM_BLOCKS<INT16_MAX, "Check MAX_NUM_MEM_BLOCKS<INT16_MAX \n" );
  CLEAR_MEM( (void*)&g_mem_manager.mem_blocks[0], MAX_NUM_MEM_BLOCKS*sizeof(TYPE_MEM_BLOCK) );

  /* Log */
  f_DumpGlobalBlock_Short();

	/* Return */
	return;
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
void*
MEM_allocate
( size_t p_size )
{
	/* pointer to new block */
	TYPE_MEM_BLOCK* ptr_mem_block;

	/* Check number of free blocks */
	CHECK_TRUE( g_mem_manager.num_mem_blocks_allocated < MAX_NUM_MEM_BLOCKS, "Check num_mem_blocks_allocated < MAX_NUM_MEM_BLOCKS \n");

	/* Find available block */
	ptr_mem_block = f_GetNextAvaliableBlock();
	CEHCK_PTR( ptr_mem_block, "Check ptr_mem_block \n" );

	/* Allocate memory for block
	** Optionally clear memory for block */
	CHECK_TRUE( p_size > 0, "Check p_size > 0 \n" );
	ptr_mem_block->ptr_allocated = f_allocate_calloc( p_size );
	CEHCK_PTR( ptr_mem_block->ptr_allocated, "Check ptr_allocated \n" );

	/* Set Common Header */
	ptr_mem_block->common_header.start_byte     = 1;
	ptr_mem_block->common_header.primary_type   = MEM_BLOCK_TYPE;
	ptr_mem_block->common_header.secondary_type = BARE_MEM_BLOCK_SUBTYPE;
	/* checksums ... */

	/* Memory Sub-Blocks Info */
  ptr_mem_block->num_mem_blocks_defined     = 1;
  ptr_mem_block->num_mem_blocks_allocated   = 1;

	/* Memory Block Info */
  ptr_mem_block->block_id            = g_mem_manager.block_id_next;
  ptr_mem_block->num_bytes_allocated = p_size;
  ptr_mem_block->allocated_flag      = 1;

	/* Update memory manager meta
	** Track memory allocated, number of calls, etc */
	g_mem_manager.num_mem_blocks_allocated++;
	g_mem_manager.block_id_next++;
  g_mem_manager.num_bytes_allocated_now += p_size;
	g_mem_manager.num_bytes_allocated_count += p_size;
	g_mem_manager.num_mem_blocks_allocated_count++;

  /* Log */
  f_DumpGlobalBlock_Short();

	/* Final checks */
	return( ptr_mem_block->ptr_allocated );
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
void
MEM_free
( void* ptr_allocated )
{
	/* Pointer to block to free */
	TYPE_MEM_BLOCK* ptr_mem_block;

  /* Check block exists */
  CEHCK_PTR( ptr_allocated, "Check ptr_allocated \n" );

  /* Find pointer block */
  ptr_mem_block = f_FindBlockFromPayload( ptr_allocated );
  CEHCK_PTR( ptr_mem_block, "Check ptr_mem_block \n" );

  /* Free data */
  CLEAR_MEM(ptr_allocated, ptr_mem_block->num_bytes_allocated);
  free( ptr_allocated );

  /* Update manager */
  g_mem_manager.num_bytes_allocated_now -= ptr_mem_block->num_bytes_allocated;
	g_mem_manager.num_mem_blocks_allocated--;

  /* Clear block meta */
  CLEAR_MEM(ptr_mem_block, sizeof(TYPE_MEM_BLOCK));

  /* Log */
  f_DumpGlobalBlock_Short();

	/* return */
	return;
}


/*****************************************************/
/* Static Functions */
/*****************************************************/

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static void* (* const volatile f_ptr_memset)(void*, int, size_t) = memset;

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static void*
f_SecureMemset_v1
(void* ptr, int val, size_t len)
{
	(f_ptr_memset)(ptr, val, len);
	return ptr;
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static void*
f_SecureMemset_v2
(void* ptr, int val, size_t len)
{
	volatile uint8_t *p = ptr;
	while( len-- )
	{
		*p++ = val;
	}
	return ptr;
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static void*
f_allocate_calloc
( size_t p_size )
{
	void* ptr;
	ptr = (void*)calloc( p_size, (size_t)1 );
	return( ptr );
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
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

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static TYPE_MEM_BLOCK*
f_FindBlockFromPayload
( void* ptr_allocated )
{
	TYPE_MEM_BLOCK* ptr_mem_block;

	/* Find block by matching payload address */
	ptr_mem_block = &g_mem_manager.mem_blocks[MAX_NUM_MEM_BLOCKS-1];
	while( (ptr_allocated > (void*) &g_mem_manager.mem_blocks[0]) &&
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

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static void
f_DumpGlobalBlock_Long
( void )
{
	LOG( LOG_SYS_ID, "Dumping g_mem_manager \n" );
  LOG( LOG_SYS_ID, "          num_mem_blocks_defined : %i \n", g_mem_manager.num_mem_blocks_defined );
  LOG( LOG_SYS_ID, "        num_mem_blocks_allocated : %i \n", g_mem_manager.num_mem_blocks_allocated );
  LOG( LOG_SYS_ID, "                   block_id_next : %i \n", g_mem_manager.block_id_next );
  LOG( LOG_SYS_ID, "         num_bytes_allocated_now : %i \n", g_mem_manager.num_bytes_allocated_now );
  LOG( LOG_SYS_ID, "       num_bytes_allocated_count : %i \n", g_mem_manager.num_bytes_allocated_count );
  LOG( LOG_SYS_ID, "  num_mem_blocks_allocated_count : %i \n", g_mem_manager.num_mem_blocks_allocated_count );

  return;
}

/********************************************************
* FUNCTION NAME:
*
* ARGUMENT TYPE I/O DESCRIPTION
* -------- ---- --- -----------
*
* RETURNS:
*
*********************************************************/
static void
f_DumpGlobalBlock_Short
( void )
{
	LOG( LOG_SYS_ID,
			"MEM State :: Bytes Allocated Now:%i, Blocks Allocated Now:[%i/%i] (Total Bytes:%i, Total Blocks:%i) \n",
			g_mem_manager.num_bytes_allocated_now,
			g_mem_manager.num_mem_blocks_allocated,
			g_mem_manager.num_mem_blocks_defined,
			g_mem_manager.num_bytes_allocated_count,
			g_mem_manager.num_mem_blocks_allocated_count );

	return;
}



