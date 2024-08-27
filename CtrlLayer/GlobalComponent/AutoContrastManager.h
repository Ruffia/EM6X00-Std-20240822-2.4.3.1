#pragma once
#include "USB_Functions.h"

class AFX_EXT_CLASS CAutoContrastManager  
{
public:
	static CAutoContrastManager& Instance();

public:
	
	//自动功能
	
	float    fAutoBC;         //ABC
	float    *fMatrix;
	int      nCounth;
	int      nPosBright;
	int      nPosContrast;
	float    fPosGamma;
	BOOL     bOptionState;
	BOOL     bGrade;
	BOOL     bStateBC;
	BOOL     bState;
	float    *fim;
	int      m_nmin;
	int      m_nmax;
	unsigned char   *colorTable;
protected:
	CAutoContrastManager();

};
