#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdint.h>


/*
** Logging
*/

/* Log debug header */
#define _HEAD_FMT "%10s:%05i:%s():: "
#if 0 /* Use short __FILE__ format */
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define _HEAD_ARG __FILENAME__,__LINE__,__func__
#else /* Use long __FILE__ format */
#define _HEAD_ARG __FILE__,__LINE__,__func__
#endif

/* Log ID's */
#define _ID_ERR 3
#define _ID_WRN 2
#define _ID_SYS 1
#define _ID_LOG 0
/* Log Format */
#define LOG_ERR(msg_fmt, ...) fprintf( stderr, _HEAD_FMT "ERROR   :" msg_fmt, _HEAD_ARG, __VA_ARGS__ )
#define LOG_WRN(msg_fmt, ...) fprintf( stdout, _HEAD_FMT "WARNING :" msg_fmt, _HEAD_ARG, __VA_ARGS__ )
#define LOG_SYS(msg_fmt, ...) fprintf( stdout, _HEAD_FMT "SYSTEM  :" msg_fmt, _HEAD_ARG, __VA_ARGS__ )
#define LOG_LOG(msg_fmt, ...) fprintf( stdout, _HEAD_FMT "LOG     :" msg_fmt, _HEAD_ARG, __VA_ARGS__ )
/* Abstracted Call */
#define LOG(...) LOG_LOG(__VA_ARGS__)


/*
** ID Block Header
*/
typedef struct common_block_header
{
	uint16_t block_type;
	uint16_t block_subtype;
	uint16_t block_checksum;
	uint8_t  start_byte;
} TYPE_COMMON_BLOCK_HEADER;

/*
** Block Types
*/
typedef enum
{
	ID_MEM_BLOCK,
	ID_LOG_BLOCK,
	ID_MSG_OUT_BLOCK,
	ID_MSG_IN_BLOCK,
} TYPE_BLOCK_TYPES;


#endif // COMMON_H_INCLUDED
