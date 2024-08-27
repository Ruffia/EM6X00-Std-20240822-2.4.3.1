#if !defined(AFX_SCANDOC_H__425926EC_08EA_4098_BEC8_AF8C3FC7368D__INCLUDED_)
#define AFX_SCANDOC_H__425926EC_08EA_4098_BEC8_AF8C3FC7368D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanDoc.h : header file
//

#include "SEMDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CScanDoc document

class CScanDoc : public CSEMDocument
{
protected:
	CScanDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScanDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual HMENU GetDefaultMenu();			// 09.03.05
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScanDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANDOC_H__425926EC_08EA_4098_BEC8_AF8C3FC7368D__INCLUDED_)
