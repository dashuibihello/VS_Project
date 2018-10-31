#pragma once
#include "Subkey.h"
#include "Feistel.h"
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

//DES����
void DES_E(char input[], char key_char[8], int output[][64], int length);

//DES����
void DES_D(int input[][64], char key_char[8], char output[], int length);

//��һ����Ϣ����
void DES_E_Block(char input[8], char key_char[8], int output[64]);

//��һ����Ϣ����
void DES_D_Block(int input[64], char key_char[8], char output[8]);

//���ֽ�ת��Ϊ����
void ByteToBit(char input[], int output[64]);

//������ת��Ϊ�ֽ�
void BitToByte(int input[64], char output[]);

//IP�û�����IP���û�
void IP_Or_Inv_IP(int input[64], int output[64], int table[64]);