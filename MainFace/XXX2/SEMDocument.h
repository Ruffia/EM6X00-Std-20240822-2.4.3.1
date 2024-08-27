// Last Modify : 2009.04.21

#if !defined(AFX_SEMDOCUMENT_H__2BC53F18_FE70_49CA_A051_425B21E98B85__INCLUDED_)
#define AFX_SEMDOCUMENT_H__2BC53F18_FE70_49CA_A051_425B21E98B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SEMDocument.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSEMDocument document

class CSEMDocument : public CDocument
{
protected:
	CSEMDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSEMDocument)

// Attributes
public:
	HMENU	m_DefaultMenu;			// 09.03.24

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEMDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual HMENU GetDefaultMenu();			// 09.03.24
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace =TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSEMDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSEMDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMDOCUMENT_H__2BC53F18_FE70_49CA_A051_425B21E98B85__INCLUDED_)
