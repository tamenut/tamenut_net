// net_example.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include "UserServer.h"
#include "UserClient.h"
#include "TUtil.h"

int main()
{
	int inp;
	UserServer server(4567);
	std::cin >> inp;
	UserClient client("127.0.0.1", 4567);
	ClientSendPkt msg;
	
	while (true) {
		std::cin >> msg._val;
		client.send(&msg);
	}
    return 0;
}

