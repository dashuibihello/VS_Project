#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

class Client {
public:
	Client(char* operation, char* server, char* file);
	~Client();
	int sendToServer();
	int getFromServer();

private:
	char* myFileAddress;
	char* myServerAddress;
	char* myAction;
	SOCKET client;
};