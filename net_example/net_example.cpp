// net_example.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include "TameServer.h"
#include "TameClient.h"
#include "TUtil.h"

int main()
{
	int inp;
	TameServer server(4567);
	std::cin >> inp;
	//TameClient client("127.0.0.1", 3333);
	TameClient client("127.0.0.1", 4567);
	while (true) {
		std::cin >> inp;
	}
    return 0;
}

