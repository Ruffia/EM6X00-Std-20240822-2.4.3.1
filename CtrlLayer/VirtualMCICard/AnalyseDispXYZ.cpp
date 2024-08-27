// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"
#include "MCICardCommonSetting.h"
#include <vector>

#include "TestParameterManager.h"
#include "AlgorithmManager.h"
#include "ImageDataManager.h"
#include "AutoContrastManager.h"

#include <opencv2/opencv.hpp>  
#include <iostream>  
#include "time.h"   
#include <map> 


using namespace std;
using namespace cv;

#define CLIP_RANGE(value, min, max)  ( (value) > (max) ? (max) : (((value) < (min)) ? (min) : (value)) )
#define COLOR_RANGE(value)  CLIP_RANGE(value, 0, 255)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMCICard

extern	TCHAR	g_exeFullPath[255];
extern	int		g_index;
int     nTimeBC =0,nTime =0,nTimeBC1 = 0;
int     nkBC=0,nk=0;
float   fhmax = 0.0,fhmin = 255.0;
float   fv=0;
float   fpixel[800000] = {0.f};
int     numA=1,num=1;
BOOL    bGra = FALSE;
int     m_nhmin,m_nhmax;
int     nCounttotal_max = 0 , nCounttotal_min;
int		g_nBilateralFilterValue = 10;

// 11.11.28
DWORD CMCICard::ADxyz( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( lpBuff == NULL )
		return 0;

	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;

	m_mutexImageDib.Lock();

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
	}

	DWORD dwOffset = 0;
	unsigned long cbRet;
	char Buf[32];
	int	nCounts, nIndexY1, nIndexY2, nLoop2 = 0;
	unsigned short* pnFrameData;

	BOOL bLastIsX, bLastIsY, bIsX, bIsY, bIsZ;
	unsigned short dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
	DWORD dwDestWidthAbs	= m_rcDestFrame.Width();
	DWORD dwDestHeightAbs	= m_rcDestFrame.Height();
	DWORD dwDestWidthRel	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeightRel	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
	{
		bLastIsX= FALSE;		// 上一个数据是Y
	}
	else
	{
		bLastIsX= (m_dwX != 0xFFFF);// 上一个数据不是Y
	}

	unsigned long lSize = ((unsigned short*)lpBuff)[dwOffset];
	if( lSize > 31000 )
	{
		// 15.10.08 与MainLoop中有同步问题？？？
		// 冻结后，MainLoop中Read还是不等于Write，就会进入do循环
		// 而此时各数据存储区中无有效数据
		//
		// Scan中设定了dwDataSize不大于30000
		// 在此加一个总个数判断，可以避免无谓运算
		return 0;
	}

	pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);

	unsigned short FrameData[40960] = {0}; 
	memcpy(FrameData,pnFrameData,40960);

	// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
	int nLoop = lSize +1;

	bIsX		= FALSE;		// 当前数据是X
	bIsY		= FALSE;		// 当前数据是Y
	bIsZ		= FALSE;		// 当前数据是Z

	for(int m=1; m<nLoop;m++ )
	{
		nCounts		= 0;			// 有效的数据个数

		nLoop2 = m+10240 < nLoop ? m+10240:nLoop;

		for(int k=m; k<nLoop2; k++ )
		{
			dwData = pnFrameData[k];
			// 得到所需的掩码位
			dwBitsOn = dwData & 0xF000;

			if( bLastIsX )						// 上一个数据是X
			{
				bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
				if( bIsY )						// 进一步确定是否当前数据是Y值
				{
					m_dwY = dwData & 0x0FFF;
				}
			}

			if( bLastIsY )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 根据BOL恢复标志位PeakX
					bIsZ		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZ		= TRUE;
				}
				else
				{
					bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// 进一步确定是否当前数据是Z值
				}
			}

			if( bIsZ )
			{
				// 硬件坐标是从1开始，软件要从0开始，所以需要调整
				if( m_dwX > m_nXOffset && m_dwY > m_nYOffset )
				{
					// 填充数组
					if( m_dwX <= dwDestWidthAbs +m_nXOffset && m_dwY <= dwDestHeightAbs +m_nYOffset )
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							m_pPixX[nCounts] = m_dwX -m_nXOffset -1 +m_rcDestFrame.left;	// 常规面扫同样适用于区域扫描
							if( m_pPixX[nCounts] > dwDestWidthRel +m_nXOffset )		//m_szImage.cx -1 )
							{
								m_pPixX[nCounts] = dwDestWidthRel +m_nXOffset;		//m_szImage.cx -1;
							}

							m_pPixY[nCounts] = m_dwY -m_nYOffset -1 +m_rcDestFrame.top;		// 常规面扫同样适用于区域扫描
							if( m_pPixY[nCounts] > dwDestHeightRel +m_nYOffset )	//m_szImage.cy -1 )
							{
								m_pPixY[nCounts] = dwDestHeightRel +m_nYOffset;		//m_szImage.cy -1;
							}

					        CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
							m_pPixZsrc[ m_pPixY[nCounts] * m_sizeReso.cx + m_pPixX[nCounts] ] = dwData & 0x0FFF;
							if( m_bDebugZ )
							{
								dwData = dwData & 0x0FFF;
							}
							else
							{
								if( dwData > m_nMaxPix)
								{
									dwData = m_nMaxPix;
								}

								dwData = (dwData/* -0x600*/) /m_nDiv;	// 取值范围对应255---0灰度范围显示
							}
							m_pPixZ[nCounts] = dwData;
							m_pPixZOrigin[nCounts] = dwData;
							nCounts++;

							if( m_dwX == dwDestWidthAbs +m_nXOffset && m_dwY < dwDestHeightAbs +m_nYOffset )
							{
								m_bPeakX = TRUE;
							}
							else if( m_dwX == dwDestWidthAbs +m_nXOffset && m_dwY == dwDestHeightAbs +m_nYOffset )
							{
								m_bPeakY = TRUE;
							}

							m_dwX = 0xFFFF;
							m_dwY = 0xFFFF;
						}
						else
						{
							m_dwX = 0xFFFF;
							m_dwY = 0xFFFF;
						}
					}
				}
				else
				{
					// 当在X/Y的非有效数据段(valley)，要重新赋值
					m_dwX = 0xFFFF;
					m_dwY = 0xFFFF;
				}
			}

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// 进一步确定是否当前数据是X值
			{
				m_dwX = dwData & 0x0FFF;
			}

			// 刷新标志位
			bLastIsX	= bIsX;
			bLastIsY	= bIsY;
			bIsX		= FALSE;
			bIsY		= FALSE;
			bIsZ		= FALSE;
		}

		if( m_bStopScan )
			break;
		if(CAutoContrastManager::Instance().fAutoBC)
		{  
			if(FALSE ==CAutoContrastManager::Instance().bOptionState)
			{
			    if(nTimeBC < 786432)
				{
					for(int i =0;i<nCounts;i++)
					{
						if(m_pPixZOrigin[i] > fhmax)
							fhmax = m_pPixZOrigin[i];
						else if(m_pPixZOrigin[i] < fhmin)
							fhmin = m_pPixZOrigin[i];
						if(m_pPixZOrigin[i] > 127)
							nCounttotal_max ++;
						else
							nCounttotal_min ++ ;
					
					}
					if(fhmax > 255)
						fhmax =255;
					if(fhmin <0)
						fhmin =0;
					nTimeBC = nTimeBC + nCounts;
				 }

				else
				{
					float *fimgInfo = new float[3000]();
					float *m_pdst = new float[3000]();
					
					for (int i=0;i<nCounts;i++)
					{
						fimgInfo[i]=m_pPixZOrigin[i];
				
					}
					int fa,fb,ga,gb;
					if( (nCounttotal_max - nCounttotal_min) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa = 200,fb = 254;
					}
					else if( (nCounttotal_min - nCounttotal_max) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa =1,fb = 127;
					}
					else
					{
						ga = 1,gb = 254;
						fa = 40,fb = 240;
					}
						
					float k1 = ga/fa ;
					float k2 = (gb-ga)/(fb-fa);
					float k3 =  (255-gb)/(255-fb);
					int PosBri;   
					double B = CAutoContrastManager::Instance().nPosBright/100.;
					double c = CAutoContrastManager::Instance().nPosContrast/100. ;
					float G = CAutoContrastManager::Instance().fPosGamma/100. ;
					double kc = tan( (45 + 44 * c) / 180 * M_PI );
					//double kc=1;

					//hmax = k1*(hmax-hmin);

					for(int i =0;i<nCounts;i++)
					{
						
						//2022.11.18
						if(fimgInfo[i] < fa)
							m_pdst[i] = k1*fimgInfo[i];
						else if(fimgInfo[i] < fb)
							m_pdst[i] = k2*(fimgInfo[i]-fa)+ga;
						else
							m_pdst[i] = k3*(fimgInfo[i]-fb)+gb;

						m_pdst[i] = COLOR_RANGE((m_pdst[i]-127.5*(1-B))*kc+127.5*(1+B));

						m_pdst[i] = pow(((m_pdst[i]-fhmin)/(fhmax-fhmin)),(1.0/G))*(fhmax-fhmin);

						//输出
						m_pPixZ[i]=(WORD)m_pdst[i];
					}
					delete []fimgInfo ;
					delete []m_pdst;
					CAutoContrastManager::Instance().nCounth = nCounts;
					CAutoContrastManager::Instance().fMatrix = m_pdst;
					CAutoContrastManager::Instance().bStateBC = 1;
					
					
					nkBC=0;
				}
				
			}
			else
			{
				if(nTime < 786432)
				{
					for(int i =0;i<nCounts;i++)
					{
						if(m_pPixZOrigin[i] > fhmax)
							fhmax = m_pPixZOrigin[i];
						else if(m_pPixZOrigin[i] < fhmin)
							fhmin = m_pPixZOrigin[i];
						if(m_pPixZOrigin[i] > 127)
							nCounttotal_max ++;
						else
							nCounttotal_min ++ ;
									
					}
					if(fhmax > 255)
						fhmax =255;
					if(fhmin <0)
						fhmin =0;
					nTime = nTime + nCounts;

				 }
				else
				{
					nk = 0;
					float *fimgInfo = new float[10000]();
					float *m_pdst = new float[10000]();
					for (int i=0;i<nCounts;i++)
					{
						fimgInfo[i]=m_pPixZOrigin[i];

					}

					int fa,fb,ga,gb;
					if( (nCounttotal_max - nCounttotal_min) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa =200,fb = 254;
					}
					else if( (nCounttotal_min - nCounttotal_max) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa =1,fb = 127;
					}
					else
					{
						ga = 1,gb = 254;
						fa = 40,fb = 240;
					}

					float k1 = ga/fa ;
					float k2 = (gb-ga)/(fb-fa);
					float k3 =  (255-gb)/(255-fb);

					for(int i =0;i<nCounts;i++)
					{
						if(fimgInfo[i] < fa)
							m_pdst[i] = k1*fimgInfo[i];
						else if(fimgInfo[i] < fb)
							m_pdst[i] = k2*(fimgInfo[i]-fa)+ga;
						else
							m_pdst[i] = k3*(fimgInfo[i]-fb)+gb;

						//m_pPixZ[i]=(WORD)m_pdst[i];
					}

					m_nhmax = fhmax ;
					m_nhmin = fhmin ;
					CAutoContrastManager::Instance().m_nmax =m_nhmax;
					CAutoContrastManager::Instance().m_nmin =m_nhmin;
					CAutoContrastManager::Instance().nCounth = nCounts;
					CAutoContrastManager::Instance().fMatrix = m_pdst;
					CAutoContrastManager::Instance().bState = 1;
					do
					{
						bGra = CAutoContrastManager::Instance().bGrade;

					}while(!bGra);
					uchar *Gmatrix = false;
					Gmatrix = CAutoContrastManager::Instance().colorTable;
					/*in = (uchar *)m_pdst;*/
					int val;
					float imy[3000]={0.f};
					for (int i=0;i<nCounts;i++)
					{
						val = m_pdst[i];
						m_pPixZ[i]=Gmatrix[val];

					}
					delete []fimgInfo;
					delete []m_pdst;
				}
					
	        }
		 }
		if(CAutoContrastManager::Instance().fAutoBC==FALSE)
		{
			float imo[3000] = {0.f};
			for (int i=0;i<nCounts;i++)
			{
				imo[i]=m_pPixZ[i];
				
			}
			CAutoContrastManager::Instance().fMatrix = imo;
			CAutoContrastManager::Instance().bState = 0;
			CAutoContrastManager::Instance().bStateBC = 0;	
			CAutoContrastManager::Instance().nCounth = nCounts;
		   CAutoContrastManager::Instance().bGrade = 0;

			nTime = 0;
			nTimeBC =0;
			fhmax = 0.0;
			fhmin = 4095.0;
			nCounttotal_min =0;
			nCounttotal_max =0;
		}

		if( nCounts > 0 )
		{
			// 更新示波器
			m_pOscillographControler->UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx,m_bDebugZ,m_nDataStoreMode);

			if (CTestParameterManager::Instance().m_bEnableAlgoAdjust)
			{
				CAlgorithmManager::Instance().Adjust<WORD>(m_pPixZ, nCounts, CTestParameterManager::Instance().m_fAlgoAdjust);
			}

			// 刷新图像数据
			if( m_bDebugZ )
			{
				for(int i=0; i<nCounts; i++ )
				{
					// set z value to point(x,y)
					((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)(m_pPixZ[i]/16);
				}
			}
			else
			{
				if( m_bLive )
				{
					WORD wSrc;
					for(int i=0; i<nCounts; i++ )
					{
						wSrc = ((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ];
						// set z value to point(x,y)
						((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)((wSrc + m_pPixZ[i])/2);
					}
				}
				else
				{
					for( int i=0; i<nCounts; i++ )
					{
						// set z value to point(x,y)
						((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
					}
				}
			}

			// 记录数据
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( int i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		// 刷新图像区域
		if( nCounts > 0 )
		{
			nIndexY1 = m_pPixY[0];
			nIndexY2 = m_pPixY[nCounts -1];
			m_nCurrentY1	= nIndexY1;
			m_nCurrentY2	= nIndexY2;
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
		}
		else
		{
			m_UpdateRectsImage.Add( rcFrame );
		}

		// 更新索引
		m += 10239;
	}

	// 更新索引
	dwOffset = m_dwExpectantDMABlockSize +512;

	g_index++;
	if( m_bNeedMon && m_hFileLog != NULL )
	{
		unsigned long cbRet;
		char Buf[32];
		sprintf(Buf, "\\END set z value/\r\n\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
		CloseHandle(m_hFileLog);
		m_hFileLog = NULL;
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	return 0;
}

// 12.06.07
DWORD CMCICard::ADxyzLine( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( lpBuff == NULL )
		return 0;

	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;

	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;

	m_mutexImageDib.Lock();

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

	if( m_bNeedMon )
	{
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}

	DWORD dwOffset = 0;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	unsigned short* pnFrameData;

	BOOL bLastIsX, bLastIsY, bIsX, bIsY, bIsZ;
	unsigned short dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
	DWORD dwDestWidthAbs	= m_rcDestFrame.Width();
	DWORD dwDestHeightAbs	= m_rcDestFrame.top;	// m_rcDestFrame.Height();
	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
		bLastIsX= FALSE;		// 上一个数据是Y
	else
		bLastIsX= (m_dwX != 0xFFFF);// 上一个数据不是Y

	lSize = ((unsigned short*)lpBuff)[dwOffset];
	if( lSize > 31000 )
	{
		// 15.10.08 与MainLoop中有同步问题？？？
		// 冻结后，MainLoop中Read还是不等于Write，就会进入do循环
		// 而此时各数据存储区中无有效数据
		//
		// Scan中设定了dwDataSize不大于30000
		// 在此加一个总个数判断，可以避免无谓运算
		return 0;
	}

	// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
	pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
	const int nLoop = lSize +1;

	bIsX		= FALSE;		// 当前数据是X
	bIsY		= FALSE;		// 当前数据是Y
	bIsZ		= FALSE;		// 当前数据是Z
	int nCounts		= 0;			// 有效的数据个数

	for(int k=1; k<nLoop; k++ )
	{
		dwData = pnFrameData[k];
		// 得到所需的掩码位
		dwBitsOn = dwData & 0xF000;

		if( bLastIsX )						// 上一个数据是X
		{
			bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
			if( bIsY )						// 进一步确定是否当前数据是Y值
			{
				m_dwY = dwData & 0x0FFF;
			}
		}

		if( bLastIsY )
		{
			if( dwBitsOn == dwMaskBOL )
			{
				m_bPeakX	= FALSE;
				bIsZ		= TRUE;
			}
			else
			{
				bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// 进一步确定是否当前数据是Z值
			}
		}

		if( bIsZ )
		{
			// 硬件坐标是从1开始，软件要从0开始，所以需要调整
			if( m_dwX > m_nXOffset && m_dwY > m_nYOffset )
			{
				// 填充数组
				if( m_dwX <= dwDestWidthAbs +m_nXOffset && m_dwY <= dwDestHeightAbs +m_nYOffset )
				{
					if( !m_bPeakX )
					{
						m_pPixX[nCounts] = m_dwX -m_nXOffset -1;
						if( m_pPixX[nCounts] > m_szImage.cx -1 )
						{
							m_pPixX[nCounts] = m_szImage.cx -1;
						}

						m_pPixY[nCounts] = m_dwY -m_nYOffset -1;
						if( m_pPixY[nCounts] > m_szImage.cy -1 )
						{
							m_pPixY[nCounts] = m_szImage.cy -1;
						}

						CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
                        m_pPixZsrc[ m_pPixY[nCounts] * m_sizeReso.cx + m_pPixX[nCounts] ] = dwData & 0x0FFF;
						if( m_bDebugZ )
						{
							dwData = dwData & 0x0FFF;
						}
						else
						{
							if( dwData > m_nMaxPix)
							{
								dwData = m_nMaxPix;
							}

							dwData = (dwData/* -0x600*/) /m_nDiv;	// 取值范围对应255---0灰度范围显示
						}

						m_pPixZ[nCounts] = dwData;
						nCounts++;

						if( m_dwX == dwDestWidthAbs +m_nXOffset && m_dwY < dwDestHeightAbs +m_nYOffset )
						{
							m_bPeakX = TRUE;
						}
						else if( m_dwX == dwDestWidthAbs +m_nXOffset && m_dwY == dwDestHeightAbs +m_nYOffset )
						{
							m_bPeakY = TRUE;
						}

						m_dwX = 0xFFFF;
						m_dwY = 0xFFFF;
					}
					else
					{
						m_dwX = 0xFFFF;
						m_dwY = 0xFFFF;
					}
				}
			}
			else
			{
				// 当在X/Y的非有效数据段(valley)，要重新赋值
				m_dwX = 0xFFFF;
				m_dwY = 0xFFFF;
			}
		}

		bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
		if( bIsX )							// 进一步确定是否当前数据是X值
		{
			m_dwX = dwData & 0x0FFF;
		}

		// 刷新标志位
		bLastIsX	= bIsX;
		bLastIsY	= bIsY;
		bIsX		= FALSE;
		bIsY		= FALSE;
		bIsZ		= FALSE;
	}

	if( m_bStopScan )
		return 0;

	if( nCounts > 0 )
	{
		// 更新示波器
		m_pOscillographControler->UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx,m_bDebugZ,m_nDataStoreMode);

		// 记录数据
		if( m_bNeedMon && m_hFileLog != NULL )
		{
			for(int i=0; i<nCounts; i++ )
			{
				sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
				WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			}
		}
	}

	// 更新索引
	dwOffset = m_dwExpectantDMABlockSize +512;

	g_index++;
	if( m_bNeedMon && m_hFileLog != NULL )
	{
		CloseHandle(m_hFileLog);
		m_hFileLog = NULL;
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	return 0;
}

// 13.01.05
DWORD CMCICard::ADxyzSync( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( m_bSyncFinished )	// 如果一帧已经完成，就不分析了
		return 0;

	if( lpBuff == NULL )
		return 0;

	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;

	m_mutexImageDib.Lock();

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
	}

	DWORD dwOffset = 0;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	int		i, j, k, nLoop, nCounts, nIndexY1, nIndexY2;
	unsigned short* pnFrameData;

	BOOL bLastIsX, bLastIsY, bIsX, bIsY, bIsZ;
	unsigned short dwData	= 0;
	DWORD dwMaskX	= 0x8000;	// X 的掩码
	DWORD dwMaskY	= 0x4000;	// Y 的掩码
	DWORD dwMaskZ	= 0x0;		// Z 的掩码
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
	DWORD dwDestWidthAbs	= m_rcDestFrame.Width();
	DWORD dwDestHeightAbs	= m_rcDestFrame.Height();
	DWORD dwDestWidthRel	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeightRel	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
		bLastIsX= FALSE;		// 上一个数据是Y
	else
		bLastIsX= (m_dwX != 0xFFFF);// 上一个数据不是Y

	for( j=0; j<LOOP_COUNTS; j++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];
		if( lSize > 31000 )
		{
			// 15.10.08 与MainLoop中有同步问题？？？
			// 冻结后，MainLoop中Read还是不等于Write，就会进入do循环
			// 而此时各数据存储区中无有效数据
			//
			// Scan中设定了dwDataSize不大于30000
			// 在此加一个总个数判断，可以避免无谓运算
			continue;
		}

		// 分析数据
		// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
		pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
		nLoop = lSize +1;

		bIsX		= FALSE;		// 当前数据是X
		bIsY		= FALSE;		// 当前数据是Y
		bIsZ		= FALSE;		// 当前数据是Z
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
					bIsZ		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 根据BOF恢复标志位PeakY
					bIsZ		= TRUE;
				}
				else
					bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// 进一步确定是否当前数据是Z值
			}
			if( bIsZ )
			{
				// 硬件坐标是从1开始，软件要从0开始，所以需要调整
				if( m_dwX > m_nXOffset && m_dwY > m_nYOffset )
				{
					// 填充数组
					if( (m_dwX <= dwDestWidthAbs +m_nXOffset) && (m_dwY <= dwDestHeightAbs +m_nYOffset) )
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							m_pPixX[nCounts] = m_dwX -m_nXOffset -1 +m_rcDestFrame.left;	// 常规面扫同样适用于区域扫描
							if( m_pPixX[nCounts] > dwDestWidthRel +m_nXOffset )		//m_szImage.cx -1 )
							{
								m_pPixX[nCounts] = dwDestWidthRel +m_nXOffset;		//m_szImage.cx -1;
							}
							m_pPixY[nCounts] = m_dwY -m_nYOffset -1 +m_rcDestFrame.top;		// 常规面扫同样适用于区域扫描
							if( m_pPixY[nCounts] > dwDestHeightRel +m_nYOffset )	//m_szImage.cy -1 )
							{
								m_pPixY[nCounts] = dwDestHeightRel +m_nYOffset;		//m_szImage.cy -1;
							}

							CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
							m_pPixZsrc[ m_pPixY[nCounts] * m_sizeReso.cx + m_pPixX[nCounts] ] = dwData & 0x0FFF;
							if( m_bDebugZ )
								dwData = dwData & 0x0FFF;
							else
							{
								if( dwData > m_nMaxPix)
									dwData = m_nMaxPix;

								dwData = (dwData/* -0x600*/) /m_nDiv;	// 取值范围对应255---0灰度范围显示
							}
							m_pPixZ[nCounts] = dwData;
							m_pPixZOrigin[nCounts] = dwData;
							nCounts++;

							if( (m_dwX == dwDestWidthAbs +m_nXOffset) && (m_dwY < dwDestHeightAbs +m_nYOffset) )
								m_bPeakX = TRUE;
							else if( (m_dwX == dwDestWidthAbs +m_nXOffset) && (m_dwY == dwDestHeightAbs +m_nYOffset) )
							{
								m_bPeakY = TRUE;
								m_bSyncFinished = TRUE;
								break;	//return nCounts;
							}

							m_dwX = 0xFFFF;
							m_dwY = 0xFFFF;
						}
						else
						{
							// 当在X/Y的非有效数据段(peak & fall)，要重新赋值
							m_dwX = 0xFFFF;
							m_dwY = 0xFFFF;
						}
					}
				}
				else
				{
					// 当在X/Y的非有效数据段(valley)，要重新赋值
					m_dwX = 0xFFFF;
					m_dwY = 0xFFFF;
				}
			}

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// 进一步确定是否当前数据是X值
				m_dwX = dwData & 0x0FFF;

			// 刷新标志位
			bLastIsX	= bIsX;
			bLastIsY	= bIsY;
			bIsX		= FALSE;
			bIsY		= FALSE;
			bIsZ		= FALSE;
		}

		if( m_bStopScan )
			break;
		if(CAutoContrastManager::Instance().fAutoBC)
		{  
			if(FALSE ==CAutoContrastManager::Instance().bOptionState)
			{
				
					float imo[3000]={0.f},m_pdst[3000]={0.f};
		
					for (int i=0;i<nCounts;i++)
					{
						imo[i]=m_pPixZOrigin[i];
				
					}

					int fa,fb,ga,gb;
					if( (nCounttotal_max - nCounttotal_min) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa = 200,fb = 254;
					}
					else if( (nCounttotal_min - nCounttotal_max) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa =1,fb = 127;
					}
					else
					{
						ga = 1,gb = 254;
						fa = 40,fb = 240;
					}
						
					float k1 = ga/fa ;
					float k2 = (gb-ga)/(fb-fa);
					float k3 =  (255-gb)/(255-fb);
					int PosBri;   
					double B = CAutoContrastManager::Instance().nPosBright/100.;
					double c = CAutoContrastManager::Instance().nPosContrast/100. ;
					float G = CAutoContrastManager::Instance().fPosGamma/100. ;
					double kc = tan( (45 + 44 * c) / 180 * M_PI );
					//double kc=1;

					//hmax = k1*(hmax-hmin);

					for(int i =0;i<nCounts;i++)
					{
						
						//2022.11.18
						if(imo[i] < fa)
							m_pdst[i] = k1*imo[i];
						else if(imo[i] < fb)
							m_pdst[i] = k2*(imo[i]-fa)+ga;
						else
							m_pdst[i] = k3*(imo[i]-fb)+gb;

						m_pdst[i] = COLOR_RANGE((m_pdst[i]-127.5*(1-B))*kc+127.5*(1+B));
						m_pdst[i] = pow(((m_pdst[i]-fhmin)/(fhmax-fhmin)),(1.0/G))*(fhmax-fhmin);

						//输出
						m_pPixZ[i]=(WORD)m_pdst[i];
					}
/*					CAutoContrastManager::Instance().nCounth = nCounts;
					CAutoContrastManager::Instance().Matrix = m_pdst;
					CAutoContrastManager::Instance().StateABC = 1;	*/			
				
			}
			else
			{

					float imo[3000]={0.f},m_pdst[3000]={0.f};
		            for (int i=0;i<nCounts;i++)
					{
						imo[i]=m_pPixZOrigin[i];
				
					}

					int fa,fb,ga,gb;
					if( (nCounttotal_max - nCounttotal_min) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa =200,fb = 254;
					}
					else if( (nCounttotal_min - nCounttotal_max) / 786432. > 0.7)
					{
						ga = 1,gb = 254;
						fa =1,fb = 127;
					}
					else
					{
						ga = 1,gb = 254;
						fa = 40,fb = 240;
					}

					float k1 = ga/fa ;
					float k2 = (gb-ga)/(fb-fa);
					float k3 =  (255-gb)/(255-fb);

	                for(int i =0;i<nCounts;i++)
					{
						if(imo[i] < fa)
							m_pdst[i] = k1*imo[i];
						else if(imo[i] < fb)
							m_pdst[i] = k2*(imo[i]-fa)+ga;
						else
							m_pdst[i] = k3*(imo[i]-fb)+gb;

						//m_pPixZ[i]=(WORD)m_pdst[i];
					}
					
					m_nhmax = fhmax ;
					m_nhmin = fhmin ;
					CAutoContrastManager::Instance().m_nmax =m_nhmax;
					CAutoContrastManager::Instance().m_nmin =m_nhmin;

					CAutoContrastManager::Instance().nCounth = nCounts;
					CAutoContrastManager::Instance().fMatrix = m_pdst;
					CAutoContrastManager::Instance().bState = 1;

					do
					{
						bGra = CAutoContrastManager::Instance().bGrade;

					}while(!bGra);

					uchar *Gmatrix = false;
					Gmatrix = CAutoContrastManager::Instance().colorTable;
					
					int val;
					float imy[3000]={0.f};
					for (int i=0;i<nCounts;i++)
					{
						val = m_pdst[i];
						m_pPixZ[i]=Gmatrix[val];
				
					}

     
	        }
		 }
		//if(CAutoContrastManager::Instance().AutoABC==FALSE)
		 else
		{
			/*float imo[3000] = {0.f};
			for (int i=0;i<nCounts;i++)
			{
				imo[i]=m_pPixZ[i];
				
			}*/
			/*CAutoContrastManager::Instance().Matrix = imo;*/
			CAutoContrastManager::Instance().bState = 0;
			CAutoContrastManager::Instance().bStateBC = 0;	
			CAutoContrastManager::Instance().nCounth = nCounts;
		    CAutoContrastManager::Instance().bGrade = 0;
	
			

		}

		if( nCounts > 0 )
		{
			// 更新示波器
			m_pOscillographControler->UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx,m_bDebugZ,m_nDataStoreMode);

			// 刷新图像数据
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

			// 记录数据
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		// 更新索引
		dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);

		// 刷新图像区域
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
		}
		else
			m_UpdateRectsImage.Add( rcFrame );

		CRect rcIndication = CRect(0,0,CMCICardCommonSetting::Instance().m_sizeReso.cx *8 /1024,nIndexY2);
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

	m_mutexImageDib.Unlock();

	return i;
}
