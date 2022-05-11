#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include<vector>
#include<ctime>
#include <fstream>
#include<algorithm>
#include"Region_Selection.h"
#include"Quasi_Temporal_Diff.h"
#include"Count_Particles.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

int main() {
	auto start = system_clock::now();
	cv::Mat img1 = imread("D:/work/p2/p2/picture/1.bmp");	
	ofstream ofs;
	ofs.open("D:/work/p2/p2/picture/Pppparticle Number_all.txt", ios::trunc);
	ofs.close();
	cv::UMat img; img1.copyTo(img);
	Region_Selection(img);		
	auto end = system_clock::now();
	duration<double>diff = end - start;
	printf("程序总用时：%.6f\n", diff.count());
	system("pause");
	return 0;
}
