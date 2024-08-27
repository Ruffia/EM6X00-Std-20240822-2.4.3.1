#pragma once
//修改时间：  2020-08-24
//修改人：  周宏
//功能： 控制层，从UI控制底层硬件的中介

#include "USB_Functions.h"
#include "LanguageManagerCtrl.h"

///////////////////////////////////////////////////////////
//手动调节器类型
#define	SORT_MANUAL_OBJECTIVE_FINE		0
#define	SORT_MANUAL_OBJECTIVE_CORSE		1
#define	SORT_MANUAL_CONDENSER_FINE		2
#define	SORT_MANUAL_CONDENSER_CORSE		3
#define	SORT_MANUAL_STIGMATOR_X			4
#define	SORT_MANUAL_STIGMATOR_X2		5
#define	SORT_MANUAL_STIGMATOR_Y			6
#define	SORT_MANUAL_STIGMATOR_Y2		7
#define	SORT_MANUAL_BEAM_DISPLACEMENT_X	8
#define	SORT_MANUAL_BEAM_DISPLACEMENT_Y	9
#define	SORT_MANUAL_BEAM_ALIGNMENT_X	10
#define	SORT_MANUAL_BEAM_ALIGNMENT_Y	11
#define	SORT_MANUAL_BRIGHT				12
#define	SORT_MANUAL_CONTRAST			13
#define	SORT_MANUAL_NULL				14

class CScenario;

class AFX_EXT_CLASS CControlLayer 
{

public:
	static CControlLayer& Instance();

	void  Release();

public:
	void  MCB_Adjust_Ratio( double dMag );

	void  SyncFineMag();   // 将当前放大倍数实送的FineMag值同步显示在控制面板上
	void  Co_UpdateScan();

	//MCI   媒体
	void	MCI_Det03SB_Change( int nPos, int nChn );
	void	MCI_Det45SB_Change( int nPos, int nChn );
	void	MCI_DetGainSB_Change( int nPos, int nChn );
	void	MCI_BWselSB_Change( int nPos, int nChn );
	void	MCI_UpdateSpotX( int nPos, BOOL bUpdate );
	void	MCI_UpdateSpotY( int nPos, BOOL bUpdate );

	//SCN   扫描板
	void	SCN_XstigDACSB_Change( int nPos);
	void	SCN_YstigDACSB_Change( int nPos);
	void	SCN_XshiftDACSB_Change( int nPos );
	void	SCN_YshiftDACSB_Change( int nPos );
	void	SCN_XxoverDACSB_Change( int nPos );
	void	SCN_YxoverDACSB_Change( int nPos );
	void    SCN_SyncXOverPos( int nPosX, int nPosY ); 

	//LNS   透镜
	void	LNS_L1DACSB_Change( int nPos);
	void	LNS_L2SDACSB_Change( int nPos);
	void	LNS_L2DDACSB_Change( int nPos );
	void	LNS_XalDACSB_Change( int nPos );
	void	LNS_YalDACSB_Change( int nPos );
	void	LNS_WobDACSB_Change( int nPos );

	void     Diag_Init( int nID, BOOL bSet );
	CString  Diag_SCN_LNS();
	CString  Diag_MCI();
	CString  Diag_Logging(); 
	CString  Diag_Logging2(); 

	void    CheckDet2nd( int nCheck );

	void    MCI_BWselSB_Change( int nPos ); 
	void    MCI_DetGainSB_Change( int nPos );
	void    MCI_Det45SB_Change( int nPos ); 
	void    MCI_Det45SBY_Change( int nPos ); 
	void    MCI_DetGain2SB_Change( int nPos ); 
	void    MCI_Sync_BrightnessParam(); 
	void    CHECKVideo( int nCheck );
	void    CHECKTestLEDs( int nCheck ); 
	void    ClearAllChannels(); 
	void    CHECKStigCtrl( int nCheck ); 
	void    CHECKDebugModeCoadjust( int nCheck ); 

	void    CHECKFakeMainslock( int nCheck );
	void    CHECKMainsLock( int nCheck ); 
	void    MainsLockFrameSync(); 
	void    CHECKCompCond( int nCheck ); 
	void    CHECKCompObj( int nCheck ); 
	void    CHECKCompAlignX( int nCheck );
	void    CHECKCompAlignY( int nCheck ); 

	BOOL	SCN_ClearAnalogSwitches();
	BOOL	SCN_AmpMain();
	BOOL    SCN_UpdateStrobe( BYTE bID, BOOL bHigh );
	BOOL	SCN_AmpLow1();
	BOOL	SCN_AmpLow2();
	BOOL	SCN_AmpLow3();
	BOOL	SCN_AmpLow4();

	void    SetDetPair(const WORD wData ); 
	void    GetMagnifierParam(double* dRateMag);

	void    CHECKFPGArelayEn11(); 
	void    SPR_HVOn( BOOL bHVOn );
	void    SyncDate( BOOL bNewVal );

public:
	MCI_Param	m_BrdMCI;
	DL_Sum_Param	m_DL;
	AutoTest_Param	m_AT;
	Cali_Param		m_Cali;
	Net_Param		m_Net;

	unsigned short	m_nAdjustorManualSort;	// 手控盒中间旋钮做多功能旋钮用，此变量为当前选定的手动旋钮功能类型

	BOOL	m_bDFforFF;		// Mag小于10000时为0；Mag不小于10000时为1，表示DF复用为FF
	BOOL    m_bHasBSD;
	// 23.09 pA计
	int		m_nPAType;		// 0: none; 1:D22

	CScenario*              m_pScenario;

	FastChanges		m_Fast;

	int		m_nDACTCvalue[12];
	int		m_nSPRDACvalues[4];

protected:
	CControlLayer();

	void _InitAutoTest(); 
	void _InitCalibration(); 
	void _InitNet();
	void _InitBrdMCI(); 
};
