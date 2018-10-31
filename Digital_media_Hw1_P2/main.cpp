#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>


using namespace cv;
using namespace std;

typedef struct myRGB {
	int R;
	int G;
	int B;
}myRGB;

vector<myRGB> colorTable;

void medianCut(myRGB *colorList, int colortype, int length, myRGB *newcolorList);
void merge_sort_R(myRGB *data, int start, int end, myRGB *result);
void merge_sort_G(myRGB *data, int start, int end, myRGB *result);
void merge_sort_B(myRGB *data, int start, int end, myRGB *result);
void merge_R(myRGB *data, int start, int end, myRGB *result);
void merge_G(myRGB *data, int start, int end, myRGB *result);
void merge_B(myRGB *data, int start, int end, myRGB *result);

int main() {
	Mat img = imread("redapple.jpg");
	int height = img.rows;
	int width = img.cols;
	

	myRGB *colorList = new myRGB[height * width];
	myRGB *newcolorList = new myRGB[height * width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			myRGB temp;
			temp.R = img.at<Vec3b>(i, j)[2];
			temp.G = img.at<Vec3b>(i, j)[1];
			temp.B = img.at<Vec3b>(i, j)[0];
			colorList[width * i + j] = temp;
		}		
	}
	medianCut(colorList, 0, width * height, newcolorList);
	Mat newimg = img;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			double min;
			myRGB color;
			for (int a = 0; a < colorTable.size(); a++) {
				double sum = 0;
				sum += pow(double(img.at<Vec3b>(j, i)[2] - colorTable[a].R), 2);
				sum += pow(double(img.at<Vec3b>(j, i)[1] - colorTable[a].G), 2);
				sum += pow(double(img.at<Vec3b>(j, i)[0] - colorTable[a].B), 2);
				double dis = sqrt(sum);
				if (a == 0 || dis < min) {
					min = dis;
					color = colorTable[a];
				}
			}			
			newimg.at<Vec3b>(j, i)[2] = color.R;
			newimg.at<Vec3b>(j, i)[1] = color.G;
			newimg.at<Vec3b>(j, i)[0] = color.B;
			
		}
	}
	

	Mat colorsquare(800, 800, CV_8UC3, Scalar(0, 0, 0));
	int square = 50;
	for (int a = 0; a < colorTable.size(); a++) {
		for (int i = (a / 16) * square; i < (a / 16) * square + square; i++) {
			for (int j = (a % 16) * square; j < (a % 16) * square + square; j++) {
				colorsquare.at<Vec3b>(i, j)[2] = colorTable[a].R;
				colorsquare.at<Vec3b>(i, j)[1] = colorTable[a].G;
				colorsquare.at<Vec3b>(i, j)[0] = colorTable[a].B;
			}
		}
	}
	imshow("test", newimg);
	imshow("test1", colorsquare);

	imwrite("solution256.jpg", newimg);
	imwrite("colorsquare256.jpg", colorsquare);
	waitKey(0);
	return 0;
}


void medianCut(myRGB *colorList, int colortype, int length, myRGB *newcolorList) {
	if (colortype == 8) {
		int sumR = 0, sumG = 0, sumB = 0;
		for (int i = 0; i < length; i++) {
			sumR += colorList[i].R;
			sumG += colorList[i].G;
			sumB += colorList[i].B;
		}
		myRGB temp;
		temp.R = sumR / length;
		temp.G = sumG / length;
		temp.B = sumB / length;

		colorTable.push_back(temp);
		return;
	}
	else if (colortype % 3 == 0) {
		merge_sort_R(colorList, 0, length - 1, newcolorList);
	}
	else if (colortype % 3 == 1) {
		merge_sort_G(colorList, 0, length - 1, newcolorList);
	}
	else if(colortype % 3 == 2){
		merge_sort_B(colorList, 0, length - 1, newcolorList);
	}
	length /= 2;
	myRGB* colorList1 = new myRGB[length];
	myRGB* colorList2 = new myRGB[length];
	myRGB* newcolorList1 = new myRGB[length];
	myRGB* newcolorList2 = new myRGB[length];
	for (int i = 0; i < length; i++) {
		colorList1[i] = newcolorList[i];
		colorList2[i] = newcolorList[i + length];
	}

	medianCut(colorList1, colortype + 1, length, newcolorList1);
	medianCut(colorList2, colortype + 1, length, newcolorList2);
}

void merge_sort_R(myRGB *data, int start, int end, myRGB *result) {
	if (1 == end - start) {
		if (data[start].R >= data[end].R) {
			myRGB temp = data[start];
			data[start] = data[end];
			data[end] = temp;
		}
		return;
	}
	else if (0 == end - start)
		return;
	else {
		merge_sort_R(data, start, (end - start + 1) / 2 + start, result);
		merge_sort_R(data, (end - start + 1) / 2 + start + 1, end, result);
		merge_R(data, start, end, result);
		for (int i = start; i <= end; ++i)
			data[i] = result[i];
	}
}

void merge_sort_G(myRGB *data, int start, int end, myRGB *result) {
	if (1 == end - start) {
		if (data[start].G >= data[end].G) {
			myRGB temp = data[start];
			data[start] = data[end];
			data[end] = temp;
		}
		return;
	}
	else if (0 == end - start)
		return;
	else {
		merge_sort_G(data, start, (end - start + 1) / 2 + start, result);
		merge_sort_G(data, (end - start + 1) / 2 + start + 1, end, result);
		merge_G(data, start, end, result);
		for (int i = start; i <= end; ++i)
			data[i] = result[i];
	}
}

void merge_sort_B(myRGB *data, int start, int end, myRGB *result) {
	if (1 == end - start) {
		if (data[start].B >= data[end].B) {
			myRGB temp = data[start];
			data[start] = data[end];
			data[end] = temp;
		}
		return;
	}
	else if (0 == end - start)
		return;
	else {
		merge_sort_B(data, start, (end - start + 1) / 2 + start, result);
		merge_sort_B(data, (end - start + 1) / 2 + start + 1, end, result);
		merge_B(data, start, end, result);
		for (int i = start; i <= end; ++i)
			data[i] = result[i];
	}
}

void merge_R(myRGB *data, int start, int end, myRGB *result)
{
	int left_length = (end - start + 1) / 2 + 1;
	int left_index = start;
	int right_index = start + left_length;
	int result_index = start;
	while (left_index < start + left_length && right_index < end + 1) {
		if (data[left_index].R <= data[right_index].R)
			result[result_index++] = data[left_index++];
		else
			result[result_index++] = data[right_index++];
	}
	while (left_index < start + left_length)
		result[result_index++] = data[left_index++];
	while (right_index < end + 1)
		result[result_index++] = data[right_index++];
}

void merge_G(myRGB *data, int start, int end, myRGB *result)
{
	int left_length = (end - start + 1) / 2 + 1;
	int left_index = start;
	int right_index = start + left_length;
	int result_index = start;
	while (left_index < start + left_length && right_index < end + 1) {
		if (data[left_index].G <= data[right_index].G)
			result[result_index++] = data[left_index++];
		else
			result[result_index++] = data[right_index++];
	}
	while (left_index < start + left_length)
		result[result_index++] = data[left_index++];
	while (right_index < end + 1)
		result[result_index++] = data[right_index++];
}

void merge_B(myRGB *data, int start, int end, myRGB *result) {
	int left_length = (end - start + 1) / 2 + 1;
	int left_index = start;
	int right_index = start + left_length;
	int result_index = start;
	while (left_index < start + left_length && right_index < end + 1) {
		if (data[left_index].B <= data[right_index].B)
			result[result_index++] = data[left_index++];
		else
			result[result_index++] = data[right_index++];
	}
	while (left_index < start + left_length)
		result[result_index++] = data[left_index++];
	while (right_index < end + 1)
		result[result_index++] = data[right_index++];
}