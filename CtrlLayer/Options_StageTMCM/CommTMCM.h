#if !defined(AFX_COMMTMCM_H__CE2C24EE_29C6_430C_B50F_2141A3E6ABE1__INCLUDED_)
#define AFX_COMMTMCM_H__CE2C24EE_29C6_430C_B50F_2141A3E6ABE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommTMCM.h : header file
//

#pragma once
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CCommTMCM window

class AFX_EXT_CLASS CStageTMCMManager  
{
public:
	static CStageTMCMManager& Instance();

public:
	BOOL	m_bIsMoving[5];
	vector<UCHAR*>	m_vecCmdTMCM;

protected:
	CStageTMCMManager();
};

class AFX_EXT_CLASS CCommTMCM
{
// Construction
public:
	CCommTMCM();
	~CCommTMCM();

// Attributes
public:

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Open( short port );
	BOOL	Close();
	void	Reset(short motor);

	BOOL	close(short motor);
	void	Stop(short motor);
	void    EmergecyStop(short motor);
	float	GetCurPosition( short motor );

	float	MoveDir(short motor, float fSteps, BOOL bBacklash = TRUE);
	void	MoveLimit(short motor, float fSteps);
	void	GoOrigion(short motor, float fCurPos );
	void	GoHome(short motor, float fSteps );

	void	CurrPosR(short motor);
	float	CurrPosO(short motor);
	float	BackOff( unsigned char ucAxis , float fLimDirID);
	void	SetBacklashParams(short nIndex,short nDir, float fCount);

	void	LimitStatus( short ID,short motor,int nable );
	void    LimitExchange( short motor ,BOOL bExchangeLimit);
	void	LimitLevel( short ID,short motor,int nLimitLevel );
	// ÄÚ²¿
	float	CoherenceCheck_Backlash( short motor, float& fSteps, BOOL bRelative);
	void	LocaInit(unsigned char ucAxis, short anxi);
	int		SendCmd(const float* cmd ,short ucMotor  );

	///////////////////////////////////
	void	Move( short motor, float fSteps, BOOL bRelative, BOOL bBacklash = TRUE );

	void	Pospeed(float* fPosSpeed,float* fAccSpeed, float* fDecSpeed);
	void	PosSpeed(short anxi,const float* data);
	void	SetSpeed(unsigned char ucAxis, float fSpeed);

	void	LimitParam(int* nLeftLimit,int* nRightParam, int* nExchangeLimit);
	void	LimitLevelParam(int* nLeftLimitLevel,int* nRightLimitLevel);
	void	LimitStatusRead(short motor);
	void	GroundStatusRead();
	void	Current(float* fCurSpeed,float* fStandbySpeed);
	float*	StatusRead(short motor);
	void	LimRelease(float ID, unsigned char ucAxis);
	void	LocaZero(short motor);

	void	SetVersion( int nVersion );
	void	PowerOfforOn(short ID ,short motor);
//	void	ConvertStagePos(float Actualoc,unsigned char Axial);
//	BYTE	JudgeAxial(unsigned char Axial);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTMCM_H__CE2C24EE_29C6_430C_B50F_2141A3E6ABE1__INCLUDED_)
