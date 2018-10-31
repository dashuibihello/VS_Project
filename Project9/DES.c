#include "DES.h"

int IP_Table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

int IP_Inv_Table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};

void DES_E(char input[], char key_char[8], int output[][64], int length) {
	int count = 0;
	int output_count = 0;
	while (length >= 0) {
		char block[8];
		if (length / 8 > 0) {
			for (int i = 0; i < 8; i++) {
				block[i] = input[count];
				count++;
			}
		}
		else {
			for (int i = 0; i < length % 8; i++) {
				block[i] = input[count];
				count++;
			}
			for (int i = length % 8; i < 8; i++) {
				block[i] = 8 - length % 8 + 48;
			}
		}
		DES_E_Block(block, key_char, output[output_count]);
		output_count++;
		length -= 8;
	}
}

void DES_D(int input[][64], char key_char[8], char output[], int length) {
	char last_num;
	for (int i = 0; i < length; i++) {
		char block[8];
		DES_D_Block(input[i], key_char, block);
		if (i == length - 1) {
			last_num = block[7];
		}
		for (int j = 0; j < 8; j++) {
			output[i * 8 + j] = block[j];
		}
	}
	int flag = 0;
	for (int i = 0; i < last_num - 48; i++) {
		if (output[length * 8 - 1 - i] == last_num) {
			flag++;
		}
		else {
			break;
		}
	}
	if (flag == last_num - 48) {
		for (int i = 0; i < last_num - 48; i++) {
			output[length * 8 - 1 - i] = '\0';
		}
	}
}

void DES_E_Block(char input[8], char key_char[8], int output[64]) {
	int inputToBit[64] = { 0 };
	ByteToBit(input, inputToBit);

	int IP[64] = { 0 };
	IP_Or_Inv_IP(inputToBit, IP, IP_Table);

	int key[64] = { 0 };
	ByteToBit(key_char, key);

	int subKeys[16][48] = { 0 };
	GetSubKey(key, subKeys);

	int L[17][32] = { 0 }, R[17][32] = { 0 };
	for (int i = 0; i < 32; i++) {
		L[0][i] = IP[i];
		R[0][i] = IP[i + 32];
	}
	for (int i = 1; i < 17; i++) {
		for (int j = 0; j < 32; j++) {
			L[i][j] = R[i - 1][j];
		}
		Feistel(R[i - 1], R[i], subKeys[i - 1]);
		XOR(R[i], L[i - 1], 32);
	}
	int temp[64] = { 0 };
	for (int i = 0; i < 32; i++) {
		temp[i] = R[16][i];
		temp[i + 32] = L[16][i];
	}
	IP_Or_Inv_IP(temp, output, IP_Inv_Table);
}

void DES_D_Block(int input[64], char key_char[8], char output[8]) {
	int IP[64] = { 0 };
	IP_Or_Inv_IP(input, IP, IP_Table);

	int key[64] = { 0 };
	ByteToBit(key_char, key);
	int subKeys[16][48] = { 0 };
	GetSubKey(key, subKeys);

	int L[17][32] = { 0 }, R[17][32] = { 0 };
	for (int i = 0; i < 32; i++) {
		L[0][i] = IP[i];
		R[0][i] = IP[i + 32];
	}
	for (int i = 1; i < 17; i++) {
		for (int j = 0; j < 32; j++) {
			L[i][j] = R[i - 1][j];
		}
		Feistel(R[i - 1], R[i], subKeys[16 - i]);
		XOR(R[i], L[i - 1], 32);
	}
	int temp[64] = { 0 };
	for (int i = 0; i < 32; i++) {
		temp[i] = R[16][i];
		temp[i + 32] = L[16][i];
	}
	int output_temp[64] = { 0 };
	IP_Or_Inv_IP(temp, output_temp, IP_Inv_Table);
	memset(output, 0, 8);
	BitToByte(output_temp, output);
}

void ByteToBit(char input[], int output[64]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			output[8 * i + j] = (input[i] >> j) & 1;
		}
	}
}

void BitToByte(int input[64], char output[]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			output[i] |= (input[8 * i + j]) << j;
		}
	}
}

void IP_Or_Inv_IP(int input[64], int output[64], int table[64]) {
	for (int i = 0; i < 64; i++) {
		output[i] = input[table[i] - 1];
	}
}