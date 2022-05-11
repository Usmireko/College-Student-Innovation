#pragma once
#ifndef STATISTICS_H
#define STATISTICS_H
#include <opencv2/opencv.hpp>

class particalVector {
public:
    void Input(int i, int S, double W, double H)//输入数据,id,S,W,H
    {
        id = i;
        square = S;
        width = W;
        height = H;
        //rate = (double)S / W / H;
        whrate = W / H;
    }
    double width;//横向，标记为宽
    double height;//纵向，标记为高
    //double rate;//最小外接矩形面积与粒子面积比
    double whrate;//长宽比
    int id;
    int square;
};

void Counters(cv::Mat src[],int ssize);

#endif
