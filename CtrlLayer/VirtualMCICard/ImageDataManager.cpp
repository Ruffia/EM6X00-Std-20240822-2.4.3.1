#include "stdafx.h"
#include "ImageDataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CImageDataManager& CImageDataManager::Instance()
{
	static CImageDataManager Inst;
	return Inst;
}

CImageDataManager::CImageDataManager()
{
	for (int i = 0; i < ImageData_RowCount;i++)
	{
		for (int j = 0; j < ImageData_ColumnCount;j++)
		{
			m_ImageColor[i][j] = 0;
		}
	}

	m_bSaveImageData = false;
}

void CImageDataManager::SetData(int nRow,int nColumn,WORD dwColor)
{
	CSingleLock lock(&m_cs,TRUE);
	if (nRow >= ImageData_RowCount || nColumn >= ImageData_ColumnCount)
	{
		return;
	}
	m_ImageColor[nRow][nColumn] = dwColor;
}



WORD CImageDataManager::GetData(int nRow,int nColumn)
{
	CSingleLock lock(&m_cs,TRUE);
	if (nRow >= ImageData_RowCount || nColumn >= ImageData_ColumnCount)
	{
		return -1;
	}
	return m_ImageColor[nRow][nColumn];
}


void  CImageDataManager::ReadFromINI(char* path)
{
	m_bSaveImageData = GetPrivateProfileInt( "Options", "SaveImageData", 0, path );
}