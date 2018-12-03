#include "Server.hpp"

Server::Server() {
	//加载winsock库 
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData) != 0) {
		return;
	}
	//创建套接字
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//创建本机地址 
	Addr_server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Addr_server.sin_family = AF_INET;
	Addr_server.sin_port = htons(6000);
	//绑定地址到套接字 
	bind(server, (SOCKADDR*)&Addr_server, sizeof(Addr_server));
	Addr_server_len = sizeof(Addr_server);
	cout << "server is on" << endl;
}

Server::~Server() {
	//释放套接字资源 
	closesocket(server);
	//清除winsock库 
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
	/* 发送id */
	int send_id = 0;

	/* 接收id */
	int receive_id = 0;
	
	/* 定义一个地址，用于捕获客户端地址 */
	SOCKADDR_IN client_addr;
	int client_addr_length = sizeof(client_addr);

	/* 接收数据 */
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	if (recvfrom(server, buffer, BUFFER_SIZE, 0, (SOCKADDR*)&client_addr, &client_addr_length) == -1) {
		perror("Receive Data Failed:");
		exit(1);
	}

	/* 从buffer中拷贝出file_name */
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));

	/* 打开文件 */
	ifstream fp(file_name, ios::in);
	if (!fp.is_open()) {
		printf("File:%s Not Found.\n", file_name);
	}
	else {
		printf("File:%s \n", file_name);
		/* 每读取一段数据，便将其发给客户端 */
		while (1) {
			PackInfo pack_info;
			if (receive_id == send_id) {
				++send_id;

				if (!fp.eof()) {	
					fp.getline(data.buf, BUFFER_SIZE);
					data.head.id = send_id; /* 发送id放进包头,用于标记顺序 */
					data.head.buf_size = sizeof(data); /* 记录数据长度 */
					if (sendto(server, (char*)&data, sizeof(data), 0, (SOCKADDR*)&client_addr, client_addr_length) < 0) {
						perror("Send File Failed:");
						break;
					}
					/* 接收确认消息 */
					recvfrom(server, (char*)&pack_info, sizeof(pack_info), 0, (SOCKADDR*)&client_addr, &client_addr_length);
					receive_id = pack_info.id;
				}
				else {
					sendto(server, "", 0, 0, (SOCKADDR*)&client_addr, client_addr_length);
					break;
				}
			}
			else {
				/* 如果接收的id和发送的id不相同,重新发送 */
				if (sendto(server, (char*)&data, sizeof(data), 0, (struct sockaddr*)&client_addr, client_addr_length) < 0) {
					perror("Send File Failed:");
					break;
				}
				/* 接收确认消息 */
				recvfrom(server, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&client_addr, &client_addr_length);
				receive_id = pack_info.id;
			}
		}
		/* 关闭文件 */
		fp.close();
		printf("File:%s Transfer Successful!\n", file_name);
	}
	return 0;
}