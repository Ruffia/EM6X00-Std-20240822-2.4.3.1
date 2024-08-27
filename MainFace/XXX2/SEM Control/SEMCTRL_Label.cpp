// SEMCTRL_Label.cpp : implementation file
//
// Last Modify : 2006.10.24

#include "stdafx.h"
#include "XXX2.h"

#include "SEMCTRL_Label.h"
#include "StageXYManager.h"
#include "ControlLayer.h"
#include "GlobalParamSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
CString FormatHV( WORD value, double& dblHV );
CString FormatMAG( WORD value, double& dblMag );
CString CalculateUScale( int nScreenWidth, double dfMag, int& nScalePixels );
///////////////////////////////////////////////////////////////////////

void SemLabel_Init( int nScreenWidth )
{
	CString		strTemp;
	int			nScalPixelNum;
	CString		csUScale;
	CString		csMobileUScaleText;
	int			nMobileUScalePixelNum;
	CPoint		ptMobileUScale;

	CSemVirtualMCICard::Instance().UpdateHV( _T("0.0 KV") );
	CSemVirtualMCICard::Instance().GetMobileUScaleParam(nMobileUScalePixelNum, ptMobileUScale );

	strTemp = _T("150 X");
	CGlobalParamSetting::Instance().g_dfMag = 150;
	csUScale = CSemVirtualMCICard::Instance().CalculateUScale2( nScreenWidth, CGlobalParamSetting::Instance().g_dfMag, nScalPixelNum );
	csMobileUScaleText = CSemVirtualMCICard::Instance().CalculateMobileUScale( nMobileUScalePixelNum );

	// 2005-1-17_0.0KV问题
	if ( !CSemVirtualMCICard::Instance().IsStopScan() )
		CSemVirtualMCICard::Instance().UpdateMAG(
			strTemp,
			nScalPixelNum,
			csUScale,
			csMobileUScaleText );

	CSemVirtualMCICard::Instance().UpdateMAG(strTemp,nScalPixelNum,csUScale,csMobileUScaleText);
}


///////////////////////////////////////////////////////////////////////

CString FormatHV( WORD value, double& dblHV )
{
	CString strRet;
	unsigned char hv0,hv1,hv2;
	unsigned char string[8];

	value=((~value)>>7)&(0x01ff);

	hv0= (unsigned char)(value/100);
	value = value-100*hv0;
	hv1 = (unsigned char)(value/10);
	value = value - 10*hv1;
	hv2 = (unsigned char) value;

	if( hv0 > 0 ) 
	{
		string[0]=hv0 |0x30;
		string[1]=hv1 |0x30;
		string[2]=0x20;
		string[3]=0x4b;
		string[4]=0x56;
		string[5]=0x20;

		dblHV = hv0 * 10 + hv1;
	}
	else if( hv0 == 0 ) 
	{
		string[0]=hv1 |0x30;
		string[1]=0x2e;
		string[2]=hv2 |0x30;
		string[3]=0x20;
		string[4]=0x4b;
		string[5]=0x56;

		dblHV = hv0 + hv1 / 10.0;
	}

	string[6]='\0';

	strRet = _T(string);
	return strRet;
}

CString FormatMAG( WORD mag_new_value, double& dfMag )
{
	CString			strRet;
	unsigned char   high1,high2,low1,low2;
	unsigned char   flag0,flag1;
	unsigned char   string[7];
	double          mag_double_value;

	low1=mag_new_value&(0x000f);
	low2=(mag_new_value&0x00f0)>>4;
	high1=(mag_new_value&(0x0f00))>>8;
	high2=(mag_new_value&(0xf000))>>12;

	flag0=low1&(0x3);
	flag1=(low1&(0xC))>>2;

	if(mag_new_value==0xffff)
	{
		string[0]=0x2d;
		string[1]=0x2d;
		string[2]=0x2d;
		string[3]=0x2d;
	}
	else
	{
					 string[0]=high2 |0x30;
		if(flag1==0) string[1]=high1 |0x30;
		if(flag1==1) string[1]=high1 |0x30;
		if(flag1==2) string[1]=0x2e;
		if(flag1==0) string[2]=low2 |0x30;
		if(flag1==1) string[2]=0x2e;
		if(flag1==2) string[2]=high1 |0x30;
		if(flag1==0) string[3]=0x20;
		if(flag1==1) string[3]=low2 |0x30;
		if(flag1==2) string[3]=low2 |0x30;

		if(string[0] == 0x30)
		{
		   if(string[1] != 0x2e)  string[0] = 0x20;

		   if(string[1] == 0x30)
		      if(string[2] != 0x2e) string[1] = 0x20;
		}
	}

	if(flag0==3)
	{
	    string[4]=0x58;
	    string[5]=0x20;
	}

	if(flag0==2)
	{
	    string[4]=0x4b;
	    string[5]=0x58;
	}
	    string[6]='\0';


	mag_double_value = high2*100. + high1*10. + low2;
	if(flag1==2) mag_double_value = mag_double_value/100.;
	if(flag1==1) mag_double_value = mag_double_value/10.;
	if(flag0==2) mag_double_value = mag_double_value*1000.;

	dfMag = mag_double_value;
	strRet = _T(string);
	return strRet;
}



//CString CalculateStageUScale( int nScalePixelsX, int nScalePixelsY, DWORD &dwScaleX, DWORD &dwScaleY )
CString CalculateStageUScale( BOOL	bIsX,			// 是X或Y：1 is X, 0 is Y
							 BOOL	bDir,			// 方向：1 is positive, 0 is negative
							 int	nScalePixels,	// 像素长度
							 int	&nScale )		// 实际移动步数
{
	if ( CGlobalParamSetting::Instance().g_dfMobileUScaleParameter == -1 )
		return CGlobalParamSetting::Instance().g_csUScaleUnit;

	// 实际相对移动距离
	double dbScale = nScalePixels / CGlobalParamSetting::Instance().g_dfMobileUScaleParameter;
//	if( bIsX )
//		dbScale *= 2;
	// 换算成纳米单位
	if( CGlobalParamSetting::Instance().g_csUScaleUnit == "cm" )
		dbScale *= 10000000;
	else if( CGlobalParamSetting::Instance().g_csUScaleUnit == "mm" )
		dbScale *= 1000000;
	else if( CGlobalParamSetting::Instance().g_csUScaleUnit ==  "nm" )
		dbScale *= 1;
	else	// if( g_csUScaleUnit ==  "um" )
		dbScale *= 1000;

	// 实际相对移动步数
	if( bIsX )
		nScale = (int)(dbScale/CStageXYManager::Instance().m_Stage.xy.nGratingX);
	else
		nScale = (int)(dbScale/CStageXYManager::Instance().m_Stage.xy.nGratingY);
	if( nScale < 2 )
		nScale = 2;
/*
	// 更新绝对位置
	if( bIsX )
	{
		if( bDir )
			theApp.m_ptCurPosAb.x += dbScale;
		else
			theApp.m_ptCurPosAb.x -= dbScale;
	}
	else
	{
		if( bDir )
			theApp.m_ptCurPosAb.y += dbScale;
		else
			theApp.m_ptCurPosAb.y -= dbScale;
	}
*/
	return CGlobalParamSetting::Instance().g_csUScaleUnit;
}
