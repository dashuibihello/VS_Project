#pragma once

//ÂÖº¯Êı
void Feistel(int input[32], int output[32], int subKey[48]);

//Òì»ò²Ù×÷
void XOR(int input_A[], int input_B[], int length);

//EÀ©Õ¹
void E(int input[32], int output[48], int table[48]);

//SºĞ6-4×ª»»
void S(int input[48], int output[32], int table[8][4][16]);

//PÖÃ»»
void P(int input[32], int output[32], int table[32]);