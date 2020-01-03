#pragma once

#include "CommonDllDefine.h"
#if defined(WIN32)
//#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
//============= VxWorks ================
#elif defined(VXWORKS)
  #include <semLib.h>
  #include <semaphore.h>
  #include <sysLib.h>
  #include <stdlib.h>
//============= VxWorks ================

#else
#include <pthread.h>
#include <semaphore.h>
#include <errno.h> 
#endif


class TMutex
{
public:
	UTIL_DLLFunction TMutex();
	UTIL_DLLFunction ~TMutex();
	//락을 걸어주는 함수
	UTIL_DLLFunction void lock();
	//락을 풀어주는 함수
	UTIL_DLLFunction void unlock();


#if defined(WIN32)
	//HANDLE	mutex_id;
	CRITICAL_SECTION _mutex_id;
#elif defined(VXWORKS)
	SEM_ID	_mutex_id;
#else
	pthread_mutex_t _mutex_id;
#endif

private:
	
};


