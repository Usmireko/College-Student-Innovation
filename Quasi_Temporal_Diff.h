#pragma once
#ifndef QUASI_TEMPORAL_DIFF_H
#define QUASI_TEMPORAL_DIFF_H
#include <opencv2/opencv.hpp>

#define w_gap 2           //导电薄膜间隔宽度

cv::Mat Q_Temp_Diff(cv::UMat,int);
//函数说明
// 功能 : 对输入的图像进行伪帧差二值化处理
//输入: Mat类型(单通道或三通道图像)(CV_8UC1||CV_8UC3),阈值，Mat类型，导电薄膜宽度，导电薄膜长度
//输出: Mat类型单通道二值图(CV_8UC1):
//      返回值:伪帧差二值图
//      Mat承接:伪帧差正值二值图
#endif