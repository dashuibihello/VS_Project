#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
#include <opencv2\opencv.hpp>

#define N 8

using namespace std;
using namespace cv;

//��RGBת��ΪYCrCb
void myRGB2YCrCb(Mat src, Mat &dst);

//��YCrCbת��ΪRGb
void myYCrCb2RGB(Mat src, Mat &dst);

//���β���
void getSample2(Mat &src);

//�����������Ƿ���0-255 ��С��0�򷵻�0��������255�򷵻�255
double checkNum(double num);

//��λ�ô�ͼƬ�õ�8*8�Ŀ�
void getBlockFromImg(Mat src, double block[N][N], int x, int y);

//��λ�ý���д��ͼƬ
void writeBlockToImg(Mat &dst, double Y[N][N], double Cr[N][N], double Cb[N][N], int x, int y);

//ѹ��
void compression(double Y[N][N], double Cr[N][N], double Cb[N][N]);

//��ÿ����ֵ��ȥ128��Ȼ��һһ����DCT�任��ʽ�����ɴﵽDCT�任��Ŀ�ġ�ͼ�������ֵ�����ȥ128������ΪDCT��ʽ�����ܵ����ַ�Χ��-128��127֮��
void sub128(double F[][N], double f[][N]);

//��IDCT�õ�������ÿ����128
void add128(double F[][N], double f[][N]);

//DCT�任
void DCT(double F[][N], double f[][N]);

//��Y��������
void Quantification_Y(int F[][N], double f[][N]);

//��CrCb��������
void Quantification_C(int F[][N], double f[][N]);

//IDCT�任
void IDCT(double F[][N], double f[][N]);

//��Y���з�����
void I_Quantification_Y(double F[][N], int f[][N]);

//��CrCb���з�����
void I_Quantification_C(double F[][N], int f[][N]);

void Zig_Zag(int array[64], int F[][N]);
