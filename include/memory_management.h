#ifndef MEMORY_MANAGEMENT_H_INCLUDED
#define MEMORY_MANAGEMENT_H_INCLUDED

/*****************************************************/

#include "../include/common.h"

/*****************************************************/

#define MAX_NUM_MEM_BLOCKS 10
#define USE_SECURE_MEMSET TRUE

#define MEM_ZERO_FUNCTION(_PTR, _SIZE) MEM_ZeroMemory(_PTR, _SIZE)
#define CLEAR_MEM MEM_ZERO_FUNCTION

/*****************************************************/

void
MEM_ZeroMemory
( void* ptr, size_t p_size );

void
MEM_init
( void );

void*
MEM_allocate
( size_t p_size );

void
MEM_free
( void* ptr_allocated );

/*****************************************************/

/*
** Memory Block Header
*/
typedef struct
{
	/* Header Type Info */
	TYPE_COMMON_HEADER common_header;

	/* Memory Sub-Blocks Info */
	int16_t num_mem_blocks_defined;
	int16_t num_mem_blocks_allocated;

	/* Memory Block Info */
	int16_t block_id;
	int16_t num_bytes_allocated;
	int16_t allocated_flag;
	void*   ptr_allocated;
} TYPE_MEM_BLOCK;

/*
** Memory Manager Type
*/
typedef struct
{
	/* Memory Blocks Info */
	int16_t num_mem_blocks_defined;
	int16_t num_mem_blocks_allocated;
	int16_t block_id_next;

	/* Tracking Info */
	int16_t num_bytes_allocated_now;
	int16_t num_bytes_allocated_count;
	int16_t num_mem_blocks_allocated_count;

	/* Memory Blocks*/
	TYPE_MEM_BLOCK mem_blocks[MAX_NUM_MEM_BLOCKS];
} TYPE_MEM_BLOCKS;

typedef enum
{
	BARE_MEM_BLOCK_SUBTYPE,
} ENUM_MEM_BLOCK_SUBTYPES;


#endif // MEMORY_MANAGEMENT_H_INCLUDED
