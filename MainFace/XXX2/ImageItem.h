//Last modify : 2007.04.26

#ifndef ImageItem_h
#define ImageItem_h

#include <afxtempl.h>
#include "ximage.h"

class CImageItem : public CObject									//�Ҷ�ͼ����Ŀ��
{
public:
	CImageItem();
	CImageItem( CxImage* pIImage );									//�������Ĺ��캯��
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
	CRect				m_rect;										//��ͼ�ľ�������
	CString				m_strFileName;								//ͼ���ļ���
	BOOL				m_bSel;										//��ѡ�б�־

	CxImage*			m_pIImage;									//ͼ�� main image
	CxImage*			m_pImageLayer;								//Image Layer
	CxImage*			m_pGraphLayer;								//Graph image
	int					m_nMag;
	int					m_nHV;

	DECLARE_DYNAMIC( CImageItem );
};

typedef CTypedPtrList< CPtrList, CImageItem*>		ListImageItem;

#endif
