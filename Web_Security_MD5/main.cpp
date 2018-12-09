#include "MD5.hpp"

int main() {
	printf("Please input message which need to encrypt:\n");
	char input[10000];
	gets_s(input, 10000);
	unsigned char *answer = MD5(input);
	showAnswer(answer);
	getchar();
	delete[]answer;
	return 0;
}