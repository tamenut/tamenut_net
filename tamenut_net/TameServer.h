#pragma once
#include "define_dll.h"
#include "TameServerImpl.h"

class TameServer : public TAMENUT::TameServerImpl
{
public:
	DLL_FUNC TameServer(unsigned short bind_port);
	DLL_FUNC ~TameServer();
};

