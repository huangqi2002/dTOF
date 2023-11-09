#include "Dtof_data_T.h"

int demo()
{
  short port = 2368;//�˿�
  int row = 30;//����
  int col = 40;//����
  int width = 600;//��ʾͼ����
  int height = 400;//��ʾͼ��߶�
  int bufflen = 4873;//���ݰ�����
  Dtof_data_T dt_data(port, row, col, bufflen);
  cv::Mat image_mat;
  int ret = -1;
  int depth = 5;
  //���ļ���
  dt_data.read_file("D:\\VZ\\tod_qt_ui_new\\file\\data_2023_11_7_17_11_41.dat");
  //���豸��
  if (dt_data.addr_connect() != 0) return -1;

  while (1)
  {
    //������30*40�ľ������ͼ
    ret = dt_data.get_mat(Dtof_data_T::READ_SRC_FILE, image_mat);
    if (ret != 0)
      break;
    //��һ��
    //std::cout << image_mat << std::endl;
    image_mat = image_mat / depth;
    //resize
    cv::Mat image_mat_resize;
    cv::resize(image_mat, image_mat_resize, cv::Size(600,450), 0, 0, cv::INTER_NEAREST);
    //clip
    cv::Mat image_mat_clip;
    cv::flip(image_mat_resize, image_mat_clip, 0);
    //show
    cv::imshow("dTOF", image_mat_clip);
    if ((char)cv::waitKey(1) == 'q')
      break;

  }
}