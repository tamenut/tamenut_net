#pragma once
#if defined _WIN32
#include <windows.h>
#else

#endif
#include <time.h>
#include "CommonDllDefine.h"

#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW 4
#endif

class HighResolutionTime
{
public:
	UTIL_DLLFunction HighResolutionTime(void);
	UTIL_DLLFunction ~HighResolutionTime(void);
	UTIL_DLLFunction bool start(void);

	UTIL_DLLFunction long long get_elapsed_time(int unit_resolution);
	UTIL_DLLFunction inline long long get_elapsed_sec(void) 
	{ return get_elapsed_time(1); }
	UTIL_DLLFunction inline long long get_elapsed_milli_sec(void) 
	{ return get_elapsed_time(1000); }
	UTIL_DLLFunction inline long long get_elapsed_micro_sec(void) 
	{ return get_elapsed_time(1000000); }
	UTIL_DLLFunction inline long long get_elapsed_nano_sec(void) 
	{ return get_elapsed_time(1000000000); }

	UTIL_DLLFunction static double get_day_time();

	UTIL_DLLFunction static long long get_current(int unit_resolution);
	UTIL_DLLFunction static inline long long get_current_sec(void) 
	{ return get_current(1); }
	UTIL_DLLFunction static inline long long get_current_milli_sec(void) 
	{ return get_current(1000); }
	UTIL_DLLFunction static inline long long get_current_micro_sec(void) 
	{ return get_current(1000000); }
	UTIL_DLLFunction static inline long long get_current_nano_sec(void) 
	{ return get_current(1000000000); }

	UTIL_DLLFunction static struct timespec get_current_s(int unit_resolution);


	UTIL_DLLFunction static long long get_current_system_time(int unit_resolution);
	UTIL_DLLFunction static inline long long get_current_system_time_sec(void) 
	{ return get_current_system_time(1); }
	UTIL_DLLFunction static inline long long get_current_system_time_milli_sec(void)
	{ return get_current_system_time(1000); }
	UTIL_DLLFunction static inline long long get_current_system_time_micro_sec(void)
	{ return get_current_system_time(1000000); }
	UTIL_DLLFunction static inline long long get_current_system_time_nano_sec(void)
	{ return get_current_system_time(1000000000); }
protected:
#if 1
	long long _start_time;
	long long _end_time;
#else
#if defined _WIN32
	LARGE_INTEGER _frequency;
	LARGE_INTEGER _start_time;
#else
	struct timespec _start_time;
#endif
#endif

};
