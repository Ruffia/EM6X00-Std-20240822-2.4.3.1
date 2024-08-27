// ImageStandStyle.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"

#include "ImageStandStyle.h"
#include "ControlLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const DWORD FrameMaxW	= 1024*2;
const DWORD FrameMaxH	= 768*2;

/////////////////////////////////////////////////////////////////////////////
// ImageStandStyle

void ImageStandStyle0()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 256; j++)
		{
			cBuff[j*2]		= (BYTE)j;
			cBuff[j*2 + 1]	= (BYTE)j;
		}
		for (j = 256; j < 512; j++)
		{
			cBuff[j*2]		= (BYTE)(511 - j);
			cBuff[j*2 + 1]	= (BYTE)(511 - j);
		}
		
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle1()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 256; j++)
		{
			cBuff[j*2]		= (BYTE)(255 - j);
			cBuff[j*2 + 1]	= (BYTE)(255 - j);
		}
		for (j = 256; j < 512; j++) 
		{
			cBuff[j*2]		= (BYTE)(j - 255);
			cBuff[j*2 + 1]	= (BYTE)(j - 255);
		}
		
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle2()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 256; j++)
		{
			cBuff[j] = (BYTE)j;
		}
		for (j = 256; j < 512; j++)
		{
			cBuff[j] = (BYTE)(511-j);
		}
		for (j = 512; j <512 + 256; j++)
		{
			cBuff[j] = (BYTE)(j - 512);
		}
		for (j = 512 + 256; j <1024; j++)
		{
			cBuff[j] = (BYTE)(1023 - j);
		}
		
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle3()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 256; j++)
		{
			cBuff[j] = (BYTE)( 255 - j );
		}
		for (j = 256; j < 512; j++)
		{
			cBuff[j] = (BYTE)( 255 - (511-j) );
		}
		for (j = 512; j <512 + 256; j++)
		{
			cBuff[j] = (BYTE)( 255 - (j - 512) );
		}
		for (j = 512 + 256; j <1024; j++)
		{
			cBuff[j] = (BYTE)( 255 - (1023 - j) );
		}
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle4()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	M = 1024 / 16;	// 64
	const long	N = 768 / 12;	// 64
	BOOL	boolSwitch;
	
	for (i = 0; i < 768; i++)
	{
		boolSwitch = (i/N)%2;
		if ( boolSwitch )
		{
			for (j = 0; j < 1024; j++)
				cBuff[j] = (BYTE)( (j/M)%2 == 0 ? 0 : 255 );
		}
		else
		{
			for (j = 0; j < 1024; j++)
				cBuff[j] = (BYTE)( (j/M)%2 == 1 ? 0 : 255 );
		}
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle5()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	M = 1024 / 8;	// 128
	const long	N = 768 / 6;	// 128
	BOOL	boolSwitch;
	
	for (i = 0; i < 768; i++)
	{
		boolSwitch = (i/N)%2;
		if ( boolSwitch )
		{
			for (j = 0; j < 1024; j++)
				cBuff[j] = (BYTE)( (j/M)%2 == 0 ? 0 : 255 );
		}
		else
		{
			for (j = 0; j < 1024; j++)
				cBuff[j] = (BYTE)( (j/M)%2 == 1 ? 0 : 255 );
		}
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle6()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	M = 1024 / 16;	// 64
	const long	N = 768 / 12;	// 64
	BOOL	boolSwitch;
	BOOL	boolSwitchLine;
	
	for (i = 0; i < 768; i++)
	{
		boolSwitch = (i/N)%2;
		if ( boolSwitch )
		{
			for (j = 0; j < 1024; j++)
			{
				boolSwitchLine = (j/M)%2 == 0 ? 1 : 0;
				if ( boolSwitchLine )
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 0) ? 0 : 255 );
				else
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 1) ? 0 : 255 );
			}
		}
		else
		{
			for (j = 0; j < 1024; j++)
			{
				boolSwitchLine = (j/M)%2 == 0 ? 1 : 0;
				if ( boolSwitchLine )
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 1) ? 0 : 255 );
				else
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 0) ? 0 : 255 );
			}
		}
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle7()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	M = 1024 / 8;	// 128
	const long	N = 768 / 6;	// 128
	BOOL	boolSwitch;
	BOOL	boolSwitchLine;
	
	for (i = 0; i < 768; i++)
	{
		boolSwitch = (i/N)%2;
		if ( boolSwitch )
		{
			for (j = 0; j < 1024; j++)
			{
				boolSwitchLine = (j/M)%2 == 0 ? 1 : 0;
				if ( boolSwitchLine )
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 0) ? 0 : 255 );
				else
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 1) ? 0 : 255 );
			}
		}
		else
		{
			for (j = 0; j < 1024; j++)
			{
				boolSwitchLine = (j/M)%2 == 0 ? 1 : 0;
				if ( boolSwitchLine )
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 1) ? 0 : 255 );
				else
					cBuff[j] = (BYTE)( (((j+i)/M)%2 == 0) ? 0 : 255 );
			}
		}
		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle8()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	MN = 64;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 1024; j++)
			cBuff[j] = (BYTE)( ((i+j)/MN)%2 == 0 ? 0 : 255 );

		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle9()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	MN = 128;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 1024; j++)
			cBuff[j] = (BYTE)( ((i+j)/MN)%2 == 0 ? 0 : 255 );

		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle10()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	MN = 64;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 1024; j++)
			cBuff[j] = (BYTE)( ((i+1023-j)/MN)%2 == 0 ? 0 : 255 );

		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle11()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	MN = 128;
	
	for (i = 0; i < 768; i++)
	{
		for (j = 0; j < 1024; j++)
			cBuff[j] = (BYTE)( ((i+1023-j)/MN)%2 == 0 ? 0 : 255 );

		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle12()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	M = 1024 / 16;	// 64
	const long	N = 768 / 12;	// 64
	CPoint	ptQuadrant;
	
	for (i = 0; i < 768; i++)
	{
		ptQuadrant.x = (i/N)%2;
		for (j = 0; j < 1024; j++)
		{
			ptQuadrant.y = (j/M)%2;

			if ( ptQuadrant == CPoint(0,0) )
				cBuff[j] = (BYTE)( ((N-1) - i%N)*M < (j%M)*N ? 255 : 0 );
			else if ( ptQuadrant == CPoint(0,1) )
				cBuff[j] = (BYTE)( (i%N)*M < (j%M)*N ? 0 : 255 );
			else if ( ptQuadrant == CPoint(1,0) )
				cBuff[j] = (BYTE)( (i%N)*M <= (j%M)*N ? 255 : 0 );
			else //if ( ptQuadrant == CPoint(1,1) )
				cBuff[j] = (BYTE)( ((N-1) - i%N)*M < (j%M)*N ? 0 : 255 );
		}

		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

void ImageStandStyle13()
{
	BYTE	cBuff[FrameMaxW];
	long	i, j;

	const long	M = 1024 / 8;	// 128
	const long	N = 768 / 6;	// 128
	CPoint	ptQuadrant;
	
	for (i = 0; i < 768; i++)
	{
		ptQuadrant.x = (i/N)%2;
		for (j = 0; j < 1024; j++)
		{
			ptQuadrant.y = (j/M)%2;

			if ( ptQuadrant == CPoint(0,0) )
				cBuff[j] = (BYTE)( ((N-1) - i%N)*M < (j%M)*N ? 255 : 0 );
			else if ( ptQuadrant == CPoint(0,1) )
				cBuff[j] = (BYTE)( (i%N)*M < (j%M)*N ? 0 : 255 );
			else if ( ptQuadrant == CPoint(1,0) )
				cBuff[j] = (BYTE)( (i%N)*M <= (j%M)*N ? 255 : 0 );
			else //if ( ptQuadrant == CPoint(1,1) )
				cBuff[j] = (BYTE)( ((N-1) - i%N)*M < (j%M)*N ? 0 : 255 );
		}

		////////////////////////////////////////////////////
		CRect	rcTemp;
		long	lArea;

		rcTemp.left		= 0;
		rcTemp.right	= 1024;
		rcTemp.top		= i;
		rcTemp.bottom	= i+1;
		lArea = rcTemp.Width() * rcTemp.Height();

		CSemVirtualMCICard::Instance().Write( cBuff, lArea, rcTemp );
		////////////////////////////////////////////////////
	}
}

/////////////////////////////////////////////////////////////////////////////

