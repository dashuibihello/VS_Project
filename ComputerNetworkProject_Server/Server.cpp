#include "Server.hpp"

Server::Server() {
	//加载winsock库 
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) != 0)
	{
		return;
	}
	//创建套接字
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//创建本机地址 
	SOCKADDR_IN Addr_server;
	Addr_server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Addr_server.sin_family = AF_INET;
	Addr_server.sin_port = htons(6000);
	//绑定地址到套接字 
	bind(server, (SOCKADDR*)&Addr_server, sizeof(Addr_server));
	cout << "server is on" << endl;
}

Server::~Server() {
	//释放套接字资源 
	closesocket(server);
	//清除winsock库 
	WSACleanup();
}

void Server::work() {
	SOCKADDR_IN addr_client;
	int len = sizeof(addr_client);
	char recvBuf[100];
	int count = 0;
	int size = recvfrom(server, recvBuf, 99, 0, (SOCKADDR*)&addr_client, &len);
	string temp = recvBuf;
	cout << temp.length() << endl;
	if (temp == "lsend") {
		uploadByClient();
	}
	else if (temp == "lget") {
		downloadByClient();
	}
}

int Server::uploadByClient() {
	cout << "upload" << endl;
	return 0;
}

int Server::downloadByClient() {
	cout << "download" << endl;
	return 0;
}