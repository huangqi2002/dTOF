#include "Dtof_data_T.h"

Dtof_data_T::Dtof_data_T()
{
		port_ = 2368;//�˿�
		row_ = 30;//����
		col_ = 40;//����
		bufflen_ = 4873;//���ݰ�����
		skfd_ = -1;//�׽���
		file_ = NULL;//���ļ�
}

Dtof_data_T::Dtof_data_T(short port, int row, int col, int bufflen)
{
		port_ = port;//�˿�
		row_ = row;//����
		col_ = col;//����
		bufflen_ = bufflen;//���ݰ�����
		skfd_ = -1;//�׽���
		file_ = NULL;//���ļ�
}

Dtof_data_T::~Dtof_data_T()
{
		if (file_ != NULL)
		{
				fclose(file_);
				file_ = NULL;
		}
}

int Dtof_data_T::addr_connect()
{
		//��ʼ��wsa
		WORD sockVision = MAKEWORD(2, 2);
		WSADATA wsadata;
		if (WSAStartup(sockVision, &wsadata) != 0)
		{
				printf("WSA��ʼ��ʧ��\n");
				return -1;
		}

		//��������
		int ret = -1;

		//�����׽���
		skfd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (skfd_ == INVALID_SOCKET)
		{
				std::cout << "socket����������ʧ��" << std::endl;
				return -1;
		}

		//��IP�Ͷ˿�
		struct sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port_);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);

		//�˿ڸ���
		int opt = 1;
		setsockopt(skfd_, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
		if (bind(skfd_, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
				return -1;
		}
		std::cout << "��IP�Ͷ˿�\n" << std::endl;
		return 0;
}

void Dtof_data_T::addr_disconnect()
{
		if (file_ != NULL)
		{
				fclose(file_);
				file_ = NULL;
		}
}

int Dtof_data_T::read_file(std::string src_path)
{
		//��ֻ���ļ�
		if (file_ != NULL)
		{
				fclose(file_);
				file_ = NULL;
		}

		file_ = fopen(src_path.c_str(), "rb+");

		if (file_ == NULL)
		{
				perror("fopen");
				return -1;
		}

	return 0;
}

int Dtof_data_T::get_mat(enum m_read_src read, cv::Mat &img)
{
		cv::Mat ret_mat(30, 40, CV_32FC1);//���������Ϣ
		int flag = 0;//�ж��Ƿ�հ�
		while(flag == 0)
		{
				int recvlen = sizeof(DWORD) + sizeof(char) * (bufflen_);

				//����buff
				char* recvbuff = (char*)malloc(recvlen);//����buff
				if (recvbuff == NULL)
						return -1;
				memset(recvbuff, 0, recvlen);
				//�����Ϣ��ʼ��ַ
				short* img_pixel_depth;

				int ret = -1;
				DWORD read_time = 0;//ʱ���

				if (read == READ_SRC_FILE)//���ļ�
				{
						//��ȡʱ��� + �����Ϣ
						ret = (int)fread(recvbuff, 1, recvlen, file_);
						if (ret != recvlen)
						{
								if (feof(file_))
								{
										std::cout << "(recvbuff)read finish\n" << std::endl;
										if (recvbuff != NULL)
												free(recvbuff);
										return -1;
								}
								else
								{
										std::cout << "read recvbuff error\n" << std::endl;
										if (recvbuff != NULL)
												free(recvbuff);
										return -1;
								}
						}
						img_pixel_depth = (short*)(recvbuff + sizeof(DWORD) + 73);
						Sleep(50);
				}
				else //���豸����
				{
						ret = recvfrom(skfd_, recvbuff, bufflen_, 0, NULL, NULL);
						if (ret != bufflen_)
						{
								perror("skfd recvbuff");
								if (recvbuff != NULL)
										free(recvbuff);
								return -1;
						}
						img_pixel_depth = (short*)(recvbuff + 73);
				}

				for (int i = 0; i < row_; i++)
				{
						for (int j = 0; j < col_; j++)
						{
								//std::cout<<*img_pixel_depth<<" ";
								float temp = (*img_pixel_depth) * 30.0f / 0x7FFF;//��ʵ���

								if (temp != 0)
										flag = 1;

								//�����д��matͼ��
								//if (temp > 255) temp = 255;
								ret_mat.at<float>(i, j) = temp;
								img_pixel_depth += 2;
						}
				}

				if (flag != 0)//���ǿհ�
				{
						if (recvbuff != NULL)
								free(recvbuff);
						img = ret_mat.clone();
						return 0;
				}
		}
}


