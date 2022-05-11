#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/highgui/highgui_c.h>
#include <fstream>
#include "statistics.h"
using namespace cv;
using namespace std;

//用于储存粒子信息

//int getContourData(vector<Point> contour);
//void draw_c(particalVector* container, int size, cv::Mat src);
//void OutPutHistogram(particalVector* container, int size);
/*
* src:二值图像 num:算法中计算的粒子数量，引用传参
*/
extern vector<vector<Point>> contours;

//contour为一个连通域一周的点集
//函数作用：引用返回外接矩形宽高，返回值为连通域的面积
static int getContourData(vector<Point> contour)
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
	vector<Point> p;
	for (int i = 0; i < contour.size(); i++) {
		p.push_back(contour[i]);
	}
	//排序
	for (int i = 0; i < p.size(); i++) {
		for (int j = 0; j < p.size() - i - 1; j++) {
			if (p[j + 1].x < p[j].x) {
				Point temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
		}
	}
	int sign = minx;
	for (int i = 0; i < p.size();) {
		int max = 0, min = p[i].y;
		for (int j = 0; j < height && i < p.size(); j++) {
			if (p[i].x > sign) { break; }
			if (p[i].y > max) { max = p[i].y; }
			if (p[i].y < min) { min = p[i].y; }
			i++;
		}
		area += max - min + 1;
		sign++;
		if (sign > maxx) { break; }
	}
	return area;
}

//参数列表：粒子信息容器，容器大小
//函数作用：输出统计信息
static void OutPutHistogram(particalVector* container, int size) {
	//-----------------------------------长宽比基础上的长边统计--------------------------------         
	/*int h[10][15];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			h[i][j] = 0;
		}
	}
	for (int i = 0; i < size; i++)
	{
		if (container[i].whrate == 1)
		{

			if (container[i].width < 2)
				h[0][0]++;
			else if (container[i].width < 3)
				h[0][1]++;
			else if (container[i].width < 4)
				h[0][2]++;
			else if (container[i].width < 5)
				h[0][3]++;
			else if (container[i].width < 6)
				h[0][4]++;
			else if (container[i].width < 7)
				h[0][5]++;
			else if (container[i].width < 8)
				h[0][6]++;
			else if (container[i].width < 9)
				h[0][7]++;
			else if (container[i].width < 10)
				h[0][8]++;
			else if (container[i].width < 11)
				h[0][9]++;
			else if (container[i].width < 12)
				h[0][10]++;
			else if (container[i].width < 13)
				h[0][11]++;
			else if (container[i].width < 14)
				h[0][12]++;
			else if (container[i].width < 15)
				h[0][13]++;
			else h[0][14]++;
		}
		if (container[i].whrate >= 1.1 && container[i].whrate < 1.3)
		{
			if (container[i].width < 2)
				h[1][0]++;
			else if (container[i].width < 3)
				h[1][1]++;
			else if (container[i].width < 4)
				h[1][2]++;
			else if (container[i].width < 5)
				h[1][3]++;
			else if (container[i].width < 6)
				h[1][4]++;
			else if (container[i].width < 7)
				h[1][5]++;
			else if (container[i].width < 8)
				h[1][6]++;
			else if (container[i].width < 9)
				h[1][7]++;
			else if (container[i].width < 10)
				h[1][8]++;
			else if (container[i].width < 11)
				h[1][9]++;
			else if (container[i].width < 12)
				h[1][10]++;
			else if (container[i].width < 13)
				h[1][11]++;
			else if (container[i].width < 14)
				h[1][12]++;
			else if (container[i].width < 15)
				h[1][13]++;
			else h[1][14]++;
		}
		if (container[i].whrate >= 1.3 && container[i].whrate < 1.4)
		{
			if (container[i].width < 2)
				h[2][0]++;
			else if (container[i].width < 3)
				h[2][1]++;
			else if (container[i].width < 4)
				h[2][2]++;
			else if (container[i].width < 5)
				h[2][3]++;
			else if (container[i].width < 6)
				h[2][4]++;
			else if (container[i].width < 7)
				h[2][5]++;
			else if (container[i].width < 8)
				h[2][6]++;
			else if (container[i].width < 9)
				h[2][7]++;
			else if (container[i].width < 10)
				h[2][8]++;
			else if (container[i].width < 11)
				h[2][9]++;
			else if (container[i].width < 12)
				h[2][10]++;
			else if (container[i].width < 13)
				h[2][11]++;
			else if (container[i].width < 14)
				h[2][12]++;
			else if (container[i].width < 15)
				h[2][13]++;
			else h[2][14]++;
		}
		if (container[i].whrate >= 1.4 && container[i].whrate <= 1.8)
		{
			if (container[i].width < 2)
				h[3][0]++;
			else if (container[i].width < 3)
				h[3][1]++;
			else if (container[i].width < 4)
				h[3][2]++;
			else if (container[i].width < 5)
				h[3][3]++;
			else if (container[i].width < 6)
				h[3][4]++;
			else if (container[i].width < 7)
				h[3][5]++;
			else if (container[i].width < 8)
				h[3][6]++;
			else if (container[i].width < 9)
				h[3][7]++;
			else if (container[i].width < 10)
				h[3][8]++;
			else if (container[i].width < 11)
				h[3][9]++;
			else if (container[i].width < 12)
				h[3][10]++;
			else if (container[i].width < 13)
				h[3][11]++;
			else if (container[i].width < 14)
				h[3][12]++;
			else if (container[i].width < 15)
				h[3][13]++;
			else h[3][14]++;
		}
		if (container[i].whrate >= 2.0 && container[i].whrate <= 2.1)
		{
			if (container[i].width < 2)
				h[4][0]++;
			else if (container[i].width < 3)
				h[4][1]++;
			else if (container[i].width < 4)
				h[4][2]++;
			else if (container[i].width < 5)
				h[4][3]++;
			else if (container[i].width < 6)
				h[4][4]++;
			else if (container[i].width < 7)
				h[4][5]++;
			else if (container[i].width < 8)
				h[4][6]++;
			else if (container[i].width < 9)
				h[4][7]++;
			else if (container[i].width < 10)
				h[4][8]++;
			else if (container[i].width < 11)
				h[4][9]++;
			else if (container[i].width < 12)
				h[4][10]++;
			else if (container[i].width < 13)
				h[4][11]++;
			else if (container[i].width < 14)
				h[4][12]++;
			else if (container[i].width < 15)
				h[4][13]++;
			else h[4][14]++;
		}
		if (container[i].whrate >= 2.3 && container[i].whrate <= 2.4)
		{
			if (container[i].width < 2)
				h[5][0]++;
			else if (container[i].width < 3)
				h[5][1]++;
			else if (container[i].width < 4)
				h[5][2]++;
			else if (container[i].width < 5)
				h[5][3]++;
			else if (container[i].width < 6)
				h[5][4]++;
			else if (container[i].width < 7)
				h[5][5]++;
			else if (container[i].width < 8)
				h[5][6]++;
			else if (container[i].width < 9)
				h[5][7]++;
			else if (container[i].width < 10)
				h[5][8]++;
			else if (container[i].width < 11)
				h[5][9]++;
			else if (container[i].width < 12)
				h[5][10]++;
			else if (container[i].width < 13)
				h[5][11]++;
			else if (container[i].width < 14)
				h[5][12]++;
			else if (container[i].width < 15)
				h[5][13]++;
			else h[5][14]++;
		}
		if (container[i].whrate >= 2.5 && container[i].whrate <= 2.6)
		{
			if (container[i].width < 2)
				h[6][0]++;
			else if (container[i].width < 3)
				h[6][1]++;
			else if (container[i].width < 4)
				h[6][2]++;
			else if (container[i].width < 5)
				h[6][3]++;
			else if (container[i].width < 6)
				h[6][4]++;
			else if (container[i].width < 7)
				h[6][5]++;
			else if (container[i].width < 8)
				h[6][6]++;
			else if (container[i].width < 9)
				h[6][7]++;
			else if (container[i].width < 10)
				h[6][8]++;
			else if (container[i].width < 11)
				h[6][9]++;
			else if (container[i].width < 12)
				h[6][10]++;
			else if (container[i].width < 13)
				h[6][11]++;
			else if (container[i].width < 14)
				h[6][12]++;
			else if (container[i].width < 15)
				h[6][13]++;
			else h[6][14]++;
		}
		if (container[i].whrate >= 3.0 && container[i].whrate <= 3.1)
		{
			if (container[i].width < 2)
				h[7][0]++;
			else if (container[i].width < 3)
				h[7][1]++;
			else if (container[i].width < 4)
				h[7][2]++;
			else if (container[i].width < 5)
				h[7][3]++;
			else if (container[i].width < 6)
				h[7][4]++;
			else if (container[i].width < 7)
				h[7][5]++;
			else if (container[i].width < 8)
				h[7][6]++;
			else if (container[i].width < 9)
				h[7][7]++;
			else if (container[i].width < 10)
				h[7][8]++;
			else if (container[i].width < 11)
				h[7][9]++;
			else if (container[i].width < 12)
				h[7][10]++;
			else if (container[i].width < 13)
				h[7][11]++;
			else if (container[i].width < 14)
				h[7][12]++;
			else if (container[i].width < 15)
				h[7][13]++;
			else h[7][14]++;
		}
		if (container[i].whrate >= 4.0 && container[i].whrate <= 4.1)
		{
			if (container[i].width < 2)
				h[8][0]++;
			else if (container[i].width < 3)
				h[8][1]++;
			else if (container[i].width < 4)
				h[8][2]++;
			else if (container[i].width < 5)
				h[8][3]++;
			else if (container[i].width < 6)
				h[8][4]++;
			else if (container[i].width < 7)
				h[8][5]++;
			else if (container[i].width < 8)
				h[8][6]++;
			else if (container[i].width < 9)
				h[8][7]++;
			else if (container[i].width < 10)
				h[8][8]++;
			else if (container[i].width < 11)
				h[8][9]++;
			else if (container[i].width < 12)
				h[8][10]++;
			else if (container[i].width < 13)
				h[8][11]++;
			else if (container[i].width < 14)
				h[8][12]++;
			else if (container[i].width < 15)
				h[8][13]++;
			else h[8][14]++;
		}
		if (container[i].whrate >= 5.0 && container[i].whrate <= 5.1)
		{
			if (container[i].width < 2)
				h[9][0]++;
			else if (container[i].width < 3)
				h[9][1]++;
			else if (container[i].width < 4)
				h[9][2]++;
			else if (container[i].width < 5)
				h[9][3]++;
			else if (container[i].width < 6)
				h[9][4]++;
			else if (container[i].width < 7)
				h[9][5]++;
			else if (container[i].width < 8)
				h[9][6]++;
			else if (container[i].width < 9)
				h[9][7]++;
			else if (container[i].width < 10)
				h[9][8]++;
			else if (container[i].width < 11)
				h[9][9]++;
			else if (container[i].width < 12)
				h[9][10]++;
			else if (container[i].width < 13)
				h[9][11]++;
			else if (container[i].width < 14)
				h[9][12]++;
			else if (container[i].width < 15)
				h[9][13]++;
			else h[9][14]++;
		}
	}
	cout << "长宽比为1的粒子长边长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[0][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[0][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[0][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[0][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[0][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[0][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[0][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[0][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[0][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[0][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[0][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[0][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[0][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[0][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[0][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[1.1, 1.3)粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[1][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[1][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[1][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[1][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[1][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[1][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[1][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[1][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[1][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[1][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[1][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[1][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[1][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[1][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[1][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[1.3, 1.4)粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[2][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[2][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[2][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[2][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[2][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[2][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[2][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[2][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[2][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[2][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[2][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[2][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[2][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[2][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[2][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[1.4, 1.8)粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[3][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[3][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[3][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[3][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[3][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[3][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[3][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[3][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[3][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[3][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[3][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[3][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[3][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[3][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[3][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[2.0, 2.1]粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[4][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[4][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[4][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[4][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[4][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[4][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[4][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[4][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[4][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[4][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[4][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[4][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[4][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[4][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[4][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[2.3, 2.4]粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[5][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[5][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[5][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[5][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[5][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[5][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[5][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[5][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[5][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[5][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[5][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[5][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[5][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[5][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[5][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[2.5, 2.6]粒子长度：" << endl;
	cout << "位于[2, 3)" << ":" << h[6][0] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[6][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[6][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[6][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[6][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[6][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[6][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[6][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[6][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[6][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[6][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[6][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[6][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[6][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[3.0, 3.1]粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[7][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[7][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[7][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[7][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[7][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[7][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[7][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[7][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[7][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[7][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[7][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[7][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[7][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[7][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[7][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[4.0, 4.1]粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[8][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[8][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[8][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[8][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[8][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[8][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[8][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[8][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[8][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[8][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[8][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[8][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[8][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[8][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[8][14] << "个" << endl;
	cout << "------------------------------------------" << endl;
	cout << "长宽比[5.0, 5.1]粒子长度：" << endl;
	cout << "位于(-∞, 2)" << ":" << h[9][0] << "个" << endl;
	cout << "位于[2, 3)" << ":" << h[9][1] << "个" << endl;
	cout << "位于[3, 4)" << ":" << h[9][2] << "个" << endl;
	cout << "位于[4, 5)" << ":" << h[9][3] << "个" << endl;
	cout << "位于[5, 6)" << ":" << h[9][4] << "个" << endl;
	cout << "位于[6, 7)" << ":" << h[9][5] << "个" << endl;
	cout << "位于[7, 8)" << ":" << h[9][6] << "个" << endl;
	cout << "位于[8, 9)" << ":" << h[9][7] << "个" << endl;
	cout << "位于[9, 10)" << ":" << h[9][8] << "个" << endl;
	cout << "位于[10, 11)" << ":" << h[9][9] << "个" << endl;
	cout << "位于[11, 12)" << ":" << h[9][10] << "个" << endl;
	cout << "位于[12, 13)" << ":" << h[9][11] << "个" << endl;
	cout << "位于[13, 14)" << ":" << h[9][12] << "个" << endl;
	cout << "位于[14, 15)" << ":" << h[9][13] << "个" << endl;
	cout << "位于[15, +∞)" << ":" << h[9][14] << "个" << endl;
	cout << "------------------------------------------" << endl;*/
	//-----------------------------------长宽比基础上的短边统计--------------------------------     
	/*int h[10][8];
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				h[i][j] = 0;
			}
		}
		for (int i = 0; i < size; i++)
		{
			if (container[i].whrate == 1)
			{
				if (container[i].height < 1)
					h[0][0]++;
				else if (container[i].height < 2)
					h[0][1]++;
				else if (container[i].height < 3)
					h[0][2]++;
				else if (container[i].height < 4)
					h[0][3]++;
				else if (container[i].height < 5)
					h[0][4]++;
				else if (container[i].height < 6)
					h[0][5]++;
				else if (container[i].height < 7)
					h[0][6]++;
				else h[0][7]++;
			}
			if (container[i].whrate >= 1.1 && container[i].whrate < 1.3)
			{
				if (container[i].height < 1)
					h[1][0]++;
				else if (container[i].height < 2)
					h[1][1]++;
				else if (container[i].height < 3)
					h[1][2]++;
				else if (container[i].height < 4)
					h[1][3]++;
				else if (container[i].height < 5)
					h[1][4]++;
				else if (container[i].height < 6)
					h[1][5]++;
				else if (container[i].height < 7)
					h[1][6]++;
				else h[1][7]++;
			}
			if (container[i].whrate >= 1.3 && container[i].whrate < 1.4)
			{
				if (container[i].height < 1)
					h[2][0]++;
				else if (container[i].height < 2)
					h[2][1]++;
				else if (container[i].height < 3)
					h[2][2]++;
				else if (container[i].height < 4)
					h[2][3]++;
				else if (container[i].height < 5)
					h[2][4]++;
				else if (container[i].height < 6)
					h[2][5]++;
				else if (container[i].height < 7)
					h[2][6]++;
				else h[2][7]++;
			}
			if (container[i].whrate >= 1.4 && container[i].whrate <= 1.8)
			{
				if (container[i].height < 1)
					h[3][0]++;
				else if (container[i].height < 2)
					h[3][1]++;
				else if (container[i].height < 3)
					h[3][2]++;
				else if (container[i].height < 4)
					h[3][3]++;
				else if (container[i].height < 5)
					h[3][4]++;
				else if (container[i].height < 6)
					h[3][5]++;
				else if (container[i].height < 7)
					h[3][6]++;
				else h[3][7]++;
			}
			if (container[i].whrate >= 2.0 && container[i].whrate <= 2.1)
			{
				if (container[i].height < 1)
					h[4][0]++;
				else if (container[i].height < 2)
					h[4][1]++;
				else if (container[i].height < 3)
					h[4][2]++;
				else if (container[i].height < 4)
					h[4][3]++;
				else if (container[i].height < 5)
					h[4][4]++;
				else if (container[i].height < 6)
					h[4][5]++;
				else if (container[i].height < 7)
					h[4][6]++;
				else h[4][7]++;
			}
			if (container[i].whrate >= 2.3 && container[i].whrate <= 2.4)
			{
				if (container[i].height < 1)
					h[5][0]++;
				else if (container[i].height < 2)
					h[5][1]++;
				else if (container[i].height < 3)
					h[5][2]++;
				else if (container[i].height < 4)
					h[5][3]++;
				else if (container[i].height < 5)
					h[5][4]++;
				else if (container[i].height < 6)
					h[5][5]++;
				else if (container[i].height < 7)
					h[5][6]++;
				else h[5][7]++;
			}
			if (container[i].whrate >= 2.5 && container[i].whrate <= 2.6)
			{
				if (container[i].height < 1)
					h[6][0]++;
				else if (container[i].height < 2)
					h[6][1]++;
				else if (container[i].height < 3)
					h[6][2]++;
				else if (container[i].height < 4)
					h[6][3]++;
				else if (container[i].height < 5)
					h[6][4]++;
				else if (container[i].height < 6)
					h[6][5]++;
				else if (container[i].height < 7)
					h[6][6]++;
				else h[6][7]++;
			}
			if (container[i].whrate >= 3.0 && container[i].whrate <= 3.1)
			{
				if (container[i].height < 1)
					h[7][0]++;
				else if (container[i].height < 2)
					h[7][1]++;
				else if (container[i].height < 3)
					h[7][2]++;
				else if (container[i].height < 4)
					h[7][3]++;
				else if (container[i].height < 5)
					h[7][4]++;
				else if (container[i].height < 6)
					h[7][5]++;
				else if (container[i].height < 7)
					h[7][6]++;
				else h[7][7]++;
			}
			if (container[i].whrate >= 4.0 && container[i].whrate <= 4.1)
			{
				if (container[i].height < 1)
					h[8][0]++;
				else if (container[i].height < 2)
					h[8][1]++;
				else if (container[i].height < 3)
					h[8][2]++;
				else if (container[i].height < 4)
					h[8][3]++;
				else if (container[i].height < 5)
					h[8][4]++;
				else if (container[i].height < 6)
					h[8][5]++;
				else if (container[i].height < 7)
					h[8][6]++;
				else h[8][7]++;
			}
			if (container[i].whrate >= 5.0 && container[i].whrate <= 5.1)
			{
				if (container[i].height < 1)
					h[9][0]++;
				else if (container[i].height < 2)
					h[9][1]++;
				else if (container[i].height < 3)
					h[9][2]++;
				else if (container[i].height < 4)
					h[9][3]++;
				else if (container[i].height < 5)
					h[9][4]++;
				else if (container[i].height < 6)
					h[9][5]++;
				else if (container[i].height < 7)
					h[9][6]++;
				else h[9][7]++;
			}
		}
		cout << "长宽比为1的粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[0][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[0][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[0][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[0][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[0][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[0][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[0][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[0][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[1.1, 1.3)粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[1][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[1][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[1][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[1][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[1][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[1][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[1][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[1][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[1.3, 1.4)粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[2][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[2][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[2][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[2][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[2][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[2][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[2][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[2][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[1.4, 1.8)粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[3][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[3][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[3][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[3][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[3][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[3][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[3][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[3][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[2.0, 2.1]粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[4][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[4][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[4][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[4][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[4][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[4][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[4][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[4][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[2.3, 2.4]粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[5][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[5][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[5][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[5][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[5][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[5][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[5][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[5][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[2.5, 2.6]粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[6][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[6][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[6][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[6][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[6][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[6][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[6][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[6][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[3.0, 3.1]粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[7][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[7][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[7][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[7][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[7][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[7][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[7][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[7][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[4.0, 4.1]粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[8][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[8][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[8][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[8][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[8][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[8][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[8][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[8][7] << "个" << endl;
		cout << "------------------------------------------" << endl;
		cout << "长宽比[5.0, 5.1]粒子短边长度：" << endl;
		cout << "位于(-∞, 1)" << ":" << h[9][0] << "个" << endl;
		cout << "位于[1, 2)" << ":" << h[9][1] << "个" << endl;
		cout << "位于[2, 3)" << ":" << h[9][2] << "个" << endl;
		cout << "位于[3, 4)" << ":" << h[9][3] << "个" << endl;
		cout << "位于[4, 5)" << ":" << h[9][4] << "个" << endl;
		cout << "位于[5, 6)" << ":" << h[9][5] << "个" << endl;
		cout << "位于[6, 7)" << ":" << h[9][6] << "个" << endl;
		cout << "位于[7, +∞)" << ":" << h[9][7] << "个" << endl;
		cout << "------------------------------------------" << endl;*/
		//-----------------------------------长宽比基础上的面积统计--------------------------------     
		/*int S[10][30];
		int s[10];
		for (int i = 0; i < 10; i++) {s[i] = 0;}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				S[i][j] = 0;
			}
		}
		for (int i = 0; i < size; i++)
		{
			if (container[i].whrate == 1 )//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[0][7]++; break;
			case 9:S[0][8]++; break;
			case 10:S[0][9]++; break;
			case 11:S[0][10]++; break;
			case 12:S[0][11]++; break;
			case 13:S[0][12]++; break;
			case 14:S[0][13]++; break;
			case 15:S[0][14]++; break;
			case 16:S[0][15]++; break;
			case 17:S[0][16]++; break;
			case 18:S[0][17]++; break;
			case 19:S[0][18]++; break;
			case 20:S[0][19]++; break;
			case 1:S[0][0]++; break;
			case 2:S[0][1]++; break;
			case 3:S[0][2]++; break;
			case 4:S[0][3]++; break;
			case 5:S[0][4]++; break;
			case 6:S[0][5]++; break;
			case 7:S[0][6]++; break;
			case 21:S[0][20]++; break;
			case 22:S[0][21]++; break;
			case 23:S[0][22]++; break;
			case 24:S[0][23]++; break;
			case 25:S[0][24]++; break;
			case 26:S[0][25]++; break;
			case 27:S[0][26]++; break;
			case 28:S[0][27]++; break;
			case 29:S[0][28]++; break;
			case 30:S[0][29]++; break;
			default:
				s[0]++;
			}
			if (container[i].whrate >= 1.1 && container[i].whrate < 1.3)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[1][7]++; break;
			case 9:S[1][8]++; break;
			case 10:S[1][9]++; break;
			case 11:S[1][10]++; break;
			case 12:S[1][11]++; break;
			case 13:S[1][12]++; break;
			case 14:S[1][13]++; break;
			case 15:S[1][14]++; break;
			case 16:S[1][15]++; break;
			case 17:S[1][16]++; break;
			case 18:S[1][17]++; break;
			case 19:S[1][18]++; break;
			case 20:S[1][19]++; break;
			case 1:S[1][0]++; break;
			case 2:S[1][1]++; break;
			case 3:S[1][2]++; break;
			case 4:S[1][3]++; break;
			case 5:S[1][4]++; break;
			case 6:S[1][5]++; break;
			case 7:S[1][6]++; break;
			case 21:S[1][20]++; break;
			case 22:S[1][21]++; break;
			case 23:S[1][22]++; break;
			case 24:S[1][23]++; break;
			case 25:S[1][24]++; break;
			case 26:S[1][25]++; break;
			case 27:S[1][26]++; break;
			case 28:S[1][27]++; break;
			case 29:S[1][28]++; break;
			case 30:S[1][29]++; break;
			default:
				s[1]++;
			}
			if (container[i].whrate >= 1.3 && container[i].whrate < 1.4)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[2][7]++; break;
			case 9:S[2][8]++; break;
			case 10:S[2][9]++; break;
			case 11:S[2][10]++; break;
			case 12:S[2][11]++; break;
			case 13:S[2][12]++; break;
			case 14:S[2][13]++; break;
			case 15:S[2][14]++; break;
			case 16:S[2][15]++; break;
			case 17:S[2][16]++; break;
			case 18:S[2][17]++; break;
			case 19:S[2][18]++; break;
			case 20:S[2][19]++; break;
			case 1:S[2][0]++; break;
			case 2:S[2][1]++; break;
			case 3:S[2][2]++; break;
			case 4:S[2][3]++; break;
			case 5:S[2][4]++; break;
			case 6:S[2][5]++; break;
			case 7:S[2][6]++; break;
			case 21:S[2][20]++; break;
			case 22:S[2][21]++; break;
			case 23:S[2][22]++; break;
			case 24:S[2][23]++; break;
			case 25:S[2][24]++; break;
			case 26:S[2][25]++; break;
			case 27:S[2][26]++; break;
			case 28:S[2][27]++; break;
			case 29:S[2][28]++; break;
			case 30:S[2][29]++; break;
			default:
				s[2]++;
			}
			if (container[i].whrate >= 1.4 && container[i].whrate <= 1.8)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[3][7]++; break;
			case 9:S[3][8]++; break;
			case 10:S[3][9]++; break;
			case 11:S[3][10]++; break;
			case 12:S[3][11]++; break;
			case 13:S[3][12]++; break;
			case 14:S[3][13]++; break;
			case 15:S[3][14]++; break;
			case 16:S[3][15]++; break;
			case 17:S[3][16]++; break;
			case 18:S[3][17]++; break;
			case 19:S[3][18]++; break;
			case 20:S[3][19]++; break;
			case 1:S[3][0]++; break;
			case 2:S[3][1]++; break;
			case 3:S[3][2]++; break;
			case 4:S[3][3]++; break;
			case 5:S[3][4]++; break;
			case 6:S[3][5]++; break;
			case 7:S[3][6]++; break;
			case 21:S[3][20]++; break;
			case 22:S[3][21]++; break;
			case 23:S[3][22]++; break;
			case 24:S[3][23]++; break;
			case 25:S[3][24]++; break;
			case 26:S[3][25]++; break;
			case 27:S[3][26]++; break;
			case 28:S[3][27]++; break;
			case 29:S[3][28]++; break;
			case 30:S[3][29]++; break;
			default:
				s[3]++;
			}
			if (container[i].whrate >= 2.0 && container[i].whrate <= 2.1)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[4][7]++; break;
			case 9:S[4][8]++; break;
			case 10:S[4][9]++; break;
			case 11:S[4][10]++; break;
			case 12:S[4][11]++; break;
			case 13:S[4][12]++; break;
			case 14:S[4][13]++; break;
			case 15:S[4][14]++; break;
			case 16:S[4][15]++; break;
			case 17:S[4][16]++; break;
			case 18:S[4][17]++; break;
			case 19:S[4][18]++; break;
			case 20:S[4][19]++; break;
			case 1:S[4][0]++; break;
			case 2:S[4][1]++; break;
			case 3:S[4][2]++; break;
			case 4:S[4][3]++; break;
			case 5:S[4][4]++; break;
			case 6:S[4][5]++; break;
			case 7:S[4][6]++; break;
			case 21:S[4][20]++; break;
			case 22:S[4][21]++; break;
			case 23:S[4][22]++; break;
			case 24:S[4][23]++; break;
			case 25:S[4][24]++; break;
			case 26:S[4][25]++; break;
			case 27:S[4][26]++; break;
			case 28:S[4][27]++; break;
			case 29:S[4][28]++; break;
			case 30:S[4][29]++; break;
			default:
				s[4]++;
			}
			if (container[i].whrate >= 2.3 && container[i].whrate <= 2.4)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[5][7]++; break;
			case 9:S[5][8]++; break;
			case 10:S[5][9]++; break;
			case 11:S[5][10]++; break;
			case 12:S[5][11]++; break;
			case 13:S[5][12]++; break;
			case 14:S[5][13]++; break;
			case 15:S[5][14]++; break;
			case 16:S[5][15]++; break;
			case 17:S[5][16]++; break;
			case 18:S[5][17]++; break;
			case 19:S[5][18]++; break;
			case 20:S[5][19]++; break;
			case 1:S[5][0]++; break;
			case 2:S[5][1]++; break;
			case 3:S[5][2]++; break;
			case 4:S[5][3]++; break;
			case 5:S[5][4]++; break;
			case 6:S[5][5]++; break;
			case 7:S[5][6]++; break;
			case 21:S[5][20]++; break;
			case 22:S[5][21]++; break;
			case 23:S[5][22]++; break;
			case 24:S[5][23]++; break;
			case 25:S[5][24]++; break;
			case 26:S[5][25]++; break;
			case 27:S[5][26]++; break;
			case 28:S[5][27]++; break;
			case 29:S[5][28]++; break;
			case 30:S[5][29]++; break;
			default:
				s[5]++;
			}
			if (container[i].whrate >= 2.5 && container[i].whrate <= 2.6)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[6][7]++; break;
			case 9:S[6][8]++; break;
			case 10:S[6][9]++; break;
			case 11:S[6][10]++; break;
			case 12:S[6][11]++; break;
			case 13:S[6][12]++; break;
			case 14:S[6][13]++; break;
			case 15:S[6][14]++; break;
			case 16:S[6][15]++; break;
			case 17:S[6][16]++; break;
			case 18:S[6][17]++; break;
			case 19:S[6][18]++; break;
			case 20:S[6][19]++; break;
			case 1:S[6][0]++; break;
			case 2:S[6][1]++; break;
			case 3:S[6][2]++; break;
			case 4:S[6][3]++; break;
			case 5:S[6][4]++; break;
			case 6:S[6][5]++; break;
			case 7:S[6][6]++; break;
			case 21:S[6][20]++; break;
			case 22:S[6][21]++; break;
			case 23:S[6][22]++; break;
			case 24:S[6][23]++; break;
			case 25:S[6][24]++; break;
			case 26:S[6][25]++; break;
			case 27:S[6][26]++; break;
			case 28:S[6][27]++; break;
			case 29:S[6][28]++; break;
			case 30:S[6][29]++; break;
			default:
				s[6]++;
			}
			if (container[i].whrate >= 3.0 && container[i].whrate <= 3.1)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[7][7]++; break;
			case 9:S[7][8]++; break;
			case 10:S[7][9]++; break;
			case 11:S[7][10]++; break;
			case 12:S[7][11]++; break;
			case 13:S[7][12]++; break;
			case 14:S[7][13]++; break;
			case 15:S[7][14]++; break;
			case 16:S[7][15]++; break;
			case 17:S[7][16]++; break;
			case 18:S[7][17]++; break;
			case 19:S[7][18]++; break;
			case 20:S[7][19]++; break;
			case 1:S[7][0]++; break;
			case 2:S[7][1]++; break;
			case 3:S[7][2]++; break;
			case 4:S[7][3]++; break;
			case 5:S[7][4]++; break;
			case 6:S[7][5]++; break;
			case 7:S[7][7]++; break;
			case 21:S[7][20]++; break;
			case 22:S[7][21]++; break;
			case 23:S[7][22]++; break;
			case 24:S[7][23]++; break;
			case 25:S[7][24]++; break;
			case 26:S[7][25]++; break;
			case 27:S[7][26]++; break;
			case 28:S[7][27]++; break;
			case 29:S[7][28]++; break;
			case 30:S[7][29]++; break;
			default:
				s[7]++;
			}
			if (container[i].whrate >= 4.0 && container[i].whrate <= 4.1)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[8][7]++; break;
			case 9:S[8][8]++; break;
			case 10:S[8][9]++; break;
			case 11:S[8][10]++; break;
			case 12:S[8][11]++; break;
			case 13:S[8][12]++; break;
			case 14:S[8][13]++; break;
			case 15:S[8][14]++; break;
			case 16:S[8][15]++; break;
			case 17:S[8][16]++; break;
			case 18:S[8][17]++; break;
			case 19:S[8][18]++; break;
			case 20:S[8][19]++; break;
			case 1:S[8][0]++; break;
			case 2:S[8][1]++; break;
			case 3:S[8][2]++; break;
			case 4:S[8][3]++; break;
			case 5:S[8][4]++; break;
			case 6:S[8][5]++; break;
			case 7:S[8][6]++; break;
			case 21:S[8][20]++; break;
			case 22:S[8][21]++; break;
			case 23:S[8][22]++; break;
			case 24:S[8][23]++; break;
			case 25:S[8][24]++; break;
			case 26:S[8][25]++; break;
			case 27:S[8][26]++; break;
			case 28:S[8][27]++; break;
			case 29:S[8][28]++; break;
			case 30:S[8][29]++; break;
			default:
				s[8]++;
			}

			if (container[i].whrate >= 5.0 && container[i].whrate <= 5.1)//条件判断
			//面积统计
			switch (container[i].square)
			{
			case 8:S[9][7]++; break;
			case 9:S[9][8]++; break;
			case 10:S[9][9]++; break;
			case 11:S[9][10]++; break;
			case 12:S[9][11]++; break;
			case 13:S[9][12]++; break;
			case 14:S[9][13]++; break;
			case 15:S[9][14]++; break;
			case 16:S[9][15]++; break;
			case 17:S[9][16]++; break;
			case 18:S[9][17]++; break;
			case 19:S[9][18]++; break;
			case 20:S[9][19]++; break;
			case 1:S[9][0]++; break;
			case 2:S[9][1]++; break;
			case 3:S[9][2]++; break;
			case 4:S[9][3]++; break;
			case 5:S[9][4]++; break;
			case 6:S[9][5]++; break;
			case 7:S[9][6]++; break;
			case 21:S[9][20]++; break;
			case 22:S[9][21]++; break;
			case 23:S[9][22]++; break;
			case 24:S[9][23]++; break;
			case 25:S[9][24]++; break;
			case 26:S[9][25]++; break;
			case 27:S[9][26]++; break;
			case 28:S[9][27]++; break;
			case 29:S[9][28]++; break;
			case 30:S[9][29]++; break;
			default:
				s[9]++;
			}
		}
		cout << "长宽比为1的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[0][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[0] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比[1.1, 1.3]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[1][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[1] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比(1.3, 1.4]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[2][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[2] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比(1.4, 1.8]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[3][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[3] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比[2.0, 2.1]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[4][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[4] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比[2.3, 2.4]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[5][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[5] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比(2.5, 2.6)的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[6][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[6] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比[3.0, 3.1]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[7][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[7] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比[4.0, 4.1]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[8][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[8] << endl;
		cout << "------------------------------------------" << endl;

		cout << "长宽比[5.0, 5.1]的粒子面积：" << endl;
		for (int j = 0; j < 30; j++) {
			cout << "面积为" << j + 1 << ":" << S[9][j] << "个" << endl;
		}
		cout << "面积大于30：" << s[9] << endl;
		cout << "------------------------------------------" << endl;*/
		//------------------------------------长宽比-----------------------------------------------------
		/*int R0[92];
		for (int i = 0; i < 92; i++) { R0[i] = 0; }
		for (int i = 0; i < size; i++) {
			if (container[i].whrate == 1.0)
				R0[0]++;
			else if (container[i].whrate < 1.1)
				R0[1]++;
			else if (container[i].whrate < 1.2)
				R0[2]++;
			else if (container[i].whrate < 1.3)
				R0[3]++;
			else if (container[i].whrate < 1.4)
				R0[4]++;
			else if (container[i].whrate < 1.5)
				R0[5]++;
			else if (container[i].whrate < 1.6)
				R0[6]++;
			else if (container[i].whrate < 1.7)
				R0[7]++;
			else if (container[i].whrate < 1.8)
				R0[8]++;
			else if (container[i].whrate < 1.9)
				R0[9]++;
			else if (container[i].whrate < 2.0)
				R0[10]++;
			else if (container[i].whrate < 2.1)
				R0[11]++;
			else if (container[i].whrate < 2.2)
				R0[12]++;
			else if (container[i].whrate < 2.3)
				R0[13]++;
			else if (container[i].whrate < 2.4)
				R0[14]++;
			else if (container[i].whrate < 2.5)
				R0[15]++;
			else if (container[i].whrate < 2.6)
				R0[16]++;
			else if (container[i].whrate < 2.7)
				R0[17]++;
			else if (container[i].whrate < 2.8)
				R0[18]++;
			else if (container[i].whrate < 2.9)
				R0[19]++;
			else if (container[i].whrate < 3.0)
				R0[20]++;
			else if (container[i].whrate < 3.1)
				R0[21]++;
			else if (container[i].whrate < 3.2)
				R0[22]++;
			else if (container[i].whrate < 3.3)
				R0[23]++;
			else if (container[i].whrate < 3.4)
				R0[24]++;
			else if (container[i].whrate < 3.5)
				R0[25]++;
			else if (container[i].whrate < 3.6)
				R0[26]++;
			else if (container[i].whrate < 3.7)
				R0[27]++;
			else if (container[i].whrate < 3.8)
				R0[28]++;
			else if (container[i].whrate < 3.9)
				R0[29]++;
			else if (container[i].whrate < 4.0)
				R0[30]++;
			else if (container[i].whrate < 4.1)
				R0[31]++;
			else if (container[i].whrate < 4.2)
				R0[32]++;
			else if (container[i].whrate < 4.3)
				R0[33]++;
			else if (container[i].whrate < 4.4)
				R0[34]++;
			else if (container[i].whrate < 4.5)
				R0[35]++;
			else if (container[i].whrate < 4.6)
				R0[36]++;
			else if (container[i].whrate < 4.7)
				R0[37]++;
			else if (container[i].whrate < 4.8)
				R0[38]++;
			else if (container[i].whrate < 4.9)
				R0[39]++;
			else if (container[i].whrate < 5.0)
				R0[40]++;
			else if (container[i].whrate < 5.1)
				R0[41]++;
			else if (container[i].whrate < 5.2)
				R0[42]++;
			else if (container[i].whrate < 5.3)
				R0[43]++;
			else if (container[i].whrate < 5.4)
				R0[44]++;
			else if (container[i].whrate < 5.5)
				R0[45]++;
			else if (container[i].whrate < 5.6)
				R0[46]++;
			else if (container[i].whrate < 5.7)
				R0[47]++;
			else if (container[i].whrate < 5.8)
				R0[48]++;
			else if (container[i].whrate < 5.9)
				R0[49]++;
			else if (container[i].whrate < 6.0)
				R0[50]++;
			else if (container[i].whrate < 6.1)
				R0[51]++;
			else if (container[i].whrate < 6.2)
				R0[52]++;
			else if (container[i].whrate < 6.3)
				R0[53]++;
			else if (container[i].whrate < 6.4)
				R0[54]++;
			else if (container[i].whrate < 6.5)
				R0[55]++;
			else if (container[i].whrate < 6.6)
				R0[56]++;
			else if (container[i].whrate < 6.7)
				R0[57]++;
			else if (container[i].whrate < 6.8)
				R0[58]++;
			else if (container[i].whrate < 6.9)
				R0[59]++;
			else if (container[i].whrate < 7.0)
				R0[60]++;
			else if (container[i].whrate < 7.1)
				R0[61]++;
			else if (container[i].whrate < 7.2)
				R0[62]++;
			else if (container[i].whrate < 7.3)
				R0[63]++;
			else if (container[i].whrate < 7.4)
				R0[64]++;
			else if (container[i].whrate < 7.5)
				R0[65]++;
			else if (container[i].whrate < 7.6)
				R0[66]++;
			else if (container[i].whrate < 7.7)
				R0[67]++;
			else if (container[i].whrate < 7.8)
				R0[68]++;
			else if (container[i].whrate < 7.9)
				R0[69]++;
			else if (container[i].whrate < 8.0)
				R0[70]++;
			else if (container[i].whrate < 8.1)
				R0[71]++;
			else if (container[i].whrate < 8.2)
				R0[72]++;
			else if (container[i].whrate < 8.3)
				R0[73]++;
			else if (container[i].whrate < 8.4)
				R0[74]++;
			else if (container[i].whrate < 8.5)
				R0[75]++;
			else if (container[i].whrate < 8.6)
				R0[76]++;
			else if (container[i].whrate < 8.7)
				R0[77]++;
			else if (container[i].whrate < 8.8)
				R0[78]++;
			else if (container[i].whrate < 8.9)
				R0[79]++;
			else if (container[i].whrate < 9.0)
				R0[80]++;
			else if (container[i].whrate < 9.1)
				R0[81]++;
			else if (container[i].whrate < 9.2)
				R0[82]++;
			else if (container[i].whrate < 9.3)
				R0[83]++;
			else if (container[i].whrate < 9.4)
				R0[84]++;
			else if (container[i].whrate < 9.5)
				R0[85]++;
			else if (container[i].whrate < 9.6)
				R0[86]++;
			else if (container[i].whrate < 9.7)
				R0[87]++;
			else if (container[i].whrate < 9.8)
				R0[88]++;
			else if (container[i].whrate < 9.9)
				R0[89]++;
			else if (container[i].whrate < 10.0)
				R0[90]++;
			else { R0[91]++; }
		}*/

		/* ofstream ofs;

		 ofs.open("C:/picture/1/whrate.txt", ios::app);

		 for (int i = 0;i < 92;i++)
		 {
			 ofs << R0[i] << endl;
		 }
		 ofs.close();*/
}

static void draw_c(particalVector* container, int size, cv::Mat src)
{
	double l[12] = { 1, 1.1, 1.3, 1.4, 2.0, 2.3, 2.5, 3.0, 4.0, 5.0 };
	double r[12] = { 1, 1.2999, 1.3999, 1.8, 2.1, 2.4, 2.6, 3.1, 4.1, 5.1 };
	static vector<vector<Point>> contourss[12];
	vector<vector<Point>> con0;
	vector<vector<Point>> con1;
	vector<vector<Point>> con2;
	vector<vector<Point>> con3;
	vector<vector<Point>> con4;
	vector<vector<Point>> con5;
	vector<vector<Point>> con6;
	vector<vector<Point>> con7;
	vector<vector<Point>> con8;
	vector<vector<Point>> con9;
	vector<vector<Point>> con10;
	vector<vector<Point>> con11;
	vector<vector<Point>> con12;
	vector<vector<Point>> con13;
	vector<vector<Point>> con14;
	static vector<vector<Point>> contours_c[12][17];
	static vector<vector<Point>> contours_d[12][10];
	static vector<vector<Point>> contours_s[12][32];
	//for (int i = 0; i < 10; i++)
	//{
		// 长宽比
		/*for (int j = 0; j < size; j++)
		{
			if (container[j].whrate >= l[i] && container[j].whrate <= r[i])
			{
				contourss[i].push_back(contours[j]);
			}
		}*/
		// 长边
		/*for (int j = 0; j < size; j++)
		{
			if (container[j].whrate >= l[i] && container[j].whrate <= r[i])
			{
				for (int k = 1; k < 16; k++)
				{

					if ((k + 1) > 15)
					{
						contours_c[i][k].push_back(contours[j]);
					}
					else if (container[j].width >= k && container[j].width < (k + 1))
					{
						contours_c[i][k].push_back(contours[j]);
					}
				}
			}
		}*/
		// 短边
		/*for (int j = 0; j < size; j++)
		{
			if (container[j].whrate >= l[i] && container[j].whrate <= r[i])
			{
				for (int k = 0; k < 8; k++)
				{

					if ((k + 1) > 7)
					{
						contours_d[i][k].push_back(contours[j]);
					}
					else if (container[j].height >= k && container[j].height < (k + 1))
					{
						contours_d[i][k].push_back(contours[j]);
					}
				}
			}
		}*/
		// 面积
		/*for (int j = 0; j < size; j++)
		{
			if (container[j].whrate >= l[i] && container[j].whrate <= r[i])
			{
				for (int k = 1; k < 31; k++)
				{

					if ((k + 1) > 30)
					{
						contours_s[i][k].push_back(contours[j]);
					}
					else if (container[j].square == k)
					{
						contours_s[i][k].push_back(contours[j]);
					}
				}
			}
		}*/

		//}
	for (int j = 0; j < size; j++)
	{
		if (container[j].width >= 8)
		{
			if ((container[j].whrate > 1.0 && container[j].whrate < 1.2))
			{
				con0.push_back(contours[j]);
			}
			if ((container[j].whrate > 1.8 && container[j].whrate < 2.0) && (container[j].square >= 40))
			{
				con1.push_back(contours[j]);
			}
			if ((container[j].whrate > 2.1 && container[j].whrate < 2.3) && (container[j].square >= 36))
			{
				con2.push_back(contours[j]);
			}
			if ((container[j].whrate > 2.4 && container[j].whrate < 2.5))
			{
				con3.push_back(contours[j]);
			}
			if (container[j].whrate >= 2.5 && container[j].whrate <= 2.6)
			{
				con4.push_back(contours[j]);
			}
			if ((container[j].whrate > 2.6 && container[j].whrate < 3.0) && (container[j].width >= 10))
			{
				con5.push_back(contours[j]);
			}
			if (container[j].whrate >= 3.0 && container[j].whrate <= 3.1)
			{
				con6.push_back(contours[j]);
			}
			if ((container[j].whrate > 3.1 && container[j].whrate <= 3.7) && ((container[j].square <= 50) || ((container[j].square > 50) && (container[j].width <= 15))))
			{
				con7.push_back(contours[j]);
			}
			if ((container[j].whrate > 3.1 && container[j].whrate <= 3.7) && (container[j].square > 50) && container[j].width > 15)
			{
				con13.push_back(contours[j]);
			}

			if ((container[j].whrate > 3.7 && container[j].whrate < 4.0) && (container[j].square <= 55))
			{
				con10.push_back(contours[j]);
			}
			if ((container[j].whrate > 3.7 && container[j].whrate < 4.0) && (container[j].square > 55))
			{
				con14.push_back(contours[j]);
			}

			if ((container[j].whrate > 4.1 && container[j].whrate < 4.6) && (container[j].width <= 16.5))
			{
				con8.push_back(contours[j]);
			}
			if ((container[j].whrate > 4.1 && container[j].whrate < 4.6) && (container[j].width > 16.5))
			{
				con12.push_back(contours[j]);
			}
			if (container[j].whrate >= 4.6 && container[j].whrate < 5.0)
			{
				con11.push_back(contours[j]);
			}
			if (container[j].whrate >= 5.0 && container[j].whrate <= 5.1)
			{
				con9.push_back(contours[j]);
			}
		}
	}


	//drawContours(src, con0, -1, Scalar(255, 255, 0), -1);
	//drawContours(src, con1, -1, Scalar(0, 255, 51), -1);
	//drawContours(src, con2, -1, Scalar(0, 255, 255), -1);
	//drawContours(src, con3, -1, Scalar(0, 0, 255), -1);
	//drawContours(src, con4, -1, Scalar(255, 0, 0), -1);
	//drawContours(src, con5, -1, Scalar(255, 0, 255), -1);
	//drawContours(src, con6, -1, Scalar(0, 102, 255), -1);

	//drawContours(src, con7, -1, Scalar(255, 0, 153), -1);//紫
	//drawContours(src, con13, -1, Scalar(0, 0, 255), -1);//红



	drawContours(src, con10, -1, Scalar(151, 133, 224), -1);//粉色
	drawContours(src, con14, -1, Scalar(0, 0, 255), -1);

	//drawContours(src, con8, -1, Scalar(204, 174, 51), -1);
	//drawContours(src, con11, -1, Scalar(138, 179, 77), -1);
	//drawContours(src, con12, -1, Scalar(0, 0, 255), -1);
	//drawContours(src, con9, -1, Scalar(138, 179, 77), -1);

	imwrite("D:/work/p2/p2/picture/ans2_fenlei2.bmp", src);
	// 长宽比染色
   /* drawContours(src, contourss[0], -1, Scalar(255, 255, 0), -1);
	drawContours(src, contourss[1], -1, Scalar(0, 255, 51), -1);
	drawContours(src, contourss[2], -1, Scalar(0, 255, 255), -1);
	drawContours(src, contourss[3], -1, Scalar(0, 0 , 255), -1);
	drawContours(src, contourss[4], -1, Scalar(255, 0 , 0), -1);
	drawContours(src, contourss[5], -1, Scalar(255, 0, 255), -1);
	drawContours(src, contourss[6], -1, Scalar(0, 102, 255), -1);
	drawContours(src, contourss[7], -1, Scalar(255, 0, 153), -1);
	drawContours(src, contourss[8], -1, Scalar(0, 255, 204), -1);
	drawContours(src, contourss[9], -1, Scalar(34, 184, 221), -1);

	imwrite("C:/picture/1/ans_all/ans2_changkuanbi.bmp", src);*/
	// 长边染色
	/*for (int i = 0; i < 10; i++)
	{

		if (i == 0) // 这里只能改着用了
		{
			drawContours(src, contours_c[i][1], -1, Scalar(255, 255, 0), -1);

			//drawContours(src, contours_c[i][2], -1, Scalar(0, 255, 51), -1);
			//drawContours(src, contours_c[i][3], -1, Scalar(0, 255, 255), -1);
			//drawContours(src, contours_c[i][4], -1, Scalar(0, 0, 255), -1);
			//drawContours(src, contours_c[i][5], -1, Scalar(255, 0, 0), -1);
			//drawContours(src, contours_c[i][6], -1, Scalar(255, 0, 255), -1);
			//drawContours(src, contours_c[i][7], -1, Scalar(0, 102, 255), -1);
			//drawContours(src, contours_c[i][8], -1, Scalar(255, 0, 153), -1);
			//drawContours(src, contours_c[i][9], -1, Scalar(0, 255, 204), -1);
			//drawContours(src, contours_c[i][10], -1, Scalar(34, 184, 221), -1);
			//drawContours(src, contours_c[i][11], -1, Scalar(138, 179, 77), -1);
			//drawContours(src, contours_c[i][12], -1, Scalar(204, 174, 51), -1);
			//drawContours(src, contours_c[i][13], -1, Scalar(0, 204, 255), -1);
			//drawContours(src, contours_c[i][14], -1, Scalar(151, 133, 224), -1);
			//drawContours(src, contours_c[i][15], -1, Scalar(242, 133, 176), -1);
		}
	}
	imwrite("C:/picture/1/ans_all/ans2_changbian.bmp", src);*/
	// 短边染色
	/*for (int i = 0; i < 10; i++)
	{
		if (i == 0) // 这里只能改着用了
		{
			drawContours(src, contours_c[i][1], -1, Scalar(255, 255, 0), -1);

			//drawContours(src, contours_c[i][2], -1, Scalar(0, 255, 51), -1);
			//drawContours(src, contours_c[i][3], -1, Scalar(0, 255, 255), -1);
			//drawContours(src, contours_c[i][4], -1, Scalar(0, 0, 255), -1);
			//drawContours(src, contours_c[i][5], -1, Scalar(255, 0, 0), -1);
			//drawContours(src, contours_c[i][6], -1, Scalar(255, 0, 255), -1);
			//drawContours(src, contours_c[i][7], -1, Scalar(0, 102, 255), -1);
			//drawContours(src, contours_c[i][8], -1, Scalar(255, 0, 153), -1);

		}

	}
	imwrite("C:/picture/1/ans_all/ans2_duanbian.bmp", src);*/
	// 面积染色
	/*for (int i = 0; i < 10; i++)
	{
		for (int k = 1; k < 31; k++)
		{
			drawContours(src, contours_s[i][k], -1, Scalar(2 * (i + 100) * (k + 50) % 256, 2 * (i + 100) * (k + 50) % 256, 2 * (i + 100) * (k + 50) % 256), -1);
		}
	}
	imwrite("C:/picture/1/ans_all/ans2_mianji.bmp", src);*/
}

void Counters(cv::Mat src[], int ssize) {
	Mat dist_8U; int num = 0;
	//------------------------------------------数个数-------------------------//
	//cvtColor(src, dist_8U, COLOR_BGR2GRAY);
	//将图像信息录入到contours容器中，容器内为每个连通域的一周坐标，从最低点起顺时针
	for (int i = 0; i < ssize; i++) {
		findContours(src[i], contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		
		
		vector<vector<Point>> connnn;
		for (int i = 0; i < contours.size(); i++)
		{
			RotatedRect rect = minAreaRect(contours[i]);//求最小外接矩形
			double w = max(rect.size.height, rect.size.width);
			double h = min(rect.size.height, rect.size.width);//定义长边为宽，窄边为高
			int ss = getContourData(contours[i]);
			if (ss <= 8 && ss > 0)
			{
				connnn.push_back(contours[i]);
			}
			if (ss <= 7 || h == 0)
			{
				contours.erase(contours.begin() + i);
				i--;
			}
		}
		drawContours(src[i], connnn, -1, Scalar(0, 0, 255), -1);
		int Size = contours.size();
		cout << Size << endl;//num += contours.size();
		particalVector* container = new particalVector[max(1, Size)];
		for (int i = 0; i < contours.size(); i++)
		{
			RotatedRect rect = minAreaRect(contours[i]);//求最小外接矩形
			double w = max(rect.size.height, rect.size.width);
			double h = min(rect.size.height, rect.size.width);//定义长边为宽，窄边为高
			int ss = getContourData(contours[i]);
			container[i].Input(i, ss, w, h);
		}
		OutPutHistogram(container, contours.size());
		imwrite("D:/work/p2/p2/picture/"+to_string(i)+"_ans2.bmp", src[i]);
		//return src;
	}
	cout << num << endl;
}

