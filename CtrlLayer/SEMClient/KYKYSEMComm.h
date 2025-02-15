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
	bool	KYKYSEM_GetServerStatus(int* status);	// check if com server is connected
	/////////////////////////////////////////////

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	bool	KYKYSEM_GetVersion(WORD* ver);
	bool	KYKYSEM_GetSEMType(int* type);			// SEM type: 6200 or 6900
	bool	KYKYSEM_GetHVPowerType(int* type);		// HV Power type: 0 for CPS; 1 for SPM

	bool	KYKYSEM_GetMagnRange(double* mgn1, double* mgn2);
	bool	KYKYSEM_GetMagn(double* mgn);			// Get Magnification
	bool	KYKYSEM_SetMagn(double mgn);			// Set Magnification
	bool	KYKYSEM_GetFOV(double* x, double* y, int* unit);	// Get FOV
	bool	KYKYSEM_GetWD(double* wd);				// Get working distance in mm
	bool	KYKYSEM_SetWD(double wd);				// Set working distance in mm
	bool	KYKYSEM_GetBrightness(double* brt);		// Get Brightness
	bool	KYKYSEM_SetBrightness(double brt);		// Set Brightness
	bool	KYKYSEM_GetContrast(double* cst);		// Get Contrast
	bool	KYKYSEM_SetContrast(double cst);		// Set Contrast
	bool	KYKYSEM_GetStig(double* stx, double* sty);// stigmator parameters
	bool	KYKYSEM_SetStig(double stx, double sty);// stigmator parameters
	bool	KYKYSEM_GetStigX(double* stx);
	bool	KYKYSEM_SetStigX(double stx);
	bool	KYKYSEM_GetStigY(double* sty);
	bool	KYKYSEM_SetStigY(double sty);
	bool	KYKYSEM_GetScnRotAngleInit(double* angle);//scan rotate init angle in degree
	bool	KYKYSEM_GetScnRotAngle(double* angle);	// scan rotate angle in degree
	bool	KYKYSEM_SetScnRotAngle(double angle);	// scan rotate angle in degree

	bool	KYKYSEM_GetVacuumStatus(int* status);	// check if vacuum ready
	bool	KYKYSEM_GetGunStatus(int* status);		// check current gun status
	bool	KYKYSEM_GetAccVoltage(double* volt);	// get acceleration voltage in [kV]
	bool	KYKYSEM_SetAccVoltage(double volt);		// acceleration voltage in [kV]
	bool	KYKYSEM_GetFilament(double* fila);		// reads filament heater current in Amps
	bool	KYKYSEM_SetFilament(double fila);		// set filament heater current in Amps
	bool	KYKYSEM_GetBeamCurrent(double* curr);	// reads beam current in [uA]
	bool	KYKYSEM_SetBeamCurrent(double curr);	// set beam current in [uA]
	bool	KYKYSEM_GetBias(double* bias);			// reads bias in [V]
	bool	KYKYSEM_SetBias(double bias);			// set bias in [V]: range (0 ~ 300V)
	bool	KYKYSEM_GetEMI(double* emi);			// read emi in [uA]

	bool	KYKYSEM_StageGetGratings(int* xSt, int* ySt);	// X,Y grating ruler[nm]
	bool	KYKYSEM_StageGetStatus(int* status);			// status 0- Idle, 1-moving, 2,... Errors
	bool	KYKYSEM_StageGetRange( int axis, float* range );
	bool	KYKYSEM_StageGetRange2( float* range );
	bool	KYKYSEM_StageGetRange5( float* range );
	bool	KYKYSEM_StageGetPos( int axis, float* pos );
	bool	KYKYSEM_StageGetPos2( float* pos );
	bool	KYKYSEM_StageGetPos5( float* pos );
	bool	KYKYSEM_StageSetPos( int axis, float pos );
	bool	KYKYSEM_StageSetPos2( float xSt, float ySt );
	bool	KYKYSEM_StageSetPos5( float xSt, float ySt, float zSt, float rSt, float tSt );
	bool	KYKYSEM_StageStop();
	bool	KYKYSEM_StageGoHome();

	bool	KYKYSEM_FuncAutoCstBrt();
	bool	KYKYSEM_FuncShutdownBeam();				// shutdown the beam

	bool	KYKYSEM_ScanGetResolution(int* rx, int* ry);	// get current image resolution
	bool	KYKYSEM_ScanSetResolution(int gear);	// change scan resolution gear: gear = 1,2
	bool	KYKYSEM_ScanGetSpeed(double* sp);		// get current scan speed
	bool	KYKYSEM_ScanSetSpeed(int gear);			// change scan speed gear: gear = 1,2,3,4
	bool	KYKYSEM_ScanStart();					// start normal scan
	bool	KYKYSEM_ScanFreeze();					// freeze scan
	bool	KYKYSEM_ScanStartSpot(int x, int y);	// start spot scan at [x,y] pixel
	bool	KYKYSEM_ScanStartHLine(int y);			// start horizontal line scan at y pixel
	bool	KYKYSEM_ScanStartVLine(int x);			// start vertical line scan at x pixel
	bool	KYKYSEM_ScanStartArea(int left, int top, int right, int bottom);
	bool	KYKYSEM_ScanStartSync();
	bool	KYKYSEM_ScanIsStop(int* status);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	bool	KYKYSEM_ImgGetSize(int* size);
	bool	KYKYSEM_ImgGetLastRecvIndex( int* index);// 获取上次的图像保存索引
	LPVOID	KYKYSEM_ImgRequestFrame();				// 向服务器发送请求图像整帧数据
	LPVOID	KYKYSEM_ImgRequest();					// 向服务器发送请求图像实时数据
	bool	KYKYSEM_ImgStop();
	///////////////////////////////////////////////////////
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_)
