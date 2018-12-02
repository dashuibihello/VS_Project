#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
#include <opencv2\opencv.hpp>

#define N 8

using namespace std;
using namespace cv;

//将RGB转换为YCrCb
void myRGB2YCrCb(Mat src, Mat &dst);

//将YCrCb转换为RGb
void myYCrCb2RGB(Mat src, Mat &dst);

//二次采样
void getSample2(Mat &src);

//检测输入的数是否在0-255 若小于0则返回0，若大于255则返回255
double checkNum(double num);

//按位置从图片得到8*8的块
void getBlockFromImg(Mat src, double block[N][N], int x, int y);

//按位置将块写入图片
void writeBlockToImg(Mat &dst, double Y[N][N], double Cr[N][N], double Cb[N][N], int x, int y);

//压缩
void compression(double Y[N][N], double Cr[N][N], double Cb[N][N]);

//将每个数值减去128，然后一一带入DCT变换公式，即可达到DCT变换的目的。图像的数据值必须减去128，是因为DCT公式所接受的数字范围是-128到127之间
void sub128(double F[][N], double f[][N]);

//将IDCT得到的数据每个加128
void add128(double F[][N], double f[][N]);

//DCT变换
void DCT(double F[][N], double f[][N]);

//对Y进行量化
void Quantification_Y(int F[][N], double f[][N]);

//对CrCb进行量化
void Quantification_C(int F[][N], double f[][N]);

//IDCT变换
void IDCT(double F[][N], double f[][N]);

//对Y进行反量化
void I_Quantification_Y(double F[][N], int f[][N]);

//对CrCb进行反量化
void I_Quantification_C(double F[][N], int f[][N]);

void Zig_Zag(int array[64], int F[][N]);
