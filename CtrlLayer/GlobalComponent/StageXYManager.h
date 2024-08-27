#pragma once
#include "USB_Functions.h"
#include "DMCWin.h"

class CLanguageManagerCtrl;

//�޸�ʱ�䣺  2020-07-21
//�޸��ˣ�  �ܺ�
//�޸�ԭ���ع�����XY ����̨���ܴ�CXXX2App �ж���������
//          ����CXXX2App �ĸ��Ӷ�
//���ܣ� XY ����̨ģ�飬ר�ſ���̨����߼�


class AFX_EXT_CLASS CStageXYManager  
{
	/////////////////////////////////////////////
	// 15.10 XY�Զ�̨
public:
	static CStageXYManager& Instance();

	void Attach(HWND hWnd,CLanguageManagerCtrl* pLanguageManagerCtrl);
	void Release();

	void	Stage_XY_Init();
	void	Stage_XY_FindIndex();
	int		Stage_XY_GetPosition( long& lX, long& lY, BOOL bDisplay = TRUE );
	void	Stage_XY_Command( CString strCmd, int nValue, BOOL bIsX, BOOL bDir );
	void	Stage_XY_Command2( CString strCmd, int nValueX, int nValueY, BOOL bDirX, BOOL bDirY, BOOL bSerial );
	CDMCWin m_DMCWin;
	ListStagePos*	m_plistStagePos;
	int		m_nStagePosTotal;

	ConfigStage	m_Stage;


protected:
	CStageXYManager();
	CLanguageManagerCtrl* m_pLanguageManagerCtrl;
	HWND m_hWnd;     //�����ھ��
};

