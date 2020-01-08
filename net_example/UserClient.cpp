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

void UserClient::on_message(const TAMENUT::SerializedPkt  * pkt)
{
//	INFO_LOG("", pkt._)
}
