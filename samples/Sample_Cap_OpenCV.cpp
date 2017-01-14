#include "MoveSenseCamera.h"
#include "CameraMode.h"
#include "opencv.hpp"
#include <iostream>
#include <stdio.h>
#include "Windows.h"

#define TEST_FPS  1

int main()
{
	//选择模式： D_VPU_320X240_LR_30FPS(左图右图)   D_VPU_320X240_LD_30FPS（左图和视差图） D_VPU_320X240_LRD_30FPS（左图右图和视差图）
	int sel = D_VPU_320X240_LD_30FPS;  
	movesense::MoveSenseCamera c(sel);
	if(!(movesense::MS_SUCCESS==c.OpenCamera()))
	{
		std::cout << "Open Camera Failed!" << std::endl;
		return false;
	}
	else{
		std::cout << "Open Camera Success!" << std::endl;
		c.GetCameraID();//获取设备ID
	}

	//获取相机参数-t_P2
	//MsgPkg pkg;
	//c.GetCamParas(pkg);
	//printf("t_P2 is :\n t_P2(0,0)=%0.8f\n t_P2(1,1)=%0.8f\n t_P2(0,2)=%0.8f\n t_P2(1,2)=%0.8f\n t_P2(0,3)=%0.8f\n\n",pkg.fdata[0],pkg.fdata[1],pkg.fdata[2],pkg.fdata[3],pkg.fdata[4]);
	

	int width  = 320;
	int height = 240;
	int len_two  = width*height*2;
	int len_three = width*height*3;

	cv::Mat left(height,width,CV_8UC1),right(height,width,CV_8UC1),disp(height,width,CV_8UC1);
	unsigned char * img_data_two  = new unsigned char[len_two];
	unsigned char * img_data_three  = new unsigned char[len_three];

#if TEST_FPS
	int cnt_fps = 0;
	LARGE_INTEGER m_liPerfFreq = { 0 };
	QueryPerformanceFrequency(&m_liPerfFreq);
	LARGE_INTEGER m_liPerfStart = { 0 };
#endif

	while(1)
	{
#if TEST_FPS 
		if (cnt_fps == 0)
		{
			QueryPerformanceCounter(&m_liPerfStart);
		}
		cnt_fps++;

		if (cnt_fps == 51)
		{
			LARGE_INTEGER liPerfNow = { 0 };
			QueryPerformanceCounter(&liPerfNow);
			cnt_fps = 0;
			double time = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / m_liPerfFreq.QuadPart)*1.0;
			std::cout << "帧率：" << 50 / time * 1000 << std::endl;
		}
#endif

		//two pic
		if( (sel == D_VPU_320X240_LR_30FPS) || (sel == D_VPU_320X240_LD_30FPS) )
		{
			c.GetImageData(img_data_two ,len_two);

			for(int i = 0 ; i < height; i++)
			{
				memcpy(left.data+width*i,	img_data_two+(2*i)*width,width);
				memcpy(right.data+width*i,img_data_two+(2*i+1)*width,width);
			}

			cv::imshow("left",left);
			if(D_VPU_320X240_LR_30FPS==sel)
				cv::imshow("right",right); //显示左右图
			else
				cv::imshow("disp",right);  //显示左图和视差图
		}

		//three pic
		else if( sel == D_VPU_320X240_LRD_30FPS )
		{
			c.GetImageData(img_data_three ,len_three);

			for(int i = 0 ; i < height; i++)
			{
				memcpy(left.data+width*i,	 img_data_three+(3*i)*width,width);
				memcpy(right.data+width*i, img_data_three+(3*i+1)*width,width);
				memcpy(disp.data+width*i,  img_data_three+(3*i+2)*width,width);
			}
			//显示左右和视差图
			cv::imshow("left",left);   
			cv::imshow("right",right);
			cv::imshow("disp",disp);
		}
		else
		{
			std::cout << "Selected mode is not supported!! Program is exiting!" << std::endl;
			break;
		}

		char key = cv::waitKey(5);
		if(key == 'Q' || key == 'q')
			 break;
	}
	c.CloseCamera();
	delete img_data_two;
	delete img_data_three;

	return true;
}
