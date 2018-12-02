#include "Client.hpp"

Client::Client(char* action, char* serverAddress, char* fileAddress) {
	myServerAddress = serverAddress;
	myFileAddress = fileAddress;
	myAction = action;
	//加载winsock库 
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) != 0)
	{
		return;
	}
	//创建套接字
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//创建地址 
	cout << "client is on" << endl;
}

Client ::~Client() {
	//释放套接字资源 
	closesocket(client);
	//清除winsock库 
	WSACleanup();
}

int Client::sendToServer() {
	SOCKADDR_IN Addr_des;
	Addr_des.sin_addr.S_un.S_addr = inet_addr(myServerAddress);
	Addr_des.sin_family = AF_INET;
	Addr_des.sin_port = htons(6000);
	//发送数据
	sendto(client, myAction, strlen(myAction) + 1, 0, (SOCKADDR*)&Addr_des, sizeof(Addr_des));

	return 0;
}

int Client::getFromServer() {
	SOCKADDR_IN Addr_des;
	Addr_des.sin_addr.S_un.S_addr = inet_addr(myServerAddress);
	Addr_des.sin_family = AF_INET;
	Addr_des.sin_port = htons(6000);
	//发送数据
	sendto(client, myAction, strlen(myAction) + 1, 0, (SOCKADDR*)&Addr_des, sizeof(Addr_des));
	return 0;
}