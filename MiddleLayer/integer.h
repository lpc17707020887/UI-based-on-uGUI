#ifndef _INTEGER_H_
#define _INTEGER_H_

/**********************************头文件************************************/


/**********************************宏定义************************************/
#define TRUE  1
#define FALSE 0

/**********************************类型定义************************************/
/* Embedded platform */

/* These types must be 8-bit integer */
typedef char            bool;
typedef char			CHAR;
//typedef char            s8;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;
//typedef unsigned char   u8;


/* These types must be 16-bit integer */
typedef short			SHORT;
//typedef short           s16;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;
//typedef unsigned short  u16;


/* These types must be 16-bit, 32-bit or larger integer */
//typedef int             s32;
typedef int				INT;
typedef unsigned int	UINT;
//typedef unsigned int    u32;


/* These types must be 32-bit integer */
typedef long			LONG;
//typedef long            s64;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;
//typedef unsigned long   u64;

/**********************************变量声明************************************/


/**********************************函数声明***********************************/










#endif

