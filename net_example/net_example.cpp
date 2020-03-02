// net_example.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include "UserServer.h"
#include "UserClient.h"
#include "TUtil.h"
#include "../tamenut_net/ProcPktThread.h"
#include <string>
int main()
{
	int inp;
	std::string in_str;
	ClientSendPkt msg;
	UserServer server(4567);
	UserClient client1("127.0.0.1", 4567);
	UserClient client2("127.0.0.1", 4567);
	std::cin >> inp;
	
	//type_info a = typeid(int);
	while (true) {
		//std::cin >> msg._val;
		std::cin >> in_str;
		strcpy_s(msg._str, in_str.c_str());
		
		client2.send(&msg, sizeof(msg));
	}
	

    return 0;
}

