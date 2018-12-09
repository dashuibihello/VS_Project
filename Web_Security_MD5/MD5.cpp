#include "MD5.hpp"

//各轮循环中的X表
int x[4][16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	1, 6,11, 0, 5,10,15, 4, 9,14, 3, 8,13, 2, 7,12,
	5, 8,11,14, 1, 4, 7,10,13, 0, 3, 6, 9,12,15, 2,
	0, 7,14, 5,12, 3,10, 1, 8,15, 6,13, 4,11, 2, 9
};

//各次迭代运算采用的T值
unsigned int T[4][16] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

//各次迭代运算采用的左循环移位的s值
int S[4][16] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

unsigned char* MD5(const char* str) {
	int length = 0;
	//填充
	unsigned char *newStr = padding(str, length);

	//分块处理
	int L = length / 64;
	unsigned int buffer_MD[4] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };
	for (int i = 0; i < L; i++) {
		unsigned int M[16] = { 0 };
		//转换为16*32的分块
		uchar2Uint(newStr, M, i);
		//压缩
		H_MD5(buffer_MD, M);
	}

	//将寄存器中的结果转换为unsigned char
	unsigned char *digit = new unsigned char [16];
	uint2Uchar(buffer_MD, digit);
	delete[]newStr;
	return digit;
}

unsigned char* padding(const char *str, int &len) {
	//输入字符的长度
	int size = strlen(str);
	int paddingSize = 448 - ((size * 8) % 512);
	if (paddingSize <= 0)
		paddingSize += 512;
	int paddingByte = paddingSize / 8;

	//将填充后字符的长度赋予len传出
	len = paddingByte + size + 8;
	unsigned char *newStr = new unsigned char[len];

	//先将原本的字符串的传入新的字符串
	CopyMemory(newStr, str, size);

	//填充的第一个数为1，即第一个字节为10000000
	newStr[size] = 0x80;

	//接下来全部填充0
	ZeroMemory(&newStr[size + 1], paddingByte - 1);

	//填充K值的低64位
	unsigned long long int bitLength = size * 8;
	CopyMemory(&newStr[size + paddingByte], &bitLength, 8);

	return newStr;
}

void uchar2Uint(unsigned char* src, unsigned int dst[16], int time) {
	time = time * 64;
	for (int i = 0; i < 16; i++) {
		dst[i] = src[i * 4 + time] | (src[i * 4 + 1 + time] << 8) | (src[i * 4 + 2 + time] << 16) | (src[i * 4 + 3 + time] << 24);
	}
}

void H_MD5(unsigned int CV[4], unsigned int M[16]) {
	unsigned int temp_CV[4] = { CV[0], CV[1], CV[2], CV[3] };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			myIteration(CV[0], CV[1], CV[2], CV[3], i, M[x[i][j]], T[i][j], S[i][j]);
			unsigned int temp = CV[3];
			for (int k = 3; k > 0; k--) {
				CV[k] = CV[k - 1];
			}
			CV[0] = temp;
		}
	}
	for (int i = 0; i < 4; i++) {
		CV[i] += temp_CV[i];
	}
}

void myIteration(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, int times, unsigned int X, unsigned int T, int s) {
	switch (times) {

	case 0:
			a += F(b, c, d) + X + T;
			break; 
	case 1:
			a += G(b, c, d) + X + T;
			break;
	case 2:
			a += H(b, c, d) + X + T;
			break;
	case 3:
			a += I(b, c, d) + X + T;
			break; 
	}
	a = ((a << s) | (a >> (32 - s)));
	a += b;
}

void uint2Uchar(unsigned int* src, unsigned char* dst) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dst[i * 4 + j] = src[i] >> (j * 8);
		}	
	}
}

void showAnswer(unsigned char *answer) {
	printf("密文是: ");
	for (int i = 0; i < 16; i++) {
		printf("%02x", answer[i]);
	}
	printf("\n");
}
