#pragma once
#ifndef COUNT_PARTICLES_H
#define COUNT_PARTICLES_H
#include <opencv2/opencv.hpp>

#define s_p 48 //单个粒子面积
#define max_width 33 //最大外接矩形宽度
#define max_height 19 //最大外接矩形长度


void Counter(cv::Mat,int&,int&,int);
//函数说明
// 功能 : 由输入的图像获得连通域区域，扩充黑底板，初步区别重叠粒子与正常粒子，并分类保存（计数并染色，计数将输入到txt中）
//输入: Mat类型(单通道图像)(CV_8UC1),导电粒子个数,重叠粒子个数,第几个基板
//输出: Mat类型(单通道图像)(CV_8UC1)
#endif 

