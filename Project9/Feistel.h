#pragma once

//�ֺ���
void Feistel(int input[32], int output[32], int subKey[48]);

//������
void XOR(int input_A[], int input_B[], int length);

//E��չ
void E(int input[32], int output[48], int table[48]);

//S��6-4ת��
void S(int input[48], int output[32], int table[8][4][16]);

//P�û�
void P(int input[32], int output[32], int table[32]);