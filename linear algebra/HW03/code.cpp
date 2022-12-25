#include<iostream>
#include <vector>
#include<algorithm>
#include<cmath>
#include<map>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace cv;
using namespace std;
#define id ppp.second.second
#define pi  3.14
Mat image;
double distance(Vec2d v) {//二維空間向量的長度->  distance(u,v) = sqrt(u^2 + v^2)
	double dis = ((v[0] * v[0]) + (v[1] * v[1]));
	dis = sqrt(dis);
	return dis;
}
double distance(Point a, Point b) {//二維空間兩點的距離
	double dis = abs((a.x - b.x) * (a.x - b.x)) + abs((a.y - b.y) * (a.y - b.y));
	dis = sqrt(dis);
	return dis;
}
Vec2d getVector(Point a, Point b) {
	return Vec2d((a.x - b.x), (a.y - b.y));
}
pair<double, double> getMinAndMaxSideLen(vector<Point> approx) {
	pair<double, double> pdd;
	pdd.first = 0x48763;//min
	pdd.second = 0;//max
	for (int i = 0; i < approx.size(); i++) {
		pdd.first = min(pdd.first, distance(approx[i], approx[(i + 1) % approx.size()]));
		pdd.second = max(pdd.second, distance(approx[i], approx[(i + 1) % approx.size()]));
	}
	return pdd;
}

int isIrregular(vector<Point>& contour) {
	auto rec = minAreaRect(contour);
	Point center = rec.center;
	double contourLength = arcLength(contour, true);
	double area = contourArea(contour);
	vector<Point> approx;
	approxPolyDP(contour, approx, 0.04 * contourLength, true);
	int corners = approx.size();

	circle(image, center, 4, Scalar(0, 200, 0), -1);
	for (auto p : approx) {
		circle(image, p, 4, Scalar(200, 0, 0), -1);
	}


	if (corners <= 2) {
		return 1;
	}
	double para = 1.5;
	double minSideLen = 0x48763;
	double maxSideLen = 0;
	double possibleSideLen1;
	double possibleSideLen2;
	double cos1 = 0;
	double cos2 = 0;

	if (corners == 3 || corners == 4) {
		pair<double, double> MinAndMax = getMinAndMaxSideLen(approx);
		minSideLen = MinAndMax.first;
		maxSideLen = MinAndMax.second;
		possibleSideLen1 = contourLength / corners;//利用周長
		possibleSideLen2 = (corners == 3) ? sqrt(4 * area / sqrt(3)) : sqrt(area);//利用面積
	}
	else {
		//這邊SideLen 等於半徑
		possibleSideLen1 = contourLength / pi / 2.0;//利用周長
		possibleSideLen2 = sqrt(area / pi); //利用面積
		for (auto p : approx) {
			minSideLen = min(minSideLen, distance(center, p));
			maxSideLen = max(maxSideLen, distance(center, p));
		}
	}
	bool f1 = 0;
	bool f2 = 1;
	if (corners == 4) {//利用dot來判斷四邊是否都為90度
		Vec2d v1 = getVector(approx[0], approx[1]);
		Vec2d v2 = getVector(approx[0], approx[3]);
		double DotRes1 = v1.dot(v2);
		Vec2d v3 = getVector(approx[2], approx[1]);
		Vec2d v4 = getVector(approx[2], approx[3]);
		double DotRes2 = v3.dot(v4);
		cos1 = DotRes1 / (distance(v1) * distance(v2));
		cos2 = DotRes2 / (distance(v3) * distance(v4));
		f1 = (abs(cos1) >= 0.02 || abs(cos2) >= 0.02);
		f2 = abs(minSideLen * maxSideLen - area) >= area * 0.1;
	}


	bool f3 = (minSideLen * para <= maxSideLen);// 最小邊長*1.5 <=最大邊長 -> 最小邊太小 or 最大邊太大
	bool f4 = minSideLen * para <= min(possibleSideLen1, possibleSideLen2);//最小邊長*1.5 <= 理想的邊長 -> 最小邊太小
	bool f5 = maxSideLen >= min(possibleSideLen1, possibleSideLen2) * para;//最大邊長 >= 理想的邊長*1.5 ->  最大邊太大

	return  f1 || f2 && (f3 || f4 || f5); //如果是圖片是長方形-> f2 == 0 ，那 f3 f4 f5 比較邊長的判斷方法視為無效，故要 f2 &&(f3 || f4 || f5)
}


int main(int argc, const char** argv)
{
	argv[1] = "public6.jpg";
	image = cv::imread(argv[1]);
	int arealimit = image.rows * image.cols / 1000;
	Mat imghsv;
	srand(time(0));
	cvtColor(image, imghsv, COLOR_BGR2HSV);
	Mat1b mask[5];
	inRange(imghsv, Scalar(20, 120, 100), Scalar(30, 255, 255), mask[1]);//yellow
	inRange(imghsv, Scalar(0, 120, 70), Scalar(15, 255, 255), mask[2]);//red
	inRange(imghsv, Scalar(165, 120, 70), Scalar(180, 255, 255), mask[0]);//red
	inRange(imghsv, Scalar(90, 100, 50), Scalar(128, 255, 255), mask[3]);//blue
	//inRange(imghsv, Scalar(0, 0, 0), Scalar(180, 255, 46), maskb);

	mask[2] = mask[2] | mask[0];
	Mat res = mask[1] | mask[2] | mask[3];
	vector<pair<int, pair<int, int>>> ans;

	vector<Rect> boxes;

	vector<vector<Point>> contours;

	vector<vector<Point> > contourst;

	for (int i = 1; i <= 3; i++) {
		vector<Vec4i> hierarchy;
		findContours(mask[i], contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
		for (int cnt = 0; cnt < contours.size(); cnt++) {
			if (hierarchy[cnt][3] != -1) continue;
			double area = contourArea(contours[cnt]);
			if (area > arealimit) {
				if (isIrregular(contours[cnt])) {
					continue;
				}
				Rect box = boundingRect (Mat(contours[cnt]));
				drawContours(res, contours, cnt, Scalar(160 + i * 10, 11 + i * 10, 22 + i * 10), 1, 1);
				int f = 1;
				for (auto& b : boxes) {
					if (0 <= box.x - b.x && 0 <= box.y - b.y &&
						0 >= (box.x + box.width) - (b.x + b.width) && 0 >= (box.y + box.height) - (b.y + b.height)) {
						f = 0;//新的輪廓在裡面
						continue;
					}
					if (0 >= box.x - b.x && 0 >= box.y - b.y &&
						0 <= (box.x + box.width) - (b.x + b.width) && 0 <= (box.y + box.height) - (b.y + b.height)) {
						f = 0;//新的輪廓在外面
						b = box;
					}
				}
				if (f) {
					ans.push_back({ box.x,{box.y,boxes.size()} });
					boxes.push_back(box);
				}
			}
		}
	}
	sort(ans.begin(), ans.end());
	for (auto ppp : ans) {
		auto box = boxes[id];
		rectangle(image, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(0, 0, 200), 2, 2);
		cout << boxes[id].x - rand() % 2 << " " << boxes[id].y << " " << boxes[id].x + boxes[id].width << " " << boxes[id].y + boxes[id].height << endl;
	}
	cv::imshow("0xc48763", image);
	cv::waitKey();
	return 0;

}



