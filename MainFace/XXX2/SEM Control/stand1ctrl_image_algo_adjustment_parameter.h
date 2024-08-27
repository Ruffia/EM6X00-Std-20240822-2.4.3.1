#pragma once
#include "_Font.h"

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CStand1ctrl_image_algo_adjustment_parameter 包装类

class CStand1ctrl_image_algo_adjustment_parameter : public CWnd
{
protected:
	DECLARE_DYNCREATE(CStand1ctrl_image_algo_adjustment_parameter)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xA34817AD, 0x1937, 0x4D50, { 0xAB, 0xC0, 0x7, 0x20, 0x90, 0xD2, 0xD1, 0xBF } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:


// 操作
public:

// _DSemCoreCtrlPanel_Stand1

// Functions
//

	void Bind(VARIANT ClassID, VARIANT SerialNumber)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &ClassID, &SerialNumber);
	}
	void Enable(VARIANT Enabled, VARIANT Flag)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Enabled, &Flag);
	}
	void SetRange(VARIANT Xmin, VARIANT Xmax, VARIANT Ymin, VARIANT Ymax)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Xmin, &Xmax, &Ymin, &Ymax);
	}
	void SetPos(VARIANT Xpos, VARIANT Ypos, VARIANT Flag)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Xpos, &Ypos, &Flag);
	}
	void SyncPos(VARIANT Xpos, VARIANT Ypos, VARIANT Flag)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Xpos, &Ypos, &Flag);
	}
	void SyncText(LPCTSTR strX, LPCTSTR strY, VARIANT Flag)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strX, strY, &Flag);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

BOOL GetCaptionVisible()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}
void SetCaptionVisible(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}
unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}
unsigned long GetForeColor()
{
	unsigned long result;
	GetProperty(DISPID_FORECOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetForeColor(unsigned long propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_UI4, propVal);
}
COleFont GetFont()
{
	LPDISPATCH result;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
	return COleFont(result);
}
void SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}
CString GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}
BOOL GetDebugInfoVisible()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}
void SetDebugInfoVisible(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}
CString GetXMinString()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}
void SetXMinString(CString propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}
CString GetXMaxString()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}
void SetXMaxString(CString propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}
short GetControlStyle()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}
void SetControlStyle(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}


};
