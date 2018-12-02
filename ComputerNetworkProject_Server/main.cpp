#include <string>
#include <iostream>
#include <io.h>
#include "Server.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	Server myServer;
	while (1) {
		myServer.work();
	}
	
	return 0;
}