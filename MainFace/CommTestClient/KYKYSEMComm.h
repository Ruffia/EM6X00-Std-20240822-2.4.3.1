#if !defined(AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_)
#define AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KYKYSEMComm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKYKYSEMComm window
class CSEMCommClient;
class AFX_EXT_CLASS CKYKYSEMComm
{
	CSEMCommClient*	m_pClient;

// Construction
public:
	CKYKYSEMComm();
	virtual ~CKYKYSEMComm();

// Attributes
public:

// Operations
public:
	/////////////////////////////////////////////
	// ��ʼ��
	void	KYKYSEM_Init(HWND hWnd);
	// ��ͻ��˽�������
	BOOL	KYKYSEM_Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4);
	// �Ͽ����ӣ�����bActive��״̬�������Ͽ��ͱ����Ͽ�
	void	KYKYSEM_Disconnect(BOOL bActive);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// ��ȡ�������
	BOOL	KYKYSEM_QueryAppVersion();
	BOOL	KYKYSEM_QueryMagnification();
	BOOL	KYKYSEM_QueryWorkDistance();
	BOOL	KYKYSEM_QueryScanRotAngle();
	BOOL	KYKYSEM_QueryHighVoltage();
	BOOL	KYKYSEM_QueryEMI();
	/////////////////////////////////////////////
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KYKYSEMCOMM_H__86BE8371_4B0F_4F04_BBF7_01B463B47722__INCLUDED_)
