#pragma once

//��ȡ����Կ
void GetSubKey(int key[64], int subKey[16][48]);

//PC_1�û�
void PC_1(int input[64], int output[56], int table[56]);

//PC_2�û�
void PC_2(int input[56], int output[48], int table[48]);

//ѭ������
void Rotating_L(int input[28], int output[28], int count);