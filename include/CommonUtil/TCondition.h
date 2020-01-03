#pragma once
#include "CommonDllDefine.h"
#include "time.h"

#if defined(WIN32)
//#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
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

class TMutex;

#ifdef WIN32
typedef CRITICAL_SECTION pthread_mutex_t;
#if 1
typedef CONDITION_VARIABLE pthread_cond_t;
typedef int pthread_condattr_t;
#else


typedef struct
{
	int waiters_count_;
	// Count of the number of waiters.

	CRITICAL_SECTION waiters_count_lock_;
	// Serialize access to <waiters_count_>.

	int release_count_;
	// Number of threads to release via a <pthread_cond_broadcast> or a
	// <pthread_cond_signal>. 

	int wait_generation_count_;
	// Keeps track of the current "generation" so that we don't allow
	// one thread to steal all the "releases" from the broadcast.

	HANDLE event_;
	// A manual-reset event that's used to block and release waiting
	// threads. 

} pthread_cond_t;

//typedef struct{
//	unsigned long tv_sec;
//	unsigned long tv_nsec;
//}timespec ;
#endif
#endif

class TCondition
{
public:
	UTIL_DLLFunction TCondition(void);
	UTIL_DLLFunction ~TCondition(void);
	UTIL_DLLFunction void wait(TMutex *mutex, int mili_sec = -1);
	UTIL_DLLFunction void broadcast();
	UTIL_DLLFunction void signal();
private:
	pthread_cond_t _cond;

};
