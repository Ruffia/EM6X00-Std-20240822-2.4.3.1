#if !defined(AFX_CLISOCKETTHREAD2_H__EAFDC622_B985_4C10_A8B4_A77262136B99__INCLUDED_)
#define AFX_CLISOCKETTHREAD2_H__EAFDC622_B985_4C10_A8B4_A77262136B99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CliSocketThread2.h : header file
//

#include "SemSocket.h"
#include "CommFunctions.h"

#include "Include\\Json\\Json.h"

/////////////////////////////////////////////////////////////////////////////
// CCliSocketThread2 thread

class CSEMSocket;
class CCliSocketThread2 : public CWinThread
{
	DECLARE_DYNCREATE(CCliSocketThread2)
protected:
	CCliSocketThread2();           // protected constructor used by dynamic creation

// Attributes
public:
	CString		m_strIPAddr;

// Operations
public:
	virtual ~CCliSocketThread2();
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
	bool	StageGetDoubleParam2(int index, float* xSt, float* ySt);
	bool	StageGetDoubleParam5(int index, float* xSt, float* ySt, float* zSt, float* rSt, float* tSt);
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
	//{{AFX_VIRTUAL(CCliSocketThread2)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCliSocketThread2)
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

#endif // !defined(AFX_CLISOCKETTHREAD2_H__EAFDC622_B985_4C10_A8B4_A77262136B99__INCLUDED_)
