#include <iostream>
#include <opencv2/opencv.hpp>
#include<vector>
#include<algorithm>
#include <thread>
#include<ctime>
#include"Region_Selection.h"
#include"Quasi_Temporal_Diff.h"
#include "statistics.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

inline bool judge_cont(int cnt) {
	//筛选轮廓1:删除具有父或子轮廓的轮廓
	if (hierarchy[cnt][2] != -1 || hierarchy[cnt][3] != -1)
		return true;
	//筛选轮廓2:删除长度不符合的轮廓
	if (arcLength(contours[cnt], true) > max_size * MAGN || arcLength(contours[cnt], true) < min_size * MAGN)
		return true;
	/*//筛选轮廓3:面积
	double area = contourArea(contours[cnt]);
	if (area > max_area * MAGN * MAGN || min_area * MAGN * MAGN > area)
		return true;*/
		//筛选轮廓4:长宽比
	Rect rect = boundingRect(contours[cnt]);//返回最小外接矩形
	int w = rect.width, h = rect.height;
	double ratio = double(w) / double(h);//计算长宽比
	if (ratio - min_wh < 0 || ratio - max_wh>0)
		return true;
	return false;
}

bool vis[100001];
void Region_Selection(cv::UMat img) {
	auto start = system_clock::now();
	//---------------------------------------------------------------------处理图像------------------------------------------------------------------------------
	//灰度
	cv::UMat hsv, hsv_r;
	cvtColor(img, hsv, COLOR_BGR2HSV);//转为hsv图像
	inRange(hsv, Scalar(0, 200, 0), Scalar(120, 255, 255), hsv_r);//识别红色区域
//------------------------------------------------------筛选轮廓------------------------------------------------------
	findContours(hsv_r, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
	for (int i = 0; i < contours.size(); i++) {//去掉文字
		if (judge_cont(i)) {
			hierarchy.erase(hierarchy.begin() + i);
			contours.erase(contours.begin() + i);
			vis[i] = 1;
		}
	}
	cv::Mat ans; UMat ROI;//,mat1[200]
	clock_t start1, end1, a = 0;int id=0;
	for (int i = 0; i < contours.size(); i++) {
		if (!vis[i]) {
			Rect rect = boundingRect(contours[i]);
			img(rect).copyTo(ROI);
			pyrUp(ROI, ROI, Size(ROI.cols * 2, ROI.rows * 2));//放大
			GaussianBlur(ROI, ROI, Size(5, 5), 0, 0);
			start1 = clock();
			Q_Temp_Diff(ROI, ++id);
			end1 = clock();
			a += end1 - start1;
		}

	}
	//printf("Q_Temp_Diff：%.2f\n", double(a) / CLOCKS_PER_SEC);
	auto end = system_clock::now();
	duration<double>diff = end - start;
	//printf("Region_Selection：%.6f\n", diff.count());
}

