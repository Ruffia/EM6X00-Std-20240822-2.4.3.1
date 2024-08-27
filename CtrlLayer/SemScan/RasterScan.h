#if !defined(AFX_RASTERSCAN_H__C7A45A1A_C26F_49B6_BF14_ADC15DF04E33__INCLUDED_)
#define AFX_RASTERSCAN_H__C7A45A1A_C26F_49B6_BF14_ADC15DF04E33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RasterScan.h : header file
//

#include "BasisScan.h"

/////////////////////////////////////////////////////////////////////////////
// CRasterScan thread

class CRasterScan : public CBasisScan
{
	DECLARE_DYNCREATE(CRasterScan)

public:
	CRasterScan();
protected:

// Attributes
public:
	DWORD				m_dwDMABuffSize;
	DWORD				m_dwExpectantDMABlockSize;
	DWORD				m_dwExpectantInterval;
	CRect				m_rcDestFrame;
	CRect				m_rcIndication;		//09.06.01

	BOOL				m_blRedirectToDump;

	LPVOID				m_pBuff;
	unsigned short*		m_pBuffUSB;
	DWORD				m_dwDMABlockSize;
	unsigned long		m_lBytes_transferred;

	CRect				m_rcSlipWindow;
	DWORD				m_dwLineCounter;

#ifdef _DEBUG_WITHOUT_PCICARD_
	DWORD m_dwDebugOffsetX, m_dwDebugOffsetY, m_dwDebugOffsetZ;
	DWORD m_dwPosX, m_dwPosY;

	DWORD m_dwDebugCounts;	// 一次的数据量相当于多少个像素点的数据
	DWORD m_dwDebugCountsX;	// 一行多少个点
	DWORD m_dwDebugCountsY;	// 一次的数据量相当于多少行
#endif //_DEBUG_WITHOUT_PCICARD_
	
// Operations
protected:
	//检查USB状态
	BOOL    _CheckUSB(); 

	BOOL	InputData();
	BOOL	OutputData();

	BOOL	OutputData_ZZZ(BYTE* pBuff);

	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );	// 向MCI板写数据
	void	ReadMonitorData( int nIndex );

public:
	virtual	BOOL	InitData();
	virtual	void	MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRasterScan)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRasterScan();
protected:

	// Generated message map functions
	//{{AFX_MSG(CRasterScan)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RASTERSCAN_H__C7A45A1A_C26F_49B6_BF14_ADC15DF04E33__INCLUDED_)
