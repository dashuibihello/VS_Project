#include "Jpeg.hpp"

int Q_Y[N][N]{
	16,11,10,16,24,40,51,61,
	12,12,14,19,26,58,60,55,
	14,13,16,24,40,57,69,56,
	14,17,22,29,51,87,80,62,
	18,22,37,56,68,109,103,77,
	24,35,55,64,81,104,113,92,
	49,64,78,87,103,121,120,101,
	72,92,95,98,112,100,103,99
};

int Q_C[N][N]{
	17, 18, 24, 47, 99, 99, 99, 99,
	18, 21, 26, 66, 99, 99, 99, 99,
	24, 26, 56, 99, 99, 99, 99, 99,
	47, 66, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99
};

void myRGB2YCrCb(Mat src, Mat &dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<Vec3b>(i, j)[0] = (uchar)(0.114 * src.at<Vec3b>(i, j)[0] + 0.587 * src.at<Vec3b>(i, j)[1] + 0.299 * src.at<Vec3b>(i, j)[2]);			//Y
			dst.at<Vec3b>(i, j)[1] = (uchar)(-0.0813 * src.at<Vec3b>(i, j)[0] - 0.4187 * src.at<Vec3b>(i, j)[1] + 0.5 * src.at<Vec3b>(i, j)[2] + 128);	//Cr
			dst.at<Vec3b>(i, j)[2] = (uchar)(0.5 * src.at<Vec3b>(i, j)[0] - 0.3313 * src.at<Vec3b>(i, j)[1] - 0.1687 * src.at<Vec3b>(i, j)[2] + 128);	//Cb
		}
	}
}

void myYCrCb2RGB(Mat src, Mat &dst) {
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<Vec3b>(i, j)[2] = (int)checkNum(src.at<Vec3b>(i, j)[0] + 1.402 * (src.at<Vec3b>(i, j)[1] - 128));
			dst.at<Vec3b>(i, j)[1] = (int)checkNum(src.at<Vec3b>(i, j)[0] - 0.34414 * (src.at<Vec3b>(i, j)[2] - 128) - 0.71414 * (src.at<Vec3b>(i, j)[1] - 128));
			dst.at<Vec3b>(i, j)[0] = (int)checkNum(src.at<Vec3b>(i, j)[0] + 1.722 * (src.at<Vec3b>(i, j)[2] - 128));
		}
	}
}

void getSample2(Mat &src) {
	int height_2 = src.rows / 2;
	int width_2 = src.cols / 2;
	for (int i = 0; i < height_2; i++) {
		for (int j = 0; j < width_2; j++) {
			int x = i * 2;
			int y = j * 2;
			//Cr取左上块的值
			src.at<Vec3b>(x + 1, y)[1] = src.at<Vec3b>(x, y)[1];
			src.at<Vec3b>(x, y + 1)[1] = src.at<Vec3b>(x, y)[1];
			src.at<Vec3b>(x + 1, y + 1)[1] = src.at<Vec3b>(x, y)[1];

			//Cb取左下块的值
			src.at<Vec3b>(x, y)[2] = src.at<Vec3b>(x, y + 1)[2];
			src.at<Vec3b>(x + 1, y)[2] = src.at<Vec3b>(x, y + 1)[2];
			src.at<Vec3b>(x + 1, y + 1)[2] = src.at<Vec3b>(x, y + 1)[2];
		}
	}
}

double checkNum(double num) {
	if (num > 255)
		num = 255;
	else if (num < 0)
		num = 0;
	return num;
}

void getBlockFromImg(Mat src, double block[N][N], int x, int y) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			block[i][j] = src.at<uchar>(x + i, y + j);
		}
	}
}

void writeBlockToImg(Mat &dst, double Y[N][N], double Cr[N][N], double Cb[N][N], int x, int y) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			dst.at<Vec3b>(x + i, y + j)[0] = (int)checkNum(Y[i][j]);
			dst.at<Vec3b>(x + i, y + j)[1] = (int)checkNum(Cr[i][j]);
			dst.at<Vec3b>(x + i, y + j)[2] = (int)checkNum(Cb[i][j]);
		}
	}
}

void compression(double Y[N][N], double Cr[N][N], double Cb[N][N]) {
	double F[N][N] = { 0 };
	double tempf[N][N] = { 0 };
	int Q_F[N][N] = { 0 };

	//依次对Y Cr Cb 三个块进行压缩
	//Y
	sub128(tempf, Y);
	DCT(F, tempf);
	Quantification_Y(Q_F, F);
	I_Quantification_Y(F, Q_F);
	IDCT(tempf, F);
	add128(Y, tempf);

	//Cb
	sub128(tempf, Cb);
	DCT(F, tempf);
	Quantification_C(Q_F, F);
	I_Quantification_C(F, Q_F);
	IDCT(tempf, F);
	add128(Cb, tempf);

	//Cr
	sub128(tempf, Cr);
	DCT(F, tempf);
	Quantification_C(Q_F, F);
	I_Quantification_C(F, Q_F);
	IDCT(tempf, F);
	add128(Cr, tempf);
}

void sub128(double F[][N], double f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = f[i][j] - 128;
		}
	}
}

void add128(double F[][N], double f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = f[i][j] + 128;
		}
	}
}

void Quantification_Y(int F[][N], double f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = (int)round(f[i][j] / Q_Y[i][j]);
		}
	}
}

void Quantification_C(int F[][N], double f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = (int)round(f[i][j] / Q_C[i][j]);
		}
	}
}

void DCT(double F[][N], double f[][N]) {
	for (int u = 0; u < N; u++) {
		for (int v = 0; v < N; v++) {
			double temp = 0;
			for (int x = 0; x < N; x++) {
				for (int y = 0; y < N; y++) {
					temp += f[x][y] * cos((2 * x + 1) * u * M_PI * 1.0 / 16) * cos((2 * y + 1) * v * M_PI * 1.0 / 16);
				}
			}
			F[u][v] = (u == 0 ? 1.0 / sqrt(N) : 0.5) * (v == 0 ? 1.0 / sqrt(N) : 0.5) * temp;
		}
	}
}

void IDCT(double F[][N], double f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double temp = 0;
			for (int u = 0; u < N; u++) {
				for (int v = 0; v < N; v++) {
					temp += (u == 0 ? 1.0 / sqrt(8) : 0.5) * (v == 0 ? 1.0 / sqrt(8) : 0.5) * cos((2 * i + 1) * u * M_PI * 1.0 / 16) * cos((2 * j + 1) * v * M_PI * 1.0 / 16) * f[u][v];
				}
			}
			F[i][j] = temp;
		}
	}
}

void I_Quantification_Y(double F[][N], int f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = (double)f[i][j] * Q_Y[i][j];
		}
	}
}

void I_Quantification_C(double F[][N], int f[][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = (double)f[i][j] * Q_C[i][j];
		}
	}
}

void Zig_Zag(int array[64], int F[][N]) {
	int lastx = 0, lasty = 0, x = 1, y = 0;
	array[0] = F[0][0];
	for (int i = 1; i < 64; i++) {
		int tempx = x, tempy = y;
		array[i] = F[y][x];
		if (y == 0) {
			if (lasty == y) {
				y = y + 1;
				x = x - 1;
			}
			else {
				x = x + 1;
			}
		}
		else if (x == 0) {
			if (lastx == x) {
				y = y - 1;
				x = x + 1;
			}
			else if (y == 7) {
				x = x + 1;
			}
			else {
				y = y + 1;
			}
		}
		else if (y == 7) {
			if (lasty == y) {
				y = y - 1;
				x = x + 1;
			}
			else {
				x = x + 1;
			}
		}
		else if (x == 7) {
			if (lastx == x) {
				y = y + 1;
				x = x - 1;
			}
			else {
				y = y + 1;
			}
		}
		else {
			
			x = x + x - lastx;
			y = y + y - lasty;
		}
		lastx = tempx;
		lasty = tempy;
	}
}