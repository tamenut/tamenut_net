#pragma once

#include "LogDllDefine.h"
#include "LogCommon.h"

#ifndef  NOT_USE_LOG_MODULE
//#ifdef  NOT_USE_LOG_MODULE
#define PRINT_LOG LogModule::get_instance()->print_log
#define PRINT_LOG_DEF LogModule::get_instance()->print_log_def
#define ERROR_LOG LogModule::get_instance()->print_error_log
#define WARNING_LOG LogModule::get_instance()->print_warning_log
#define INFO_LOG LogModule::get_instance()->print_info_log
#define VERBOSE_LOG LogModule::get_instance()->print_verbose_log

#else
#define PRINT_LOG(log_level, str,...) printf(str, __VA_ARGS__)
#endif

class LogModuleImpl;
class  LogModule
{
public:
	LoggerDLLFunction LogModule();
	LoggerDLLFunction virtual ~LogModule(void);
	LoggerDLLFunction static LogModule* get_instance();
	LoggerDLLFunction virtual void print_log(LOG_LEVEL log_level, const char *str,...);
	LoggerDLLFunction virtual void print_log_def(LOG_LEVEL log_level, unsigned long long log_def, const char *str, ...);
	LoggerDLLFunction void print_error_log(const char *str, ...);
	LoggerDLLFunction void print_warning_log(const char * str, ...);
	LoggerDLLFunction void print_info_log(const char *str, ...);
	LoggerDLLFunction void print_verbose_log(const char *str, ...);
protected :
	static LogModule* instance;
};



