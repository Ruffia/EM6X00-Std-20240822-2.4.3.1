#include "stdafx.h"
#include "FrameRateControler.h"
#include "RectConvertor.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"


CFrameRateControler::CFrameRateControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay):
	m_mutexLayAll(cs),m_lpDD(lpDD),m_lpDDSGraphBackLay(lpDDSGraphBackLay)
{
	m_lpDDSFrameRate		= NULL;		// Offscreen surface Info Lay

	/////////////////////////////////////////////////////////////////
	//m_boolShowFrameRate	= TRUE;
	m_boolShowFrameRate		= FALSE;

	m_hFontFrameRate		= NULL;

	m_szFrameRate[0]		= '\0';
	sprintf( m_szFPS, "%s", "FPS %03d BPS %04dK IPS %03d" );

	m_dwFrameTime			= 0;

	m_dwFrameCount			= 0;
	m_dwFramesLast			= 0;
	m_dwFrames				= 0;
	m_iFrameRateX			= 0;

	m_dwAveFrameRate		= 0;
	m_dwAveFrameRateCount	= 0;

	m_iBpsRateX				= 0;

	m_dwInputsLast			= 0;
	m_dwInputs				= 0;
	m_iIPSRateX				= 0;
}


CFrameRateControler::~CFrameRateControler()
{
}


void CFrameRateControler::ReleaseLay()
{
	if( m_lpDDSFrameRate != NULL )
	{
		m_lpDDSFrameRate->Release();
		m_lpDDSFrameRate = NULL;
	}
}


void CFrameRateControler::Close()
{
	if (m_hFontFrameRate != NULL)
	{
		DeleteObject(m_hFontFrameRate);
		m_hFontFrameRate = NULL;
	}
}



BOOL CFrameRateControler::ShowFrameRate(BOOL boolShow)
{
	m_boolShowFrameRate = boolShow;
	return boolShow;
}

// InitNumSurface
//
long CFrameRateControler::InitNumSurface()
{
    HDC				hdc;
    RECT			rc;
    int				len;
    HRESULT         ddrval = DD_OK;

    m_dwFramesLast = 0;

    len = wsprintf( m_szFrameRate, m_szFPS, 0, 0, 0 );

	if( m_lpDDSFrameRate != NULL )
	{
		ddrval = m_lpDDSFrameRate->GetDC( &hdc );
		if( ddrval == DD_OK )
		{
			SelectObject(hdc, m_hFontFrameRate);
			SetTextColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrFrameRate]);
			SetBkColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent]);
			SetBkMode(hdc, OPAQUE);
			SetRect(&rc, 0, 0, 10000, 10000);
			ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, m_szFrameRate, len, NULL);
			GetTextExtentPoint(hdc, m_szFrameRate, 4, &m_sizeFPS);
			m_iFrameRateX = m_sizeFPS.cx;
			GetTextExtentPoint(hdc, m_szFrameRate, 12, &m_sizeFPS);
			m_iBpsRateX = m_sizeFPS.cx;
			GetTextExtentPoint(hdc, m_szFrameRate, 22, &m_sizeFPS);
			m_iIPSRateX = m_sizeFPS.cx;
			GetTextExtentPoint(hdc, m_szFrameRate, len, &m_sizeFPS);

			m_lpDDSFrameRate->ReleaseDC(hdc);
		}
	}

	return ddrval;
} // InitNumSurface

//
// InitFrameRate
//
BOOL CFrameRateControler::InitFrameRate(DWORD& dwFrameMemSizeUsed,DWORD dwVidMemTotal)
{
	///////////////////////////////////////////
//	if (!m_boolShowFrameRate)
//		return TRUE;
	///////////////////////////////////////////

    HRESULT             ddrval;
    HDC             	hdc;

    if (m_hFontFrameRate != NULL)
    {
        DeleteObject(m_hFontFrameRate);
    }

    m_hFontFrameRate = CreateFont(
        24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial" );

    //
    // make a sample string so we can measure it with the current font.
    //
    if (m_hFontFrameRate != NULL)
    {
        InitNumSurface();

        hdc = GetDC(NULL);
        SelectObject(hdc, m_hFontFrameRate);
        GetTextExtentPoint(hdc, m_szFrameRate, lstrlen(m_szFrameRate), &m_sizeFPS);
        ReleaseDC(NULL, hdc);
    }
    
	//
    // Create a surface to copy our bits to.
    //
    DDSURFACEDESC			ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= m_sizeFPS.cx;
    ddsd.dwHeight		= m_sizeFPS.cy;

	DWORD dwFrameMemSize = ddsd.dwWidth * ddsd.dwHeight;
	if ( (dwFrameMemSizeUsed + dwFrameMemSize) > dwVidMemTotal )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	else
	{
		dwFrameMemSizeUsed += dwFrameMemSize;
	}

    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSFrameRate, NULL );
    if ( ddrval != DD_OK )
	{
 		m_lpDDSFrameRate = NULL;
		return FALSE;
	}

    // now draw the text for real
    InitNumSurface();

    return TRUE;
}

//
// FormatFrameRate
//
long CFrameRateControler::FormatFrameRate(DWORD& dwBytes,DWORD& dwByteCount,DWORD& m_dwInputCount,DWORD& dwBytesLast)
{
	///////////////////////////////////////////
	if (!m_boolShowFrameRate)
		return DD_OK;
	///////////////////////////////////////////

    DWORD               time2;
    char                buff[256];
    char                buff_bps[256];
    char                buff_ips[256];
    HDC                 hdc;
    HRESULT             ddrval;
    CRect               rcFrameRate;
	BOOL				blUpdate = FALSE;
    DWORD               dwKBPS;

    m_dwFrameCount++;
    time2 = timeGetTime() - m_dwFrameTime;
    if( time2 > 1000 )
    {
        m_dwFrames = (m_dwFrameCount*1000)/time2;
        m_dwFrameTime = timeGetTime();
        m_dwFrameCount = 0;

        m_dwAveFrameRate += m_dwFrames;
        m_dwAveFrameRateCount++;

		dwBytes		= (dwByteCount*1000)/time2;
		dwByteCount	= 0;

		m_dwInputs		= (m_dwInputCount*1000)/time2;
		m_dwInputCount	= 0;
    }

    if( m_dwFrames != 0 )
    {
		if( dwBytes != dwBytesLast )
		{
			dwBytesLast = dwBytes;
			blUpdate = TRUE;
		}
		if( m_dwFrames != m_dwFramesLast )
		{
			m_dwFramesLast = m_dwFrames;
			blUpdate = TRUE;
		}
		if( m_dwInputs != m_dwInputsLast )
		{
			m_dwInputsLast = m_dwInputs;
			blUpdate = TRUE;
		}
	}

	if ( blUpdate )
	{
		ddrval = m_lpDDSFrameRate->GetDC( &hdc );
		if( ddrval == DD_OK )
        {
			if( m_dwFrames > 999 )
			{
				buff[0] = (char)('9');
				buff[1] = (char)('9');
				buff[2] = (char)('9');
			}
			else if( m_dwFrames > 99 )
			{
				buff[0] = (char)((m_dwFrames / 100)     + '0');
				buff[1] = (char)((m_dwFrames / 10) % 10 + '0');
				buff[2] = (char)((m_dwFrames % 10)      + '0');
			}
			else if( m_dwFrames > 9 )
			{
				buff[0] = (char)('0');
				buff[1] = (char)((m_dwFrames / 10) + '0');
				buff[2] = (char)((m_dwFrames % 10) + '0');
			}
			else
			{
				buff[0] = (char)('0');
				buff[1] = (char)('0');
				buff[2] = (char)((m_dwFrames % 10) + '0');
			}
			//sprintf( buff, "%3d", dwFrames );

			if( m_dwInputs > 999 )
			{
				buff_ips[0] = (char)('9');
				buff_ips[1] = (char)('9');
				buff_ips[2] = (char)('9');
			}
			else if( m_dwInputs > 99 )
			{
				buff_ips[0] = (char)((m_dwInputs / 100)     + '0');
				buff_ips[1] = (char)((m_dwInputs / 10) % 10 + '0');
				buff_ips[2] = (char)((m_dwInputs % 10)      + '0');
			}
			else if( m_dwInputs > 9 )
			{
				buff_ips[0] = (char)('0');
				buff_ips[1] = (char)((m_dwInputs / 10) + '0');
				buff_ips[2] = (char)((m_dwInputs % 10) + '0');
			}
			else
			{
				buff_ips[0] = (char)('0');
				buff_ips[1] = (char)('0');
				buff_ips[2] = (char)((m_dwInputs % 10) + '0');
			}

			dwKBPS = dwBytesLast / 1024;
			if( dwKBPS > 9999 )
			{
				buff_bps[0] = (char)('9');
				buff_bps[1] = (char)('9');
				buff_bps[2] = (char)('9');
				buff_bps[3] = (char)('9');
			}
			else if( dwKBPS > 999 )
			{
				buff_bps[0] = (char)((dwKBPS / 1000)		+ '0');
				buff_bps[1] = (char)((dwKBPS / 100) % 10	+ '0');
				buff_bps[2] = (char)((dwKBPS / 10) % 10		+ '0');
				buff_bps[3] = (char)((dwKBPS % 10)			+ '0');
			}
			else if( dwKBPS > 99 )
			{
				buff_bps[0] = (char)('0');
				buff_bps[1] = (char)((dwKBPS / 100)			+ '0');
				buff_bps[2] = (char)((dwKBPS / 10) % 10		+ '0');
				buff_bps[3] = (char)((dwKBPS % 10)			+ '0');
			}
			else if( dwKBPS > 9 )
			{
				buff_bps[0] = (char)('0');
				buff_bps[1] = (char)('0');
				buff_bps[2] = (char)((dwKBPS / 10)			+ '0');
				buff_bps[3] = (char)((dwKBPS % 10)			+ '0');
			}
			else
			{
				buff_bps[0] = (char)('0');
				buff_bps[1] = (char)('0');
				buff_bps[2] = (char)('0');
				buff_bps[3] = (char)((dwKBPS % 10)			+ '0');
			}

					
            if(m_hFontFrameRate != NULL)
            {
                SelectObject(hdc, m_hFontFrameRate);
                SetTextColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrFrameRate]);
                SetBkColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent]);
                TextOut(hdc, m_iFrameRateX, 0, buff, 3);
                TextOut(hdc, m_iBpsRateX, 0, buff_bps, 4);
                TextOut(hdc, m_iIPSRateX, 0, buff_ips, 3);
            }
            m_lpDDSFrameRate->ReleaseDC(hdc);
        }		
		return ddrval;
	}

    return DD_OK;
} // FormatFrameRate



long CFrameRateControler::UpdateFrameRateLay(LPDIRECTDRAWSURFACE& lpDDSImageLay, BOOL bReady)
{
	///////////////////////////////////////////
	if (!m_boolShowFrameRate)
		return DD_OK;
	///////////////////////////////////////////

    HRESULT             ddrval = DD_OK;
	/////////////////////////////////////////////////////////////////////////
	CPoint pos;
	pos.x = (CMCICardCommonSetting::Instance().m_szGraph.cx - m_sizeFPS.cx) / 2;
	pos.y = 20;

	CRect rcSrcGrapic	= CRect(0, 0, m_sizeFPS.cx, m_sizeFPS.cy) + pos;
	CRect rcDestImage	= LayWindowMapClip_Graph_to_Image( rcSrcGrapic );

	CRect rcSrcFrameRate = rcSrcGrapic;
	rcSrcFrameRate.OffsetRect( - pos );
	rcSrcFrameRate &= CRect(0, 0, m_sizeFPS.cx, m_sizeFPS.cy);

	if ( !rcDestImage.IsRectEmpty() )
	{
		if ( !bReady )
			return DD_OK;

		ddrval = lpDDSImageLay->Blt(
			(LPRECT)rcDestImage,	// dest rect
			m_lpDDSFrameRate,		// src surface
			(LPRECT)rcSrcFrameRate,	// src rect
			DDBLT_WAIT,
			NULL);

	}
	/////////////////////////////////////////////////////////////////////////

	return ddrval;
}

