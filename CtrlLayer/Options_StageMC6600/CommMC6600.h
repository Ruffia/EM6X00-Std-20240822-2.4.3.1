#if !defined(AFX_COMMMC6600_H__C0D07B03_1212_48A7_9136_4192E3D9AD51__INCLUDED_)
#define AFX_COMMMC6600_H__C0D07B03_1212_48A7_9136_4192E3D9AD51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommMC6600.h : header file
//

#pragma once
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CCommMC6600 window

class AFX_EXT_CLASS CStageMC6600Manager  
{
public:
	static CStageMC6600Manager& Instance();

public:
	BOOL	m_bIsMoving[5];
	vector<BYTE*>	m_vecCmdMC6600;

protected:
	CStageMC6600Manager();
};

class AFX_EXT_CLASS CCommMC6600
{
// Construction
public:
	CCommMC6600();
	~CCommMC6600();

// Attributes
public:

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Open( short port );
	BOOL	Close();
	void	Reset(short anxi);

	void	Enable(short anxi);
	BOOL	close(short anxi);

	void	GetCurPosition( unsigned char ucAxis );

	float	MoveDir(unsigned char ucAxis, float fSteps, BOOL bBacklash = TRUE);
	void	MoveLimit(unsigned char ucAxis, float fSteps);
	void	GoOrigion( unsigned char ucAxis, float fCurPos );
	void	GoHome(unsigned char ucAxis, float fSteps );

	void	CurrPosR(short anxi);
	float	CurrPosO(unsigned char ucAxis);
	float	BackOff( unsigned char ucAxis , float fLimDirID);
	void	SetBacklashParams(short nIndex,short nDir, float fCount);

	// ÄÚ²¿
	float	CoherenceCheck_Backlash( unsigned char ucAxis, float& fSteps, BOOL bRelative);
	void	LocaInit(unsigned char ucAxis, short anxi);
	int		SendCmd(const float* cmd ,unsigned char ucAxis );

	///////////////////////////////////

//	void	PtoModify( unsigned char ucAxis ,float Pv);
	void	ItoModify( unsigned char ucAxis ,float Iv);
	void	DtoModify( unsigned char ucAxis ,float Dv);
//	int		GetDir( unsigned char ucAxis );
	///////////////////////////////////

	void	Move( unsigned char ucAxis, float fSteps, BOOL bRelative, BOOL bBacklash = TRUE );
	void	SlaveAddrSend(BYTE* pbyteAdd);
	void	SlaveAddrSet(BYTE* pbyteAdd, int nLength);

	/*void	LimitChanged(int nID);*/
	void	Pospeed(float* pnSpeed);
	void	SetSpeed(unsigned char ucAxis, float fSpeed);
	int 	StatusRead(unsigned char ucAxis);
	void	LimRelease(int nID, unsigned char ucAxis);
	void	LocaZero(short anxi);
	void	PosSpeed(short anxi,const float* data);
	void	SetWizard(unsigned char ucAxis ,BYTE M);
	void	SetEncoderLines(unsigned char ucAxis, int M);
	void	SetControlSignal(unsigned char ucAxis, BYTE M);
	void	SetControlMode(unsigned char ucAxis, BYTE M);
	void	SetMaxCurrent(unsigned char ucAxis, int M);
	void	PIDModify(int ID, unsigned char ucAxis, float Pv);

	void	SetOldStyle( BOOL bOldStyle, unsigned char ucAxis );
//	void	ConvertStagePos(float Actualoc,unsigned char Axial);
//	BYTE	JudgeAxial(unsigned char Axial);
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMMC6600_H__C0D07B03_1212_48A7_9136_4192E3D9AD51__INCLUDED_)
