#include "DES.h"

int main() {
	printf("Please input message which need to encrypt:\n");
	char input[1000];
	gets_s(input, 1000);

	printf("Please input the key(8 bytes):\n");
	char key_input[9];
	gets_s(key_input, 9);

	int length = 0;
	for (int i = 0; i < 1000; i++) {
		if (input[i] == '\0')
			break;
		length++;
	}
	int E_output[125][64] = { 0 };
	DES_E(input, key_input, E_output, length);

	
	char D_output[1000];
	DES_D(E_output, key_input, D_output, (length / 8) + 1);

	printf("Message after decryption:\n");
	for (int i = 0; i < 1000; i++) {
		if (D_output[i] == '\0') {
			break;
		}
		else {
			printf("%c", D_output[i]);
		}
	}
	printf("\n");
	
	getchar();
	return 0;
}