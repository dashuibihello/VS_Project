#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

class Server {
public:
	Server();
	~Server();
	void work();
	int uploadByClient();
	int downloadByClient();

private:
	SOCKET server;
};