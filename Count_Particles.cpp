#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui_c.h>
#include"Count_Particles.h"
using namespace std;
using namespace cv;
int getContourData(vector<Point> contour);
void Counter(cv::Mat src, int& parnum, int& overparnum, int id) {
	//unsigned seed;
	//seed = time(0);
	//srand(seed);
	vector<vector<Point>> contours;
	findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//	cv::Mat img = Mat(src.size(), CV_8UC3);
//	cvtColor(src, img, COLOR_GRAY2BGR);
	parnum = contours.size();
//#pragma omp parallel for
	for (int i = 0; i < contours.size(); i++)
	{
		RotatedRect rect = minAreaRect(contours[i]);//求最小外接矩形
		double w = max(rect.size.height, rect.size.width);
		double h = min(rect.size.height, rect.size.width);//定义长边为宽，窄边为高
		double whrate = w / h;
		int ss = getContourData(contours[i]);
		if ((ss <= 7) || (h == 0))
		{
			//contours.erase(contours.begin() + i);
			parnum--;
			//i--;
			continue;
		}
		if (w >= 8)
		{
			if ((whrate > 1.0 && whrate < 1.2) || ((whrate > 1.8 && whrate < 2.0) && (ss >= 40)) ||
				((whrate > 2.1 && whrate < 2.3) && (ss >= 36)) || (whrate > 2.4 && whrate < 2.5) ||
				(whrate >= 2.5 && whrate <= 2.6) || ((whrate > 2.6 && whrate < 3.0) && (w >= 10)) ||
				(whrate >= 3.0 && whrate <= 3.1) || ((whrate > 4.1 && whrate < 4.6) && (w <= 16.5)) ||
				((whrate > 3.1 && whrate <= 3.7) && ((ss <= 50) || ((ss > 50) && (w <= 15)))) ||
				((whrate > 3.7 && whrate < 4.0) && (ss <= 55)))
			{
				parnum++;
			}
			if (((whrate > 3.1 && whrate <= 3.7) && (ss > 50) && w > 15) ||
				((whrate > 3.7 && whrate < 4.0) && (ss > 55)) ||
				((whrate > 4.1 && whrate < 4.6) && w > 16.5) ||
				(whrate >= 4.6 && whrate < 5.0) || (whrate >= 5.0 && whrate <= 5.1))
			{
				parnum = parnum + 2;
			}
		}
	}
}

bool bmp(Point a, Point b) {
	return a.x<b.x;
}

int getContourData(vector<Point> contour)
{
	int area = 0, maxx = 0, minx = contour[0].x, maxy = 0, miny = 0;
	int width = 0, height = 0;
	for (int i = 1; i < contour.size(); i++) {
		if (contour[i].x > maxx) { maxx = contour[i].x; }
		if (contour[i].x < minx) { minx = contour[i].x; }
		if (contour[i].y > maxy) { maxy = contour[i].y; }
	}
	miny = contour[0].y;
	width = maxx - minx + 1;
	height = maxy - miny + 1;
	//vector<Point> p;
//#pragma omp parallel for
	/*for (int i = 0; i < contour.size(); i++) {
		p.push_back(contour[i]);
	}*/
	//排序
//#pragma omp parallel for
	/*for (int i = 0; i < contour.size(); i++) {
		for (int j = 0; j < contour.size() - i - 1; j++) {
			if (contour[j + 1].x < contour[j].x) {
				Point temp = contour[j];
				contour[j] = contour[j + 1];
				contour[j + 1] = temp;
			}
		}
	}*/
	sort(contour.begin(), contour.end(),bmp);
	int sign = minx;
	for (int i = 0; i < contour.size();) {
		int max = 0, min = contour[i].y;
		for (int j = 0; j < height && i < contour.size(); j++) {
			if (contour[i].x > sign) { break; }
			if (contour[i].y > max) { max = contour[i].y; }
			if (contour[i].y < min) { min = contour[i].y; }
			i++;
		}
		area += max - min + 1;
		sign++;
		if (sign > maxx) { break; }
	}
	return area;
}

