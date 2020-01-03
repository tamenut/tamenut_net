#pragma once

#include <string>
#include "CommonDllDefine.h"

#if defined(WIN32) && !defined(ITRON)
#include <process.h>
#include <winsock2.h>
//============= VxWorks ================
#elif defined(VXWORKS)
#include <vxWorks.h>
#include <taskLib.h>
#include <taskVarLib.h>
//============= LINUX ================
#else
#include <pthread.h>
#include <limits.h>
#include <errno.h>
#endif

#if defined(VXWORKS)

typedef enum {
	THREAD_PRIORITY_HIGH         = 68,//10,//1
	THREAD_PRIORITY_ABOVE_NORMAL = 71,//50,
	THREAD_PRIORITY_NORMAL       = 100,
	THREAD_PRIORITY_BELOW_NORMAL = 110,//150,
	THREAD_PRIORITY_LOW          = 120//200//255
} ThreadPriority;

#define THREAD_PRIORITY_DEFAULT THREAD_PRIORITY_NORMAL
#define THREAD_STACK_SIZE_DEFAULT (1024*1024)// (16*1024)

#elif defined(WIN32)

#define THREAD_PRIORITY_DEFAULT THREAD_PRIORITY_NORMAL
#define THREAD_STACK_SIZE_DEFAULT (0)
/*
#define THREAD_PRIORITY_LOWEST          THREAD_BASE_PRIORITY_MIN
#define THREAD_PRIORITY_BELOW_NORMAL    (THREAD_PRIORITY_LOWEST+1)
#define THREAD_PRIORITY_NORMAL          0
#define THREAD_PRIORITY_HIGHEST         THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL    (THREAD_PRIORITY_HIGHEST-1)
#define THREAD_PRIORITY_ERROR_RETURN    (MAXLONG)

#define THREAD_PRIORITY_TIME_CRITICAL   THREAD_BASE_PRIORITY_LOWRT
#define THREAD_PRIORITY_IDLE            THREAD_BASE_PRIORITY_IDLE
*/
#else	//LINUX
#define MINLONG     0x80000000  
#define MAXLONG     0x7fffffff  

#define THREAD_BASE_PRIORITY_LOWRT  15  // value that gets a thread to LowRealtime-1
#define THREAD_BASE_PRIORITY_MAX    2   // maximum thread base priority boost
#define THREAD_BASE_PRIORITY_MIN    (-2)  // minimum thread base priority boost
#define THREAD_BASE_PRIORITY_IDLE   (-15) // value that gets a thread to idle

#define THREAD_PRIORITY_LOWEST          THREAD_BASE_PRIORITY_MIN
#define THREAD_PRIORITY_BELOW_NORMAL    (THREAD_PRIORITY_LOWEST+1)
#define THREAD_PRIORITY_NORMAL          0
#define THREAD_PRIORITY_HIGHEST         THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL    (THREAD_PRIORITY_HIGHEST-1)
#define THREAD_PRIORITY_ERROR_RETURN    (MAXLONG)

#define THREAD_PRIORITY_TIME_CRITICAL   THREAD_BASE_PRIORITY_LOWRT
#define THREAD_PRIORITY_IDLE            THREAD_BASE_PRIORITY_IDLE


#define THREAD_PRIORITY_DEFAULT THREAD_PRIORITY_NORMAL
#define THREAD_STACK_SIZE_DEFAULT 1024*1024*1024

#endif/* setting of default thread property */

enum THREAD_CMD_STATUS
{
	THREAD_CMD_STATUS_STOP = 0,
	THREAD_CMD_STATUS_START,
	THREAD_CMD_STATUS_PAUSE,
};

class TCondition;
class TMutex;

class TThread
{

public:

	UTIL_DLLFunction TThread();
	UTIL_DLLFunction virtual ~TThread();

	UTIL_DLLFunction virtual bool start(const char* name, int priority=THREAD_PRIORITY_NORMAL, int stack_size = THREAD_STACK_SIZE_DEFAULT);
	UTIL_DLLFunction virtual void run() = 0;
	UTIL_DLLFunction virtual bool stop();
	
	UTIL_DLLFunction virtual void pause();
	UTIL_DLLFunction virtual void resume();

	UTIL_DLLFunction void set_priority(int priroity);
	UTIL_DLLFunction int get_priority();	//우선 순위 얻어오기

	UTIL_DLLFunction bool is_runnable();
	//UTIL_DLLFunction void set_runnable_flag(bool flag);

	UTIL_DLLFunction bool check_thread_status();
	UTIL_DLLFunction void set_cmd_status(THREAD_CMD_STATUS status);

	UTIL_DLLFunction unsigned int get_thread_id();
	UTIL_DLLFunction void set_thread_affinity(bool affinity);
private:

#if defined(WIN32)
	HANDLE	_hThread;
	//HWND hThread;
	DWORD	_thread_id;
	//============= VxWorks ================
#elif defined(VXWORKS)
	int _task_id;		/* native thread handle */
	//============= VxWorks ================
#else
	pthread_t _thread_id;
#endif
	THREAD_CMD_STATUS _thread_cmd_status;
	char _thread_name[256];//std::string _thread_name;
	unsigned int _stack_size;
	TCondition *_condition;
	TMutex *_mutex;
	bool _is_affinity;
public:
	const char* get_class_name();
	const char* get_thread_name();

};



