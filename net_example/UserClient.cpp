#include "stdafx.h"
#include "UserClient.h"
#include "LogModule.h"

UserClient::UserClient(const char *dst_ip_str, unsigned short bind_port)
	:TameClient(dst_ip_str, bind_port)
{
	start();
}


UserClient::~UserClient()
{
}

void UserClient::on_connect()
{
	INFO_LOG("- connect server\n");
}

void UserClient::on_disconnect(const TCHAR * pReason)
{

}

void UserClient::on_message(TAMENUT::SerializedPkt  * pkt)
{
	switch (pkt->get_pkt_id())
	{
	case 1:
	{
		ClientSendPkt *msg = (ClientSendPkt*)pkt;
		INFO_LOG("1. client pkt - val:%d, (%s)\n", msg->_val, __FUNCTION__);
		break;
	}
	case 2:
	{
		ServerSendPkt *msg = (ServerSendPkt*)pkt;
		INFO_LOG("1. server pkt - val:%d, (%s)\n", msg->_val, __FUNCTION__);
		break;
	}
	}
}
