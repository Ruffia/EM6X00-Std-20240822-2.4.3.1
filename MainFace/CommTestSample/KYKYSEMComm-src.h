#if !defined(AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_)
#define AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KYKYSEMComm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKYKYSEMComm window
class CSEMCommClient;
class AFX_EXT_CLASS CKYKYSEMComm
{
	CSEMCommClient*	m_pClient;

// Construction
public:
	CKYKYSEMComm();
	virtual ~CKYKYSEMComm();

// Attributes
public:

// Operations
public:
	/////////////////////////////////////////////
	// 初始化
	void	KYKYSEM_Init(HWND hWnd);
	// 与客户端进行连接
	bool	KYKYSEM_Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4);
	// 断开连接，根据bActive的状态有主动断开和被动断开
	void	KYKYSEM_Disconnect(bool bActive);
	bool	KYKYSEM_GetServerStatus(int* status);	//check if com server is connected
	/////////////////////////////////////////////

	bool	KYKYSEM_GetMagn(float* mgn);			//Get Magnification
	bool	KYKYSEM_SetMagn(float mgn);				//Set Magnification
	bool	KYKYSEM_GetWD(float* wd);				//working distance in mm
	bool	KYKYSEM_SetWD(float wd);				//working distance in mm
	bool	KYKYSEM_GetBrightness(float* brt);		//Get Brightness
	bool	KYKYSEM_SetBrightness(float brt);		//Set Brightness
	bool	KYKYSEM_GetContrast(float* cst);		//Get Contrast
	bool	KYKYSEM_SetContrast(float cst);			//Set Contrast
	bool	KYKYSEM_GetStig(float* stx, float* sty);//stigmator parameters
	bool	KYKYSEM_SetStig(float stx, float sty);	//stigmator parameters
	bool	KYKYSEM_GetScnRotAngleInit(float* angle);//scan rotate init angle in degree
	bool	KYKYSEM_GetScnRotAngle(float* angle);	//scan rotate angle
	bool	KYKYSEM_SetScnRotAngle(float angle);	//scan rotate angle

	bool	KYKYSEM_GetVacuumStatus(int* status);	//check if vacuum ready
	bool	KYKYSEM_GetAccVoltage(float* volt);		//get acceleration voltage in [kV]
	bool	KYKYSEM_SetAccVoltage(float volt);		//acceleration voltage in [kV]
	bool	KYKYSEM_GetFilament(float* fila);		//reads filament heater current in Amps
	bool	KYKYSEM_SetFilament(float fila);		//set filament heater current in Amps
	bool	KYKYSEM_GetBeamCurrent(float* pCurr);	//reads beam current in [uA]
	bool	KYKYSEM_SetBeamCurrent(float pCurr);	//set beam current in [uA]

	bool	KYKYSEM_GetStageGratings(int* xSt, int* ySt);// X,Y grating ruler[nm]
	bool	KYKYSEM_GetStageRanges(int* xSt, int* ySt);	// X,Y range [mm]
	bool	KYKYSEM_GetStageStatus(int* status);	// status 0- Idle, 1-moving, 2,... Errors
	bool	KYKYSEM_MoveStageXY(float xSt, float ySt);	// X,Y position [mm]
	bool	KYKYSEM_GetStageXY(float* xSt, float* ySt);	// X,Y position [mm]
	bool	KYKYSEM_StopStage();

	bool	KYKYSEM_AutoContrBright();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_)
