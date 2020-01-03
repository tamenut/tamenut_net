#pragma once

#include "TThread.h"
#include "TList.h"
#include "TMutex.h"
#include "TCircularQueue.h"
#include <vector>
#include <list>
#include "CommonDllDefine.h"

using namespace std;
//#define USE_MEMORY_MANAGER
//#define USE_MEMORY_POOL


//#define CreateMemorys(type, size) (new(TMemoryManager::get_instance()->alloc( sizeof(type) * (size)) ) type[static_cast<size_t>(size)] )
#define CreateMemorys(type, size) (type*)TMemoryManager::get_instance()->alloc(sizeof(type)*size)
//#define CreateMemoryInit(type, size, ...) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(__VA_ARGS__)

#define ALLOCATE(size) TMemoryManager::get_instance()->alloc(size)
#define CreateMemory(type) (type*)TMemoryManager::get_instance()->alloc(sizeof(type))
#define DeleteMemory(buf) TMemoryManager::get_instance()->free((void*)buf)



#define STM_ALLOCATOR STMAllocator


//#define CreateMemory(type, size) (new(TMemoryManager::get_instance()->alloc(sizeof(type) * (static_cast<size_t>(size))) ) type[static_cast<size_t>(size)] )

#if defined(WIN32)
#define NEW_UTYPE(type, size,...) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(__VA_ARGS__)

#else //#elif defined(VXWORKS)
#define NEW_UTYPE(type, size) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type()
#endif

//#define NEW_UTYPE(type, size) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type()
#define NEW_UTYPE1(type, size, a1) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1)
#define NEW_UTYPE2(type, size, a1, a2) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2)
#define NEW_UTYPE3(type, size, a1, a2, a3) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3)
#define NEW_UTYPE4(type, size, a1, a2, a3, a4) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3, a4)
#define NEW_UTYPE5(type, size, a1, a2, a3, a4, a5) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3, a4, a5)
#define NEW_UTYPE6(type, size, a1, a2, a3, a4, a5, a6) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3, a4, a5, a6)
#define NEW_UTYPE7(type, size, a1, a2, a3, a4, a5, a6, a7) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3, a4, a5, a6, a7)
#define NEW_UTYPE8(type, size, a1, a2, a3, a4, a5, a6, a7, a8) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3, a4, a5, a6, a7, a8)
#define NEW_UTYPE9(type, size, a1, a2, a3, a4, a5, a6, a7, a8, a9) ((size) > 1 ) ? new type[static_cast<size_t>(size)] : new type(a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define DELETE_PTYPE(object, size) (TMemoryManager::get_instance()->free(object, size))
#define DELETE_UTYPE(object, size) ((size) > 1 ) ? delete[] object : delete object

#define DECLARE_MEMORY_ALLOCATION_OPERATORS							\
public:																\
	static void* operator new (size_t size)							\
	{																\
		return ALLOCATE(size);									\
	}																\
	static void* operator new[] (size_t size)						\
	{																\
		return ALLOCATE(size);										\
	}																\
	static void operator delete (void * object)						\
	{																\
		DeleteMemory(object);										\
	}

/*	
	static void operator delete[] (void * object, size_t size)		\
	{																\
		TMemoryManager::get_instance()->free(object, size);			\
	}
	static void * operator new(size_t, void * ptr)					\
	{																\
		return ptr;													\
	}																\
*/
#define CHUNK_SIZE_RESOLUTION 64
#define ALLOCATE_BASE_SIZE 16384 //16*1024

#define SMALL_CHUNK_MAX_INDEX ALLOCATE_BASE_SIZE/CHUNK_SIZE_RESOLUTION
#define BIG_CHUNK_MAX_INDEX ALLOCATE_BASE_SIZE/CHUNK_SIZE_RESOLUTION


#define MEMORY_POOL_SIZE	50*1024*1024
//#define MEMORY_POOL_SIZE	20*1024


typedef struct _MemBlock
{	
	size_t signature1;
	size_t signature2;
	size_t alloc_size;
	size_t request_size;
	char* chunk;
}MemBlock;

typedef struct _FreeBlockEntry
{
	unsigned int allocated_cnt;
	unsigned int max_alloc_cnt;
	unsigned long long accumulated_alloc_cnt;
	TMutex lock;
	list<MemBlock*> mem_block_list;
}FreeBlockEntry;

typedef struct _MemPool
{
	char* payload;
	unsigned int cur_pos;
}MemPool;

const int BLOCK_HEADER_SIZE = sizeof(struct _MemBlock) - sizeof(unsigned char*);
const int BLOCK_FOOTER_SIZE = 0;

#define MEMORY_MANAGER_MAGIC_NUMBER1		0x454D4154L
#define MEMORY_MANAGER_MAGIC_NUMBER2		0x2154554EL

class TMemoryManager : public TThread
{
public:
	UTIL_DLLFunction TMemoryManager(void);
	UTIL_DLLFunction ~TMemoryManager(void);

	UTIL_DLLFunction static TMemoryManager* get_instance();

//	void dumpMemoryStatistic(MemoryDump *dump);
	UTIL_DLLFunction void showMemoryStatistic();

	UTIL_DLLFunction void *alloc(unsigned int requested_chunk_size);
	UTIL_DLLFunction void free(void *p_buf);

	void run();
#ifdef USE_MEMORY_MANAGER
private:
	MemBlock *create_small_chunk(unsigned int requested_chunk_size);
	MemBlock *create_big_chunk(unsigned int requested_chunk_size);

	void delete_small_chunk(MemBlock* mem_block);
	void delete_big_chunk(MemBlock* mem_block);

	MemBlock *pop_big_chunk(int idx, unsigned int chunk_size);
	MemBlock *_pop_big_chunk(int idx, unsigned int chunk_size);
	void push_big_chunk(int idx, MemBlock *p_block);
	void _push_big_chunk(int idx, MemBlock *p_block);

	MemBlock *pop_small_chunk(int idx);
	void push_small_chunk(int idx, MemBlock *mem_block);

#ifdef USE_MEMORY_POOL
	void create_memory_pool();
	char *get_chunk_from_memory_pool(int alloc_size);
#endif

#endif
protected:
#ifdef USE_MEMORY_MANAGER	
	FreeBlockEntry small_chunk_list[SMALL_CHUNK_MAX_INDEX];		//1~1024byte - 64 단위로 할당
	FreeBlockEntry big_chunk_list[BIG_CHUNK_MAX_INDEX];			//1024 ~ 64*1024 byte - 1024 단위로 할당
	MemBlock *recent_used_big_chunk[BIG_CHUNK_MAX_INDEX];	
	
	MemPool *current_memory_pool;
	std::vector<MemPool*> memory_pool_list;

#endif
#ifdef USE_MEMORY_POOL
	TMutex memory_pool_lock;
#endif
	static TMemoryManager* instance;
};


