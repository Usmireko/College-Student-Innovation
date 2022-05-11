#pragma once
#ifndef REGION_SELECTION_H
#define REGION_SELECTION_H
#include <opencv2/opencv.hpp>
#include"Quasi_Temporal_Diff.h"

#define min_size 350      //最小周长
#define max_size 500     //最大周长       
#define min_wh 1.0/10.0 //最小长宽比
#define max_wh 1.0/5.0    //最大长宽比
//#define max_area 500      //最大面积
//#define min_area 200      //最小面积
#define CONT_C_x 3        //轮廓偏移量X
#define CONT_C_y 1        //轮廓偏移量Y
//#define w_gap 2           //导电薄膜间隔宽度
#define MAGN 2            //放大倍数
//放大倍数影响参数:min_size,max_size,max_area,min_area

void Region_Selection(cv::UMat);
//函数说明
// 功能 : 对输入的图像进行图像区域选取
//输入: Mat类型(三通道图像)(CV_8UC3)，int&，int&，阈值
//输出: Mat类型(三通道图像)(CV_8UC3)，单位导电粒子薄膜宽度，单位导电粒子薄膜长度

#endif
