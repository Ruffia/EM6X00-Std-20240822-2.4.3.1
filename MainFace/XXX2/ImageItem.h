//Last modify : 2007.04.26

#ifndef ImageItem_h
#define ImageItem_h

#include <afxtempl.h>
#include "ximage.h"

class CImageItem : public CObject									//灰度图像项目类
{
public:
	CImageItem();
	CImageItem( CxImage* pIImage );									//带参数的构造函数
	virtual ~CImageItem();

//Operation
	virtual void	Draw( CDC* pDC, CRect& rect, BOOL bSel=FALSE );
	CxImage*	GetImage() { return m_pIImage; }
	CString		GetTitle() { return m_strFileName; }
	void		New();
	BOOL		SaveTempFile();
	BOOL		DeleteTempFile( CString strTitle );
	BOOL		DeleteTempFile();

//Attribute
	CRect				m_rect;										//画图的矩形区域
	CString				m_strFileName;								//图像文件名
	BOOL				m_bSel;										//被选中标志

	CxImage*			m_pIImage;									//图像 main image
	CxImage*			m_pImageLayer;								//Image Layer
	CxImage*			m_pGraphLayer;								//Graph image
	int					m_nMag;
	int					m_nHV;

	DECLARE_DYNAMIC( CImageItem );
};

typedef CTypedPtrList< CPtrList, CImageItem*>		ListImageItem;

#endif
