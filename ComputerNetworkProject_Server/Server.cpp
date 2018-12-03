#include "Server.hpp"

Server::Server() {
	//����winsock�� 
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) != 0) {
		return;
	}
	//�����׽���
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//����������ַ 
	Addr_server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Addr_server.sin_family = AF_INET;
	Addr_server.sin_port = htons(6000);
	//�󶨵�ַ���׽��� 
	bind(server, (SOCKADDR*)&Addr_server, sizeof(Addr_server));
	Addr_server_len = sizeof(Addr_server);
	cout << "server is on" << endl;
}

Server::~Server() {
	//�ͷ��׽�����Դ 
	closesocket(server);
	//���winsock�� 
	WSACleanup();
}

void Server::work() {
	char recvBuf[100];
	int count = 0;
	int size = recvfrom(server, recvBuf, 99, 0, (SOCKADDR*)&Addr_server, &Addr_server_len);
	string temp = recvBuf;
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
	/* ����id */
	int send_id = 0;

	/* ����id */
	int receive_id = 0;
	
	/* ����һ����ַ�����ڲ���ͻ��˵�ַ */
	SOCKADDR_IN client_addr;
	int client_addr_length = sizeof(client_addr);

	/* �������� */
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	if (recvfrom(server, buffer, BUFFER_SIZE, 0, (SOCKADDR*)&client_addr, &client_addr_length) == -1) {
		perror("Receive Data Failed:");
		exit(1);
	}

	/* ��buffer�п�����file_name */
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));

	/* ���ļ� */
	ifstream fp(file_name, ios::in);
	if (!fp.is_open()) {
		printf("File:%s Not Found.\n", file_name);
	}
	else {
		printf("File:%s \n", file_name);
		/* ÿ��ȡһ�����ݣ��㽫�䷢���ͻ��� */
		while (1) {
			PackInfo pack_info;
			if (receive_id == send_id) {
				++send_id;

				if (!fp.eof()) {	
					fp.getline(data.buf, BUFFER_SIZE);
					data.head.id = send_id; /* ����id�Ž���ͷ,���ڱ��˳�� */
					data.head.buf_size = sizeof(data); /* ��¼���ݳ��� */
					if (sendto(server, (char*)&data, sizeof(data), 0, (SOCKADDR*)&client_addr, client_addr_length) < 0) {
						perror("Send File Failed:");
						break;
					}
					/* ����ȷ����Ϣ */
					recvfrom(server, (char*)&pack_info, sizeof(pack_info), 0, (SOCKADDR*)&client_addr, &client_addr_length);
					receive_id = pack_info.id;
				}
				else {
					sendto(server, "", 0, 0, (SOCKADDR*)&client_addr, client_addr_length);
					break;
				}
			}
			else {
				/* ������յ�id�ͷ��͵�id����ͬ,���·��� */
				if (sendto(server, (char*)&data, sizeof(data), 0, (struct sockaddr*)&client_addr, client_addr_length) < 0) {
					perror("Send File Failed:");
					break;
				}
				/* ����ȷ����Ϣ */
				recvfrom(server, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&client_addr, &client_addr_length);
				receive_id = pack_info.id;
			}
		}
		/* �ر��ļ� */
		fp.close();
		printf("File:%s Transfer Successful!\n", file_name);
	}
	return 0;
}