// DiagLogging.cpp : implementation file
//
// Last Modify : 2013.03.29

#include "stdafx.h"
#include "AutoTest.h"

#include "Calc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoTestApp
/*
void	CAutoTestApp::ChangedADCselectOB( int nNum )
void	CAutoTestApp::ChangedADCsChSelect( int nChan )
CString CAutoTestApp::ReadSelectedADC_Procedure( DWORD dwMilliseconds )
long	CAutoTestApp::ReadSelectedADC( BOOL bAnalyze )
CString CAutoTestApp::ReadSelectedADC_Logging( DWORD dwMilliseconds, double &dValue )
CString CAutoTestApp::ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue )
CString CAutoTestApp::ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, BOOL bDelay )
CString CAutoTestApp::ADCvconv_Logging( double &dValue, BOOL bLog )
CString CAutoTestApp::ReadComprs_Logging()

void	CAutoTestApp::OnlySendReadSelectedADCcmd( logParam lP)
CString	CAutoTestApp::ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, BOOL bLog )
CString CAutoTestApp::ReadComprs_Logging( int nData )
*/
void CAutoTestApp::ChangedADCselectOB( int nNum )
{
	// SCN(1x16),LNS(1x8),VAC(2x16),SPR(2x16) (all LTC2448 type ADC's except for LNS board)
	theApp.m_nADCnum = nNum;

	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	switch( theApp.m_nADCnum )
	{
	case	0:									// SCN board LTC2448 (one per board)
		theApp.m_nADCmid		= USB_MID_SCN;			// SCN board
		theApp.m_nADCcid		= USB_CID_SCN_ADC_Read;	// Circuit ID 0xC8
		theApp.m_bADCtype		= TRUE;					// Arbitrary assignment for this software to properly format ADC write data for channel select, etc.
		theApp.m_dADCvref		= 4;
		theApp.m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	case	1:									// LNS board LTC1859 (one per board)
		theApp.m_nADCmid		= USB_MID_LNS;			// LNS board
		theApp.m_nADCcid		= USB_CID_LNS_ADC_Read;	// Circuit ID 0xC8
		theApp.m_bADCtype		= FALSE;				// Type = LTC1859
		theApp.m_dADCvref		= 10;//2.5;
		theApp.m_dADCCOMoffset	= 0;
		str = "% of Vref*2";
		break;
	case	2:									// VAC board LTC2448 #0
		theApp.m_nADCmid		= USB_MID_VAC;			// VAC board
		theApp.m_nADCcid		= USB_CID_VAC_ADC0_Read;// Circuit ID 0xC6
		theApp.m_bADCtype		= TRUE;					// Type = LTC2448
		theApp.m_dADCvref		= 4;
		theApp.m_dADCCOMoffset	= 1.607;
		str = "% of Vref/2";
		break;
	case	3:									// VAC board LTC2448 #1
		theApp.m_nADCmid		= USB_MID_VAC;			// VAC board
		theApp.m_nADCcid		= USB_CID_VAC_ADC1_Read;// Circuit ID 0xCE
		theApp.m_bADCtype		= TRUE;					// Type = LTC2448
		theApp.m_dADCvref		= 4;
		theApp.m_dADCCOMoffset	= 1.607;
		str = "% of Vref/2";
		break;
	case	4:									// SPR board LTC2448 #0
		theApp.m_nADCmid		= USB_MID_SPR;			// SPR board
		theApp.m_nADCcid		= USB_CID_SPR_ADC0_Read;// Circuit ID 0xC6
		theApp.m_bADCtype		= TRUE;					// Type = LTC2448
		theApp.m_dADCvref		= 4.5;
		theApp.m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	case	5:									// SPR board LTC2448 #1
		theApp.m_nADCmid		= USB_MID_SPR;			// SPR board
		theApp.m_nADCcid		= USB_CID_SPR_ADC1_Read;// Circuit ID 0xCE
		theApp.m_bADCtype		= TRUE;					// Type = LTC2448
		theApp.m_dADCvref		= 4.5;
		theApp.m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	}
//	GetDlgItem( IDC_STATIC_PctVrefLabel )->SetWindowText( str );
//	str.Format( "%.1f V", theApp.m_dADCvref );
//	GetDlgItem( IDC_EDIT_ADCvrefTB )->SetWindowText( str );
}

void CAutoTestApp::ChangedADCsChSelect( int nChan )
{
	// SCN(1x16),LNS(1x8),VAC(2x16),SPR(2x16) (all LTC2448 type ADC's except for LNS board)
	m_nADCchn = nChan;
	
//	m_dADCdiffSE		= m_dADCchanModes[m_nADCnum*16*2 +m_nADCchn *2 +0];	// If value = 1 then S.E. / If value = 2 then diff mode
//	m_dADCcurrentMeas	= m_dADCchanModes[m_nADCnum*16*2 +m_nADCchn *2 +1];	// if non-zero value, then convert voltage to current measurement
	m_bADCdiff			= m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].bDiff;	// If FALSE then S.E. / If TRUE then diff mode
	m_dADCcurrentMeas	= m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dFactor;// if non-zero value, then convert voltage to current measurement
	
	if( theApp.m_dADCcurrentMeas > 0 )		// Don't calculate unless non-zero, otherwise generate divide-by-0 error:
	{
//		m_dADCvoltsPerAmp = m_dADCchanModes[m_nADCnum*16*2 +m_nADCchn*2 +1];
		m_dADCvoltsPerAmp = m_DL.adc.chn[m_nADCnum*16 +m_nADCchn].dFactor;
		m_dADCampsPerVolt = 1 / m_dADCvoltsPerAmp;
	}
}

CString CAutoTestApp::ReadSelectedADC_Procedure( DWORD dwMilliseconds, BOOL bLogging )
{
	CString strRes = "";
	double dValue = 0.0;
	if( bLogging )
		strRes = ReadSelectedADC_Logging2( dwMilliseconds, dValue );
	else
	{
		if( theApp.m_bADCtype )				// 2448
		{
			theApp.ReadSelectedADC( FALSE );	// 残留值
			theApp.ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
			theApp.Delay( dwMilliseconds );		// 等待转换完成
			theApp.ReadSelectedADC( TRUE );		// 真正的回读值
			theApp.Delay( dwMilliseconds );		// 等待转换完成
			// Two back-to-back 16 bit transfers to receive RB data
			strRes.Format( "%04X %04X", theApp.m_nBoardReadDataBuf[2*2], theApp.m_nBoardReadDataBuf[3*2] );
		}
		else								// 1859
		{
			theApp.ReadSelectedADC( FALSE );	// 残留值
			theApp.ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
			theApp.Delay( dwMilliseconds );		// 等待转换完成
			theApp.ReadSelectedADC( TRUE );		// 真正的回读值
			// Only 16 bits max of readback data
			strRes.Format( "%04X", theApp.m_nBoardReadDataBuf[2*2] );
		}
	}

	return strRes;
}

long CAutoTestApp::ReadSelectedADC( BOOL bAnalyze )
{
	int nRsize, nModID;
	BYTE bCktAddr, bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	// Uses globals: g_TTID, g_MID, g_CID, g_ADCtype, g_ADCnum, g_ADCchan
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( theApp.m_bADCdiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( theApp.m_bADCtype )	// LTC2448 type
	{
		nRsize	= 32;
		switch( theApp.m_nADCchn )
		{
		case	0:
			bChanNib	= 0;
//			dwWdata = (DWROD)( 0xB000 | dwOSR & 0xFFFF );	// Select ADC channel 0 (Odd = 0), OSR level 9, TWOX = 0
			break;
		case	1:
			bChanNib	= 8;
//			dwWdata = (DWROD)( 0xB800 | dwOSR & 0xFFFF );	// Select ADC channel 1 (Odd = 1)
			break;
		case	2:
			bChanNib	= 1;
//			dwWdata = (DWROD)( 0xB100 | dwOSR & 0xFFFF );	// Select ADC channel 2 (Odd = 0)
			break;
		case	3:
			bChanNib	= 9;
//			dwWdata = (DWROD)( 0xB900 | dwOSR & 0xFFFF );	// Select ADC channel 3 (Odd = 1)
			break;
		case	4:
			bChanNib	= 2;
//			dwWdata = (DWROD)( 0xB200 | dwOSR & 0xFFFF );	// Select ADC channel 4 (Odd = 0)
			break;
		case	5:
			bChanNib	= 10;
//			dwWdata = (DWROD)( 0xBA00 | dwOSR & 0xFFFF );	// Select ADC channel 5 (Odd = 1)
			break;
		case	6:
			bChanNib	= 3;
//			dwWdata = (DWROD)( 0xB300 | dwOSR & 0xFFFF );	// Select ADC channel 6 (Odd = 0)
			break;
		case	7:
			bChanNib	= 11;
//			dwWdata = (DWROD)( 0xBB00 | dwOSR & 0xFFFF );	// Select ADC channel 7 (Odd = 1)
			break;
		case	8:
			bChanNib	= 4;
//			dwWdata = (DWROD)( 0xB400 | dwOSR & 0xFFFF );	// Select ADC channel 8 (Odd = 0)
			break;
		case	9:
			bChanNib	= 12;
//			dwWdata = (DWROD)( 0xBC00 | dwOSR & 0xFFFF );	// Select ADC channel 9 (Odd = 1)
			break;
		case	10:
			bChanNib	= 5;
//			dwWdata = (DWROD)( 0xB500 | dwOSR & 0xFFFF );	// Select ADC channel 10 (Odd = 0)
			break;
		case	11:
			bChanNib	= 13;
//			dwWdata = (DWROD)( 0xBD00 | dwOSR & 0xFFFF );	// Select ADC channel 11 (Odd = 1)
			break;
		case	12:
			bChanNib	= 6;
//			dwWdata = (DWROD)( 0xB600 | dwOSR & 0xFFFF );	// Select ADC channel 12 (Odd = 0)
			break;
		case	13:
			bChanNib	= 14;
//			dwWdata = (DWROD)( 0xBE00 | dwOSR & 0xFFFF );	// Select ADC channel 13 (Odd = 1)
			break;
		case	14:
			bChanNib	= 7;
//			dwWdata = (DWROD)( 0xB700 | dwOSR & 0xFFFF );	// Select ADC channel 14 (Odd = 0)
			break;
		case	15:
			bChanNib	= 15;
//			dwWdata = (DWROD)( 0xBF00 | dwOSR & 0xFFFF );	// Select ADC channel 15 (Odd = 1)
			break;
		default:
			break;
		}
		if( theApp.m_bADCdiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else	// LTC1859 type, format write data for channel select, etc.
	{
		nRsize	= 16;
		switch( theApp.m_nADCchn )
		{
		case	0:				// Condenser Lens current monitor
			dwWdata	= 0x8000;	// ADC Channel 0, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	1:				// Objective Lens current monitor
			dwWdata	= 0xC000;	// ADC Channel 1, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	2:				// X alignment coil current monitor
			dwWdata	= 0x9000;	// ADC Channel 2, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	3:				// Y alignment coil current monitor
			dwWdata	= 0xD000;	// ADC Channel 3, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	4:				// Amplifier positive power supplies voltages monitor
			dwWdata	= 0xA000;	// ADC Channel 4, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	5:				// Amplifier positive power supplies voltages monitor
			dwWdata	= 0xE000;	// ADC Channel 5, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	6:				// Thermistor input 1
			dwWdata	= 0xB000;	// ADC Channel 6, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	7:				// Thermistor input 2
			dwWdata	= 0xF000;	// ADC Channel 7, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		}
	}	// LTC1859 type
	
	nModID		= theApp.m_nADCmid;
	bCktAddr	= (BYTE)theApp.m_nADCcid;
	
	long lRet = 0;
	CString strMsgCounts, strMsgData;
	if( theApp.m_bADCtype )
		lRet = theApp.BoardRead2432( nRsize, nModID, bCktAddr, dwWdata, strMsgCounts, strMsgData, bAnalyze );
	else
		lRet = theApp.BoardRead0816( nRsize, nModID, bCktAddr, (WORD)dwWdata, strMsgCounts, strMsgData, bAnalyze );
	
	return lRet;
}

CString CAutoTestApp::ReadSelectedADC_Logging( DWORD dwMilliseconds, double &dValue )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes;
	strRes = "";

	long lRet1 = 1;
	long lRet2 = 1;
	long lTimes = 0;
	while( (lRet1 >0 || lRet2 > 0) && (lTimes < 5) )
	{
		lRet1 = ReadSelectedADC( FALSE );
		theApp.Delay( dwMilliseconds );//Sleep( dwMilliseconds );
		lRet2 = ReadSelectedADC( TRUE );
		// 12.11.09 for comm test
		if( lRet1 > 0 || lRet2 > 0 )
		{
			if( lRet1 > 0 && lRet2 == 0 )
				strRes += "1F2S,";
			else if( lRet1 == 0 && lRet2 > 0 )
				strRes += "1S2F,";
			else
				strRes += "12F,";
			lTimes++;
		}
		// 12.11.09 for comm test
	}

	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn *2].dMin;
	double dRangeMax = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn *2].dMax;
	double dEquivCurrent = 0.0;

	if( theApp.m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)theApp.m_nBoardReadDataBuf[2*2]<<16) | (DWORD)theApp.m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			theApp.Delay( dwMilliseconds +40 );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
			dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
		else
			dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2

		dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
		if( bADCvalSignBit )
		{
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			// g_ADCvref varies per board design
			str.Format( "%.6f\t", dADCreadVrefFrac * theApp.m_dADCvref /2 );
			strRes += str;
			dValue = dADCreadVrefFrac * theApp.m_dADCvref /2 +theApp.m_dADCCOMoffset;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "-%.6f\t", dADCreadVrefFrac * theApp.m_dADCvref /2 );
			strRes += str;
			dValue = theApp.m_dADCCOMoffset - dADCreadVrefFrac * theApp.m_dADCvref /2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(theApp.m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			theApp.Delay( dwMilliseconds +40 );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * theApp.m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * theApp.m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * theApp.m_dADCvref / 2 * theApp.m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( theApp.m_dADCcurrentMeas > 0 )
	{
		str.Format( "%.6f\t", dEquivCurrent );
		strRes += str;
	}
	else
	{
		str.Format( "n/a\t" );
		strRes += str;
	}

//	theApp.Delay( dwMilliseconds +40);//Sleep( dwMilliseconds +40 );
	theApp.Delay( dwMilliseconds );//Sleep( dwMilliseconds );
	if( strRes.IsEmpty() )
		strRes.Format( "\t\t\t\t" );
	return strRes;
}

CString CAutoTestApp::ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue )
{
	CString str, strRes = "";
	int i;
	if( theApp.m_bADCtype )				// 2448
	{
		theApp.ReadSelectedADC( FALSE );	// 残留值
		theApp.ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
		theApp.Delay( dwMilliseconds );		// 等待建立及转换
		if( theApp.m_DL.adc.nAveCounts > 0 )
		{
			// 读多次，求平均
			double dTotal = 0.0;
			int nCounts = theApp.m_DL.adc.nAveCounts +1;
			for( i=0; i<nCounts; i++ )
			{
				theApp.ReadSelectedADC( TRUE );
				theApp.Delay( dwMilliseconds );
				str = theApp.ADCvconv_Logging( theApp.m_DL.log.dAve[i] );
				dTotal += theApp.m_DL.log.dAve[i];
//				strRes += str;
			}
			dValue = dTotal /nCounts;
			strRes = str;
		}
		else
		{
			theApp.ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
			theApp.Delay( dwMilliseconds );
			strRes = ADCvconv_Logging( dValue );
		}
	}
	else								// 1859
	{
		theApp.ReadSelectedADC( FALSE );	// 残留值
		theApp.ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
 		theApp.Delay( dwMilliseconds );		// 等待转换完成
		if( theApp.m_DL.adc.nAveCounts > 0 )
		{
			// 读多次，求平均
			double dTotal = 0.0;
			int nCounts = theApp.m_DL.adc.nAveCounts +1;
			for( i=0; i<nCounts; i++ )
			{
				theApp.ReadSelectedADC( TRUE );
				theApp.Delay( dwMilliseconds );
				str = theApp.ADCvconv_Logging( theApp.m_DL.log.dAve[i] );
				dTotal += theApp.m_DL.log.dAve[i];
//				strRes += str;
			}
			dValue = dTotal /nCounts;
			strRes = str;
		}
		else
		{
			theApp.ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
			theApp.Delay( dwMilliseconds );
			strRes = ADCvconv_Logging( dValue );
		}
	}
	return strRes;
}

CString CAutoTestApp::ADCvconv_Logging( double &dValue, BOOL bLog )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes = "";
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn *2].dMin;
	double dRangeMax = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn *2].dMax;
	double dEquivCurrent = 0.0;

	if( theApp.m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)theApp.m_nBoardReadDataBuf[2*2]<<16) | (DWORD)theApp.m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		if( bLog )
		{
			str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
			dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
		else
			dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2

		dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
		if( bADCvalSignBit )
		{
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
				// g_ADCvref varies per board design
				str.Format( "%.6f\t", dADCreadVrefFrac * theApp.m_dADCvref /2 );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * theApp.m_dADCvref /2 +theApp.m_dADCCOMoffset;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
		}
		else
		{
			if( bLog )
			{
				str.Format( "-%.4f\t", dADCreadVrefPct );
				strRes += str;
				str.Format( "-%.6f\t", dADCreadVrefFrac * theApp.m_dADCvref /2 );
				strRes += str;
			}
			dValue = theApp.m_dADCCOMoffset - dADCreadVrefFrac * theApp.m_dADCvref /2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(theApp.m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		if( bLog )
		{
			str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * theApp.m_dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * theApp.m_dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * theApp.m_dADCvref / 2 * theApp.m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( bLog )
	{
		if( theApp.m_dADCcurrentMeas > 0 )
		{
			str.Format( "%.6f\t", dEquivCurrent );
			strRes += str;
		}
		else
		{
			str.Format( "n/a\t" );
			strRes += str;
		}

		if( strRes.IsEmpty() )
			strRes.Format( "\t\t\t\t" );
	}
	return strRes;
}

CString CAutoTestApp::ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, BOOL bDelay )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes;
	strRes = "";

	long lRet1 = 1;
	long lTimes = 0;
	while( (lRet1 >0) && (lTimes < 5) )
	{
		lRet1 = ReadSelectedADC( TRUE );
		// 12.11.09 for comm test
		if( lRet1 > 0 )
		{
			strRes += "Fail,";
			lTimes++;
		}
		// 12.11.09 for comm test
	}
	strRes = ADCvconv_Logging( dValue );
	if( bDelay )
		theApp.Delay( dwMilliseconds );
	return strRes;

	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn *2].dMin;
	double dRangeMax = theApp.m_DL.adc.chn[theApp.m_nADCnum*16 +theApp.m_nADCchn *2].dMax;
	double dEquivCurrent = 0.0;

	if( theApp.m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)theApp.m_nBoardReadDataBuf[2*2]<<16) | (DWORD)theApp.m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			if( bDelay )
				theApp.Delay( dwMilliseconds );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
			dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
		else
			dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2

		dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
		if( bADCvalSignBit )
		{
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			// g_ADCvref varies per board design
			str.Format( "%.6f\t", dADCreadVrefFrac * theApp.m_dADCvref /2 );
			strRes += str;
			dValue = dADCreadVrefFrac * theApp.m_dADCvref /2 +theApp.m_dADCCOMoffset;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "-%.6f\t", dADCreadVrefFrac * theApp.m_dADCvref /2 );
			strRes += str;
			dValue = theApp.m_dADCCOMoffset - dADCreadVrefFrac * theApp.m_dADCvref /2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(theApp.m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			if( bDelay )
				theApp.Delay( dwMilliseconds );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * theApp.m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * theApp.m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		dEquivCurrent = dValue * theApp.m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * theApp.m_dADCvref / 2 * theApp.m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( theApp.m_dADCcurrentMeas > 0 )
	{
		str.Format( "%.6f\t", dEquivCurrent );
		strRes += str;
	}
	else
	{
		str.Format( "n/a\t" );
		strRes += str;
	}

	if( bDelay )
	{
		theApp.Delay( dwMilliseconds );//Sleep( dwMilliseconds );
	}
	if( strRes.IsEmpty() )
		strRes.Format( "\t\t\t\t" );
	return strRes;
}

void CAutoTestApp::OnlySendReadSelectedADCcmd( logParam lP)
{
	if( lP.nType == 2 )
	{
		// read gauge compr
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ReadThermoCCGComp,	// OP1 = ckt addr. of comparator read register on VAC board
											USB_TM_VAC_Read_16,				// OP2 = 0xA3 (16 bit read from VAC board)
											0x0,							// OP3 = 0 (No data to be written)
											0x0 );							// OP4 = 0 (No data to be written)
		return;
	}
	if( lP.nType == 3 )
	{
		// read Ext. switch
		return;
	}

	BYTE bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	// Uses globals: g_TTID, g_MID, g_CID, g_ADCtype, g_ADCnum, g_ADCchan
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( lP.bDiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( lP.bType )	// LTC2448 type
	{
		switch( lP.nChn )
		{
		case	0:
			bChanNib	= 0;
//			dwWdata = (DWROD)( 0xB000 | dwOSR & 0xFFFF );	// Select ADC channel 0 (Odd = 0), OSR level 9, TWOX = 0
			break;
		case	1:
			bChanNib	= 8;
//			dwWdata = (DWROD)( 0xB800 | dwOSR & 0xFFFF );	// Select ADC channel 1 (Odd = 1)
			break;
		case	2:
			bChanNib	= 1;
//			dwWdata = (DWROD)( 0xB100 | dwOSR & 0xFFFF );	// Select ADC channel 2 (Odd = 0)
			break;
		case	3:
			bChanNib	= 9;
//			dwWdata = (DWROD)( 0xB900 | dwOSR & 0xFFFF );	// Select ADC channel 3 (Odd = 1)
			break;
		case	4:
			bChanNib	= 2;
//			dwWdata = (DWROD)( 0xB200 | dwOSR & 0xFFFF );	// Select ADC channel 4 (Odd = 0)
			break;
		case	5:
			bChanNib	= 10;
//			dwWdata = (DWROD)( 0xBA00 | dwOSR & 0xFFFF );	// Select ADC channel 5 (Odd = 1)
			break;
		case	6:
			bChanNib	= 3;
//			dwWdata = (DWROD)( 0xB300 | dwOSR & 0xFFFF );	// Select ADC channel 6 (Odd = 0)
			break;
		case	7:
			bChanNib	= 11;
//			dwWdata = (DWROD)( 0xBB00 | dwOSR & 0xFFFF );	// Select ADC channel 7 (Odd = 1)
			break;
		case	8:
			bChanNib	= 4;
//			dwWdata = (DWROD)( 0xB400 | dwOSR & 0xFFFF );	// Select ADC channel 8 (Odd = 0)
			break;
		case	9:
			bChanNib	= 12;
//			dwWdata = (DWROD)( 0xBC00 | dwOSR & 0xFFFF );	// Select ADC channel 9 (Odd = 1)
			break;
		case	10:
			bChanNib	= 5;
//			dwWdata = (DWROD)( 0xB500 | dwOSR & 0xFFFF );	// Select ADC channel 10 (Odd = 0)
			break;
		case	11:
			bChanNib	= 13;
//			dwWdata = (DWROD)( 0xBD00 | dwOSR & 0xFFFF );	// Select ADC channel 11 (Odd = 1)
			break;
		case	12:
			bChanNib	= 6;
//			dwWdata = (DWROD)( 0xB600 | dwOSR & 0xFFFF );	// Select ADC channel 12 (Odd = 0)
			break;
		case	13:
			bChanNib	= 14;
//			dwWdata = (DWROD)( 0xBE00 | dwOSR & 0xFFFF );	// Select ADC channel 13 (Odd = 1)
			break;
		case	14:
			bChanNib	= 7;
//			dwWdata = (DWROD)( 0xB700 | dwOSR & 0xFFFF );	// Select ADC channel 14 (Odd = 0)
			break;
		case	15:
			bChanNib	= 15;
//			dwWdata = (DWROD)( 0xBF00 | dwOSR & 0xFFFF );	// Select ADC channel 15 (Odd = 1)
			break;
		default:
			break;
		}
		if( lP.bDiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else	// LTC1859 type, format write data for channel select, etc.
	{
		switch( lP.nChn )
		{
		case	0:				// Condenser Lens current monitor
			dwWdata	= 0x8000;	// ADC Channel 0, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	1:				// Objective Lens current monitor
			dwWdata	= 0xC000;	// ADC Channel 1, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	2:				// X alignment coil current monitor
			dwWdata	= 0x9000;	// ADC Channel 2, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	3:				// Y alignment coil current monitor
			dwWdata	= 0xD000;	// ADC Channel 3, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	4:				// Amplifier positive power supplies voltages monitor
			dwWdata	= 0xA000;	// ADC Channel 4, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	5:				// Amplifier positive power supplies voltages monitor
			dwWdata	= 0xE000;	// ADC Channel 5, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	6:				// Thermistor input 1
			dwWdata	= 0xB000;	// ADC Channel 6, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	7:				// Thermistor input 2
			dwWdata	= 0xF000;	// ADC Channel 7, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		}
	}	// LTC1859 type

	BYTE bOP2, bOP3, bOP4;
	if( lP.bType )	// 2448, 32 bit read adc
		// Upper nibble of OP2 = Transfer Type (C for 32 bit Read)
		// OR the target Module ID into lower nibble of OP2
		bOP2	= 0xC0 | (BYTE)lP.nMID;
	else			// 1859, 16 bit read adc
		// Upper nibble of OP2 = Transfer Type (A for 16 bit Read)
		// OR the target Module ID into lower nibble of OP2
		bOP2	= 0xA0 | (BYTE)lP.nMID;
	bOP3	= (BYTE)(dwWdata & 0xFF);
	bOP4	= (BYTE)((dwWdata /256) & 0xFF);// Shift data down 8 bits before assigning to OP4
	CSemCtrl::Instance().USB_CommToBoards2(	(BYTE)lP.nCID,		// Operand 1 = ckt address
										bOP2,				// Operand 2 = Transfer type and Module ID
										bOP3,
										bOP4 );
}

CString CAutoTestApp::ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, BOOL bLog )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes = "";
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;

	double dRangeMin = m_DL.adc.chn[lP.nNum*16 +lP.nChn].dMin;
	double dRangeMax = m_DL.adc.chn[lP.nNum*16 +lP.nChn].dMax;
	double dEquivCurrent = 0.0;

	double dADCvref, dADCCOMoffset;
	switch( lP.nNum )
	{
	case	0:
		dADCvref		= 4;
		dADCCOMoffset	= 0;
		break;
	case	1:
		dADCvref		= 10;
		dADCCOMoffset	= 0;
		break;
	case	2:
		dADCvref		= 4;
		dADCCOMoffset	= 1.607;
		break;
	case	3:
		dADCvref		= 4;
		dADCCOMoffset	= 1.607;
		break;
	case	4:
		dADCvref		= 4.5;
		dADCCOMoffset	= 0;
		break;
	case	5:
		dADCvref		= 4.5;
		dADCCOMoffset	= 0;
		break;
	}
	double dADCcurrentMeas	= m_DL.adc.chn[lP.nNum*16 +lP.nChn].dFactor;// if non-zero value, then convert voltage to current measurement
	double dADCvoltsPerAmp, dADCampsPerVolt;
	dADCvoltsPerAmp = dADCampsPerVolt = 0;
	if( dADCcurrentMeas > 0 )		// Don't calculate unless non-zero, otherwise generate divide-by-0 error:
	{
		dADCvoltsPerAmp = m_DL.adc.chn[lP.nNum*16 +lP.nChn].dFactor;
		dADCampsPerVolt = 1 / dADCvoltsPerAmp;
	}

	dValue = 0.0;
	if( lP.bType )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)nData1<<16) | (DWORD)nData2;
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		if( bLog )
		{
			str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
			dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
		else
			dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2

		dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
		if( bADCvalSignBit )
		{
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
				// g_ADCvref varies per board design
				str.Format( "%.6f\t", dADCreadVrefFrac * dADCvref /2 );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref /2 +dADCCOMoffset;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * dADCampsPerVolt;
		}
		else
		{
			if( bLog )
			{
				str.Format( "-%.4f\t", dADCreadVrefPct );
				strRes += str;
				str.Format( "-%.6f\t", dADCreadVrefFrac * dADCvref /2 );
				strRes += str;
			}
			dValue = dADCCOMoffset - dADCreadVrefFrac * dADCvref /2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)nData1;
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		if( bLog )
		{
			str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		dEquivCurrent = dValue * dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * dADCvref / 2 * dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( bLog )
	{
		if( dADCcurrentMeas > 0 )
		{
			str.Format( "%.6f\t", dEquivCurrent );
			strRes += str;
		}
		else
		{
			str.Format( "n/a\t" );
			strRes += str;
		}

		if( strRes.IsEmpty() )
			strRes.Format( "\t\t\t\t" );
	}
	return strRes;
}

CString CAutoTestApp::ReadComprs_Logging( int nData )
{
	// TODO: Add your control notification handler code here
	CString strRes = "";
/*	
	// 4th readback word (7th and 8th ASCII characters) contains Comparator data
	WORD wData, wRBit, wMskBit;
	wData = (WORD)nData;
	for( wRBit=0; wRBit<14; wRBit += 2 )
	{
		wMskBit = (WORD)(pow(2, wRBit) );	// Odd numbered bits are Upper trip point comparator states
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );
		
		wMskBit *= 2;					// Shift up one bit to check Lower trip point comparator state
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
	}
*/	
	strRes.Format( "%04X\t", nData );
	return strRes;
}

CString CAutoTestApp::ReadComprs_Logging()
{
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	//	unsigned short* pnFrameData = new unsigned short[ lReadSize +256 ];
	//	ZeroMemory(pnFrameData, sizeof(pnFrameData));
	//	delete [] pnFrameData;

	BOOL bFound = FALSE;
	CString strRes = "";
	if( CSemCtrl::Instance().USB_LoopbackAndReadData( 1, 256, theApp.m_pnReadComprData, &lBytesTransferred, -1 ) )
	{
//		UpdateReadbackData( theApp.m_pnReadComprData );
		CString str, strMsg, strList;
		unsigned long lCount = theApp.m_pnReadComprData[0];
		for( unsigned long i=1; i<=lCount; i++ )
		{
			// 直到找到0xD22D才开始计数，前边的数据忽略
			if( theApp.m_pnReadComprData[i] == 0xD22D )
				bFound = TRUE;
			if( !bFound )
				continue;
			
			m_nRBwdCnt++;
			m_nBoardReadDataBuf[ theApp.m_nRBwdCnt*2 ] = theApp.m_pnReadComprData[i];	// Numeric value (0-FFFF range)
			if( m_nRBwdCnt % 2 == 0 )	// 0xD22D后面的数就是需要的状态数据
			{
				// 4th readback word (7th and 8th ASCII characters) contains Comparator data
				strRes.Format( "%04X\t", theApp.m_pnReadComprData[i] );
				return strRes;
			}
		}
	}
	return strRes;
}

// ADC回读后使用ADCvconv将模拟量转换为数字量
// 现在要由ADC回读值数字量得到实际物理量（电压、电流）
double CAutoTestApp::GetPhysicsFromAdcs( int nIndex, double dVadc )
{
	// 由曲线图索引得到ADC通道号索引
//	int nIndex = theApp.m_DL.graph[nIndexGraph].nIndex;
	// 取出该通道的公式字符串
	CString strFunc = theApp.m_DL.log.strCalc[nIndex];
	if( strFunc == "#" )
		return dVadc;
	
	// 进行公式改写
	CString strVfront, strVback, strNew;	// V前字符串，V后字符串，加入数值的公式字符串
	int nPos = strFunc.Find( "V" );
	if( nPos > 0 )	// V不是公式中的第一个字符
	{
		strVfront	= strFunc.Left( nPos );
		strVback	= strFunc.Right( strFunc.GetLength() - nPos -1 );
		strNew.Format( "%s%.6f%s", strVfront, dVadc, strVback );
	}
	else
	{
		strVback	= strFunc.Right( strFunc.GetLength() -1 );	// 取除去V的所有字符串
		strNew.Format( "%.6f%s", dVadc, strVback );
	}
	
	// 调用公式分析计算
	char cFunc[100];
	sprintf( cFunc, "%s", strNew );
	return counter( cFunc );
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CAutoTestApp::MCI_Det03SB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos;
	m_DL.diag[Diag_Offset_0 + nChn].nCurPos = nPos;
	m_BrdMCI.nOffset[nChn] = 2048 -nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (nChn << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det03StoreDAC, TRUE, wValue );
}

void CAutoTestApp::MCI_Det45SB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos;
	m_DL.diag[Diag_Offset_4 + nChn].nCurPos = nPos;
	if( nChn < 2 )
		m_BrdMCI.nOffset[nChn +4] = 2048 -nPos;

	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (nChn << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
}

void CAutoTestApp::MCI_DetGainSB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue = nPos;
	m_BrdMCI.nGain[nChn] = nPos;

	WORD wValue = (WORD)( (nChn << 8) | (BYTE)nValue );
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetGain, TRUE, wValue );
}

void CAutoTestApp::MCI_BWselSB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue = nPos;
	m_BrdMCI.nFilter[nChn] = nPos;

	// filter pin in operand low byte
	// detector number in operand high byte
	WORD wValue = (WORD)( (nChn << 8) | (BYTE)nValue );
	CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
}

void CAutoTestApp::MCI_UpdateSpotX( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	theApp.m_DL.diag[Diag_Pos_X].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)nPos );
}

void CAutoTestApp::MCI_UpdateSpotY( int nPos, BOOL bUpdate )
{
	double dValue = 0.0;
	theApp.m_DL.diag[Diag_Pos_Y].nCurPos = nPos;

	if( bUpdate )
		CSemCtrl::Instance().USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)nPos );
}



void CAutoTestApp::SCN_XstigDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Stig_X].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
									USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
/*
	// Stigmator
	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		VARIANT Xpos, Ypos, Flag;
		Xpos.lVal = nPos /2;
		Flag.ulVal	= (DWORD)0x01;
		pMainFrame->m_wndImpPanelAll2.m_ctrlStigmation.SyncPos( Xpos, Ypos, Flag );
	}

	m_DL.diag[Diag_Stig_X].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal	= 1;
		Xpos.lVal	= nPos;
		SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}*/
}

void CAutoTestApp::SCN_YstigDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Stig_Y].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
									USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
/*
	// Stigmator
	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		VARIANT Xpos, Ypos, Flag;
		Ypos.lVal = nPos /2;
		Flag.ulVal	= (DWORD)0x02;
		pMainFrame->m_wndImpPanelAll2.m_ctrlStigmation.SyncPos( Xpos, Ypos, Flag );
	}

	m_DL.diag[Diag_Stig_Y].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_Stigmator;
		Flag.lVal	= 2;
		Ypos.lVal	= nPos;
		SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}*/
}

void CAutoTestApp::SCN_XshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Shift_X].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
									USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
}

void CAutoTestApp::SCN_YshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Shift_Y].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
									USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
}

void CAutoTestApp::SCN_XxoverDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_xover_X].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
									USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
}

void CAutoTestApp::SCN_YxoverDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_xover_Y].nCurPos = nPos;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
									USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
}







void CAutoTestApp::LNS_L1DACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 32768;
	m_DL.diag[Diag_CL].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x12 as low byte (L1DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_CondenserLens,	// Serial Comm operand low byte (Condenser Lens DAC circuit address)
									USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
/*
	m_DL.diag[Diag_CL].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// CondenserLens
	{
		ClassID.lVal = SemControlPanelClass_CondenserLens;
		Flag.lVal	= 1;
		Xpos.lVal	= nPos;
		SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}*/
}

void CAutoTestApp::LNS_L2SDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 32768;
	m_DL.diag[Diag_OL].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x22 as low byte (L2SDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_ObjectiveLens_Static,// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
									USB_TM_LNS_Write_16,			// Serial Comm operand high byte (16 bit write to LNS board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
/*
	m_DL.diag[Diag_OL].nCurPos = nPos;
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;
	// ObjectiveLens
	{
		ClassID.lVal = SemControlPanelClass_ObjectiveLens;
		Flag.lVal	= 1;
		Xpos.dblVal	= nPos;
		Xpos.lVal	= nPos;
		SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	}*/
}

void CAutoTestApp::LNS_L2DDACSB_Change( int nPos )
{
}

void CAutoTestApp::LNS_XalDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 32768;
	m_DL.diag[Diag_Align_X].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x42 as low byte (XalDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_XAlignment,		// Serial Comm operand low byte (Obj Lens Static DAC circuit address)
									USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte

	// BeemAlignment
/*	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		VARIANT Xpos, Ypos, Flag;
		Xpos.lVal = nPos /2;
		Flag.ulVal	= (DWORD)0x01;
		pMainFrame->m_wndImpPanelAll2.m_ctrlBeemAlignment.SyncPos( Xpos, Ypos, Flag );
	}*/
}

void CAutoTestApp::LNS_YalDACSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 32768;
	m_DL.diag[Diag_Align_Y].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x52 as low byte (YalDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_YAlignment,	// Serial Comm operand low byte (Y alignment DAC circuit address)
									USB_TM_LNS_Write_16,	// Serial Comm operand high byte (16 bit write to LNS board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte

	// BeemAlignment
/*	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	if( pMainFrame && pMainFrame->m_wndImpPanelAll2.m_hWnd != NULL )
	{
		VARIANT Xpos, Ypos, Flag;
		Ypos.lVal = nPos /2;
		Flag.ulVal	= (DWORD)0x02;
		pMainFrame->m_wndImpPanelAll2.m_ctrlBeemAlignment.SyncPos( Xpos, Ypos, Flag );
	}*/
}

void CAutoTestApp::LNS_WobDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	m_DL.diag[Diag_Wob].nCurPos = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	// Write via MCI to LNS board.  Instruction 1: write 0x22 as high byte (16 bit write to Board 2), 0x62 as low byte (WobDAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_LNS_WobblerSetting,	// Serial Comm operand low byte (Y alignment DAC circuit address)
									USB_TM_LNS_Write_16,		// Serial Comm operand high byte (16 bit write to LNS board)
									byteLo,		// Serial Comm operand low byte
									byteHi );	// Serial Comm operand high byte
}

void CAutoTestApp::SPR_DACupdate( int nPos )
{
	int nValue = nPos + 32768;
	BYTE bSPRDACfirst24	= (BYTE)( 0x10 | (0 * 2) );	// Selects immediate load, channel select shifted up 1 bit
	DWORD dwWdata = (DWORD)(bSPRDACfirst24 * 65536 + nValue);
	
	theApp.BoardWrite24(24, USB_MID_SPR, USB_CID_SPR_General_Purpose_DACs, dwWdata);
}
