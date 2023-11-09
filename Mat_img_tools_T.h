#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <stdio.h>


class Mat_img_tools_T
{
public:
  enum m_clip_code { M_CLIP_X = 0, M_CLIP_Y };

  void m_resize(const int width, const int height, cv::Mat& img);//resize
  void m_clip(const enum m_clip_code flip_code, cv::Mat& img);//clip
  void m_filter_static(const cv::Mat img_avg, cv::Mat& img);//�޳���������ĵ�
  void filter_isolated(const float color_threshold, const int contours_threshold, cv::Mat& img);//�޳�������
  void erode(cv::Mat& image_mat);//��ʴ
  void dilate(cv::Mat& image_mat);//����
private:
  cv::Mat* img_;
};

