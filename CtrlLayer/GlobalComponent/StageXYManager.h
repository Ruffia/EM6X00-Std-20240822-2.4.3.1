#pragma once
#include "USB_Functions.h"
#include "DMCWin.h"

class CLanguageManagerCtrl;

//修改时间：  2020-07-21
//修改人：  周宏
//修改原因：重构，将XY 控制台功能从CXXX2App 中独立出来，
//          减少CXXX2App 的复杂度
//功能： XY 控制台模块，专门控制台相关逻辑


class AFX_EXT_CLASS CStageXYManager  
{
	/////////////////////////////////////////////
	// 15.10 XY自动台
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
	HWND m_hWnd;     //主窗口句柄
};

