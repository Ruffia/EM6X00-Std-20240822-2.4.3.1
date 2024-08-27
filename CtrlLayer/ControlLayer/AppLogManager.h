#pragma once
#include "USB_Functions.h"

//�޸�ʱ�䣺  2020-07-20
//�޸��ˣ�  �ܺ�
//�޸�ԭ���ع�������־��¼���ܴ�CXXX2App �ж���������
//          ����CXXX2App �ĸ��Ӷ�
//���ܣ� ��־��¼ģ�飬ר�Ÿ����¼��־

typedef struct tagLog_HVVACData
{
	double dCCG;
	double dHV;
	double dFil;
	double dEMI;
}Log_HVVACData;

class AFX_EXT_CLASS CAppLogManager  
{
public:
	static	CAppLogManager& Instance();

	BOOL	LogHVVACInit(HWND hWnd,DWORD dwHVtype);
	void	LogHVVACClose();
	static	DWORD CommThreadLogAll(LPVOID lpParam);

	void	LogHVVAC( int nVACBoardType );
	void	LogHVVAC_CPS3603(int nVACBoardType, CString& str);
	void	LogHVVAC_SPM771(int nVACBoardType, CString& str);
	void	LogHVVAC_WSM30KV2(int nVACBoardType, CString& str);

	void	SetupLogs( BOOL bVal );
	BOOL	InitLogFile();

	Log_HVVACData	m_logHVVACdata;

protected:
	CAppLogManager();

public:
	HANDLE	m_hThreadLogAll;
	DWORD	m_dwThreadLogAll;
	HANDLE	m_hEventLogAll;
	HANDLE	m_hEventTeminateLogAll;
	HANDLE	m_hFileHVVAC;
	int		m_nLoopsHVVAC;
	BOOL	m_bLogHVVAC;
	DWORD   m_dwHVtype;
	HWND    m_hWnd;     //��������


	///////////////////////////////////////////////////////////
	BOOL	m_boolHVLog;
	BOOL	m_boolScanShiftLog;
	BOOL	m_boolBeemAlignmentLog;
	BOOL	m_boolBrightnessLog;
	BOOL	m_boolContrastLog;
	BOOL	m_boolStigmatorLog;
	BOOL	m_boolObjectiveLensLog;
	BOOL	m_boolCondenserLensLog;
	BOOL	m_boolFilamentCurrentLog;
	BOOL	m_boolMagnifierLog;
	BOOL	m_boolWorkDistanceLog;
	BOOL	m_boolObjectiveLensReverseLog;
	BOOL	m_boolCondenserLensReverseLog;
	BOOL	m_boolAuxiliaryStigmationLog;
	BOOL	m_boolDCondenserLensLog;
	BOOL	m_boolScanRotateLog;
	BOOL	m_boolSACPLog;
	BOOL	m_boolDFocusLog;
	BOOL	m_boolDFocusSwitchLog;
	BOOL	m_boolDObjectiveLensLog;
	BOOL	m_boolDObjectiveLensSwitchLog;
	BOOL	m_boolRobinsonDetectorSwitchLog;
	BOOL	m_boolSignal2BrightnessLog;
	BOOL	m_boolSignal2ContrastLog;
	BOOL	m_boolYAdjustLog;
	BOOL	m_boolImageReverseLog;
	BOOL	m_boolGunCurrentLog;
	BOOL	m_boolGunCurrentUpdate;
	BOOL	m_boolMsgFromMicroPCLog;	// 11.07.19
	///////////////////////////////////////////////////////////
	BOOL	m_boolLogMsgPcMc;
	BOOL	m_boolLogMsgRemoteGet;		// 22.03.11 �Ƿ��¼Զ��ͨѶ����
};

