#pragma once
#include <stdio.h>
#include <Windows.h>

#define F(b, c, d) ((b & c) | (~b & d))
#define G(b, c, d) ((b & d) | (c & ~d))
#define H(b, c, d) (b ^ c ^ d)
#define I(b, c, d) (c ^ (b | ~d))

//MD5�����߼�����
unsigned char* MD5(const char* str);

//���
unsigned char* padding(const char *str, int &len);

//��8λunsigned char ת��Ϊ32λunsigned int
void uchar2Uint(unsigned char* src, unsigned int dst[16], int time);

//ѹ������H_MD5
void H_MD5(unsigned int CV[4], unsigned int M[16]);

//��������
void myIteration(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, int times, unsigned int X, unsigned int T, int s);

//��32λunsigned int ת��Ϊ8λunsigned char
void uint2Uchar(unsigned int* MD, unsigned char* digit);

//���õ�������Ϊ8λunsigned char�Ľ����16�������32λ
void showAnswer(unsigned char *answer);