#include "Client.hpp"

const char *testAddress = "C:\\Users\\DELL\\Desktop\\test1.txt";

Client::Client(char* action, char* serverAddress, char* fileAddress) {
	myServerAddress = serverAddress;
	myFileAddress = fileAddress;
	myAction = action;
	//����winsock�� 
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) != 0) {
		return;
	}
	//�����׽���
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//������ַ 
	Addr_des.sin_addr.S_un.S_addr = inet_addr(myServerAddress);
	Addr_des.sin_family = AF_INET;
	Addr_des.sin_port = htons(6000);
	Addr_des_len = sizeof(Addr_des);

	cout << "client is on" << endl;
}

Client ::~Client() {
	//�ͷ��׽�����Դ 
	closesocket(client);
	//���winsock�� 
	WSACleanup();
}

int Client::sendToServer() {
	//��������
	sendto(client, myAction, strlen(myAction) + 1, 0, (SOCKADDR*)&Addr_des, sizeof(Addr_des));





	return 0;
}

int Client::getFromServer() {
	//��������
	sendto(client, myAction, strlen(myAction) + 1, 0, (SOCKADDR*)&Addr_des, Addr_des_len);

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy(buffer, myFileAddress, strlen(myFileAddress) > BUFFER_SIZE ? BUFFER_SIZE : strlen(myFileAddress));

	/* �����ļ��� */
	if (sendto(client, buffer, BUFFER_SIZE, 0, (SOCKADDR*)&Addr_des, Addr_des_len) < 0) {
		perror("Send File Name Failed:");
		exit(1);
	}

	/* ���ļ���׼��д�� */
	//FILE *fp = fopen(testAddress, "w");
	ofstream fp(testAddress, ios::out);
	if (!fp.is_open()) {
		printf("File:\t%s Can Not Open To Write\n", testAddress);
		exit(1);
	}

	/* �ӷ������������ݣ���д���ļ� */
	int len = 0;
	int id = 1;
	while (1) {
		PackInfo pack_info;
		
		if ((len = recvfrom(client, (char*)&data, sizeof(data), 0, (SOCKADDR*)&Addr_des, &Addr_des_len)) > 0) {
			cout << len << endl;
			if (data.head.id == id) {
				pack_info.id = data.head.id;
				cout << pack_info.id << endl;
				pack_info.buf_size = data.head.buf_size;
				++id;
				/* �������ݰ�ȷ����Ϣ */
				if (sendto(client, (char*)&pack_info, sizeof(pack_info), 0, (SOCKADDR*)&Addr_des, Addr_des_len) < 0) {
					printf("Send confirm information failed!");
				}
				/* д���ļ� */
				if (id != 2)
					fp << endl;
				fp << data.buf;
			}

			/* ������ط��İ� */
			else if (data.head.id < id) {
				pack_info.id = data.head.id;
				pack_info.buf_size = data.head.buf_size;
				/* �ط����ݰ�ȷ����Ϣ */
				if (sendto(client, (char*)&pack_info, sizeof(pack_info), 0, (SOCKADDR*)&Addr_des, Addr_des_len) < 0) {
					printf("Send confirm information failed!");
				}
			}
		}
		else {
			break;
		}
	}
	printf("Receive File:\t%s From Server IP Successful!\n", testAddress);
	fp.close();

	return 0;
}