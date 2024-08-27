#if !defined(AFX_THUMBNAILLISTCTRLEX_H__86F549CC_855F_4E63_B889_C14A53C20F33__INCLUDED_)
#define AFX_THUMBNAILLISTCTRLEX_H__86F549CC_855F_4E63_B889_C14A53C20F33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThumbnailListCtrlEx.h : header file
//

#include <afxtempl.h>
#include "ximage.h"
#include "USB_Functions.h"

#define	ITEM_WIDTH		130
#define	ITEM_HEIGHT		100

#define	THUMB_WIDTH		100
#define	THUMB_HEIGHT	75

#define	WIDTH_STEP		10
#define	HEIGHT_STEP		10

class CThumbImageItem : public CObject
{
public:
	CThumbImageItem();
	CThumbImageItem( CxImage* pImage );
	virtual ~CThumbImageItem();

//Operation
	CxImage*	GetImage();
	CString		GetDefaultTitle();
	CString		GetTitleFromPathName( CString strPathName );
	void		New();
	BOOL		SaveImage( CString strPathName, int nType, CString& strError );

	BOOL		IsSemImage();
	BOOL		SaveSemImage( CString strPathName );
	void		SaveParams( CString strPathName );

	BOOL		SaveTempFile();
	BOOL		DeleteTempFile( CString strTitle );
	BOOL		DeleteTempFile();

//Attribute
	CString				m_strPathName;
	CString				m_strTitle;
	CString				m_strTempName;
	BOOL				m_bSel;

	CxImage*			m_pImageSrc;
	CxImage*			m_pImageThumb;
	CxImage*			m_pImageLayer;

	int					m_nMag;
	int					m_nHV;

	AcquireCondition	m_ac;

	// 2021.06 原始数据存储
	WORD				*m_pPixZsrc, *m_pPixZsrc2;

	DECLARE_DYNAMIC( CThumbImageItem );
};

typedef CTypedPtrList< CPtrList, CThumbImageItem*>		ListThumbImageItem;

/////////////////////////////////////////////////////////////////////////////
// CThumbnailListCtrlEx window

class CThumbnailListCtrlEx : public CListCtrl
{
// Construction
public:
	CThumbnailListCtrlEx();

// Attributes
public:
	ListThumbImageItem*	m_plistThumbImageItem;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThumbnailListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CThumbnailListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThumbnailListCtrlEx)
	//}}AFX_MSG
	afx_msg void OnCustomdraw ( NMHDR* pNMHDR, LRESULT* pResult );
	DECLARE_MESSAGE_MAP()
};
/*
// COwnerListBox 
class COwnerListBox : public CListBox 
{
	DECLARE_DYNAMIC(COwnerListBox) 
public:
	COwnerListBox();
	virtual ~COwnerListBox();
	struct DataItem{
			CString m_string; 
			}; 
	void Add(CString m_str); 
protected: 
	DECLARE_MESSAGE_MAP() 
public: */
//	virtual void DrawItem(LPDRAWITEMSTRUCT/*lpDrawItemStruct*/); 
//	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/); 
/*
protected: 
	virtual void PreSubclassWindow();
}; 

//// OwnerListBox.cpp : 实现文件 
#include "stdafx.h" 
#include "CompStroke.h" 
#include "OwnerListBox.h" 
#include ".\ownerlistbox.h" 

// COwnerListBox 
IMPLEMENT_DYNAMIC(COwnerListBox, CListBox) 
COwnerListBox::COwnerListBox() 
{ 
} 

COwnerListBox::~COwnerListBox()
{ 
} 

BEGIN_MESSAGE_MAP(COwnerListBox, CListBox) 
END_MESSAGE_MAP() 

// COwnerListBox 消息处理程序 
void COwnerListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{ 
	// TODO: 添加您的代码以绘制指定项 
	COLORREF clrItemText; 
	LPDRAWITEMSTRUCT lpDIS = lpDrawItemStruct; 
	switch ( lpDIS->itemAction ) //lpDIS为一个LPMEASUREITEMSTRUCT（记录控件信息） 
	{ 
	case ODA_SELECT: 
	case ODA_DRAWENTIRE: // Is the item selected? 
		if ( lpDIS->itemState & ODS_SELECTED ) 
		{ 
			clrItemText = GetSysColor( COLOR_HIGHLIGHTTEXT ); // Clear the rectangle FillRect( lpDIS->hDC, &lpDIS->rcItem, (HBRUSH)(COLOR_ACTIVECAPTION+1) ); 
		} 
		else 
		{ 
			clrItemText = GetSysColor( COLOR_WINDOWTEXT ); // Clear the rectangle FillRect( lpDIS->hDC, &lpDIS->rcItem, (HBRUSH)(COLOR_WINDOW+1) ); 
		} 
	} 

	CDC dc; 
	dc.Attach(lpDIS->hDC); 

	CRect rectFull = lpDIS->rcItem; 
	CRect rect2 = rectFull; 
	rect2.right = rectFull.Width() - 1; 
	UINT nIndex = lpDIS->itemID; 
	if (nIndex != (UINT) -1) 
	{ 
		LOGFONT lf; 
		dc.GetCurrentFont()->GetLogFont(&lf); 
		lf.lfCharSet = SHIFTJIS_CHARSET; 
		lstrcpy(lf.lfFaceName,_T("MS Mincho")); 
		lf.lfHeight=50; 
		// lf.lfWidth=0; 

		CFont font, *pOldFont; 
		font.CreateFontIndirect(&lf); 
		pOldFont=dc.SelectObject(&font); 

		CString sss; 
		this->GetText(nIndex,sss); 
		dc.SetBkMode(TRANSPARENT); 
		dc.SetTextColor(RGB(255,0,0)); 
		dc.DrawText(sss,CRect(rect2.left+3,rect2.top+3, rect2.right-3,rect2.bottom+3), DT_LEFT | DT_SINGLELINE);
		dc.SelectObject(pOldFont); 
	} 
	dc.Detach(); 
} 

void COwnerListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{ 
	// TODO: 添加您的代码以确定指定项的大小 
	lpMeasureItemStruct->itemHeight = 60; 
	lpMeasureItemStruct->itemWidth = 100; 
} 

void COwnerListBox::PreSubclassWindow() 
{ 
	// TODO: 在此添加专用代码和/或调用基类 
	CListBox::PreSubclassWindow(); 
} 

void COwnerListBox::Add(CString m_str) 
{ 
	DataItem *prtData = new DataItem; 
	prtData->m_string = m_str; 
	int index = AddString(m_str); 
	SetItemDataPtr(index, prtData); 
	delete prtData; 
}
*/
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THUMBNAILLISTCTRLEX_H__86F549CC_855F_4E63_B889_C14A53C20F33__INCLUDED_)
