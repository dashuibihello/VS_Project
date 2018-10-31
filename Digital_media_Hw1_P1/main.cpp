#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat lena_src = imread("lena.jpg");
	Mat nobel_src = imread("Åµ±´¶û.jpg");
	Mat gray(nobel_src.size(), CV_8U, Scalar(0));

	int height = lena_src.rows;
	int width = lena_src.cols;							  

	VideoWriter writer = VideoWriter("solution.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, lena_src.size(), 0);
	int r = 0;

	while (r < sqrt(pow(height / 2, 2) + pow(width / 2, 2))) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (sqrt(pow(height / 2 - j, 2) + pow(width / 2 - i, 2)) < r) {
					gray.at<uchar>(i, j) = lena_src.at<Vec3b>(i, j)[2];
				}
				else {
					gray.at<uchar>(i, j) = nobel_src.at<Vec3b>(i, j)[2];
				}
			}
		}
		imshow("test", gray);
		writer.write(gray);
		r += 2;
		if (cvWaitKey(20) == 27)
			break;		
	}
	imwrite("newlena.jpg", gray);
	writer.release();
	return 0;
}


