#if !defined(AFX_LINESCAN_H__8E872E33_287F_4891_88D7_EC973848B8E9__INCLUDED_)
#define AFX_LINESCAN_H__8E872E33_287F_4891_88D7_EC973848B8E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineScan.h : header file
//

#include "BasisScan.h"


/////////////////////////////////////////////////////////////////////////////
// CLineScan thread

class CLineScan : public CBasisScan
{
	DECLARE_DYNCREATE(CLineScan)
public:
	CLineScan();           // protected constructor used by dynamic creation
protected:

// Attributes
public:
	DWORD				m_dwDMABuffSize;
	DWORD				m_dwExpectantDMABlockSize;
	DWORD				m_dwExpectantInterval;
	CRect				m_rcDestFrame;

	LPVOID				m_pBuff;
	unsigned short*		m_pBuffUSB;
	DWORD				m_dwDMABlockSize;
	unsigned long		m_lBytes_transferred;

	long				m_lIndex;
	long				m_lOffset;
	
#ifdef _DEBUG_WITHOUT_PCICARD_
	DWORD m_dwDebugOffsetZ;
	DWORD m_dwDebugOffsetY;
	
	DWORD m_dwDebugCounts;	// 一次的数据量相当于多少个像素点的数据
	DWORD m_dwDebugCountsX;	// 一行多少个点
	DWORD m_dwDebugCountsY;	// 一次的数据量相当于多少行
#endif //_DEBUG_WITHOUT_PCICARD_

// Operations
protected:
	BOOL InputData();
	BOOL OutputData();

// Operations
public:
	virtual BOOL InitData();
	virtual void MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineScan)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineScan();
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineScan)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINESCAN_H__8E872E33_287F_4891_88D7_EC973848B8E9__INCLUDED_)
