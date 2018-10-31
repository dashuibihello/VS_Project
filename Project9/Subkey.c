#include "Subkey.h"

//PC_1置换表
int PC_1_Table[56] = {
	57, 49, 41, 33, 25, 17,  9,
	1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

//PC_2置换表
int PC_2_Table[48] = {
	14, 17, 11, 24,  1,  5,
	3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

void GetSubKey(int key[64], int subKey[16][48]) {
	int out_PC_1[56] = { 0 };
	PC_1(key, out_PC_1, PC_1_Table);

	int C[17][28] = { 0 }, D[17][28] = { 0 };
	int in_PC_2[16][56] = { 0 };

	//得到C0D0
	for (int i = 0; i < 28; i++) {
		C[0][i] = out_PC_1[i];
		D[0][i] = out_PC_1[i + 28];
	}

	//计算CiDi，循环左移，并进行PC_2压缩置换
	for (int i = 1; i < 17; i++) {
		if (i == 1 || i == 2 || i == 9 || i == 16) {
			Rotating_L(C[i - 1], C[i], 1);
			Rotating_L(D[i - 1], D[i], 1);
		}
		else {
			Rotating_L(C[i - 1], C[i], 2);
			Rotating_L(D[i - 1], D[i], 2);
		}

		for (int j = 0; j < 28; j++) {
			in_PC_2[i - 1][j] = C[i][j];
			in_PC_2[i - 1][j + 28] = D[i][j];
		}
		PC_2(in_PC_2[i - 1], subKey[i - 1], PC_2_Table);
	}

}

void PC_1(int input[64], int output[56], int table[56]) {
	for (int i = 0; i < 56; i++) {
		output[i] = input[table[i] - 1];
	}
}

void PC_2(int input[56], int output[48], int table[48]) {
	for (int i = 0; i < 48; i++) {
		output[i] = input[table[i] - 1];
	}
}

void Rotating_L(int input[28], int output[28], int count) {
	for (int i = 0; i < 28; i++) {
		output[i] = input[(i + count) % 28];
	}
}