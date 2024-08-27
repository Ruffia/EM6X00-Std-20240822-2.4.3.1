// ImageView.h : Header file
//
// Last Modify : 2010.02.08

/////////////////////////////////////////////////////////////////////////////
// 10.02.08 对快照后的图片增加临时文件存储，并对文件存储做保护
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEDOC_H__0D9E2F6E_9F84_4B5D_9101_0AB6FE487B12__INCLUDED_)
#define AFX_IMAGEDOC_H__0D9E2F6E_9F84_4B5D_9101_0AB6FE487B12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageDoc.h : header file
//

#include "SEMDocument.h"
#include "ximage.h"
#include "ximajpg.h"

#define SEMIMAGE_SUPPORT_SEM	1

/////////////////////////////////////////////////////////////////////////////
// CImageDoc document
#define MAX_UNDO_LEVELS 8
#define MAX_SEL_POINTS 8000

class CxImage;
class CImageDoc : public CSEMDocument
{
protected:
	CImageDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CImageDoc)

// Attributes
public:
	CxImage *image;					//main image
	HANDLE	hThread,hProgress;		//elaboration thread

	///////////////////////////////////////////////////
	CxImage *ImageLayer;			//Image Layer
	CxImage *GraphLayer;			//Graph image
	int		m_nMag;
	int		m_nHV;
	WORD   m_ImageColor[ImageData_RowCount][ImageData_ColumnCount];
	AcquireCondition	m_ac;
	///////////////////////////////////////////////////

	void	*m_fp[4];				//function parameters
	long	m_MenuCommand;

	POINT	m_Sel[MAX_SEL_POINTS];	//Selection
	long	m_NumSel;

#ifndef VATI_EXTENSIONS
	LOGFONT	m_font;
	CString	m_text;
	COLORREF	m_color;
#else
	int		m_isRectSel;
#endif

	long	m_hr[256]; //histogram
	long	m_hg[256];
	long	m_hb[256];
	long	m_hgray[256];
	long	m_hmax;

	CxImageJPG m_exif;

protected:
	BOOL	stretchMode;
	CxImage *imageUndo[MAX_UNDO_LEVELS];
	int		m_UndoLevel;
	double	m_dScale;
	BOOL	m_WaitingClick;
	LARGE_INTEGER m_swFreq, m_swStart, m_swStop;	//stopwatch
	float	m_etime;								//elapsed time
	long	m_progress;

// Operations
public:
	CView*	GetView( CRuntimeClass* prt );
	BOOL	IsSemImage(){ return (ImageLayer != NULL); };
	bool	SaveSemImage(const CString& filename, DWORD imagetype);
	bool	OpenSemImage(const CString& filename, DWORD imagetype);
	void	SaveParams( LPCTSTR lpszPathName );

	void	OpenParams( LPCTSTR lpszPathName );
	BOOL	OpenParams_FindKey( char* infoSrc, char Separator, char Unit, char* infoKey );
	BOOL	OpenParams_FindKey2( char* infoSrc, char Separator, char Unit, char* infoKey );

	void	SubmitUndo();
	inline	CxImage	*GetImage() { return image; }
	inline	BOOL	GetStretchMode() { return stretchMode; }
	inline	BOOL	GetWaitingClick() { return m_WaitingClick; }
	inline	void	SetWaitingClick(BOOL b) { m_WaitingClick=b; }
	void	RegionRotateLeft();
	void	RegionRotateRight();

	inline	double	GetScale() { return m_dScale; }
	void	UpdateScale();			// 05.06.23 图像缩放尺寸小于客户区大小时居中显示
	void	SetScale( double dScale );
	void	SetZoom( int nStyle );	// 09.03.20
	
	int		ComputePixel(float x, float y, float &x1, float &y1);
	void	ComputeNewImage(void);
	CString	FindExtension(const CString& name);

	BOOL	SaveTempFile();							// 10.02.08 存储临时文件
	BOOL	DeleteTempFile( CString strFileName );	// 10.02.08 删除临时文件

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual HMENU GetDefaultMenu();			// 09.03.16
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace =TRUE);

	void _SaveImageData2File(const CString &strFolder, const CString& strFileName, BOOL bShowHeader = FALSE);

	//}}AFX_VIRTUAL

// Implementation
public:
	void	Stopwatch(int start0stop1);
	void	UpdateStatusBar();
	void    SetData(int nRow,int nColumn,WORD dwData);

	virtual ~CImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:

	//{{AFX_MSG(CImageDoc)
	afx_msg void OnSemPhoto();
	afx_msg void OnUpdateSemPhoto(CCmdUI* pCmdUI);
	afx_msg void OnStretchMode();
	afx_msg void OnUpdateStretchMode(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomIn();
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnViewActualSize();
	afx_msg void OnUpdateViewActualSize(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnWindowDuplicate();
	afx_msg void OnCximageMirror();
	afx_msg void OnUpdateCximageMirror(CCmdUI* pCmdUI);
	afx_msg void OnCximageFlip();
	afx_msg void OnUpdateCximageFlip(CCmdUI* pCmdUI);
	afx_msg void OnCximageNegative();
	afx_msg void OnUpdateCximageNegative(CCmdUI* pCmdUI);
	afx_msg void OnCximageGrayscale();
	afx_msg void OnUpdateCximageGrayscale(CCmdUI* pCmdUI);
	afx_msg void OnCximageRotatel();
	afx_msg void OnUpdateCximageRotatel(CCmdUI* pCmdUI);
	afx_msg void OnCximageRotater();
	afx_msg void OnUpdateCximageRotater(CCmdUI* pCmdUI);
	afx_msg void OnCximageRotate();
	afx_msg void OnUpdateCximageRotate(CCmdUI* pCmdUI);
	afx_msg void OnCximageResample();
	afx_msg void OnUpdateCximageResample(CCmdUI* pCmdUI);
	afx_msg void OnCximageDecreasebpp();
	afx_msg void OnCximageIncreasebpp();
	afx_msg void OnCximageThreshold();
	afx_msg void OnUpdateCximageThreshold(CCmdUI* pCmdUI);
	afx_msg void OnCximageColorize();
	afx_msg void OnUpdateCximageColorize(CCmdUI* pCmdUI);
	afx_msg void OnCximageLighten();
	afx_msg void OnUpdateCximageLighten(CCmdUI* pCmdUI);
	afx_msg void OnCximageDarken();
	afx_msg void OnUpdateCximageDarken(CCmdUI* pCmdUI);
	afx_msg void OnCximageContrast();
	afx_msg void OnUpdateCximageContrast(CCmdUI* pCmdUI);
	afx_msg void OnCximageLessContrast();
	afx_msg void OnUpdateCximageLessContrast(CCmdUI* pCmdUI);
	afx_msg void OnCximageErode();
	afx_msg void OnUpdateCximageErode(CCmdUI* pCmdUI);
	afx_msg void OnCximageDilate();
	afx_msg void OnUpdateCximageDilate(CCmdUI* pCmdUI);
	afx_msg void OnCximageBlur();
	afx_msg void OnUpdateCximageBlur(CCmdUI* pCmdUI);
	afx_msg void OnCximageSoften();
	afx_msg void OnUpdateCximageSoften(CCmdUI* pCmdUI);
	afx_msg void OnCximageSharpen();
	afx_msg void OnUpdateCximageSharpen(CCmdUI* pCmdUI);
	afx_msg void OnCximageEdge();
	afx_msg void OnUpdateCximageEdge(CCmdUI* pCmdUI);
	afx_msg void OnCximageEmboss();
	afx_msg void OnUpdateCximageEmboss(CCmdUI* pCmdUI);
	afx_msg void OnCximageDither();
	afx_msg void OnUpdateCximageDither(CCmdUI* pCmdUI);
	afx_msg void OnCximageOptions();
	afx_msg void OnCximageSettransparency();
	afx_msg void OnUpdateCximageSettransparency(CCmdUI* pCmdUI);
	afx_msg void OnCximageRemovetransparency();
	afx_msg void OnUpdateCximageRemovetransparency(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplithsl();
	afx_msg void OnUpdateCximageSplithsl(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplitrgb();
	afx_msg void OnUpdateCximageSplitrgb(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplityuv();
	afx_msg void OnUpdateCximageSplityuv(CCmdUI* pCmdUI);
	afx_msg void OnCximagePseudocolors();
	afx_msg void OnUpdateCximagePseudocolors(CCmdUI* pCmdUI);
	afx_msg void OnCximageOpacity();
	afx_msg void OnCximageRemovealphachannel();
	afx_msg void OnUpdateCximageRemovealphachannel(CCmdUI* pCmdUI);
	afx_msg void OnCximageAlphastrip();
	afx_msg void OnUpdateCximageAlphastrip(CCmdUI* pCmdUI);
	afx_msg void OnCximageInvetalpha();
	afx_msg void OnUpdateCximageInvetalpha(CCmdUI* pCmdUI);
	afx_msg void OnCximageAlphapalettetoggle();
	afx_msg void OnUpdateCximageAlphapalettetoggle(CCmdUI* pCmdUI);
	afx_msg void OnCximageGamma();
	afx_msg void OnUpdateCximageGamma(CCmdUI* pCmdUI);
	afx_msg void OnCximageFft();
	afx_msg void OnUpdateCximageFft(CCmdUI* pCmdUI);
	afx_msg void OnCximageRepair();
	afx_msg void OnUpdateCximageRepair(CCmdUI* pCmdUI);
	afx_msg void OnCximageJpeg();
	afx_msg void OnUpdateCximageJpeg(CCmdUI* pCmdUI);
	afx_msg void OnCximageShadow();
	afx_msg void OnUpdateCximageShadow(CCmdUI* pCmdUI);
	afx_msg void OnToolsResoFactor();
	afx_msg void OnUpdateToolsResoFactor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEDOC_H__0D9E2F6E_9F84_4B5D_9101_0AB6FE487B12__INCLUDED_)
