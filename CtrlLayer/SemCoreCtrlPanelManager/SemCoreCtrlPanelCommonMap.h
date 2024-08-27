#if !defined(AFX_SEMCORECTRLPANELCOMMONMAP_H__CC70D943_1E16_4589_9802_A64787390C85__INCLUDED_)
#define AFX_SEMCORECTRLPANELCOMMONMAP_H__CC70D943_1E16_4589_9802_A64787390C85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SemCoreCtrlPanelCommonMap.h : header file
//

#include "SemDataType.H"


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanelCommonMap window

class AFX_EXT_CLASS CSemCoreCtrlPanelCommonMap : public CWnd
{
protected:
	DECLARE_DYNCREATE(CSemCoreCtrlPanelCommonMap)
// Construction
public:
	CSemCoreCtrlPanelCommonMap();

// Attributes
private:
	SemCtrlFigures	m_scfCtrlX;
	SemCtrlFigures	m_scfCtrlY;
	LPCTSTR			m_strX;
	LPCTSTR			m_strY;

	VARIANT			m_varPanelClassID;
	VARIANT			m_varPanelSerialNumber;

public:
	VARIANT	GetPanelClassID(){ return m_varPanelClassID; }
	VARIANT	GetPanelSerialNumber(){ return m_varPanelSerialNumber; }

// Operations
public:
	virtual void Bind(const VARIANT& ClassID, const VARIANT& SerialNumber);
	virtual void Enable(const VARIANT& Enabled, const VARIANT& Flag);
	virtual void SetRange(const VARIANT& Xmin, const VARIANT& Xmax, const VARIANT& Ymin, const VARIANT& Ymax);
	virtual void SetPos(const VARIANT& Xpos, const VARIANT& Ypos, const VARIANT& Flag);
	virtual void SyncPos(const VARIANT& Xpos, const VARIANT& Ypos, const VARIANT& Flag);
	virtual void SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT& Flag);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSemCoreCtrlPanelCommonMap)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSemCoreCtrlPanelCommonMap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSemCoreCtrlPanelCommonMap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANELCOMMONMAP_H__CC70D943_1E16_4589_9802_A64787390C85__INCLUDED_)
