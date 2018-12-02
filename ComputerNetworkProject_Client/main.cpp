#include <string>
#include <iostream>
#include <io.h>
#include "Client.hpp"

using namespace std;

int checkArg(int argc, char *argv[]) {
	if (argc != 4) {
		cout << argc << endl;
		fprintf(stderr, "usage:main<lsend/lget><IP/URL><File address>\n");
		return -1;
	}
	if ((string)argv[1] != "lsend" && (string)argv[1] != "lget") {
		cout << argv[1] << endl;
		fprintf(stderr, "usage:main<lsend/lget><IP/URL><File address>\n");
		return -1;
	}
	return 0;
}

int checkFile(char *address) {
	if (_access(address, 0) == -1) {
		fprintf(stderr, "The file doesn't exist\n");
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if (checkArg(argc, argv) == -1) {
		return -1;
	}
	Client myClient(argv[1], argv[2], argv[3]);
	if ((string)argv[1] == "lsend") {
		myClient.sendToServer();
	}
	else {
		myClient.getFromServer();
	}

	return 0;
}