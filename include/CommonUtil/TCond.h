#pragma once

#include "CommonDllDefine.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#endif

/*
Cond 클래스는 멀티스레드시에 동기화 처리를 하는 클래스이다.
*/
//class SDDS_DLL_EXPORT Cond
class TCond
{
public:
	UTIL_DLLFunction TCond(bool manualReset = false, bool initState = false);
	UTIL_DLLFunction ~TCond();
	//락을 거는 함수
	//bool lock();

	//락을 푸는 함수
	//bool unlock();

	//signal을 주기전까지 대기하는 함수
	UTIL_DLLFunction bool wait();

	//일정시간 만큼 대기하는 함수
	UTIL_DLLFunction bool wait(unsigned int timeout);

	//대기하고 있는 함수에 시그널을 주는 함수
	//전체 시그널을 깨을지 여부 결정
	UTIL_DLLFunction bool signal();

	//수동리셋모드인경우 다시 잠금모드로 변경
	UTIL_DLLFunction bool waitsignal();//reset()
#ifndef WIN32
	static bool timed_wait(sem_t* sem, const int timeout);
#endif
private:
#if defined(WIN32)
	HANDLE _cond_id;
	//============= VxWorks ================
#elif defined(VXWORKS)
	//SEM_ID	mutex_id;
	SEM_ID	_cond_id;
	SEM_ID  _tm_cond_id;
	//============= VxWorks ================
#else
	sem_t _cond_id;
	sem_t _tm_cond_id;
#endif


};


