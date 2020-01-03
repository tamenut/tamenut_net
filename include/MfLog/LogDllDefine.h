#ifdef WIN32
#ifndef LoggerDLLFunction

#ifdef MULTIFUNCTIONLOGGER_EXPORTS
#define LoggerDLLFunction  __declspec(dllexport)
#else
#define LoggerDLLFunction  __declspec(dllimport)
#endif

#endif
#else
#define LoggerDLLFunction  
#endif