#pragma once

#include "CameraMode.h"
#include "MoveSenseErrorCode.h"
namespace movesense{

class _declspec(dllexport) MoveSenseCamera
{
public:
	MoveSenseCamera(movesense::CameraMode mode = D_VPU_320X240_LR_30FPS);
	~MoveSenseCamera(void);

	//������������ɼ�
	MoveSenseErrorCode OpenCamera();

	//�ر����
	void CloseCamera();

	//�����ʹ�ܿ���
	void EnableCam(bool val);

	//��������
	void SetUndistort(bool val);

	//��ȡ�豸ID
	int GetCameraID();

	//��ȡ�������-t_p2
	void GetCamParas(MsgPkg& p);

	//��ȡͼƬ
	int GetImageData(unsigned char * &data, int &len);


//**************************************************
	//----�����ݲ������ṩstart
	void SetSM_Para(unsigned char val);
	void SetSM_P1(unsigned char val);
	void SetSM_P2(unsigned char val);
	void SetSM_P3(unsigned char val);
	void SetDesiredBin(unsigned char val);
	//������ƼĴ���
	void SetCamCmd(unsigned char val);
	//----�����ݲ������ṩend

	//******��ʱ�ò���-start
	//ģʽ���
	void SetCameraMode(CameraMode cameraMode);
	CameraMode GetCameraMode();

	//�汾ID
	void SetVersionID(unsigned char id);
	unsigned char GetVersionID();
	//�豸ID
	void SetCameraID(int id);

	//�����λ����--Reset����
	void ResetCamera(bool res_val);
	//*****��ʱ�ò���-end
//**************************************************

};

}
