#pragma once
#ifndef STATISTICS_H
#define STATISTICS_H
#include <opencv2/opencv.hpp>

class particalVector {
public:
    void Input(int i, int S, double W, double H)//��������,id,S,W,H
    {
        id = i;
        square = S;
        width = W;
        height = H;
        //rate = (double)S / W / H;
        whrate = W / H;
    }
    double width;//���򣬱��Ϊ��
    double height;//���򣬱��Ϊ��
    //double rate;//��С��Ӿ�����������������
    double whrate;//�����
    int id;
    int square;
};

void Counters(cv::Mat src[],int ssize);

#endif
