#pragma once
#include <afxmt.h>
#include "USB_Functions.h"

class AFX_EXT_CLASS CImageDataManager
{

public:
	static CImageDataManager& Instance();

	void SetData(int nRow,int nColumn,WORD dwColor);
	WORD GetData(int nRow,int nColumn);
	void  ReadFromINI(char* path);
	bool  SaveImageData(){return m_bSaveImageData;}

protected:
	CImageDataManager();

protected:
	CCriticalSection m_cs;
	WORD   m_ImageColor[ImageData_RowCount][ImageData_ColumnCount];
	bool   m_bSaveImageData;     //是否保存原始图像数据
};

