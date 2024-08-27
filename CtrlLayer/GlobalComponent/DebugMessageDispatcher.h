#pragma once
#include "USB_Functions.h"

//修改时间：  2020-08-24
//修改人：  周宏
//功能：  消息派发器

class AFX_EXT_CLASS CDebugMessageDispatcher
{

public:
	static CDebugMessageDispatcher& Instance();

	void Attach(HWND hWnd)
	{
		m_hDebugMessageWnd = hWnd;
	}


	void DisplayMsg(const CString& sMsg);

    void SyncFineMag(double dblVal, double dblVal2, double dblStage);
	void  Co_UpdateScan();
	void  UpdateBeemAlignmentX(int nPos, int n);
	void  UpdateBeemAlignmentY(int nPos, int n);
	void  ShowDFocusCtrl(int nShow);
	void  SyncContrastCtrlText(const CString& strX, const VARIANT& Flag);

protected:
	HWND m_hDebugMessageWnd;

protected:
	CDebugMessageDispatcher();

};
