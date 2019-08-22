#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdint.h>


/* ***************************************************************************
**
** **************************************************************************/
#define TRUE	1
#define FALSE	0


/* ***************************************************************************
**
** **************************************************************************/
#define DEBUG TRUE
//#define DCHECK_IS_ON


/* ***************************************************************************
** Logging
** **************************************************************************/

//#	define USE_SHORT_LOG_FORMAT
#define _LOG_FID stdout
/* LOG ID's */
#define LOG_ERR_ID "ERROR   : "
#define LOG_WRN_ID "WARNING : "
#define LOG_SYS_ID "SYSTEM  : "
#define LOG_LOG_ID "LOG     : "
#define LOG_ID LOG_LOG_ID

/* Log debug header */
#define _HEAD_FMT "%10s:%05i:%s():: "
#ifdef USE_SHORT_LOG_FORMAT /* Use base name __FILE__ format */
#	define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#	define _HEAD_ARG __FILENAME__,__LINE__,__func__
#else /* Use long __FILE__ format */
#	define _HEAD_ARG __FILE__,__LINE__,__func__
#endif
/* Hidden Logging Macros */
#define _FPRINTF(...) fprintf(_LOG_FID, __VA_ARGS__); fflush(_LOG_FID);
#define _LOG_HEAD(_ID) _FPRINTF(_HEAD_FMT "%s", _HEAD_ARG, _ID)
/* Abstracted LOG Call */
#define LOG(_ID, ...) _LOG_HEAD(_ID); _FPRINTF(__VA_ARGS__);


/* ***************************************************************************
** Assert / Check
** **************************************************************************/

#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#	define _IS_TRUE(_EXPR) (__builtin_expect(!!(_EXPR), TRUE))
#	define _IS_FALSE(_EXPR) (__builtin_expect(!!(_EXPR), FALSE))
#else
#	define _IS_TRUE(_EXPR) (TRUE == !!(_EXPR))
#	define _IS_FALSE(_EXPR) (FALSE == !!(_EXPR))
#endif

/* Define (if check fail) execution abort function */
#ifndef CHECK_ABORT
#	include <stdlib.h> /* needed for abort() */
#	define CHECK_ABORT abort()
#endif

/* Check if expression is true */
#define CHECK_TRUE(_EXPR) if(_IS_FALSE(_EXPR)){ CHECK_ABORT; }
/* Check if value is positive */
#define CHECK_POSITIVE(_VAL) if(_IS_FALSE((_VAL)>0)){ CHECK_ABORT; }
/* Check if value is non-zero */
#define CHECK_NONZERO(_VAL) if(_IS_FALSE(_VAL==0)){ CHECK_ABORT; }

/* Equivalent to CHECK_TRUE(false)
** Useful for indicating parts of the code which should never be reached */
#define NOT_REACHABLE() CHECK_TRUE(0)

/* ***************************************************************************
** Common Types
** **************************************************************************/

/*
** ID Block Header
*/
typedef struct
{
	/* Start of Block */
	uint8_t  start_byte;

	/* Block Info */
	uint16_t type;
	uint16_t subtype;
} TYPE_COMMON_HEADER;

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
