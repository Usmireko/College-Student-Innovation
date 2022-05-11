#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <fstream>
#include "opencv2/core/simd_intrinsics.hpp"
#include"Quasi_Temporal_Diff.h"
#include "Count_Particles.h"
#include "statistics.h"

using namespace std;
using namespace cv;

cv::Mat cut(cv::Mat src) {//切去外圈
	Mat dst1, dst2;
	for (int i = 1; i <= 3; i++)
		src.pop_back(1);
	dst1 = src.t();
	for (int i = 1; i <= 5; i++)
		dst1.pop_back(1);
	dst2 = dst1.t();
	//去掉左边和上边
	flip(dst2, dst1, -1);
	for (int i = 1; i <= 3; i++)
		dst1.pop_back(1);
	dst2 = dst1.t();
	for (int i = 1; i <= 6; i++) //此处5->6
		dst2.pop_back(1);
	dst1 = dst2.t();
	flip(dst1, src, -1);
	return src;
}

void qtd_all(cv::Mat A, cv::Mat& ans, cv::Mat& ans2, int l, int r, int u, int d) {
/*	cv::Mat ROI_rows = Mat::zeros(1, A.cols, CV_32F);
	cv::Mat ROI_cols = Mat::zeros(A.rows, 1, CV_32F);
	cv::Mat L, R, U, D,AL2,AR2,AD2,AU2;
#pragma omp sections
	{
#pragma omp section
		{
			hconcat(ROI_cols, A, R);
			R = R.t(); R.pop_back(1); R = R.t();
			AR2 = R - A;
			//absdiff(R, A, AR);
			if (r)ans2 = ans2 + AR2;//ans = ans + AR, 
		}
#pragma omp section
		{
			hconcat(A, ROI_cols, L);
			flip(L, L, 1); L = L.t(); L.pop_back(1); L = L.t(); flip(L, L, 1);
			AL2 = L - A;
			//absdiff(L, A, AL);
			if (l)ans2 = ans2 + AL2;//ans = ans + AL, 
		}
#pragma omp section
		{
			vconcat(ROI_rows, A, D);
			D.pop_back(1);
			AD2 = D - A;
			//absdiff(D, A, AD);
			if (d)ans2 = ans2 + AD2;//ans = ans + AD
		}
#pragma omp section
		{
			vconcat(A, ROI_rows, U);
			flip(U, U, 0); U.pop_back(1); flip(U, U, 0);
			AU2 = U - A;
			//absdiff(U, A, AU);
			if (u)ans2 = ans2 + AU2;//ans = ans + AU, 
		}
	}
*/
	cv::Mat ROI_rows = Mat::zeros(1, A.cols, CV_32F);
	cv::Mat ROI_cols = Mat::zeros(A.rows, 1, CV_32F);
	cv::Mat L, R, U, D;
	hconcat(ROI_cols, A, R);
	R = R.t(); R.pop_back(1); R = R.t();
	hconcat(A, ROI_cols, L);
	flip(L, L, 1); L = L.t(); L.pop_back(1); L = L.t(); flip(L, L, 1);
	vconcat(ROI_rows, A, D);
	D.pop_back(1);
	vconcat(A, ROI_rows, U);
	flip(U, U, 0); U.pop_back(1); flip(U, U, 0);

	cv::Mat AL2, AR2, AU2, AD2;
	AR2 = R - A;
	//absdiff(R, A, AR);
	AL2 = L - A;
	//absdiff(L, A, AL);
	AD2 = D - A;
	//absdiff(D, A, AD);
	AU2 = U - A;
	//absdiff(U, A, AU);
	if (l)ans2 = ans2 + AL2;//ans = ans + AL, 
	if (r)ans2 = ans2 + AR2;//ans = ans + AR, 
	if (u)ans2 = ans2 + AU2;//ans = ans + AU, 
	if (d)ans2 = ans2 + AD2;//ans = ans + AD, 

	//ans.convertTo(ans, CV_8U);
	//threshold(ans, ans, num, 255, THRESH_OTSU);       //二值化
//---------------------------------------------------------------伪帧差正值图-------------------------------------------------------------------------
//#pragma omp parallel for 
	for (int i = 0; i < ans2.rows; i++) {
		for (int j = 0; j < ans2.cols; j++) {
			if (ans2.ptr<float>(i)[j] < 0) {
				ans2.ptr<float>(i)[j] = 0;
			}
		}
	}
	ans2.convertTo(ans2, CV_8U);
	threshold(ans2, ans2, 0, 255, THRESH_OTSU);   //二值化
}

cv::Mat Q_Temp_Diff(cv::UMat img, int id) {
	clock_t start, end;
	start = clock();
	//---------------------------------------------------------------伪帧差----------------------------------------------------------------------------
	cv::UMat src;                                 //ans_mat1伪帧差 ans_mat2伪帧差正值图
	cvtColor(img, src, COLOR_BGR2GRAY);                              //灰度图像，转换为单通道
	cv::Mat A;
	src.convertTo(A, CV_32F);//转码为float
	cv::Mat ans = Mat::zeros(A.rows, A.cols, CV_32F);
	cv::Mat ans2 = Mat::zeros(A.rows, A.cols, CV_32F);
	qtd_all(A, ans, ans2, 1, 1, 1, 1);//A,二值化阈值，ans，ans2，左右上下开关
	//ans = cut(ans);
	ans2 = cut(ans2);
	//imwrite("D:/work/p2/p2/picture/haha"+to_string(id)+".bmp",ans2);
	//--------------------------------------------数数+写入文件----------------------------------------------//		
	int parnum = 0; //粒子个数
	int overparnum = 0;
	Counter(ans2, parnum, overparnum, id);
	ofstream ofs;
	ofs.open("D:/work/p2/p2/picture/Pppparticle Number_all.txt", ios::app);
	ofs << parnum << endl;
	ofs.close();
	end = clock();
	return ans2;
}