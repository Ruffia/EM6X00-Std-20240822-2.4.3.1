#if !defined(AFX_SSIMGTHREAD_H__24DDDDFC_7AED_4C48_A4EE_92A6123B25D3__INCLUDED_)
#define AFX_SSIMGTHREAD_H__24DDDDFC_7AED_4C48_A4EE_92A6123B25D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SSImgThread.h : header file
//

#include "SSImg.h"
#include "CommFunctions.h"
//#include "Include\\Json\\Json.h"

/////////////////////////////////////////////////////////////////////////////
// CSSImgThread thread

class CSSImgThread : public CWinThread
{
	DECLARE_DYNCREATE(CSSImgThread)
protected:
	CSSImgThread();           // protected constructor used by dynamic creation

// Attributes
public:
	HWND	m_destHandle;		// 主程序句柄
	SOCKET	m_hSocket;			// Socket句柄
	int		m_nPort;
	CSSImg	m_SSocketImg;		// 附加在这个线程上的CSocket

	KYKYCommMsg_ImageFrame	m_MsgImageFrame;
	KYKYCommMsg_Image		m_MsgImage;
	char					m_szSSImgSend[1024];
	KYKYCommMsg_SendDataS1i	m_1iData;

// Operations
public:
//	void	Send_SetCommand( BYTE bcommand );
//	void	SSImgSend_JsonString( Json::Value valueSend );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSImgThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSSImgThread();

	// Generated message map functions
	//{{AFX_MSG(CSSImgThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg void	OnMsgRead(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnMsgSend(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnMsgSendImg(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSIMGTHREAD_H__24DDDDFC_7AED_4C48_A4EE_92A6123B25D3__INCLUDED_)
