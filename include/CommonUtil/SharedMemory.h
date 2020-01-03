#pragma once

#include "TMutex.h"
#include <vector>

#define SHM_NAME_SIZE 64

struct SHM_MEMORY_INFO// : public NamedMutex
{
#if defined(WIN32)
	HANDLE hShmId;
	char* pShmMap;
#else
	int hShmId;
	char* pShmMap;
#endif
};


#define GET_SHM SharedMemory::Instance()->get_shared_memory

class SharedMemory// : public NamedMutex
{
public:
	UTIL_DLLFunction SharedMemory();
	UTIL_DLLFunction ~SharedMemory();
	UTIL_DLLFunction SHM_MEMORY_INFO *get_shared_memory(const char *memory_name, int memory_size);

	//½Ì±ÛÅæ ±¸Çö À§ÇÑ ÇÔ¼ö
	UTIL_DLLFunction static SharedMemory* Instance();
private:
	//½Ì±ÛÅæ ±¸Çö À§ÇÑ º¯¼ö
	int get_count_openfile(const char* name);
	static SharedMemory* instance;

};

#if defined(LINUX)
int get_count_openfile(const char* name);
#endif


