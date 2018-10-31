#pragma once
#include "Subkey.h"
#include "Feistel.h"
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

//DES加密
void DES_E(char input[], char key_char[8], int output[][64], int length);

//DES解密
void DES_D(int input[][64], char key_char[8], char output[], int length);

//对一块信息加密
void DES_E_Block(char input[8], char key_char[8], int output[64]);

//对一块信息解密
void DES_D_Block(int input[64], char key_char[8], char output[8]);

//将字节转换为比特
void ByteToBit(char input[], int output[64]);

//将比特转换为字节
void BitToByte(int input[64], char output[]);

//IP置换或者IP逆置换
void IP_Or_Inv_IP(int input[64], int output[64], int table[64]);