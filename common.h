
#ifndef RETRO
#define UInt32 uint32_t
#else
#define UInt32 unsigned
#endif

typedef struct
{
		char recordType;               // 1 byte
		char syncBytes[5];       // 5 bytes
		char recordVersion;            // 1 byte
		char flags;                    // 1 byte
		char comment[32];        // 32 bytes
		char label[32];          // 32 bytes
		UInt32 id;                     // 4 bytes
		UInt32 blockSize;              // 4 bytes
		UInt32 blockCount;             // 4 bytes
		UInt32 rootDirId;              // 4 bytes
		UInt32 rootDirBlocks;          // 4 bytes
		UInt32 rootDirBlockSize;       // 4 bytes
		UInt32 lastRootDirCopy;        // 4 bytes
		UInt32 rootDirCopies[8]; // 32 bytes
}NvRamStr;

extern void	_3do_Init();
