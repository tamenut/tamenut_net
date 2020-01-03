#pragma once
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")

#if defined(DLL_EXPORT)
    #include <Winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <windows.h>
#endif
#include <io.h>

#else
#endif




enum LOG_LEVEL
{
	LOG_LEVEL_ALL=0,
	LOG_LEVEL_VERBOSE,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_NONE,
	LOG_LEVEL_MAX
};

enum LOG_DEF
{
	LOG_DEF_STR=0,
};

enum CONFIG_TYPE
{
	CONFIG_LOG_LEVEL =0,
	CONFIG_REFRESH_TIME,
	CONFIG_START_SEND_LOG,
	CONFIG_STOP_SEND_LOG,
	CONFIG_RESET_REMOTE_CONTROL,
};

#define DISPLAY_NOT_READ	-1		
#define DISPLAY_NONE	0		
#define DISPLAY_CONSOLE 1	// 1<<1
#define DISPLAY_FILE	2	// 1<<2

//#define NOT_USE_REDUNDANT -1
//#define MAX_LINE_LENGTH 1024
//#define MAX_CONF_MSG_LENGTH 128
//#define MAX_FILE_NAME_LENGTH 256


