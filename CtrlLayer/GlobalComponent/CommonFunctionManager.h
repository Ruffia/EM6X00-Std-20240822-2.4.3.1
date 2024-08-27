#pragma once
#include "USB_Functions.h"

class AFX_EXT_CLASS CCommonFunctionManager  
{
public:
	static CCommonFunctionManager& Instance();

	BOOL  GetMonitorSizeFromEDID(double &dblPixelsPerMM_HORZ, double &dblPixelsPerMM_VERT);
	int	  ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');
	int	  SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');

protected:
	CCommonFunctionManager();
};
