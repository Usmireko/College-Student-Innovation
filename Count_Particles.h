#pragma once
#ifndef COUNT_PARTICLES_H
#define COUNT_PARTICLES_H
#include <opencv2/opencv.hpp>

#define s_p 48 //�����������
#define max_width 33 //�����Ӿ��ο��
#define max_height 19 //�����Ӿ��γ���


void Counter(cv::Mat,int&,int&,int);
//����˵��
// ���� : �������ͼ������ͨ����������ڵװ壬���������ص��������������ӣ������ౣ�棨������Ⱦɫ�����������뵽txt�У�
//����: Mat����(��ͨ��ͼ��)(CV_8UC1),�������Ӹ���,�ص����Ӹ���,�ڼ�������
//���: Mat����(��ͨ��ͼ��)(CV_8UC1)
#endif 

