// CSemCtrl_Channel2C.h: interface for the CSemCtrl_Channel2C class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_CHANNEL2C_H__75854045_3D1C_4C72_85A7_A41A45EB652C__INCLUDED_)
#define AFX_SEMCTRL_CHANNEL2C_H__75854045_3D1C_4C72_85A7_A41A45EB652C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_Channel2C : public CSemBasisCtrl
{
public:
	CSemCtrl_Channel2C();
	virtual ~CSemCtrl_Channel2C();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_CHANNEL2C_H__75854045_3D1C_4C72_85A7_A41A45EB652C__INCLUDED_)
