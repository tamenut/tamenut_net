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
Cond Ŭ������ ��Ƽ������ÿ� ����ȭ ó���� �ϴ� Ŭ�����̴�.
*/
//class SDDS_DLL_EXPORT Cond
class TCond
{
public:
	UTIL_DLLFunction TCond(bool manualReset = false, bool initState = false);
	UTIL_DLLFunction ~TCond();
	//���� �Ŵ� �Լ�
	//bool lock();

	//���� Ǫ�� �Լ�
	//bool unlock();

	//signal�� �ֱ������� ����ϴ� �Լ�
	UTIL_DLLFunction bool wait();

	//�����ð� ��ŭ ����ϴ� �Լ�
	UTIL_DLLFunction bool wait(unsigned int timeout);

	//����ϰ� �ִ� �Լ��� �ñ׳��� �ִ� �Լ�
	//��ü �ñ׳��� ������ ���� ����
	UTIL_DLLFunction bool signal();

	//�������¸���ΰ�� �ٽ� ��ݸ��� ����
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


