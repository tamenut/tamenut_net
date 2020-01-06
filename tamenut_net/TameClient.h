#pragma once
#include "define_dll.h"
#include "TameClientImpl.h"

class TameClient : public TAMENUT::TameClientImpl
{
public:
	DLL_FUNC TameClient(const char *dst_ip_str, unsigned short bind_port);
	DLL_FUNC ~TameClient();
};

