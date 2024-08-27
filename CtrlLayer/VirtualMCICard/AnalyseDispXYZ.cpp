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
	DWORD dwMaskX	= 0x8000;	// X ������
	DWORD dwMaskY	= 0x4000;	// Y ������
	DWORD dwMaskZ	= 0x0;		// Z ������
	DWORD dwMaskBOL	= 0x1000;	// BOL ������
	DWORD dwMaskBOF	= 0x3000;	// BOF ������
	DWORD dwBitsOn;
	DWORD dwDestWidthAbs	= m_rcDestFrame.Width();
	DWORD dwDestHeightAbs	= m_rcDestFrame.Height();
	DWORD dwDestWidthRel	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeightRel	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
	{
		bLastIsX= FALSE;		// ��һ��������Y
	}
	else
	{
		bLastIsX= (m_dwX != 0xFFFF);// ��һ�����ݲ���Y
	}

	unsigned long lSize = ((unsigned short*)lpBuff)[dwOffset];
	if( lSize > 31000 )
	{
		// 15.10.08 ��MainLoop����ͬ�����⣿����
		// �����MainLoop��Read���ǲ�����Write���ͻ����doѭ��
		// ����ʱ�����ݴ洢��������Ч����
		//
		// Scan���趨��dwDataSize������30000
		// �ڴ˼�һ���ܸ����жϣ����Ա�����ν����
		return 0;
	}

	pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);

	unsigned short FrameData[40960] = {0}; 
	memcpy(FrameData,pnFrameData,40960);

	// ��Ϊÿ�����ݵĵ�һ�������ܸ���������nLoopҪ+1����ѭ��ʱ��1��ʼ
	int nLoop = lSize +1;

	bIsX		= FALSE;		// ��ǰ������X
	bIsY		= FALSE;		// ��ǰ������Y
	bIsZ		= FALSE;		// ��ǰ������Z

	for(int m=1; m<nLoop;m++ )
	{
		nCounts		= 0;			// ��Ч�����ݸ���

		nLoop2 = m+10240 < nLoop ? m+10240:nLoop;

		for(int k=m; k<nLoop2; k++ )
		{
			dwData = pnFrameData[k];
			// �õ����������λ
			dwBitsOn = dwData & 0xF000;

			if( bLastIsX )						// ��һ��������X
			{
				bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
				if( bIsY )						// ��һ��ȷ���Ƿ�ǰ������Yֵ
				{
					m_dwY = dwData & 0x0FFF;
				}
			}

			if( bLastIsY )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 ����BOL�ָ���־λPeakX
					bIsZ		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 ����BOF�ָ���־λPeakY
					bIsZ		= TRUE;
				}
				else
				{
					bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// ��һ��ȷ���Ƿ�ǰ������Zֵ
				}
			}

			if( bIsZ )
			{
				// Ӳ�������Ǵ�1��ʼ�����Ҫ��0��ʼ��������Ҫ����
				if( m_dwX > m_nXOffset && m_dwY > m_nYOffset )
				{
					// �������
					if( m_dwX <= dwDestWidthAbs +m_nXOffset && m_dwY <= dwDestHeightAbs +m_nYOffset )
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							m_pPixX[nCounts] = m_dwX -m_nXOffset -1 +m_rcDestFrame.left;	// ������ɨͬ������������ɨ��
							if( m_pPixX[nCounts] > dwDestWidthRel +m_nXOffset )		//m_szImage.cx -1 )
							{
								m_pPixX[nCounts] = dwDestWidthRel +m_nXOffset;		//m_szImage.cx -1;
							}

							m_pPixY[nCounts] = m_dwY -m_nYOffset -1 +m_rcDestFrame.top;		// ������ɨͬ������������ɨ��
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

								dwData = (dwData/* -0x600*/) /m_nDiv;	// ȡֵ��Χ��Ӧ255---0�Ҷȷ�Χ��ʾ
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
					// ����X/Y�ķ���Ч���ݶ�(valley)��Ҫ���¸�ֵ
					m_dwX = 0xFFFF;
					m_dwY = 0xFFFF;
				}
			}

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// ��һ��ȷ���Ƿ�ǰ������Xֵ
			{
				m_dwX = dwData & 0x0FFF;
			}

			// ˢ�±�־λ
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

						//���
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
			// ����ʾ����
			m_pOscillographControler->UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx,m_bDebugZ,m_nDataStoreMode);

			if (CTestParameterManager::Instance().m_bEnableAlgoAdjust)
			{
				CAlgorithmManager::Instance().Adjust<WORD>(m_pPixZ, nCounts, CTestParameterManager::Instance().m_fAlgoAdjust);
			}

			// ˢ��ͼ������
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

			// ��¼����
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( int i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		// ˢ��ͼ������
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

		// ��������
		m += 10239;
	}

	// ��������
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
	DWORD dwMaskX	= 0x8000;	// X ������
	DWORD dwMaskY	= 0x4000;	// Y ������
	DWORD dwMaskZ	= 0x0;		// Z ������
	DWORD dwMaskBOL	= 0x1000;	// BOL ������
	DWORD dwMaskBOF	= 0x3000;	// BOF ������
	DWORD dwBitsOn;
	DWORD dwDestWidthAbs	= m_rcDestFrame.Width();
	DWORD dwDestHeightAbs	= m_rcDestFrame.top;	// m_rcDestFrame.Height();
	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
		bLastIsX= FALSE;		// ��һ��������Y
	else
		bLastIsX= (m_dwX != 0xFFFF);// ��һ�����ݲ���Y

	lSize = ((unsigned short*)lpBuff)[dwOffset];
	if( lSize > 31000 )
	{
		// 15.10.08 ��MainLoop����ͬ�����⣿����
		// �����MainLoop��Read���ǲ�����Write���ͻ����doѭ��
		// ����ʱ�����ݴ洢��������Ч����
		//
		// Scan���趨��dwDataSize������30000
		// �ڴ˼�һ���ܸ����жϣ����Ա�����ν����
		return 0;
	}

	// ��Ϊÿ�����ݵĵ�һ�������ܸ���������nLoopҪ+1����ѭ��ʱ��1��ʼ
	pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
	const int nLoop = lSize +1;

	bIsX		= FALSE;		// ��ǰ������X
	bIsY		= FALSE;		// ��ǰ������Y
	bIsZ		= FALSE;		// ��ǰ������Z
	int nCounts		= 0;			// ��Ч�����ݸ���

	for(int k=1; k<nLoop; k++ )
	{
		dwData = pnFrameData[k];
		// �õ����������λ
		dwBitsOn = dwData & 0xF000;

		if( bLastIsX )						// ��һ��������X
		{
			bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
			if( bIsY )						// ��һ��ȷ���Ƿ�ǰ������Yֵ
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
				bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// ��һ��ȷ���Ƿ�ǰ������Zֵ
			}
		}

		if( bIsZ )
		{
			// Ӳ�������Ǵ�1��ʼ�����Ҫ��0��ʼ��������Ҫ����
			if( m_dwX > m_nXOffset && m_dwY > m_nYOffset )
			{
				// �������
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

							dwData = (dwData/* -0x600*/) /m_nDiv;	// ȡֵ��Χ��Ӧ255---0�Ҷȷ�Χ��ʾ
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
				// ����X/Y�ķ���Ч���ݶ�(valley)��Ҫ���¸�ֵ
				m_dwX = 0xFFFF;
				m_dwY = 0xFFFF;
			}
		}

		bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
		if( bIsX )							// ��һ��ȷ���Ƿ�ǰ������Xֵ
		{
			m_dwX = dwData & 0x0FFF;
		}

		// ˢ�±�־λ
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
		// ����ʾ����
		m_pOscillographControler->UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx,m_bDebugZ,m_nDataStoreMode);

		// ��¼����
		if( m_bNeedMon && m_hFileLog != NULL )
		{
			for(int i=0; i<nCounts; i++ )
			{
				sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
				WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			}
		}
	}

	// ��������
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
	if( m_bSyncFinished )	// ���һ֡�Ѿ���ɣ��Ͳ�������
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
	DWORD dwMaskX	= 0x8000;	// X ������
	DWORD dwMaskY	= 0x4000;	// Y ������
	DWORD dwMaskZ	= 0x0;		// Z ������
	DWORD dwMaskBOL	= 0x1000;	// BOL ������
	DWORD dwMaskBOF	= 0x3000;	// BOF ������
	DWORD dwBitsOn;
	DWORD dwDestWidthAbs	= m_rcDestFrame.Width();
	DWORD dwDestHeightAbs	= m_rcDestFrame.Height();
	DWORD dwDestWidthRel	= m_rcDestFrame.left + m_rcDestFrame.Width();
	DWORD dwDestHeightRel	= m_rcDestFrame.top + m_rcDestFrame.Height();
	bLastIsY	= (m_dwY != 0xFFFF);
	if( bLastIsY )
		bLastIsX= FALSE;		// ��һ��������Y
	else
		bLastIsX= (m_dwX != 0xFFFF);// ��һ�����ݲ���Y

	for( j=0; j<LOOP_COUNTS; j++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];
		if( lSize > 31000 )
		{
			// 15.10.08 ��MainLoop����ͬ�����⣿����
			// �����MainLoop��Read���ǲ�����Write���ͻ����doѭ��
			// ����ʱ�����ݴ洢��������Ч����
			//
			// Scan���趨��dwDataSize������30000
			// �ڴ˼�һ���ܸ����жϣ����Ա�����ν����
			continue;
		}

		// ��������
		// ��Ϊÿ�����ݵĵ�һ�������ܸ���������nLoopҪ+1����ѭ��ʱ��1��ʼ
		pnFrameData = &(((unsigned short*)lpBuff)[dwOffset]);
		nLoop = lSize +1;

		bIsX		= FALSE;		// ��ǰ������X
		bIsY		= FALSE;		// ��ǰ������Y
		bIsZ		= FALSE;		// ��ǰ������Z
		nCounts		= 0;			// ��Ч�����ݸ���

		for( k=1; k<nLoop; k++ )
		{
			dwData = pnFrameData[k];
			// �õ����������λ
			dwBitsOn = dwData & 0xF000;

			if( bLastIsX )						// ��һ��������X
			{
				bIsY = (dwBitsOn == dwMaskY);	//(dwBitsOn == dwMaskY); ? TRUE : FALSE;
				if( bIsY )						// ��һ��ȷ���Ƿ�ǰ������Yֵ
					m_dwY = dwData & 0x0FFF;
			}
			if( bLastIsY )
			{
				if( dwBitsOn == dwMaskBOL )
				{
					m_bPeakX	= FALSE;		// 13.02.28 ����BOL�ָ���־λPeakX
					bIsZ		= TRUE;
				}
				else if( dwBitsOn == dwMaskBOF )
				{
					m_bPeakY	= FALSE;		// 13.02.28 ����BOF�ָ���־λPeakY
					bIsZ		= TRUE;
				}
				else
					bIsZ = (dwBitsOn == dwMaskZ);	//(dwBitsOn == dwMaskZ) ? TRUE : FALSE;// ��һ��ȷ���Ƿ�ǰ������Zֵ
			}
			if( bIsZ )
			{
				// Ӳ�������Ǵ�1��ʼ�����Ҫ��0��ʼ��������Ҫ����
				if( m_dwX > m_nXOffset && m_dwY > m_nYOffset )
				{
					// �������
					if( (m_dwX <= dwDestWidthAbs +m_nXOffset) && (m_dwY <= dwDestHeightAbs +m_nYOffset) )
					{
						if( !m_bPeakX && !m_bPeakY )
						{
							m_pPixX[nCounts] = m_dwX -m_nXOffset -1 +m_rcDestFrame.left;	// ������ɨͬ������������ɨ��
							if( m_pPixX[nCounts] > dwDestWidthRel +m_nXOffset )		//m_szImage.cx -1 )
							{
								m_pPixX[nCounts] = dwDestWidthRel +m_nXOffset;		//m_szImage.cx -1;
							}
							m_pPixY[nCounts] = m_dwY -m_nYOffset -1 +m_rcDestFrame.top;		// ������ɨͬ������������ɨ��
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

								dwData = (dwData/* -0x600*/) /m_nDiv;	// ȡֵ��Χ��Ӧ255---0�Ҷȷ�Χ��ʾ
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
							// ����X/Y�ķ���Ч���ݶ�(peak & fall)��Ҫ���¸�ֵ
							m_dwX = 0xFFFF;
							m_dwY = 0xFFFF;
						}
					}
				}
				else
				{
					// ����X/Y�ķ���Ч���ݶ�(valley)��Ҫ���¸�ֵ
					m_dwX = 0xFFFF;
					m_dwY = 0xFFFF;
				}
			}

			bIsX = (dwBitsOn == dwMaskX);		//(dwBitsOn == dwMaskX) ? TRUE : FALSE;
			if( bIsX )							// ��һ��ȷ���Ƿ�ǰ������Xֵ
				m_dwX = dwData & 0x0FFF;

			// ˢ�±�־λ
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

						//���
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
			// ����ʾ����
			m_pOscillographControler->UpdateOscillographNew( nCounts, m_pPixX, m_pPixY, m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx,m_bDebugZ,m_nDataStoreMode);

			// ˢ��ͼ������
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

			// ��¼����
			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<nCounts; i++ )
				{
					sprintf(Buf, "%04X\t%04X\t%04X\t%d\t%d\t%d\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i], m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		// ��������
		dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);

		// ˢ��ͼ������
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
