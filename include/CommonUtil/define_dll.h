#ifdef WIN32
#ifndef UTIL_DLLFunction

#ifdef COMMON_UTIL_EXPORTS
#define UTIL_DLLFunction  __declspec(dllexport)
#else
#define UTIL_DLLFunction  __declspec(dllimport)
#endif

#endif
#else
#define UTIL_DLLFunction  
#endif