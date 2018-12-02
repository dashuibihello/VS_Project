#include "Jpeg.hpp"
 
int main() {
	Mat img = imread("cartoon.jpg");

	//��ͼƬ���Ϳ���N�ı���
	int x = img.cols % N;
	int y = img.rows % N;
	int height = img.rows - y;
	int width = img.cols - x;
	resize(img, img, Size(width, height), (0, 0), (0, 0));

	//��ɫ�ռ�ת��
	Mat YCrCb_Img(img.size(), CV_8UC3, Scalar(0, 0, 0));
	myRGB2YCrCb(img, YCrCb_Img);

	//���β���
	getSample2(YCrCb_Img);

	//��YCrCb�ֿ����д���
	Mat Y, Cr, Cb;
	vector<Mat> channels;
	split(YCrCb_Img, channels);
	Y = channels.at(0);
	Cr = channels.at(1);
	Cb = channels.at(2);
	
	//�ֿ鲢����ѹ��
	Mat new_YCrCb_Img(img.size(), CV_8UC3, Scalar(0, 0, 0));	
	int height_8 = height / N;
	int width_8 = width / N;
	for (int i = 0; i < width_8 * height_8; i++) {
		double block_Y[N][N] = { 0 };
		double block_Cr[N][N] = { 0 };
		double block_Cb[N][N] = { 0 };

		//��λ�ô�ͼ��ȡ8*8�Ŀ�
		getBlockFromImg(Y, block_Y, (i / width_8) * N, (i % width_8) * N);
		getBlockFromImg(Cr, block_Cr, (i / width_8) * N, (i % width_8) * N);
		getBlockFromImg(Cb, block_Cb, (i / width_8) * N, (i % width_8) * N);

		//����ѹ������������ÿ�����ּ�128 -> DCT -> ���� -> ������ -> IDCT -> ��ÿ�����ݼ�28��
		compression(block_Y, block_Cr, block_Cb);

		//�����λ�õĿ�д���µ�ͼƬ
		writeBlockToImg(new_YCrCb_Img, block_Y, block_Cr, block_Cb, (i / width_8) * N, (i % width_8) * N);
	}

	//����ɫ�ռ�ָ���RGB
	Mat newImg(img.size(), CV_8UC3, Scalar(0, 0, 0));
	myYCrCb2RGB(new_YCrCb_Img, newImg);

	//��ͼƬ�ָ�Ϊԭ�ߴ�
	resize(newImg, newImg, Size(width + x, height + y), (0, 0), (0, 0));

	//���ͼƬ
	imwrite("cartoon_compression.jpg", newImg);
	
	waitKey(0);
	return 0;
}