// CntrItem.h : interface of the CXXX2CntrItem class
//

#if !defined(AFX_CNTRITEM_H__04A31C77_78E5_4593_8B85_90B1624470D7__INCLUDED_)
#define AFX_CNTRITEM_H__04A31C77_78E5_4593_8B85_90B1624470D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXXX2Doc;
class CXXX2View;

class CXXX2CntrItem : public COleDocObjectItem
{
	DECLARE_SERIAL(CXXX2CntrItem)

// Constructors
public:
	CXXX2CntrItem(CXXX2Doc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CXXX2Doc* GetDocument()
		{ return (CXXX2Doc*)COleDocObjectItem::GetDocument(); }
	CXXX2View* GetActiveView()
		{ return (CXXX2View*)COleDocObjectItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXXX2CntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL CanActivate();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CXXX2CntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__04A31C77_78E5_4593_8B85_90B1624470D7__INCLUDED_)
