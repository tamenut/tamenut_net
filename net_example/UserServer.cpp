#include "stdafx.h"
#include "UserServer.h"
#include "LogModule.h"

UserServer::UserServer(unsigned short bind_port)
	:TameServer(bind_port)
{
	start();
}

UserServer::~UserServer()
{
}

void UserServer::on_connect(unsigned int cid)
{
	INFO_LOG("- connect client (id:%u)\n", cid);
}

void UserServer::on_disconnect(unsigned int cid, const TCHAR * pReason)
{

}

void UserServer::on_message(unsigned int cid, const TAMENUT::SerializedPkt * Packet)
{
}


