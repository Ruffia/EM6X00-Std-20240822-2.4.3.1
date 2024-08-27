// SrvrItem.h : interface of the CXXX2SrvrItem class
//

#if !defined(AFX_SRVRITEM_H__DE6133E6_893C_46EB_8F18_F6D7A7657104__INCLUDED_)
#define AFX_SRVRITEM_H__DE6133E6_893C_46EB_8F18_F6D7A7657104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXXX2SrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CXXX2SrvrItem)

// Constructors
public:
	CXXX2SrvrItem(CXXX2Doc* pContainerDoc);

// Attributes
	CXXX2Doc* GetDocument() const
		{ return (CXXX2Doc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXXX2SrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CXXX2SrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__DE6133E6_893C_46EB_8F18_F6D7A7657104__INCLUDED_)
