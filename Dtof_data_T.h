#define _CRT_SECURE_NO_WARNINGS

#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <string>
#include <opencv2/opencv.hpp>

class Dtof_data_T
{
public:
  enum m_read_src { READ_SRC_DEVICE = 0, READ_SRC_FILE};

  Dtof_data_T();
  Dtof_data_T(short port, int row, int col, int bufflen);
  ~Dtof_data_T();

  int addr_connect();
  void addr_disconnect();

  int read_file(std::string src_path);

  int get_mat(enum m_read_src read, cv::Mat& img);

private:
  short port_;//�˿�
  int row_;//����
  int col_;//����
  int bufflen_;//���ݰ�����
  SOCKET skfd_;//�׽���
  FILE* file_;//���ļ�
};