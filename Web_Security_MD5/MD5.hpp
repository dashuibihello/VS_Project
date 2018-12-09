#pragma once
#include <stdio.h>
#include <Windows.h>

#define F(b, c, d) ((b & c) | (~b & d))
#define G(b, c, d) ((b & d) | (c & ~d))
#define H(b, c, d) (b ^ c ^ d)
#define I(b, c, d) (c ^ (b | ~d))

//MD5主体逻辑函数
unsigned char* MD5(const char* str);

//填充
unsigned char* padding(const char *str, int &len);

//将8位unsigned char 转换为32位unsigned int
void uchar2Uint(unsigned char* src, unsigned int dst[16], int time);

//压缩函数H_MD5
void H_MD5(unsigned int CV[4], unsigned int M[16]);

//迭代运算
void myIteration(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, int times, unsigned int X, unsigned int T, int s);

//将32位unsigned int 转换为8位unsigned char
void uint2Uchar(unsigned int* MD, unsigned char* digit);

//将得到的类型为8位unsigned char的结果以16进制输出32位
void showAnswer(unsigned char *answer);