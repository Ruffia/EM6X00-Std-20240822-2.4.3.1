// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCICard

extern	TCHAR	g_exeFullPath[255];
extern	int		g_index;

// 11.11.28
DWORD CMCICard::AnalyseDispXYZOdd( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( lpBuff == NULL )
		return 0;

	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;

	////////////////////////////////////////////////////////////////////
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Lock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	lImageLineOffset	= m_szImage.cx;
	////////////////////////////////////////////////////////////////////
	
	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		sprintf(Buf, "/BEGIN set z value\\\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
//		sprintf(Buf, "dwCounts = %ld\r\n", dwCounts );
//		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
	}
	
	DWORD dwOffset = 0;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	int		i, j, k, nLoop, nCounts, nIndexY1, nIndexY2;
	unsigned short* pnFrameData;

	BOOL bLastIsX, bLastIsY, bLastIsZa, bIsX, bIsY, bIsZa, bIsZb;
	unsigned short dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
	DWORD dwDestWidth	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeight	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsZa	= (m_dwZa != 0xFFFF);
	if( bLastIsZa )
	{
		bLastIsX = FALSE;
		bLastIsY = FALSE;
	}
	else	//if( bLastIsZa )
	{
		bLastIsY = (m_dwY != 0xFFFF);
		if( bLastIsY )
			bLastIsX = FALSE;
		else
			bLastIsX = (m_dwX != 0xFFFF);
	}

	for( j=0; j<LOOP_COUNTS; j++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];

		/////////////////////////////////////////////////////////////
		// 分析数据
		/////////////////////////////////////////////////////////////
//		dwCounts = PixConvertXYZ( &(((unsigned short*)lpBuff)[dwOffset]), lSize*2, m_pPixX, m_pPixY, m_pPixZ );
		// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
		pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
		nLoop = lSize +1;

		bIsX		= FALSE;		// 当前数据是X
		bIsY		= FALSE;		// 当前数据是Y
		bIsZa		= FALSE;		// 当前数据是Za
		bIsZb		= FALSE;		// 当前数据是Zb
		nCounts		= 0;			// 有效的数据个数

		for( k=1; k<nLoop; k++ )
		{
			dwData = pnFrameData[k];
			// 得到所需的掩码位
			dwBitsOn = dwData & 0xF000;

			if( bLastIsX )						// 上一个数据是X
			{
				bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
				if( bIsY )						// 进一步确定是否当前数据是Y值
					m_dwY = dwData & 0x0FFF;
			}
			if( bLastIsY )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZa		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZa		= TRUE;
				}
				else
					bIsZa = (dwBitsOn == dwMaskZ);// 进一步确定是否当前数据是Z值
				if( bIsZa )
					m_dwZa = dwData & 0x0FFF;
			}
			if( bLastIsZa )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZb		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZb		= TRUE;
				}
				else
					bIsZb = (dwBitsOn == dwMaskZ);
			}
			if( bIsZb )
			{
/*				if( !m_bUrgency && (dwData & 0x0001) == 0x0001 )
				{
					// 如果有中断请求，可能是高压掉了之类的紧急问题
					// 在Z的最低位，通过m_pMPComm向上位机发出警告信息
					if( m_pMPComm != NULL )
					{
						// 有中断发生，通知上位机
						USB_SEM_ControlMessage	mciMSG;
						mciMSG.bType			= 0x0;
						mciMSG.bCtlMSGLength	= 2;
						mciMSG.bMessageID		= 0x02;	//USB_TTTT_INT;
						mciMSG.bData[1]			= 0xFF;	// 不知道中断号，需要上位机读取端口B
						m_pMPComm->USB_PostMSGtoHost( mciMSG );
						m_bUrgency = TRUE;
					}
				}
*/
				// 硬件坐标是从1开始，软件要从0开始，所以需要调整
				if( m_dwX > 0 && m_dwY > 0 )
				{
					// 填充数组
					if( (m_dwX <= dwDestWidth) && (m_dwY <= dwDestHeight) )	// 13.02.22
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							// 如果Zb也找到了
//							if( bIsZb )
							{
								m_pPixX[nCounts] = m_dwX -1 +m_rcDestFrame.left;	// 常规面扫同样适用于区域扫描
								if( m_pPixX[nCounts] > m_szImage.cx -1 )
								{
									m_pPixX[nCounts] = m_szImage.cx -1;
								}
								m_pPixY[nCounts] = m_dwY -1 +m_rcDestFrame.top;		// 常规面扫同样适用于区域扫描
								if( m_pPixY[nCounts] > m_szImage.cy -1 )
								{
									m_pPixY[nCounts] = m_szImage.cy -1;
								}
//								m_pPixZ[nCounts] = (dwData & 0x0FFF ) /16;			// 12位的Z取高8位(右移4位)
								if( m_bDebugZ )
									dwData = dwData & 0x0FFF;
								else
								{
									if( (dwData & 0x0FFF) > 0x7FF )
										dwData = 0x7FF;
									dwData = (dwData & 0x0FFF) /8;	// 12位的Z取bit10---bit3(右移3位)
								}
								m_pPixZ[nCounts] = dwData;
								nCounts++;
							}

							if( m_dwX == dwDestWidth && m_dwY < dwDestHeight )	// 13.02.22
								m_bPeakX = TRUE;
							else if( m_dwX == dwDestWidth && m_dwY == dwDestHeight )
								m_bPeakY = TRUE;

							m_dwX	= 0xFFFF;
							m_dwY	= 0xFFFF;
							m_dwZa	= 0xFFFF;
						} //if( !m_bPeakX && !m_bPeakY )
						else //if( !m_bPeakX && !m_bPeakY )
						{
							m_dwX	= 0xFFFF;
							m_dwY	= 0xFFFF;
							m_dwZa	= 0xFFFF;
							bIsZa	= FALSE;
						} //if( !m_bPeakX && !m_bPeakY )
					}
				} //if( m_dwX > 0 && m_dwY > 0 )
				else //if( m_dwX > 0 && m_dwY > 0 )
				{
					// 当在X/Y的非有效数据段(valley)，要重新赋值
					m_dwX	= 0xFFFF;
					m_dwY	= 0xFFFF;
					m_dwZa	= 0xFFFF;
					bIsZa	= FALSE;
				} //if( m_dwX > 0 && m_dwY > 0 )
			} //if( bLastIsZa || bIsZb )

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// 进一步确定是否当前数据是X值
				m_dwX = dwData & 0x0FFF;

			// 刷新标志位
			bLastIsX	= bIsX;
			bLastIsY	= bIsY;
			bLastIsZa	= bIsZa;
			bIsX		= FALSE;
			bIsY		= FALSE;
			bIsZa		= FALSE;
			bIsZb		= FALSE;
		} //for( k=1; k<nLoop; k++ )
		///////////////////////////////////////////////////////////////////////////

		if( m_bStopScan )
			break;

		if( nCounts > 0 )
		{
			/////////////////////////////////////////////////////////////
			// 更新示波器
			/////////////////////////////////////////////////////////////
			UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_szOscill.cx );
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 刷新图像数据
			/////////////////////////////////////////////////////////////
			if( m_bDebugZ )
			{
				for( i=0; i<nCounts; i++ )
				{
					// set z value to point(x,y)
					((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)(m_pPixZ[i]/16);
				}
			}
			else
			{
				for( i=0; i<nCounts; i++ )
				{
					// set z value to point(x,y)
					((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
				}
			}
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 记录数据
			/////////////////////////////////////////////////////////////
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		// 更新索引
		dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		// 刷新图像区域
		/////////////////////////////////////////////////////////////
		if( nCounts > 0 )
		{
			nIndexY1 = m_pPixY[0];
			nIndexY2 = m_pPixY[nCounts -1];
		}
		if( m_szImage.cy > 512 )
		{
			if( nIndexY1 < nIndexY2 )
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, nIndexY1, m_rcDestFrame.right, nIndexY2+1) );
			}
			else
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, nIndexY1, m_rcDestFrame.right, m_szImage.cy) );
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_rcDestFrame.top, m_szImage.cx, nIndexY2+1) );
			}
//			m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_dwMinY, m_rcDestFrame.right, m_dwMaxY) );
		}
		else
			m_UpdateRectsImage.Add( rcFrame );
	}
	/////////////////////////////////////////////////////////////
	g_index++;
	if( m_bNeedMon )
	{
		if( m_hFileLog != NULL )
		{
			unsigned long cbRet;
			char Buf[32];
			sprintf(Buf, "\\END set z value/\r\n\r\n");
			WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			CloseHandle(m_hFileLog);
			m_hFileLog = NULL;
		}
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);
	////////////////////////////////////////////////////////////////////

	m_mutexImageDib.Unlock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	return i;
}

// 12.06.07
DWORD CMCICard::AnalyseDispXYZLineOdd( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( lpBuff == NULL )
		return 0;
	
	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	
	////////////////////////////////////////////////////////////////////
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Lock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}
	
	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}
	
	lImageLineOffset	= m_szImage.cx;
	////////////////////////////////////////////////////////////////////
	
	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		sprintf(Buf, "/BEGIN set z value\\\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
//		sprintf(Buf, "dwCounts = %ld\r\n", dwCounts );
//		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
	}
	
	DWORD dwOffset = 0;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	int		i, j, k, nLoop, nCounts, nIndexY1, nIndexY2;
	unsigned short* pnFrameData;
	
	BOOL bLastIsX, bLastIsY, bLastIsZa, bIsX, bIsY, bIsZa, bIsZb;
	unsigned short dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
	DWORD dwDestWidth	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeight	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsZa	= (m_dwZa != 0xFFFF);
	if( bLastIsZa )
	{
		bLastIsX = FALSE;
		bLastIsY = FALSE;
	}
	else	//if( bLastIsZa )
	{
		bLastIsY = (m_dwY != 0xFFFF);
		if( bLastIsY )
			bLastIsX = FALSE;
		else
			bLastIsX = (m_dwX != 0xFFFF);
	}

	for( j=0; j<LOOP_COUNTS; j++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];
		
		/////////////////////////////////////////////////////////////
		// 分析数据
		/////////////////////////////////////////////////////////////
//		dwCounts = PixConvertXYZ( &(((unsigned short*)lpBuff)[dwOffset]), lSize*2, m_pPixX, m_pPixY, m_pPixZ );
		// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
		pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
		nLoop = lSize +1;

		bIsX		= FALSE;		// 当前数据是X
		bIsY		= FALSE;		// 当前数据是Y
		bIsZa		= FALSE;		// 当前数据是Za
		bIsZb		= FALSE;		// 当前数据是Zb
		nCounts		= 0;			// 有效的数据个数

		for( k=1; k<nLoop; k++ )
		{
			dwData = pnFrameData[k];
			// 得到所需的掩码位
			dwBitsOn = dwData & 0xF000;

			if( bLastIsX )						// 上一个数据是X
			{
				bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
				if( bIsY )						// 进一步确定是否当前数据是Y值
					m_dwY = dwData & 0x0FFF;
			}
			if( bLastIsY )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZa		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZa		= TRUE;
				}
				else
					bIsZa = (dwBitsOn == dwMaskZ);	// 进一步确定是否当前数据是Z值
				if( bIsZa )
					m_dwZa = dwData & 0x0FFF;
			}
			if( bLastIsZa )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZb		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZb		= TRUE;
				}
				else
					bIsZb = (dwBitsOn == dwMaskZ);
			}
			if( bIsZb )
			{
/*				if( !m_bUrgency && (dwData & 0x0001) == 0x0001 )
				{
					// 如果有中断请求，可能是高压掉了之类的紧急问题
					// 在Z的最低位，通过m_pMPComm向上位机发出警告信息
					if( m_pMPComm != NULL )
					{
						// 有中断发生，通知上位机
						USB_SEM_ControlMessage	mciMSG;
						mciMSG.bType			= 0x0;
						mciMSG.bCtlMSGLength	= 2;
						mciMSG.bMessageID		= 0x02;	//USB_TTTT_INT;
						mciMSG.bData[1]			= 0xFF;	// 不知道中断号，需要上位机读取端口B
						m_pMPComm->USB_PostMSGtoHost( mciMSG );
						m_bUrgency = TRUE;
					}
				}
*/
				// 硬件坐标是从1开始，软件要从0开始，所以需要调整
				if( m_dwX > 0 && m_dwY > 0 )
				{
					// 填充数组
					if( (m_dwX <= dwDestWidth) && (m_dwY <= dwDestHeight) )	// 13.02.22
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							// 如果Zb也找到了
//							if( bIsZb )
							{
								m_pPixX[nCounts] = m_dwX -1;// +m_rcDestFrame.left;	// 常规面扫同样适用于区域扫描
								if( m_pPixX[nCounts] > m_szImage.cx -1 )
								{
									m_pPixX[nCounts] = m_szImage.cx -1;
								}
								m_pPixY[nCounts] = m_dwY -1;// +m_rcDestFrame.top;		// 常规面扫同样适用于区域扫描
								if( m_pPixY[nCounts] > m_szImage.cy -1 )
								{
									m_pPixY[nCounts] = m_szImage.cy -1;
								}
//								m_pPixZ[nCounts] = (dwData & 0x0FFF ) /16;			// 12位的Z取高8位(右移4位)
								if( m_bDebugZ )
									dwData = dwData & 0x0FFF;
								else
								{
									if( (dwData & 0x0FFF) > 0x7FF )
										dwData = 0x7FF;
									dwData = (dwData & 0x0FFF) /8;	// 12位的Z取bit10---bit3(右移3位)
								}
								m_pPixZ[nCounts] = dwData;
								nCounts++;
							}

							if( m_dwX == dwDestWidth && m_dwY < dwDestHeight )	// 13.02.22
								m_bPeakX = TRUE;
							else if( m_dwX == dwDestWidth && m_dwY == dwDestHeight )
								m_bPeakY = TRUE;

							m_dwX	= 0xFFFF;
							m_dwY	= 0xFFFF;
							m_dwZa	= 0xFFFF;
						} //if( !m_bPeakX && !m_bPeakY )
						else //if( !m_bPeakX && !m_bPeakY )
						{
							m_dwX	= 0xFFFF;
							m_dwY	= 0xFFFF;
							m_dwZa	= 0xFFFF;
							bIsZa	= FALSE;
						} //if( !m_bPeakX && !m_bPeakY )
					}
				} //if( m_dwX > 0 && m_dwY > 0 )
				else //if( m_dwX > 0 && m_dwY > 0 )
				{
					// 当在X/Y的非有效数据段(valley)，要重新赋值
					m_dwX	= 0xFFFF;
					m_dwY	= 0xFFFF;
					m_dwZa	= 0xFFFF;
					bIsZa	= FALSE;
				} //if( m_dwX > 0 && m_dwY > 0 )
			} //if( bLastIsZa || bIsZb )

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// 进一步确定是否当前数据是X值
				m_dwX = dwData & 0x0FFF;

			// 刷新标志位
			bLastIsX	= bIsX;
			bLastIsY	= bIsY;
			bLastIsZa	= bIsZa;
			bIsX		= FALSE;
			bIsY		= FALSE;
			bIsZa		= FALSE;
			bIsZb		= FALSE;
		} //for( k=1; k<nLoop; k++ )
		///////////////////////////////////////////////////////////////////////////

		if( m_bStopScan )
			break;

		if( nCounts > 0 )
		{
			/////////////////////////////////////////////////////////////
			// 更新示波器
			/////////////////////////////////////////////////////////////
			UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_szOscill.cx );
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 刷新图像数据（屏蔽，线扫描无需刷新图像数据）
			/////////////////////////////////////////////////////////////
//			for( i=0; i<nCounts; i++ )
//			{
				// set z value to point(x,y)
//				((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
//			}
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 记录数据
			/////////////////////////////////////////////////////////////
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}
		/////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////
		// 更新索引
		dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);
		/////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////
		// 刷新图像区域（屏蔽，线扫描无需刷新图像数据）
		/////////////////////////////////////////////////////////////
/*		if( nCounts > 0 )
		{
			nIndexY1 = m_pPixY[0];
			nIndexY2 = m_pPixY[nCounts -1];
		}
		if( m_szImage.cy > 512 )
		{
			if( nIndexY1 < nIndexY2 )
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, nIndexY1, m_rcDestFrame.right, nIndexY2+1) );
			}
			else
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, nIndexY1, m_rcDestFrame.right, m_szImage.cy) );
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_rcDestFrame.top, m_szImage.cx, nIndexY2+1) );
			}
//			m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_dwMinY, m_rcDestFrame.right, m_dwMaxY) );
		}
		else
			m_UpdateRectsImage.Add( rcFrame );
*/	}
	/////////////////////////////////////////////////////////////
	g_index++;
	if( m_bNeedMon )
	{
		if( m_hFileLog != NULL )
		{
			unsigned long cbRet;
			char Buf[32];
			sprintf(Buf, "\\END set z value/\r\n\r\n");
			WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			CloseHandle(m_hFileLog);
			m_hFileLog = NULL;
		}
	}
	
	::GlobalUnlock((HGLOBAL) m_hdibImage);
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Unlock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	
	return i;
}

// 13.01.05
DWORD CMCICard::AnalyseDispXYZSyncOdd( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( m_bSyncFinished )	// 如果一帧已经完成，就不分析了
		return 0;
	
	if( lpBuff == NULL )
		return 0;

	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;

	////////////////////////////////////////////////////////////////////
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Lock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	lImageLineOffset	= m_szImage.cx;
	////////////////////////////////////////////////////////////////////
	
	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		sprintf(Buf, "/BEGIN set z value\\\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
//		sprintf(Buf, "dwCounts = %ld\r\n", dwCounts );
//		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
	}
	
	DWORD dwOffset = 0;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	int		i, j, k, nLoop, nCounts, nIndexY1, nIndexY2;
	unsigned short* pnFrameData;

	BOOL bLastIsX, bLastIsY, bLastIsZa, bIsX, bIsY, bIsZa, bIsZb;
	unsigned short dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
	DWORD dwDestWidth	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeight	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsZa	= (m_dwZa != 0xFFFF);
	if( bLastIsZa )
	{
		bLastIsX = FALSE;
		bLastIsY = FALSE;
	}
	else	//if( bLastIsZa )
	{
		bLastIsY = (m_dwY != 0xFFFF);
		if( bLastIsY )
			bLastIsX = FALSE;
		else
			bLastIsX = (m_dwX != 0xFFFF);
	}
	
	for( j=0; j<LOOP_COUNTS; j++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];
		
		/////////////////////////////////////////////////////////////
		// 分析数据
		/////////////////////////////////////////////////////////////
//		dwCounts = PixConvertXYZ( &(((unsigned short*)lpBuff)[dwOffset]), lSize*2, m_pPixX, m_pPixY, m_pPixZ );
		// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
		pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
		nLoop = lSize +1;
		
		bIsX		= FALSE;		// 当前数据是X
		bIsY		= FALSE;		// 当前数据是Y
		bIsZa		= FALSE;		// 当前数据是Za
		bIsZb		= FALSE;		// 当前数据是Zb
		nCounts		= 0;			// 有效的数据个数
		
		for( k=1; k<nLoop; k++ )
		{
			dwData = pnFrameData[k];
			// 得到所需的掩码位
			dwBitsOn = dwData & 0xF000;

/*			if( (dwBitsOn & dwMaskBOF) == dwMaskBOF )
			{
				if( !m_bFindFirstBOF )
					m_bFindFirstBOF = TRUE;		// 第一帧的开始
				else
					m_bSyncFinished = TRUE;		// 第二帧的开始，也即同步扫描一帧已经结束了
			}
*/
			if( bLastIsX )						// 上一个数据是X
			{
				bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
				if( bIsY )						// 进一步确定是否当前数据是Y值
					m_dwY = dwData & 0x0FFF;
			}
			if( bLastIsY )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZa		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZa		= TRUE;
				}
				else
					bIsZa = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// 进一步确定是否当前数据是Z值
				if( bIsZa )
					m_dwZa = dwData & 0x0FFF;
			}
			if( bLastIsZa )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZb		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZb		= TRUE;
				}
				else
					bIsZb = (dwBitsOn == dwMaskZ);
			}
			if( bIsZb )
			{
/*				if( !m_bUrgency && (dwData & 0x0001) == 0x0001 )
				{
					// 如果有中断请求，可能是高压掉了之类的紧急问题
					// 在Z的最低位，通过m_pMPComm向上位机发出警告信息
					if( m_pMPComm != NULL )
					{
						// 有中断发生，通知上位机
						USB_SEM_ControlMessage	mciMSG;
						mciMSG.bType			= 0x0;
						mciMSG.bCtlMSGLength	= 2;
						mciMSG.bMessageID		= 0x02;	//USB_TTTT_INT;
						mciMSG.bData[1]			= 0xFF;	// 不知道中断号，需要上位机读取端口B
						m_pMPComm->USB_PostMSGtoHost( mciMSG );
						m_bUrgency = TRUE;
					}
				}
*/
				// 硬件坐标是从1开始，软件要从0开始，所以需要调整
				if( m_dwX > 0 && m_dwY > 0 )
				{
					// 填充数组
					if( (m_dwX <= dwDestWidth) && (m_dwY <= dwDestHeight) )	// 13.02.22
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							// 如果Zb也找到了
//							if( bIsZb )
							{
								m_pPixX[nCounts] = m_dwX -1 +m_rcDestFrame.left;	// 常规面扫同样适用于区域扫描
								if( m_pPixX[nCounts] > m_szImage.cx -1 )
								{
									m_pPixX[nCounts] = m_szImage.cx -1;
								}
								m_pPixY[nCounts] = m_dwY -1 +m_rcDestFrame.top;		// 常规面扫同样适用于区域扫描
								if( m_pPixY[nCounts] > m_szImage.cy -1 )
								{
									m_pPixY[nCounts] = m_szImage.cy -1;
								}
//								m_pPixZ[nCounts] = (dwData & 0x0FFF ) /16;			// 12位的Z取高8位(右移4位)
								if( m_bDebugZ )
									dwData = dwData & 0x0FFF;
								else
								{
									if( (dwData & 0x0FFF) > 0x7FF )
										dwData = 0x7FF;
									dwData = (dwData & 0x0FFF) /8;	// 12位的Z取bit10---bit3(右移3位)
								}
								m_pPixZ[nCounts] = dwData;
								nCounts++;
							}

							if( m_dwX == dwDestWidth && m_dwY < dwDestHeight )	// 13.02.22
								m_bPeakX = TRUE;
							else if( m_dwX == dwDestWidth && m_dwY == dwDestHeight )
							{
								m_bPeakY = TRUE;
								m_bSyncFinished = TRUE;
								break;	//return nCounts;
							}

							m_dwX	= 0xFFFF;
							m_dwY	= 0xFFFF;
							m_dwZa	= 0xFFFF;
						} //if( !m_bPeakX && !m_bPeakY )
						else //if( !m_bPeakX && !m_bPeakY )
						{
							m_dwX	= 0xFFFF;
							m_dwY	= 0xFFFF;
							m_dwZa	= 0xFFFF;
							bIsZa	= FALSE;
						} //if( !m_bPeakX && !m_bPeakY )
					}
				} //if( m_dwX > 0 && m_dwY > 0 )
				else //if( m_dwX > 0 && m_dwY > 0 )
				{
					// 当在X/Y的非有效数据段(valley)，要重新赋值
					m_dwX	= 0xFFFF;
					m_dwY	= 0xFFFF;
					m_dwZa	= 0xFFFF;
					bIsZa	= FALSE;
				} //if( m_dwX > 0 && m_dwY > 0 )
			} //if( bLastIsZa || bIsZb )

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// 进一步确定是否当前数据是X值
				m_dwX = dwData & 0x0FFF;

			// 刷新标志位
			bLastIsX	= bIsX;
			bLastIsY	= bIsY;
			bIsX		= FALSE;
			bIsY		= FALSE;
			bIsZa		= FALSE;
			bIsZb		= FALSE;
		} //for( k=1; k<nLoop; k++ )
		///////////////////////////////////////////////////////////////////////////

		if( m_bStopScan )
			break;

		if( nCounts > 0 )
		{
			/////////////////////////////////////////////////////////////
			// 更新示波器
			/////////////////////////////////////////////////////////////
			UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_szOscill.cx );
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 刷新图像数据
			/////////////////////////////////////////////////////////////
			if( m_bDebugZ )
			{
				for( i=0; i<nCounts; i++ )
				{
					// set z value to point(x,y)
					((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)(m_pPixZ[i]/16);
				}
			}
			else
			{
				for( i=0; i<nCounts; i++ )
				{
					// set z value to point(x,y)
					((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
				}
			}
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 记录数据
			/////////////////////////////////////////////////////////////
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}
		/////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////
		// 更新索引
		dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);
		/////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////
		// 刷新图像区域
		/////////////////////////////////////////////////////////////
		if( nCounts > 0 )
		{
			nIndexY1 = m_pPixY[0];
			nIndexY2 = m_pPixY[nCounts -1];
		}
		if( m_szImage.cy > 512 )
		{
			if( nIndexY1 < nIndexY2 )
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, nIndexY1, m_rcDestFrame.right, nIndexY2+1) );
			}
			else
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, nIndexY1, m_rcDestFrame.right, m_szImage.cy) );
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_rcDestFrame.top, m_szImage.cx, nIndexY2+1) );
			}
//			m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_dwMinY, m_rcDestFrame.right, m_dwMaxY) );
		}
		else
			m_UpdateRectsImage.Add( rcFrame );
		
		CRect rcIndication = CRect(0,0,8,nIndexY2);
		PhotoIndication( rcIndication, RGB(0,0,255) );
	}
	g_index++;
	if( m_bNeedMon )
	{
		if( m_hFileLog != NULL )
		{
			unsigned long cbRet;
			char Buf[32];
			sprintf(Buf, "\\END set z value/\r\n\r\n");
			WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			CloseHandle(m_hFileLog);
			m_hFileLog = NULL;
		}
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);
	////////////////////////////////////////////////////////////////////

	m_mutexImageDib.Unlock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	return i;
}
