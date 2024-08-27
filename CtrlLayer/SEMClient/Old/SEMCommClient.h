// SEMCommClient.h: interface for the CSEMCommClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCOMMCLIENT_H__04E9C346_5520_40CB_828F_5781D37AFCCE__INCLUDED_)
#define AFX_SEMCOMMCLIENT_H__04E9C346_5520_40CB_828F_5781D37AFCCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommFunctions.h"
#include "ThreadRecvImage.h"
class CCSCmdThread;
class CThreadRecvImage;

class CSEMCommClient  
{
	// 发送命令
	BOOL	TCP_SendCommand(const int nCommand);
	BOOL	TCP_SendData(const int nCommand, void* pData);

	CCSCmdThread*		m_pThreadCmd;
	CThreadRecvImage*	m_pThreadRecvImg;
//	MSGIMAGEDATA		m_msgData;
//	int					m_nImageSize;
//	CSize				m_szImageReso;
	CSize				m_szData;

public:
	CSEMCommClient();
	virtual ~CSEMCommClient();

	/////////////////////////////////////////////
	// 初始化
	void	Init(HWND hWnd);
	// 与客户端进行连接
	bool	Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4);
	// 断开连接，根据bActive的状态有主动断开和被动断开
	void	Disconnect(bool bActive);
	bool	GetServerStatus(int* status);
	bool	GetVersion(WORD* ver);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// 获取图像相关
	bool	Img_GetMsgImgSize(int* size);
	bool	Img_GetLastRecvIndex(int* index);	// 获取上次的图像保存索引
	LPVOID	Img_RequestImg();		// 向服务器发送请求图像数据
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	bool	GetDoubleParam(int index, double* param);
	bool	GetDoubleParams(int index, double* param1, double* param2);
	bool	SetDoubleParam(int index, double param);
	bool	SetDoubleParams(int index, double param1, double param2);
	bool	GetIntParam(int index, int* param);
	bool	SetIntParam(int index, int param);
	bool	SetCommand(int cmd);
	/////////////////////////////////////////////
	bool	StageGetStatus(int* status);
	bool	StageGetGratings(int* xSt, int* ySt);
	bool	StageGetDoubleParam(int index, int axis, float* param);
	bool	StageGetDoubleParam2(int index, float* xSt, float* ySt);
	bool	StageGetDoubleParam5(int index, float* xSt, float* ySt, float* zSt, float* rSt, float* tSt);
	bool	StageSetPos(int axis, float pos);
	bool	StageSetPos2(float xSt, float ySt);
	bool	StageSetPos5(float xSt, float ySt, float zSt, float rSt, float tSt);
	bool	StageStop();
	bool	StageGoHome();

	/////////////////////////////////////////////
	/////////////////////////////////////////////
	/////////////////////////////////////////////

	bool	FuncAutoCstBrt();
	bool	FuncShutdownBeam();

	bool	ScanChangeSpeedGear(int gear);
	bool	ScanChangeResoGear(int gear);
	bool	ScanStart();
	bool	ScanFreeze();
	bool	ScanStartSpot(int x, int y);
	bool	ScanStartHLine(int y);
	bool	ScanStartVLine(int x);
	bool	ScanStartArea(int left, int top, int right, int bottom);
	bool	ScanStartSync();
};

#endif // !defined(AFX_SEMCOMMCLIENT_H__04E9C346_5520_40CB_828F_5781D37AFCCE__INCLUDED_)
