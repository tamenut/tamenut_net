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

void UserServer::on_connect(unsigned int client_id)
{
	INFO_LOG("- connect client (id:%u)\n", client_id);
}

void UserServer::on_disconnect(unsigned int client_id, const TCHAR * pReason)
{
	TAMENUT::SerializedPkt pkt(1);
	pkt.get_pkt_id();
}

void UserServer::on_message(unsigned int client_id, TAMENUT::SerializedPkt * pkt)
{
	switch (pkt->get_pkt_id())
	{
	case 1:
	{
		ClientSendPkt *msg = (ClientSendPkt*)pkt;
		//INFO_LOG("2. client pkt - val:%d, str:%s, (%s)\n", msg->_val, msg->_str, __FUNCTION__);
		INFO_LOG("2. client pkt - str:%s, (%s)\n", msg->_str, __FUNCTION__);
		break;
	}
	case 2:
	{
		ServerSendPkt *msg = (ServerSendPkt*)pkt;
		INFO_LOG("2. server pkt - val:%d, (%s)\n", msg->_val, __FUNCTION__);
		break;
	}
	}
	
	
}


