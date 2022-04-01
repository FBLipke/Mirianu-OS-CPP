#pragma once
#if defined(_MSC_EXTENSIONS)

// Use Microsoft C compiler integer with declarations 

typedef unsigned __int64    uint64_t;
typedef __int64             int64_t;
typedef unsigned __int32    uint32_t;
typedef __int32             int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef char                int8_t;
#else
typedef unsigned long		uint64_t;
typedef long		        int64_t;
typedef unsigned int	    uint32_t;
typedef int		            int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef char                int8_t;
#endif

typedef uint64_t			size_t;
typedef void*				address_t;


#define MAX_BYTE			256
#define MAX_USHORT			65536
#define MAX_SHORT			MAX_USHORT / 2
#define MAX_UINT			4294967296
#define MAX_INT				MAX_UINT / 2
#define MAX_ULONG			18446744073709551616
#define MAX_LONG			MAX_ULONG / 2
