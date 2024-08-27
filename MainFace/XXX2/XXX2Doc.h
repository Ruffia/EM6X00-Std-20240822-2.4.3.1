// XXX2Doc.h : interface of the CXXX2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXX2DOC_H__DE8CC802_F307_4574_A2EE_644EA038CC7E__INCLUDED_)
#define AFX_XXX2DOC_H__DE8CC802_F307_4574_A2EE_644EA038CC7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXXX2SrvrItem;

class CXXX2Doc : public COleServerDoc
{
protected: // create from serialization only
	CXXX2Doc();
	DECLARE_DYNCREATE(CXXX2Doc)

// Attributes
public:
	CXXX2SrvrItem* GetEmbeddedItem()
		{ return (CXXX2SrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXXX2Doc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXXX2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
	//{{AFX_MSG(CXXX2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CXXX2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XXX2DOC_H__DE8CC802_F307_4574_A2EE_644EA038CC7E__INCLUDED_)
