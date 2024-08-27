#if !defined(AFX_BASISSCAN_H__7FA4B4D5_912E_480E_AE30_EE72F7214B06__INCLUDED_)
#define AFX_BASISSCAN_H__7FA4B4D5_912E_480E_AE30_EE72F7214B06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BasisScan.h : header file
//
#include "ScanControlStructure.h"
#include <USB_Functions.h>
//#include "AdapterUSB.h"
#include "SemVirtualMCICard.h"

#include "MP.h"

// CBasisScan thread
class CBasisScan : public CWinThread
{
	DECLARE_DYNCREATE(CBasisScan)
protected:
	CBasisScan();           // protected constructor used by dynamic creation

	HANDLE				m_hEventKill;
	HANDLE				m_hEventDead;

	BOOL	m_boolOneOff;
	
	BOOL	m_boolFinished;

// Attributes
public:
	BOOL	IsFinish(){ return m_boolFinished; };

	BOOL	m_bNeedMon;			// 在采集过程中，是否需要监测数据

	int		m_nSharedImageBuffIndex;

#ifdef _DEBUG_RECORD_ERRINFO_
protected:
	CFIFO<CString, CString&>		m_fifoError;
	BOOL PostErrorInformation( CString& csError );
public:
	BOOL GetErrorInformation( CString& csError );
#endif

// Operations
public:
	virtual void KillThread();
	virtual void Delete();

	virtual BOOL InitData();
	virtual void MainLoop(){}


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
protected:
	virtual ~CBasisScan();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_BASISSCAN_H__7FA4B4D5_912E_480E_AE30_EE72F7214B06__INCLUDED_)
