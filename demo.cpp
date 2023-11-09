#include "Dtof_data_T.h"

int demo()
{
  short port = 2368;//端口
  int row = 30;//行数
  int col = 40;//列数
  int width = 600;//显示图像宽度
  int height = 400;//显示图像高度
  int bufflen = 4873;//数据包长度
  Dtof_data_T dt_data(port, row, col, bufflen);
  cv::Mat image_mat;
  int ret = -1;
  int depth = 5;
  //从文件读
  dt_data.read_file("D:\\VZ\\tod_qt_ui_new\\file\\data_2023_11_7_17_11_41.dat");
  //从设备读
  if (dt_data.addr_connect() != 0) return -1;

  while (1)
  {
    //读出来30*40的绝对深度图
    ret = dt_data.get_mat(Dtof_data_T::READ_SRC_FILE, image_mat);
    if (ret != 0)
      break;
    //归一化
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