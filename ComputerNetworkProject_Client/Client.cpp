#include "Client.hpp"

Client::Client(char* action, char* serverAddress, char* fileAddress) {
	myServerAddress = serverAddress;
	myFileAddress = fileAddress;
	myAction = action;
	//����winsock�� 
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) != 0)
	{
		return;
	}
	//�����׽���
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//������ַ 
	cout << "client is on" << endl;
}

Client ::~Client() {
	//�ͷ��׽�����Դ 
	closesocket(client);
	//���winsock�� 
	WSACleanup();
}

int Client::sendToServer() {
	SOCKADDR_IN Addr_des;
	Addr_des.sin_addr.S_un.S_addr = inet_addr(myServerAddress);
	Addr_des.sin_family = AF_INET;
	Addr_des.sin_port = htons(6000);
	//��������
	sendto(client, myAction, strlen(myAction) + 1, 0, (SOCKADDR*)&Addr_des, sizeof(Addr_des));

	return 0;
}

int Client::getFromServer() {
	SOCKADDR_IN Addr_des;
	Addr_des.sin_addr.S_un.S_addr = inet_addr(myServerAddress);
	Addr_des.sin_family = AF_INET;
	Addr_des.sin_port = htons(6000);
	//��������
	sendto(client, myAction, strlen(myAction) + 1, 0, (SOCKADDR*)&Addr_des, sizeof(Addr_des));
	return 0;
}