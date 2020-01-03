#pragma once

#include "CommonDllDefine.h"

#if defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
//============= VxWorks ================
#elif defined(VXWORKS)
#include <sys/fcntlcom.h>
#include <semaphore.h>
#include <errno.h> 
#define SEM_FAILED ((sem_t *)0)
//============= VxWorks ================
#else
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <errno.h> 
#endif

#define SEM_NAME_SIZE 64


/*
NamedMutex Ŭ������ ��Ƽ������ÿ� ����ȭ ó���� �ϴ� Ŭ�����̴�.
*/
class NamedMutex
{
#if defined(WIN32)
	HANDLE _mutex_id;
	HANDLE _cond_id;

#else
	sem_t* _mutex_id;
	sem_t* _cond_id;
#endif

public:
#if defined(WIN32)
	UTIL_DLLFunction NamedMutex(char* name, bool is_multireader = false);
#else
	UTIL_DLLFunction NamedMutex(const char* name, bool _is_multireader = false);
#endif
	UTIL_DLLFunction NamedMutex(int index = 0, bool is_multireader = false);
	UTIL_DLLFunction ~NamedMutex();
	//���� �Ŵ� �Լ�
	UTIL_DLLFunction bool lock(int timeout = -1);

	//���� Ǫ�� �Լ�
	UTIL_DLLFunction bool unlock();

	//signal�� �ֱ������� ����ϴ� �Լ�
	UTIL_DLLFunction bool wait(int timeout = -1);

	//����ϰ� �ִ� �Լ��� �ñ׳��� �ִ� �Լ�
	UTIL_DLLFunction bool signal();

	//bool waitsignal();
private:
	void init(const char *name, bool is_multireader);

private:
	//bool bManualReset;
	char _sem_name[SEM_NAME_SIZE];
	bool _is_multireader;
};

