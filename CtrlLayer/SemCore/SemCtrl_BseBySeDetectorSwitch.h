// BseBySeDetectorSwitch.h: interface for the CSemCtrl_BseBySeDetectorSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_BSEBYSEDETECTORSWITCH_H__DE01479B_73F6_4C34_B306_26897570D1C3__INCLUDED_)
#define AFX_SEMCTRL_BSEBYSEDETECTORSWITCH_H__DE01479B_73F6_4C34_B306_26897570D1C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_BseBySeDetectorSwitch : public CSemBasisCtrl
{
public:
	CSemCtrl_BseBySeDetectorSwitch();
	virtual ~CSemCtrl_BseBySeDetectorSwitch();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

};

#endif // !defined(AFX_SEMCTRL_BSEBYSEDETECTORSWITCH_H__DE01479B_73F6_4C34_B306_26897570D1C3__INCLUDED_)
