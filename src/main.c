#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"


int main()
{

	TYPE_COMMON_BLOCK_HEADER block;

	printf("Hello world!\n");
	LOG_ERR( " testing... %d %s %f \n", ID_MEM_BLOCK, "ham", 3.141519 );
	LOG_WRN( " testing... %d %s %f \n", ID_LOG_BLOCK, "ham", 3.141519 );
	LOG_SYS( " testing... %d %s %f \n", ID_LOG_BLOCK, "ham", 3.141519 );
	LOG( " testing... %d %s %f \n", ID_MSG_IN_BLOCK, "ham", 3.141519 );

	return 0;
}
