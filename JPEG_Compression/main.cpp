#include "Jpeg.hpp"
 
int main() {
	Mat img = imread("cartoon.jpg");

	//将图片长和宽变成N的倍数
	int x = img.cols % N;
	int y = img.rows % N;
	int height = img.rows - y;
	int width = img.cols - x;
	resize(img, img, Size(width, height), (0, 0), (0, 0));

	//颜色空间转换
	Mat YCrCb_Img(img.size(), CV_8UC3, Scalar(0, 0, 0));
	myRGB2YCrCb(img, YCrCb_Img);

	//二次采样
	getSample2(YCrCb_Img);

	//将YCrCb分开进行处理
	Mat Y, Cr, Cb;
	vector<Mat> channels;
	split(YCrCb_Img, channels);
	Y = channels.at(0);
	Cr = channels.at(1);
	Cb = channels.at(2);
	
	//分块并进行压缩
	Mat new_YCrCb_Img(img.size(), CV_8UC3, Scalar(0, 0, 0));	
	int height_8 = height / N;
	int width_8 = width / N;
	for (int i = 0; i < width_8 * height_8; i++) {
		double block_Y[N][N] = { 0 };
		double block_Cr[N][N] = { 0 };
		double block_Cb[N][N] = { 0 };

		//按位置从图获取8*8的块
		getBlockFromImg(Y, block_Y, (i / width_8) * N, (i % width_8) * N);
		getBlockFromImg(Cr, block_Cr, (i / width_8) * N, (i % width_8) * N);
		getBlockFromImg(Cb, block_Cb, (i / width_8) * N, (i % width_8) * N);

		//进行压缩处理（将块中每个数字减128 -> DCT -> 量化 -> 反量化 -> IDCT -> 将每个数据加28）
		compression(block_Y, block_Cr, block_Cb);

		//将这个位置的块写回新的图片
		writeBlockToImg(new_YCrCb_Img, block_Y, block_Cr, block_Cb, (i / width_8) * N, (i % width_8) * N);
	}

	//将颜色空间恢复成RGB
	Mat newImg(img.size(), CV_8UC3, Scalar(0, 0, 0));
	myYCrCb2RGB(new_YCrCb_Img, newImg);

	//将图片恢复为原尺寸
	resize(newImg, newImg, Size(width + x, height + y), (0, 0), (0, 0));

	//输出图片
	imwrite("cartoon_compression.jpg", newImg);
	
	waitKey(0);
	return 0;
}