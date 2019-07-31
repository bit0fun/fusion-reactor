#ifndef REACTOR_MEM_CFG_H
#define REACTOR_MEM_CFG_H

/** Settings for memory **/

/* Memory size definitions */
#define MEM_64M		67108864
#define MEM_32M		33554432
#define MEM_16M		16777216
#define MEM_8M		8388608
#define MEM_4M		4194304
#define MEM_2M		2097152
#define MEM_1M		1048576
#define MEM_512K	524288
#define MEM_256K	262144
#define MEM_128K	131072
#define MEM_64K		65536
#define MEM_32K		32768
#define MEM_16K		16384
#define MEM_8K		8192
#define MEM_4K		4096
#define MEM_2K		2048
#define MEM_1K		1024

/* Definitions for places in simulated memory space */
#define ROM_START		(uintmax_t)( 0x40000 )
#define ROM_SIZE		(uintmax_t)( MEM_16K )

#define RAM_START		(uintmax_t)( 0x20000000 )
#define RAM_SIZE		(uintmax_t)( MEM_16K )


#endif
