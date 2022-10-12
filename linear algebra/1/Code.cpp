#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc_c.h>
#include<iostream>
#include<queue>
using namespace std;
using namespace cv;
int main(int argc, char* argv[])
{
	
	clock_t s, e;
	s = clock();
	const char* _48763 = "public_03.png";
	const char* _36784 = "argv[2].png";
	Mat img = imread(_48763, IMREAD_GRAYSCALE);
	Mat ntr = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	threshold(img, ntr, 240, 255, CV_THRESH_BINARY);
	findContours(ntr, contours, CV_RETR_EXTERNAL, 1);
	Mat Masks = Mat::zeros(img.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)
		if (contours[i].size() > 900) {
			RotatedRect box = fitEllipse(contours[i]);
			float angle = box.angle;
			drawContours(Masks, contours, i, Scalar(255), -1);
			bitwise_and(img, Masks, img);
			if (angle > 90) angle = -(180 - angle);
			warpAffine(img, img, getRotationMatrix2D(box.center, angle, 1.0), img.size(), 1, 0);
			imwrite(_36784, img);
			e = clock();
			cout << "RUN TIME : " << (double)(e - s) / CLOCKS_PER_SEC << endl;
			return 0;
		}
	cout << "想不到有漏洞吧";
	/*
		=========================================================
		| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠄⠀⠛
		|  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣋⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠐⢦⣀
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠋⣤⣶⣶⣄⠙⣿⣷⢸⡀⠀⠀⠀⠀⠀⠀⠉⠉⠙⠛⢿⣶
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠁⣿⣿⠉⠉⢻⣷⣿⣿⣾⣿⠀⠀⠀⠀⠀⢠⣿⠿⢿⣿⣦⠘⣷
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⢸⣿⣿⠀⠀⠀⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⣿⠏⠀⠀⣿⣿⣷⠘
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣄⣀⣾⣿⣿⣿⠿⣿⣿⣧⠀⠀⠀⣿⡀⠀⠀⣿⣿⣿
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⣆⢸⣿⣿⣯⣭⣿⣵⣾⣿⠃⠀⣿⣿⣿⡆⡀⠀⠹⢿⣶⣿⣿⣿⣣⣿
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⢸⡀⣿⣿⣿⣿⣿⣿⣿⣿⠀⣾⣿⣿⣿⣿⣷⢰⣄⠀⠀⠀⠚⠛⢿⡇
		|   ⠀⠀⠀⠀⠀⠀⠀⠈⣌⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣿⣿⣿⣿⣿⣿⣾⣿⣿⣶⣶⣾⣿⡿
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣴⠁⠀⠀⠀⠀⠀⡀
		|   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⣴⠃
		|   ⠀⢠⣶⡀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⣀⣠⣤⠀⠞
		|   ⠀⣿⡿⣷⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣏⣈⠙⠛⠻⢿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠋
		|   ⣸⣿⠀⠈⢿⣄⠀⠀⠀⠀⠙⣿⣿⣿⣿⣟⣛⣿⢿⣻⠿⣒⣺⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⢀
		|   ⣿⡟⠀⠀⠀⠙⢿⣦⣄⡀⠀⠈⢿⣿⣿⣿⣛⣿⣿⣿⣠⣽⣾⣿⣿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⢠⣿⡆
		|
		|                       騙人的吧?
		=========================================================
	*/

}