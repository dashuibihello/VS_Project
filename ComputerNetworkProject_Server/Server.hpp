#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")
#define BUFFER_SIZE 1024 
#define FILE_NAME_MAX_SIZE 512 

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
	SOCKADDR_IN Addr_server;
	int Addr_server_len;

	/* 包头 */
	typedef struct {
		int id;
		int buf_size;
	}PackInfo;

	/* 接收包 */
	struct SendPack {
		PackInfo head;
		char buf[BUFFER_SIZE];
	} data;
};