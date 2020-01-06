#pragma once


#ifdef WIN32
#ifndef DLL_FUNC

#ifdef TAMENUT_NET_EXPORTS
#define DLL_FUNC  __declspec(dllexport)
#else
#define DLL_FUNC  __declspec(dllimport)
#endif

#endif
#else
#define DLL_FUNC  
#endif
