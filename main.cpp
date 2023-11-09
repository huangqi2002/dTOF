#include "Dtof_data_T.h"
#include "Mat_img_tools_T.h"

int main()
{
  short port = 2368;//端口
  int row = 30;//行数
  int col = 40;//列数
  int width = 600;//显示图像宽度
  int height = 400;//显示图像高度
  int bufflen = 4873;//数据包长度
  Dtof_data_T dt_data(port, row, col, bufflen);//数据读取类
  Mat_img_tools_T mat_tools;//数据处理类
  cv::Mat image_mat;
  int ret = -1;
  int depth = 10;
  //从文件读
  dt_data.read_file("D:\\VZ\\tod_qt_ui_new\\file\\data_2023_11_7_17_11_41.dat");
  //从设备读
  if (dt_data.addr_connect() != 0) return -1;

  int avg_count = 0;//计算平均计数
  cv::Mat avg_depth_mat = cv::Mat::zeros(row, col, CV_32FC1);//计算平均深度信息
  cv::Mat depth_mat[300];//计算平均深度信息
  int max_avg_count = 300;

  while (1)
  {
    //读出来30*40的绝对深度图
    ret = dt_data.get_mat(Dtof_data_T::READ_SRC_FILE, image_mat);
    if (ret != 0)
      break;
    //std::cout << image_mat << std::endl;

    cv::Mat src_mat = image_mat.clone();

    if(avg_count > max_avg_count - 1)
    {
      //计算平均深度图
      avg_depth_mat.setTo(0);
      for (int i = 0; i < max_avg_count; i++)
      {
        avg_depth_mat += depth_mat[i];
      }
      avg_depth_mat /= max_avg_count;

      //计算均值
      depth_mat[avg_count % max_avg_count] = image_mat.clone();

      //根据平均深度滤除静态点
      mat_tools.m_filter_static(avg_depth_mat, image_mat);
      ++avg_count;
    }
    else
    {
      depth_mat[avg_count % max_avg_count] = image_mat.clone();
      ++avg_count;
      continue;
    }


    //去除孤立点
    mat_tools.filter_isolated(depth / 10, 30, image_mat);

    //归一化
    image_mat = image_mat / depth;
    src_mat /= depth;

    //clip
    mat_tools.m_clip(Mat_img_tools_T::M_CLIP_X, image_mat);
    mat_tools.m_clip(Mat_img_tools_T::M_CLIP_X, src_mat);

    //resize
    mat_tools.m_resize(width, height, image_mat);
    mat_tools.m_resize(width, height, src_mat);

    //show
    cv::imshow("dTOF_src", src_mat);
    cv::waitKey(1);
    cv::imshow("dTOF", image_mat);
    if ((char)cv::waitKey(1) == 'q')
      break;

    //做差分

  }
}