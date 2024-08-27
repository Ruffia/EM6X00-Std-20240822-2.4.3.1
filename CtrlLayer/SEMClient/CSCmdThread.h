#if !defined(AFX_CSCMDTHREAD_H__6554594C_8541_4D8B_AAFE_8EBD6F0D7FF7__INCLUDED_)
#define AFX_CSCMDTHREAD_H__6554594C_8541_4D8B_AAFE_8EBD6F0D7FF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSCmdThread.h : header file
//

#include "CSCmd.h"
#include "CommFunctions.h"

#include "Include\\Json\\Json.h"

/////////////////////////////////////////////////////////////////////////////
// CCSCmdThread thread

class CCSCmd;
class CCSCmdThread : public CWinThread
{
	DECLARE_DYNCREATE(CCSCmdThread)
protected:
	CCSCmdThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CString		m_strIPAddr;

// Operations
public:
	virtual ~CCSCmdThread();
	void	SetServerIP(CString sIP);
	void	Reset(LPVOID ptr);

	bool	GetServerStatus(int* status);
	bool	GetVersion(WORD* ver);

	///////////////////////////////////
	bool	GetDoubleParam(int index, double* param);
	bool	GetDoubleParams(int index, double* param1, double* param2);
	bool	SetDoubleParam(int index, double param);
	bool	SetDoubleParams(int index, double param1, double param2);
	bool	GetIntParam(int index, int* param);
	bool	SetIntParam(int index, int param);
	bool	SetCommand(int cmd);
	///////////////////////////////////
	bool	StageGetStatus(int* status);
	bool	StageGetGratings(int* xSt, int* ySt);
	bool	StageGetDoubleParam(int index, int axis, float* param);
	bool	StageGetDoubleParam2(int index, float* param);
	bool	StageGetDoubleParam5(int index, float* param);
	bool	StageSetPos(int axis, float pos);
	bool	StageSetPos2(float xSt, float ySt);
	bool	StageSetPos5(float xSt, float ySt, float zSt, float rSt, float tSt);
	///////////////////////////////////

	bool	ScanStartSpot(int x, int y);
	bool	ScanStartHLine(int y);
	bool	ScanStartVLine(int x);
	bool	ScanStartArea(int left, int top, int right, int bottom);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSCmdThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCSCmdThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMsgRead(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMsgSend_Command(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMsgSend_Param(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSCMDTHREAD_H__6554594C_8541_4D8B_AAFE_8EBD6F0D7FF7__INCLUDED_)
