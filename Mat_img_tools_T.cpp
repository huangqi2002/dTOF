#include "Mat_img_tools_T.h"

void Mat_img_tools_T::m_resize(const int width, const int height, cv::Mat& img)
{
  cv::Mat image_mat_resize;
  cv::resize(img, image_mat_resize, cv::Size(width, height), 0, 0, cv::INTER_NEAREST);
  img = image_mat_resize.clone();
}

void Mat_img_tools_T::m_clip(const enum m_clip_code flip_code, cv::Mat& img)
{
  cv::Mat image_mat_clip;
  cv::flip(img, image_mat_clip, flip_code);
  img = image_mat_clip.clone();
}

void Mat_img_tools_T::m_filter_static(const cv::Mat img_avg, cv::Mat& img)
{
  cv::Mat mask_mat_1, mask_mat_2, mask_mat;
  cv::threshold(img - 1.1 * img_avg, mask_mat, 0, 1, cv::THRESH_BINARY);//>1.1倍平均值置1
  mask_mat.convertTo(mask_mat_1, CV_8UC1, 1, 0);
  cv::threshold(img - 0.9 * img_avg, mask_mat, 0, 1, cv::THRESH_BINARY_INV);//<0.9倍平均值置1
  mask_mat.convertTo(mask_mat_2, CV_8UC1, 1, 0);

  mask_mat_1.setTo(1, mask_mat_2);
  mask_mat = 1 - mask_mat_1;
  /*m_clip(M_CLIP_X, mask_mat);
  m_resize(800, 600, mask_mat);
  cv::imshow("dTOF", mask_mat * 255);
  cv::imshow("dTOF1", img);
  cv::waitKey(100);*/

  img.setTo(0, mask_mat);
}

void Mat_img_tools_T::filter_isolated(const float color_threshold, const int contours_threshold, cv::Mat& img)
{
  cv::Mat binary_mat, temp_mat;
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::threshold(img, temp_mat, color_threshold, 255, cv::THRESH_BINARY);
  temp_mat.convertTo(binary_mat, CV_8UC1, 1, 0);
  findContours(binary_mat, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point());

  for (int i = 0; i < (int)contours.size(); i++)
  {
    if (cv::contourArea(contours[i]) < contours_threshold)//如果轮廓较小
    {
      cv::drawContours(img, contours, i, 0, -1);//轮廓绘制
    }
  }
}

void Mat_img_tools_T::erode(cv::Mat& image_mat)
{
  cv::Mat temp_mat;
  cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::erode(image_mat, temp_mat, element);
  image_mat = temp_mat.clone();
}

void Mat_img_tools_T::dilate(cv::Mat& image_mat)
{
  cv::Mat temp_mat;
  cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::dilate(image_mat, temp_mat, element);
  image_mat = temp_mat.clone();
}
