#if !defined(AFX_VIRTUALVAC_H__FB146A93_8C32_4619_A23B_CE0594EDC2AB__INCLUDED_)
#define AFX_VIRTUALVAC_H__FB146A93_8C32_4619_A23B_CE0594EDC2AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualVAC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVirtualVAC window
class CIonPump;
class CCcg;

class AFX_EXT_CLASS CVirtualVAC
{
	CIonPump*		m_pIPG;
	CIonPump*		m_pIPC;
	CCcg*			m_pCCG;
	int				m_nInit;
	int				m_nPort[3];

// Construction
public:
	CVirtualVAC();
	virtual ~CVirtualVAC();

// Attributes
public:

// Operations
public:
	BOOL	ClearDevice();
	BOOL	Close( int nType );
	BOOL	Reset( CWnd* pWnd );
	void	SetParam( int index, int nValue );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALVAC_H__FB146A93_8C32_4619_A23B_CE0594EDC2AB__INCLUDED_)
