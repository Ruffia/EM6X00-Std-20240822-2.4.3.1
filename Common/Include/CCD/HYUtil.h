
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HYUTIL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HYUTIL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef __HYUSBSDK__
#define __HYUSBSDK__

#ifdef HYUTIL_EXPORTS
#define HYUTIL_API  //__declspec(dllexport)
#else
#define HYUTIL_API  //__declspec(dllimport)
#endif

//
#define		WM_VIDEODATA					(WM_USER + 0x3522)
#define		WM_SNAP_KEYDOWN					(WM_USER + 0x3523)

#define		OPER_SNAPSHOT					10
#define		OPER_CALLBACK					11
//

#define		MAX_DEV_SUPPORT					4

#define		ERR_SUCCESS						0
#define		ERR_FAILED						1
#define		ERR_NOTAVAILABLE				2

#define		ERR_NODEVICE					101
#define		ERR_DIRECTX_SETUP				102
#define		ERR_DEV_NOTOPEN					104
#define		ERR_NOTSUPPORT					105
#define		ERR_SET_VIDEOSRC				106
#define		ERR_GRAPH_CONNECT				107
#define		ERR_DEVICE_RUN					108
#define		ERR_NOT_MEMIMG					109

#define		ERR_SNAPSHOT_END				200
#define		ERR_WAIT_TIMEOUT				201
#define		ERR_BUFFER_TOOSMALL				202
#define		ERR_DEVICE_STATE				203
#define		ERR_INSUFFICIENT_RESOURCES		204
#define		ERR_INVALID_PARAM				205

#ifndef _DEFINE_CPXFORMAT
#define _DEFINE_CPXFORMAT

typedef enum{
	FORMAT_DATE = 1,			//DateTime∏Ò Ω
	FORMAT_TIME,
	FORMAT_STRING,				//
	FORMAT_PICTURE,				//
	FORMAT_UNKNOWN				//
}CPX_FORMAT;

#endif


HYUTIL_API UINT WINAPI HYInitialize();
HYUTIL_API void WINAPI HYUninitialize();

HYUTIL_API int	WINAPI HYGetDevCnt();

HYUTIL_API UINT	WINAPI HYOpenDev	(UINT	unIndex);
HYUTIL_API UINT	WINAPI HYCloseDev	(UINT	unIndex);

HYUTIL_API UINT	WINAPI HYPortSpeed(UINT unIndex,BOOL &bHighSpeedPort);

//video preview
HYUTIL_API UINT	WINAPI HYSetOwnerWnd(UINT	unIndex,HWND hWnd);
HYUTIL_API UINT	WINAPI HYStartDevice(UINT	unIndex);
HYUTIL_API UINT WINAPI HYStopDevice (UINT	unIndex);

//video capture
//===================================================================
HYUTIL_API UINT WINAPI HYSetCaptureFile	(UINT	unIndex,LPCTSTR szFileName);
HYUTIL_API UINT WINAPI HYStartCapture	(UINT	unIndex);
HYUTIL_API UINT WINAPI HYStopCapture	(UINT	unIndex);

typedef enum {
	VIDEO_BRIGHTNESS = 1,
	VIDEO_CONTRAST,
	VIDEO_HUE,
	VIDEO_SATURATION,
	VIDEO_SHARPNESS
}VIDEOPROP;

HYUTIL_API UINT WINAPI HYGetVideoPropRange(UINT			unIndex,
										   VIDEOPROP	enProp,
										   int			&nMax,
										   int			&nMin,
										   int			&nDefault);

HYUTIL_API UINT WINAPI HYGetVideoProp(UINT	unIndex,VIDEOPROP enProp,int &nValue);
HYUTIL_API UINT WINAPI HYSetVideoProp(UINT	unIndex,VIDEOPROP enProp,int nValue);

HYUTIL_API UINT WINAPI HYGetVideoSize(UINT	unIndex,UINT &unSizeX,UINT &unSizeY);
HYUTIL_API UINT WINAPI HYSetVideoSize(UINT	unIndex,UINT unSizeX,UINT unSizeY);

//////////////////////////////////////////////////////////////////////////////
typedef enum {
	FMT_YUY2 = 1,
	FMT_Y41P,
	FMT_RGB,
	FMT_UNKNOWN
}VIDEOFMT;

HYUTIL_API UINT WINAPI HYGetVideoFmt(UINT	unIndex,VIDEOFMT &enFmt);
HYUTIL_API UINT WINAPI HYSetVideoFmt(UINT	unIndex,VIDEOFMT enFmt);

//////////////////////////////////////////////////////////////////////////////
typedef enum {
	STD_PAL = 1,
	STD_NTSC,
	STD_SECAM,
	STD_UNKNOW
}VIDEOSTD;

HYUTIL_API UINT WINAPI HYGetVideoStd(UINT	unIndex,VIDEOSTD &enStd);
HYUTIL_API UINT WINAPI HYSetVideoStd(UINT	unIndex,VIDEOSTD enStd);		

///////////////////////////////////////////////////////////////////////////////
typedef enum {
	SCAN_FIELD = 1,
	SCAN_FRAME
}SCANMODE;

HYUTIL_API UINT WINAPI HYGetScanMode(UINT	unIndex,SCANMODE &enScanMode);
HYUTIL_API UINT WINAPI HYSetScanMode(UINT	unIndex,SCANMODE enScanMode);

//////////////////////////////////////////////////////////////////////////////
typedef enum {
	SRC_AV1 = 1,
	SRC_AV2,
	SRC_AV3,
	SRC_AV4,
	SRC_SV1
}VIDEOSRC;

HYUTIL_API UINT WINAPI HYGetVideoSrc(UINT	unIndex,VIDEOSRC &enSrc);
HYUTIL_API UINT WINAPI HYSetVideoSrc(UINT	unIndex,VIDEOSRC enSrc,BOOL bSafeSwitch);

//////////////////////////////////////////////////////////////////////////////
HYUTIL_API UINT WINAPI HYVideoPresent(UINT	unIndex,BOOL &bVideoPresent);
///////////////////////////////////////////////////////////////////////////////
typedef enum {
	METHOD_SAMPLE = 1,
	METHOD_NORMAL,
	METHOD_COMPLEX,
	METHOD_PASS,
	METHOD_UNKNOWN
}PROCMETHOD;

HYUTIL_API UINT WINAPI HYGetProcMethod(UINT unIndex,PROCMETHOD &enMethod);
HYUTIL_API UINT WINAPI HYSetProcMethod(UINT unIndex,PROCMETHOD enMethod);
////////////////////////////////////////////////////////////////////////////////

HYUTIL_API UINT WINAPI HYGetFieldSeq(UINT	unIndex,BOOL &bTopFieldFirst);
HYUTIL_API UINT WINAPI HYSetFieldSeq(UINT	unIndex,BOOL bTopFieldFirst);

////////////////////////////////////////////////////////////////////////////////
HYUTIL_API UINT WINAPI HYReadEprom	(UINT	unIndex,PBYTE pBuffer,UINT nStartPos,UINT nLen);
HYUTIL_API UINT WINAPI HYWriteEprom (UINT	unIndex,PBYTE pBuffer,UINT nStartPos,UINT nLen);

////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	EFT_NORMAL = 1,
	EFT_GOOD
}VIDEOEFFECT;

HYUTIL_API UINT WINAPI HYSetPlayEffect (UINT unIndex,VIDEOEFFECT enEft);		//twice frame rate

/////////////////////////////////////////////////////////////////////////////////////////////////
HYUTIL_API UINT WINAPI HYGetSnapRect(UINT unIndex,long &nLeft,long &nTop,long &nRight,long &nBottom);
HYUTIL_API UINT WINAPI HYSetSnapRect(UINT unIndex,long nLeft, long nTop, long nRight, long nBottom);

HYUTIL_API UINT WINAPI HYMirrorHorizon (UINT unIndex,BOOL bMirrorHorizon);
HYUTIL_API UINT WINAPI HYMirrorVertical(UINT unIndex,BOOL bMirrorVertical);

HYUTIL_API UINT WINAPI HYPreAllocBuf   (UINT unIndex,long nPreAllocFrames);
HYUTIL_API UINT WINAPI HYGetAllocBuf   (UINT unIndex,long *nAllocFrames);

HYUTIL_API UINT WINAPI HYRegNotifyWnd  (UINT unIndex,HWND hWnd,BOOL bOnlyKeyDown);
HYUTIL_API UINT WINAPI HYStartSnap     (UINT unIndex,long nPictures,long nInterval);
HYUTIL_API UINT WINAPI HYCancelSanp    (UINT unIndex);
HYUTIL_API UINT WINAPI HYGetImgBufsize (UINT unIndex,long *pnSize,VIDEOFMT enFmt);
HYUTIL_API UINT WINAPI HYGetMemSnap    (UINT unIndex,BYTE *pImgBuf,long nSize,VIDEOFMT enFmt,DWORD dwTimeout);

HYUTIL_API UINT WINAPI HYPaintMemImg	(BYTE *pImgBuf,long nSize,HWND hPaintWnd);
HYUTIL_API UINT WINAPI HYPaintDiskImg	(LPCTSTR szFileName,HWND hPaintWnd);
HYUTIL_API UINT WINAPI HYEndPaint		(HWND hPaintWnd);

HYUTIL_API UINT WINAPI HYSaveImgAsJpg	(BYTE *pImgBuf,long nSize,LPCTSTR szJpgFileName,long nQuality);
HYUTIL_API UINT WINAPI HYSaveImgAsBmp	(BYTE *pImgBuf,long nSize,LPCTSTR szBmpFileName);

HYUTIL_API UINT WINAPI HYStartCallback (UINT  unIndex);
HYUTIL_API UINT WINAPI HYStopCallback  (UINT  unIndex);
HYUTIL_API UINT WINAPI HYGetVideoData  (UINT  unIndex,
										BYTE  **ppStreamBuf, 
										long  *pnSize, 
										long  *pnWidth,
										long  *pnHeight,
										DWORD dwTimeout);

HYUTIL_API UINT WINAPI HYFreeVideoBuf(UINT  unIndex,BYTE *pStream);

HYUTIL_API UINT WINAPI HYGetScreenBmp(HWND hWnd,BYTE **ppImgBuf,long *pnSize);
HYUTIL_API UINT WINAPI HYFullScreen	 (UINT nIndex,BOOL bFullScreen);
//====use of OSD====////////////////////////////////////////////////////////////////////////////////
HYUTIL_API HANDLE WINAPI HYOpenComplexFile(UINT unIndex,LPCTSTR szFileName);
HYUTIL_API UINT WINAPI HYCloseComplexFile(UINT unIndex,HANDLE hHandle);
HYUTIL_API UINT WINAPI HYSetComplexEffect(UINT unIndex,HANDLE hHandle,BOOL bTransparence);
HYUTIL_API UINT WINAPI HYSetComplexPos   (UINT unIndex,HANDLE hHandle,long nStartX,long nStartY);
HYUTIL_API UINT WINAPI HYComplexEnable   (UINT unIndex,HANDLE hHandle,BOOL bEnable);
HYUTIL_API UINT WINAPI HYGetComplexSize  (UINT unIndex,HANDLE hHandle,long *pnSizeX,long *pnSizeY);
//====Make OSD file===//////////////////////////////////////////////////////////////////////////////
HYUTIL_API BOOL WINAPI HYStartupOsdMake();
HYUTIL_API long WINAPI HYGetFontFamilyCount();
HYUTIL_API BOOL WINAPI HYGetFontFamilyName(long nIndex,char szName[256]);
HYUTIL_API BOOL WINAPI HYCreateImgComplexFile(LPCTSTR	szFileName,
											  LPCTSTR	szImgName,
											  COLORREF	clrBack,
											  HWND		hPaintWnd);

HYUTIL_API BOOL	WINAPI HYCreateTxtComplexFile(LPCTSTR		szFileName,
											  CPX_FORMAT	enComplexFmt,
											  LPCTSTR		szText,
											  LPCTSTR		szFontFamily,
											  long			nFontHeight,
											  COLORREF		clrText, 
											  int			nSpacings,
											  HWND			hPaintWnd);

HYUTIL_API CPX_FORMAT WINAPI HYGetComplexFileFormat(LPCTSTR szFileName);
HYUTIL_API CPX_FORMAT WINAPI HYGetComplexFileFormatA(HANDLE hHandle);

HYUTIL_API BOOL WINAPI HYPreviewFile(HANDLE hHandle,HWND hPaintWnd);
HYUTIL_API BOOL WINAPI HYPreviewFileA(LPCTSTR szFileName, HWND hPaintWnd);
HYUTIL_API void WINAPI HYShutdownOsdMake();
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	BUS_SHARED		= 1,
	BUS_EXCLUSIVE
}BUSTYPE;

HYUTIL_API UINT WINAPI HYGetBusType(UINT unIndex,BUSTYPE &enBusType);
HYUTIL_API UINT WINAPI HYSetBusType(UINT unIndex,BUSTYPE enBusType);

HYUTIL_API UINT WINAPI HYGetVideoOverturn(UINT unIndex,BOOL &bOverturn);
HYUTIL_API UINT WINAPI HYSetVideoOverturn(UINT unIndex,BOOL bOverturn);

HYUTIL_API UINT WINAPI HYSignalInterlaced(UINT unIndex,BOOL &bSignalInterlaced);
HYUTIL_API UINT WINAPI HYGetUserCode(UINT unIndex,BYTE *pBuffer,long nBufSize);

HYUTIL_API UINT WINAPI HYDrawClipRect(UINT unIndex,long nLeft,long nTop,long nRight,long nBottom,COLORREF clrLine);
HYUTIL_API UINT WINAPI HYGetSelectedRect(UINT unIndex,long &nLeft,long &nTop,long &nRight,long &nBottom);
HYUTIL_API UINT WINAPI HYClearClipDraw(UINT unIndex);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//encore

#ifndef _ENCODE_DEFINITION_
#define _ENCODE_DEFINITION_

typedef enum
{
	VIDEO_ENCORE_CBR = 0,
	VIDEO_ENCORE_VBR = 1,
}ENCOREMODE;

#define		FLAG_REQUEST_VIDEO		0x00000001
#define		FLAG_REQUEST_AUDIO		0x00000002

typedef struct _tagMediaParam
{
	ENCOREMODE 	enEncoreMode;
	double		dbFrameRate;
	DWORD		dwVideoWidth;
	DWORD  		dwVideoHeight;
	DWORD  		dwBitRate;
	DWORD  		dwVBRPeakRate;
	DWORD  		dwVBRMaxRate;
	DWORD  		dwQuantizer;
	DWORD		dwMotionPrecision;
	DWORD  		dwKeyFrmInterval;
}VIDEO_ENCORE_PARAM,*PVIDEO_ENCORE_PARAM;

typedef enum
{
	RECTYPE_MP4 = 0,
	RECTYPE_AVI = 1,
}RECTYPE;

#endif

HYUTIL_API UINT WINAPI HYEnableEncode(UINT unIndex,BOOL bEnable);
HYUTIL_API UINT WINAPI HYCreateEncoder(UINT unIndex,UINT unRequestFlag,PVIDEO_ENCORE_PARAM pVideoEncoreParam);
HYUTIL_API UINT WINAPI HYDestoryEncoder(UINT unIndex);
HYUTIL_API UINT WINAPI HYCreateRecordFile(UINT unIndex,LPCTSTR szFileName,RECTYPE enType);
HYUTIL_API UINT WINAPI HYCloseRecordFile(UINT unIndex);

#endif
