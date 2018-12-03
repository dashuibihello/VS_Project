#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")

#define BUFFER_SIZE 1024 

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
	SOCKADDR_IN Addr_des;
	int Addr_des_len;


	/* 包头 */
	typedef struct {
		int id;
		int buf_size;
	}PackInfo;

	/* 接收包 */
	struct RecvPack {
		PackInfo head;
		char buf[BUFFER_SIZE];
	} data;

};