#include "stdafx.h"
#include "OscillographControler.h"
#include "RectConvertor.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"
#include "Ddutil.h"

static const double g_dGraphRange[] = {10,
	2.5,	// 0xFFF
	1.25,
	0.625,
	0.3125,
	0.15625,
	0.078125,
	0.0390625,
	0.01853125,
	0.009265625};

COscillographControler::COscillographControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay):
	m_mutexLayAll(cs),m_lpDD(lpDD),m_lpDDSGraphBackLay(lpDDSGraphBackLay)
{
	m_bDebugOsc				= FALSE;
	m_debugOSC.b90			= FALSE;
	m_debugOSC.nIndexY		= 1;					// 13.04.03 ��Ƶʾ����Y��512���ش���+/-5
	m_debugOSC.dOffset		= 0.0;					// 13.08.05 ʾ����ƫ����

	m_bNeedMon = FALSE;

	m_hOscillLog	= NULL;
	m_lpDDSOscillographLay	= NULL;		// Offscreen surface Oscillograph Lay
}


COscillographControler::~COscillographControler()
{
}


BOOL COscillographControler::InitData(CString str)
{
	for (int i=0; i<256; i++)
		m_lGrayHistogramBuff[i] = 0;

	for (int i=0; i<8192; i++)
	{
		m_bOscillographBuff[i] = 0;
		m_bOscillographData[i] = 0;
	}

	if( m_hOscillLog != NULL )
	{
		CloseHandle(m_hOscillLog);
		m_hOscillLog = NULL;
	}

	if ("" != str)
	{
		m_hOscillLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}

	return TRUE;
}


void COscillographControler::ReleaseLay()
{
	if( m_lpDDSOscillographLay != NULL )
	{
		m_lpDDSOscillographLay->Release();
		m_lpDDSOscillographLay = NULL;
	}
}

void COscillographControler::CloseLogHandle()
{
	if(m_hOscillLog != NULL )
	{
		CloseHandle(m_hOscillLog);
		m_hOscillLog = NULL;
	}
}


BOOL COscillographControler::_InitOscillographLay()
{
	//////////////////////////////////////////////////////////////////////////////
	m_boolShowOscillograph		= FALSE;
	m_boolRedrawOscillograph	= TRUE;

	m_ptOscillograph.x = 0;
	m_ptOscillograph.y = 0;
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	m_lOscillographState = 0;		// Video

	////////////////////////////////////////////////////////////
	for (int i=0; i<256; i++)
		m_lGrayHistogramBuff[i] = 0;
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	for (int i=0; i<8192; i++)
	{
		m_bOscillographBuff[i] = i % 256;
		m_bOscillographData[i] = i % 256;
	}
	////////////////////////////////////////////////////////////

	return TRUE;
}

void COscillographControler::CalculateOscillographSize( CSize &szReso ) 
{
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	switch( szReso.cx )
	{
	case 16:
	case 32:
	case 64:
	case 128:
	case 256:
		m_szOscill		= CSize( m_szOscill.cx, m_szImage.cy /2);
		break;
	default:
		if( m_bDebugOsc )
			m_szOscill		= CSize( m_szOscill.cx, m_szImage.cy/2 );
		else
			m_szOscill		= CSize( m_szOscill.cx, m_szImage.cy/4 );
		break;
	}
}

void COscillographControler::GetOscillographParam( CPoint& ptOscillograph )
{
	m_mutexLayAll.Lock();
	ptOscillograph = m_ptOscillograph;		// ʾ����λ��
	m_mutexLayAll.Unlock();
}



void COscillographControler::SetOscillographAmplifer(double dblAmplifer)
{
	m_dblOscillographAmplifer = dblAmplifer / 2.0;
}


void COscillographControler::UpdateOscillographNew(
	DWORD dwCounts, LPWORD pdwPixX, LPWORD pdwPixY, LPWORD pdwPixZ,
//	LPBYTE		pOscillographBuff,	// ʾ������ʾ����
	long		nScanLine,			// ��ǰɨ����
	long nStart, long nStop,
	bool        bDebugZ,
	int         nDataStoreMode)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
//	if (nStop > g_szUSB_FrameOscillograph.cx)
//		nStop = g_szUSB_FrameOscillograph.cx;
	nStop = m_szOscill.cx;

	if ( (nStart > nStop) || (nStart < 0) )
		nStart = 0;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	UpdateFramGrayHistogramNew(dwCounts, pdwPixX, pdwPixY, pdwPixZ);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	if ( m_lOscillographState == 0 )
	{
		// ��Ƶ�źţ�Ӧ�÷�����ǰɨ�������ڵ���һ�е�����
		if( nDataStoreMode > 0 )	// XYZ
		{
			DWORD dwMax, i;
			dwMax = ( dwCounts > m_szOscill.cx ) ? m_szOscill.cx : dwCounts;
			if( bDebugZ )			// ԭZ��12λ��
			{
				if( m_bDebugOsc )	// ʾ����ҲΪ����ģʽ����ʾ��������Ҳȡ12λ
				{
					for( i=0; i<dwMax; i++ )
					{
						if( m_debugOSC.b90 )
							m_bOscillographBuff[pdwPixY[i]] = pdwPixZ[i];
						else
							m_bOscillographBuff[pdwPixX[i]] = pdwPixZ[i];
					}
				}
				else				// ʾ����Ϊ����ģʽ����ʾ��������ȡbit11---bit4
				{
					for( i=0; i<dwMax; i++ )
					{
						if( m_debugOSC.b90 )
							m_bOscillographBuff[pdwPixY[i]] = pdwPixZ[i] /16;
						else
							m_bOscillographBuff[pdwPixX[i]] = pdwPixZ[i] /16;
					}
				}
			}
			else					// Z�Ѿ���8λ����
			{
				for( DWORD i=0; i<dwMax; i++ )
				{
					if( m_debugOSC.b90 )
						m_bOscillographBuff[pdwPixY[i]] = pdwPixZ[i];
					else
						m_bOscillographBuff[pdwPixX[i]] = pdwPixZ[i];
				}
			}
		}
		else	// ZZZ
			memcpy( &(m_bOscillographBuff[nStart]), pdwPixZ, nStop - nStart );
	}
	else
	{
		// ֱ��ͼ
		UpdateGrayHistogramNew(pdwPixZ, dwCounts,bDebugZ);
		nStart	= 0;
		nStop	= 256;
	}

	///////////////////////////////////////////////////////
	if( bDebugZ && m_bDebugOsc )
	{
		// ����ȡ12λ��ʾ�����߶ȱ��
		if ( m_lOscillographState == 0 )
		{
			double dRatio	= g_dGraphRange[m_debugOSC.nIndexY] / g_dGraphRange[1];
			double dHeight	= (double)(m_szOscill.cy /dRatio -1);
			double dHOsc	= (double)(m_szOscill.cy -1);
			double dMid		= dHeight /2 + m_debugOSC.dOffset;
			double dMax		= dHeight /2 + dHeight /2 * dRatio + m_debugOSC.dOffset;
			double dMin		= dHeight /2 - dHeight /2 * dRatio + m_debugOSC.dOffset;
			double dTemp	= 0.0;
			// ��Ƶ�ź�
			for (int i=nStart; i<nStop; i++ )
			{
/*				dTemp = dHeight * m_bOscillographBuff[i] / 0xFFF;	// ��0-0xFFFѹ����0-511
				if( dTemp > dMax )
					m_bOscillographData[i] = 0;						// �������ֵ����ͼ����Ϊ��Сֵ
				else if( dTemp < dMin )
					m_bOscillographData[i] = (WORD)dHeight;			// С����Сֵ����ͼ����Ϊ���ֵ
				else
					m_bOscillographData[i] = (WORD)(dHeight - ( dHeight * m_bOscillographBuff[i] / 0xFFF - dMin ) / dRatio);
*/
				dTemp = dHeight /0xFFF * m_bOscillographBuff[i];
				if( dTemp > dMax )
					m_bOscillographData[i] = 0;						// �������ֵ����ͼ����Ϊ��Сֵ
				else if( dTemp < dMin )
					m_bOscillographData[i] = (WORD)dHOsc;			// С����Сֵ����ͼ����Ϊ���ֵ
				else
					m_bOscillographData[i] = (WORD)(dHOsc - ( dHeight / 0xFFF * m_bOscillographBuff[i] - dMin ));
			}
		}
		else
		{
			double dRatio	= g_dGraphRange[m_debugOSC.nIndexY] / g_dGraphRange[1];
			double dHeight	= (double)(m_szOscill.cy -1);
			double dMid		= dHeight /2;
			double dMax		= dMid + dMid * dRatio;
			double dMin		= dMid - dMid * dRatio;
			double dTemp	= 0.0;
			// ֱ��ͼ
			// ��UpdateGrayHistogramNew�У��ѽ����������ֵ��Ϊ255
			// ����ֻ�轫������������ʾ������ʾ�߶ȼ���
			for( int i=nStart; i<nStop; i++ )
			{
				dTemp = dHeight * m_bOscillographBuff[i] / 0xFF;
				if( dTemp > dHeight )
					m_bOscillographData[i] = 0;
				else
					m_bOscillographData[i] = dHeight - dTemp;
			}
		}
	}
	else if( m_bDebugOsc )
	{
		// ����ȡ8λ��ʾ������ʾ�߶ȱ��
		double dHeight	= (double)(m_szOscill.cy -1);
		double dTemp = 0.0;
		for (int i=nStart; i<nStop; i++ )
		{
			dTemp = dHeight * m_bOscillographBuff[i] / 0xFF;	// ��0-0xFF����0-511
			
			if (dTemp > dHeight )
				m_bOscillographData[i] = 0;
			else
				m_bOscillographData[i] = dHeight - dTemp;
		}
	}
	else
	{
		// ����ȡ8Ϊ��ʾ������ʾ����
		double dHeight = (double)(m_szOscill.cy -1);
		double dTemp = 0.0;
		for (int i=nStart; i<nStop; i++ )
		{
			dTemp = dHeight * m_bOscillographBuff[i] / 0xFF;
			
			if (dTemp > dHeight)
				m_bOscillographData[i] = 0;
			else
				m_bOscillographData[i] = dHeight - dTemp;
		}
	}
	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		for(int i=0; i<m_szOscill.cx; i++ )
		{
			sprintf(Buf, "%d\t%04X\t%d\r\n", i, m_bOscillographData[i], m_bOscillographData[i] );
			WriteFile(m_hOscillLog, Buf, strlen(Buf), &cbRet, NULL);
		}
	}
	///////////////////////////////////////////////////////

	m_boolRedrawOscillograph = TRUE;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}


void COscillographControler::SetOscillographPos( CPoint ptOscillograph )
{
	if ( m_ptOscillograph == ptOscillograph )
		return;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	m_ptOscillograph = ptOscillograph;		// ʾ����λ��
	m_mutexLayAll.Unlock();
}





BOOL COscillographControler::ShowOscillograph(BOOL boolShow)
{
	if ( m_boolShowOscillograph == boolShow )
		return m_boolShowOscillograph;

	m_mutexLayAll.Lock();

	m_boolShowOscillograph = boolShow;

	/////////////////////////////////////////
	// Switch Oscillograph State between
	// Video & Histogram
	/////////////////////////////////////////
	if (!boolShow)
	{
		if ( m_lOscillographState == 0 )
			m_lOscillographState = 1;
		else
			m_lOscillographState = 0;
	}
	//////////////////////////////////////

	m_mutexLayAll.Unlock();

	return m_boolShowOscillograph;
}


void COscillographControler::UpdateOscillograph(
	LPBYTE		pOscillographBuff,	// ʾ������ʾ����
	long		nScanLine,			// ��ǰɨ����
	long nStart, long nStop,
	int     nDetectorMode,
	CSize&  szImage)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//	if (nStop > g_szUSB_FrameOscillograph.cx)
	//		nStop = g_szUSB_FrameOscillograph.cx;
	nStop = m_szOscill.cx;

	if ( (nStart > nStop) || (nStart < 0) )
		nStart = 0;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	UpdateFramGrayHistogram(pOscillographBuff, nStop - nStart, nScanLine);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	if ( m_lOscillographState == 0 )
	{
		memcpy( &(m_bOscillographBuff[nStart]), pOscillographBuff, nStop - nStart );
	}
	else
	{
		UpdateGrayHistogram(pOscillographBuff, nStop - nStart,nDetectorMode,szImage);
		//		nStart	= 0;
		//		nStop	= 1024;
	}

	///////////////////////////////////////////////////////
	for (int nTemp, i=nStart; i<nStop; i++ )
	{
		nTemp = (int)(m_bOscillographBuff[i] * m_dblOscillographAmplifer + 0.5);

		if (nTemp > 127)
			m_bOscillographData[i] = 0;
		else
			m_bOscillographData[i] = 127 - nTemp;
	}
	///////////////////////////////////////////////////////

	m_boolRedrawOscillograph = TRUE;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}


void COscillographControler::UpdateGrayHistogram(
	LPBYTE		pVideoDataBuff,		// ��Ƶ����
	long		nBuffLength,
	int     nDetectorMode,
	CSize&  szImage )
{
	BYTE	bGray;
	long	i, j, lBuff[256], lMax;

	for (i=0; i<256; i++)
		lBuff[i] = 0;

	for (i=0; i<nBuffLength; i++)
	{
		bGray = pVideoDataBuff[i];
		lBuff[bGray] ++;
	}

	lMax = 0;
	for (i=0; i<256; i++)
	{
		// Huffman fliter
		m_lGrayHistogramBuff[i] = (long)(m_lGrayHistogramBuff[i] * 0.75 + lBuff[i] * 0.25);

		if (lMax < m_lGrayHistogramBuff[i])
			lMax = m_lGrayHistogramBuff[i];
	}

	int nStayCounts = 0;
	if( nDetectorMode > 0 )
		nStayCounts = szImage.cx / 2 / 256;	// ˫̽����
	else
		nStayCounts = szImage.cx / 256;		// ��̽����
	for (i=0; i<256; i++)
	{
		//m_bOscillographBuff[4*i + 0 ] = (BYTE)(m_lGrayHistogramBuff[i] / 4);
		m_bOscillographBuff[nStayCounts*i] = (BYTE)((int)(m_lGrayHistogramBuff[i]*254.0 / lMax + 0.5));
	}
	if( nStayCounts > 0 )
	{
		for( i=0; i<256; i++ )
			for( j=1; j<nStayCounts; j++ )
				m_bOscillographBuff[nStayCounts*i + j] = m_bOscillographBuff[nStayCounts*i];
	}
}



void COscillographControler::UpdateGrayHistogramNew(
								   LPWORD	pVideoDataBuff,		// ��Ƶ����
								   long		nBuffLength,
								   bool     bDebugZ)
{
	BYTE	bGray;
	long	i, lBuff[256], lMax;
	
	for (i=0; i<256; i++)
		lBuff[i] = 0;
	
	for (i=0; i<nBuffLength; i++)
	{
		if( bDebugZ )	// Z��12λ��Ҫ��Z��Ϊ8λ�Ҷ�ֵ
			bGray = (BYTE)(pVideoDataBuff[i] /16);
		else
			bGray = pVideoDataBuff[i];
		lBuff[bGray] ++;
	}
	
	lMax = 0;
	for (i=0; i<256; i++)
	{
		// Huffman fliter
		m_lGrayHistogramBuff[i] = (long)(m_lGrayHistogramBuff[i] * 0.75 + lBuff[i] * 0.25);
		
		if (lMax < m_lGrayHistogramBuff[i])
			lMax = m_lGrayHistogramBuff[i];
	}
	
/*	int nStayCounts = 0;
	if( m_bSplit )
		nStayCounts = m_szImage.cx / 2 / 256;	// ˫̽����
	else
		nStayCounts = m_szImage.cx / 256;		// ��̽����
	if( nStayCounts > 0 )
	{
		for (i=0; i<256; i++)
		{
			//m_bOscillographBuff[4*i + 0 ] = (BYTE)(m_lGrayHistogramBuff[i] / 4);
			m_bOscillographBuff[nStayCounts*i] = (BYTE)((int)(m_lGrayHistogramBuff[i]*254.0 / lMax + 0.5));
			for( j=1; j<nStayCounts; j++ )
				m_bOscillographBuff[nStayCounts*i + j] = m_bOscillographBuff[nStayCounts*i];
		}
	}
	else
*/	{
		for (i=0; i<256; i++)
			m_bOscillographBuff[i] = (BYTE)((int)(m_lGrayHistogramBuff[i]*254.0 / lMax + 0.5));
	}
}





void COscillographControler::FramGrayHistogramStart()
{ 
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_boolFramGrayHistogramBuffReady = FALSE;
	m_boolFramGrayHistogramBuffStart = FALSE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}




void COscillographControler::FramGrayHistogramStop()
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_boolFramGrayHistogramBuffReady = TRUE;
	m_boolFramGrayHistogramBuffStart = FALSE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void COscillographControler::FramGrayHistogramReset()
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	for (int i=0; i<256; i++)
		m_dwFramGrayHistogramBuff[i] = 0;

	m_lFramGrayHistogramCurrentLine		= -1;
	m_lFramGrayHistogramCountLine		= 0;
	m_dwFramGrayHistogramCountPixel		= 0;
	m_boolFramGrayHistogramBuffStart	= FALSE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}


// return Pixel Counter
DWORD COscillographControler::CopyFramGrayHistogramBuff( DWORD* pFramGrayHistogramBuff )
{
	DWORD	dwRet;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	for (int i=0; i<256; i++)
		pFramGrayHistogramBuff[i] = m_dwFramGrayHistogramBuff[i];

	dwRet = m_dwFramGrayHistogramCountPixel;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////

	return dwRet;
}



void COscillographControler::UpdateFramGrayHistogram(
	LPBYTE		pVideoDataBuff,		// ��Ƶ����
	long		nBuffLength,
	long		lCurrentLine )
{
	BYTE	bGray;
	long	i;

	if ( m_boolFramGrayHistogramBuffReady )
		return;

	if ( !m_boolFramGrayHistogramBuffStart )
	{
		//////////////////////////////////////////////////////
		// ��׷�Ϸ�ȷ��һ֡ͼ��Ŀ�ʼ
		//////////////////////////////////////////////////////
		if ( m_lFramGrayHistogramCurrentLine >= lCurrentLine )
		{
			m_boolFramGrayHistogramBuffStart = TRUE;
			m_lFramGrayHistogramCurrentLine	 = -1;
		}
		else
		{
			m_lFramGrayHistogramCurrentLine = lCurrentLine;
		}
		//////////////////////////////////////////////////////

		return;
	}

	for (i=0; i<nBuffLength; i++)
	{
		bGray = pVideoDataBuff[i];
		m_dwFramGrayHistogramBuff[bGray] ++;
	}

	m_lFramGrayHistogramCountLine ++;
	m_dwFramGrayHistogramCountPixel += nBuffLength;

	//////////////////////////////////////////////////////
	// ��׷�Ϸ�ȷ��һ֡ͼ��Ľ���
	//////////////////////////////////////////////////////
	if ( m_lFramGrayHistogramCurrentLine >= lCurrentLine )
		m_boolFramGrayHistogramBuffReady = TRUE;
	else
		m_lFramGrayHistogramCurrentLine = lCurrentLine;
	//////////////////////////////////////////////////////
}




void COscillographControler::UpdateFramGrayHistogramNew(
	DWORD dwCounts, LPWORD pdwPixX, LPWORD pdwPixY, LPWORD pdwPixZ )
{
	BYTE	bGray;
	DWORD	i;

	if ( m_boolFramGrayHistogramBuffReady )
		return;

	if ( !m_boolFramGrayHistogramBuffStart )
	{
		m_boolFramGrayHistogramBuffStart = TRUE;
		m_lFramGrayHistogramCurrentLine	 = -1;

		//		return;
	}

	for (i=0; i<dwCounts; i++)
	{
		bGray = pdwPixZ[i];
		m_dwFramGrayHistogramBuff[bGray] ++;
	}
	m_dwFramGrayHistogramCountPixel += dwCounts;

	if ( m_lFramGrayHistogramCurrentLine >= pdwPixY[dwCounts-1] )
		m_boolFramGrayHistogramBuffReady = TRUE;
	else
		m_lFramGrayHistogramCurrentLine = pdwPixY[dwCounts-1];
	//////////////////////////////////////////////////////
}



CRect COscillographControler::LayWindowMapClip_Image_to_Oscillograph( CRect& rcImage )
{
	CRect	rcGrapic = LayWindowMapClip_Image_to_Graph( rcImage);
	CPoint	ptOrigin = m_ptOscillograph;

	CRect	rcOscillograph = rcGrapic;
	rcOscillograph.OffsetRect( - ptOrigin );
	rcOscillograph &= CRect(CPoint(0,0), m_szOscill);

	if( !rcOscillograph.IsRectEmpty() )
	{
		rcGrapic = rcOscillograph;
		rcGrapic.OffsetRect( ptOrigin );
		rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	}

	return rcOscillograph;
}


void COscillographControler::UpdateOscillographRect() 
{
	m_mutexLayAll.Lock();
	CRect rcGrapic;

	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	rcGrapic.left	= 0;
	rcGrapic.right	= CMCICardCommonSetting::Instance().m_szGraph.cx;
	rcGrapic.top	= m_ptOscillograph.y;
	rcGrapic.bottom = rcGrapic.top + CMCICardCommonSetting::Instance().m_szGraph.cy /2;//m_szOscill.cy;

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( m_boolShowOscillograph == TRUE )
	{
		m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		m_UpdateRectsOscillograph.Add( rcImage );
	}
	m_mutexLayAll.Unlock();
}

BOOL COscillographControler::InitOscillographLay(DWORD dwFrameMemSizeUsed,DWORD dwVidMemTotal,BOOL blSrcColorkey,BOOL bDebugZ)
{
    HRESULT             ddrval;

	_InitOscillographLay();

	//////////////////////////////////////////////////////////////////////////////
    // Create a surface to copy our bits to.
	//////////////////////////////////////////////////////////////////////////////
    DDSURFACEDESC			ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= m_szOscill.cx;
    ddsd.dwHeight		= CMCICardCommonSetting::Instance().m_szGraph.cy /2;//m_szOscill.cy;

	DWORD dwFrameMemSize = ddsd.dwWidth * ddsd.dwHeight;
	if ( (dwFrameMemSizeUsed + dwFrameMemSize) > dwVidMemTotal )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	else
	{
		dwFrameMemSizeUsed += dwFrameMemSize;
	}

    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSOscillographLay, NULL );
    if ( ddrval != DD_OK )
	{
 		m_lpDDSOscillographLay = NULL;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////
	DWORD dwColorKey = DDColorMatch(m_lpDDSOscillographLay, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent]);

    if (blSrcColorkey)
    {
		DDCOLORKEY		ddck;
        ddck.dwColorSpaceLowValue	= dwColorKey;
        ddck.dwColorSpaceHighValue	= dwColorKey;
        m_lpDDSOscillographLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
    }
	//////////////////////////////////////////////////////////////////////////////
	
	RedrawOscillographLay(bDebugZ);

	SetOscillographAmplifer();

    return TRUE;
}

long COscillographControler::RedrawOscillographLay(BOOL bDebugZ)
{
	HDC				hdc;
    HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSOscillographLay == NULL ) return DD_OK;
	ddrval = m_lpDDSOscillographLay->GetDC( &hdc );
	if( ddrval != DD_OK )
		return ddrval;
	////////////////////////////////////////////////////////////////
	HBRUSH brTransBrush = CreateSolidBrush( CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] );
	HPEN hPenOld;
	HPEN hPenGrid;
	if( m_bDebugOsc )
		hPenGrid = CreatePen(PS_SOLID, 0, RGB( 0,0,255));
	else
		hPenGrid = CreatePen(PS_SOLID, 0, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrOscillographGrid]);
	HPEN hPenGraph;
	if( m_bDebugOsc )
	{
/*		if( m_Resolution > USB_RESO_256_256 )
			hPenGraph = CreatePen(PS_SOLID, 1, RGB(255,0,0));
		else
*/			hPenGraph = CreatePen(PS_SOLID, 0, RGB(255,0,0));
	}
	else
		hPenGraph = CreatePen(PS_SOLID, 0, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrOscillographGraph]);
	/////////////////////////////////////////////////////////////////

	CRect rcFill = CRect(0,0,m_szOscill.cx,m_szOscill.cy);
	FillRect( hdc, (LPRECT)rcFill, brTransBrush );

	long nX, nY;
	CString str;

	/////////////////////////////////////////////////////////////////
	// Draw Grid
	/////////////////////////////////////////////////////////////////
	hPenOld = (HPEN)SelectObject(hdc, hPenGrid);
	
	for( nX=0; nX<m_szOscill.cx; nX+=32 )
	{
		MoveToEx ( hdc, nX, 0, NULL );
		LineTo ( hdc, nX, m_szOscill.cy );
	}
	MoveToEx ( hdc, m_szOscill.cx-1, 0, NULL );
	LineTo ( hdc, m_szOscill.cx-1, m_szOscill.cy );
	if( bDebugZ && m_bDebugOsc )
	{
		COLORREF clrText = SetTextColor(hdc, RGB(255,255,255));
		COLORREF clrBK = SetBkColor(hdc, RGB(0,0,1) );
		for( nX=0; nX<m_szOscill.cx; nX+=128 )
		{
			str.Format( "%d", nX );
			TextOut( hdc, nX, m_szOscill.cy -20, str, str.GetLength() );
		}
		SetTextColor(hdc, clrText);
		SetBkColor(hdc, clrBK);
	}

	for( nY=0; nY<m_szOscill.cy; nY+=32 )
	{
		MoveToEx ( hdc, 0, nY, NULL );
		LineTo ( hdc, m_szOscill.cx, nY );
	}
	MoveToEx ( hdc, 0, m_szOscill.cy-1, NULL );
	LineTo ( hdc, m_szOscill.cx, m_szOscill.cy-1 );

	if( bDebugZ && m_bDebugOsc && m_lOscillographState == 0 )
	{
		COLORREF clrText = SetTextColor(hdc, RGB(255,255,255));
		COLORREF clrBK = SetBkColor(hdc, RGB(0,0,1) );

		double dRatio	= g_dGraphRange[m_debugOSC.nIndexY] / g_dGraphRange[1];
		double dHeight	= (double)(m_szOscill.cy /dRatio -1);
		double dMid		= (g_dGraphRange[1] * 2) * m_debugOSC.dOffset / dHeight;
		double dMax		= dMid + g_dGraphRange[m_debugOSC.nIndexY];
		double dMin		= dMid - g_dGraphRange[m_debugOSC.nIndexY];
		str.Format( "%.4f", dMin );
		TextOut( hdc, 1, m_szOscill.cy -20, str, str.GetLength() );
		str.Format( "%.4f", dMid );
		TextOut( hdc, 1, m_szOscill.cy/2, str, str.GetLength() );
		str.Format( "%.4f", dMax );
		TextOut( hdc, 1, 0, str, str.GetLength() );

		HBRUSH hBrush = CreateSolidBrush( RGB(0,192,0) );
		int nOffset;
		if( CMCICardCommonSetting::Instance().m_sizeReso.cx > 256 )
			nOffset = 4;
		else
			nOffset = 2;
		FillRect( hdc, CRect(0,								m_szOscill.cy /4-nOffset,	nOffset*2,					m_szOscill.cy /4+nOffset),		hBrush );
		FillRect( hdc, CRect(0,								m_szOscill.cy *3/4-nOffset,	nOffset*2,					m_szOscill.cy *3/4+nOffset),	hBrush );
		FillRect( hdc, CRect(m_szOscill.cx -1-nOffset*2,	m_szOscill.cy /4-nOffset,	m_szOscill.cx -1,			m_szOscill.cy /4+nOffset),		hBrush );
		FillRect( hdc, CRect(m_szOscill.cx -1-nOffset*2,	m_szOscill.cy *3/4-nOffset,	m_szOscill.cx -1,			m_szOscill.cy *3/4+nOffset),	hBrush );
		FillRect( hdc, CRect(m_szOscill.cx -1-nOffset*2,	m_szOscill.cy /2-nOffset,	m_szOscill.cx -1,			m_szOscill.cy /2+nOffset),		hBrush );
		FillRect( hdc, CRect(m_szOscill.cx /2-nOffset,		m_szOscill.cy /4-nOffset,	m_szOscill.cx /2 +nOffset,	m_szOscill.cy /4+nOffset),		hBrush );
		FillRect( hdc, CRect(m_szOscill.cx /2-nOffset,		m_szOscill.cy *3/4-nOffset,	m_szOscill.cx /2 +nOffset,	m_szOscill.cy *3/4+nOffset),	hBrush );
		FillRect( hdc, CRect(m_szOscill.cx /2-nOffset,		m_szOscill.cy /2-nOffset,	m_szOscill.cx /2 +nOffset,	m_szOscill.cy /2+nOffset),		hBrush );
		DeleteObject( hBrush );

		SetTextColor(hdc, clrText);
		SetBkColor(hdc, clrBK);
	}

	SelectObject(hdc, hPenOld);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Draw Graph
	/////////////////////////////////////////////////////////////////
	hPenOld = (HPEN)SelectObject(hdc, hPenGraph);
	
	nX = 0;
	nY = m_bOscillographData[nX];
	MoveToEx ( hdc, nX, nY, NULL );
	if( m_lOscillographState == 0 )
	{
		// ��Ƶʾ�����������Ƕ��پ��Ƕ���
		for( nX=1; nX<m_szOscill.cx; nX++ )
		{
			nY = m_bOscillographData[nX];
			LineTo ( hdc, nX, nY );
		}
	}
	else
	{
		// ֱ��ͼ����������256��Ҫ����ɨ������
		for( int i=1; i<256; i++ )
		{
			nX = (int)( 1.0 *i *m_szOscill.cx /256 );
			nY = m_bOscillographData[i];
			LineTo ( hdc, nX, nY );
		}
	}

	SelectObject(hdc, hPenOld);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	DeleteObject( hPenGrid );
	DeleteObject( hPenGraph );
	DeleteObject( brTransBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSOscillographLay == NULL ) return DD_OK;
	m_lpDDSOscillographLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic;
	
	rcGrapic.left	= 0;
	rcGrapic.right	= CMCICardCommonSetting::Instance().m_szGraph.cx;
	rcGrapic.top	= m_ptOscillograph.y;
	rcGrapic.bottom = rcGrapic.top + CMCICardCommonSetting::Instance().m_szGraph.cy /2;//m_szOscill.cy;

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( m_boolShowOscillograph == TRUE )
	{
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}



long COscillographControler::UpdateOscillographLay(BOOL bReady)
{
	CRect			rcTemp;
    HRESULT			ddrval = S_FALSE;

	CRectManager& m_UpdateRectsGraphBack = CMCICardCommonSetting::Instance().m_UpdateRectsGraphBack;

	while ( !m_UpdateRectsOscillograph.IsEmpty() )
	{
		rcTemp = m_UpdateRectsOscillograph.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			m_UpdateRectsGraphBack.Add( rcTemp );

			if ( m_boolShowOscillograph )
			{
				CRect rcDestImageLay	= rcTemp;
				CRect rcSrcOscillographLay	= LayWindowMapClip_Image_to_Oscillograph( rcDestImageLay );

				/*
				ddrval = m_lpDDSImageLay->Blt(
					(LPRECT)rcDestImageLay,			// dest rect
					m_lpDDSOscillographLay,				// src surface
					(LPRECT)rcSrcOscillographLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
				*/
				
				CRect rcDestGraphLay = LayWindowMapClip_Image_to_Graph( rcDestImageLay);
				
				if ( !bReady ) return DD_OK;

				ddrval = m_lpDDSGraphBackLay->Blt(
					(LPRECT)rcDestGraphLay,			// dest rect
					m_lpDDSOscillographLay,				// src surface
					(LPRECT)rcSrcOscillographLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
			}
		}
	}

	return ddrval;
}
