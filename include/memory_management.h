#ifndef MEMORY_MANAGEMENT_H_INCLUDED
#define MEMORY_MANAGEMENT_H_INCLUDED

#define MAX_NUM_MEM_BLOCKS 10

void MEM_init( void );
void* MEM_allocate( size_t p_size );

/*
** Memory Block Header
*/
typedef struct
{
	/* Header Type Info */
	TYPE_COMMON_HEADER common_header;

	/* Memory Info */
	int16_t block_id;
	int16_t allocated_flag;
	size_t  allocated_size_bytes;
	void*   ptr_allocated;
} TYPE_MEM_BLOCK;

/*
** Memory Manager Type
*/
typedef struct
{
	/* Memory Management Info */
	int16_t num_mem_blocks_total;
	int16_t num_mem_blocks_allocated;

	/* Memory Blocks */
	TYPE_MEM_BLOCK mem_blocks[MAX_NUM_MEM_BLOCKS];
} TYPE_MEM_BLOCKS;

typedef enum
{
	BARE_MEM_BLOCK_SUBTYPE,
} ENUM_MEM_BLOCK_SUBTYPES;


#endif // MEMORY_MANAGEMENT_H_INCLUDED
