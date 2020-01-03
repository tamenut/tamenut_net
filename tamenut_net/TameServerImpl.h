#pragma once
#include "common_def.h"
#include "TNetData.h"

class TameServerImpl
{
public:
	TameServerImpl();
	~TameServerImpl();
	void on_connect(ClientId netId);
	void on_disconnect(ClientId netId, const TCHAR * pReason);
	void on_message(ClientId netId, const TNetData & Packet);
};

