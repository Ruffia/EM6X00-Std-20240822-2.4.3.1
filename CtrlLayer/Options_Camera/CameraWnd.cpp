// CameraWnd.cpp : implementation file
//
#include "stdafx.h"
#include "Options_Camera.h"
#include "CameraWnd.h"

#include "USB_Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IplImage*	g_pTheImageWnd;
DWORD WINAPI CommThreadWnd(LPVOID lpParam);

HANDLE	g_hThreadWnd;
DWORD	g_dwThreadWnd;
HANDLE	g_hEventWnd				= CreateEvent(NULL,FALSE, FALSE, _T("WORKcamera2"));
HANDLE	g_hEventTeminateWnd		= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEcamera2"));

/////////////////////////////////////////////////////////////////////////////
// CCameraWnd

CCameraWnd::CCameraWnd()
{
	g_pTheImageWnd	= NULL;
	m_nCapture		= -1;
}

CCameraWnd::~CCameraWnd()
{
}

BEGIN_MESSAGE_MAP(CCameraWnd, CWnd)
	//{{AFX_MSG_MAP(CCameraWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZING()
	ON_COMMAND(ID_Camera_Settings, OnCameraSettings)
	ON_UPDATE_COMMAND_UI(ID_Camera_Settings, OnUpdateCameraSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraWnd message handlers

int CCameraWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect(0,0,320,240);	// �ٶ��ͻ�����400*300��С
	CalcWindowRect(&rect);		// ���ϱ������߿�ȵĴ�������С
	SetWindowPos(NULL, 0,0,rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW );

	// ���Ի���λ�����õ�ɨ�������Ҳ�
	CRect rc;
	GetClientRect( &rc );
	ClientToScreen( &rc );
	SetWindowPos(&CWnd::wndTop,
		1260,//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
		32*3,//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
		rc.Width(), rc.Height(), SWP_NOSIZE);

	m_Menu.LoadMenu(IDR_Settings);
	SetMenu(&m_Menu);
	m_dlgCCDCameraSetting.Create(CDlgCCDCameraSetting::IDD,this);

	Localize();
	return 0;
}

void CCameraWnd::SetLanguageID( WORD wID )
{
	m_wLanguageID = wID;
	char szBuf[20];
	if( wID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED))
		sprintf_s( szBuf, "zh_CN" );
	else if( wID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT))
		sprintf_s( szBuf, "en_US" );
	else if( wID == MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT))
		sprintf_s( szBuf, "ru_RU" );
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\CCD\\CCD.ini");
	// д�������ļ�
	WritePrivateProfileString( "CCDCfg", "Language", szBuf, path );
	Localize();
}

void CCameraWnd::Localize()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\CCD\\CCD.ini");
	char szLang[50];
	int nSize = 50;
	if(GetPrivateProfileString("CCDCfg", "Language", "", szLang, nSize, path) != 0)
	{
		exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		if( strcmp( szLang, "zh_CN" ) == 0)
			strcat_s(path, "\\Locale\\zh\\CCD_zh_CN.ini");
		else if( strcmp( szLang, "en_US" ) == 0)
			strcat_s(path, "\\Locale\\en\\CCD_en_US.ini");
		else if( strcmp( szLang, "ru_RU" ) == 0)
			strcat_s(path, "\\Locale\\ru\\CCD_ru_RU.ini");
		char szBuf[50];
		// ����
		if(GetPrivateProfileString("Label", "Cap", "", szBuf, nSize, path) != 0)
			SetWindowText( szBuf );
		// �˵�
		if(GetPrivateProfileString("Label", "Menu_Set", "", szBuf, nSize, path) != 0)
		{
			m_Menu.ModifyMenuA( 0, MF_BYPOSITION, ID_Camera_Settings, szBuf );
			DrawMenuBar();
		}

		if( m_dlgCCDCameraSetting.m_hWnd != NULL )
		{
			strcpy_s( m_dlgCCDCameraSetting.m_cPathLocalize, path );
			m_dlgCCDCameraSetting.Localize();
		}
	}
}

void CCameraWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if( m_nCapture < 0 )
		dc.TextOut(10,10,"has no Camera");
	else
	{
		CRect rect;
 		GetClientRect( &rect );
// 		CBrush brush;
// 		brush.CreateSolidBrush(RGB(255,255,255));
// 		dc.FillRect(CRect(rect.left, rect.top, rect.right, rect.top+20), &brush);
// 		dc.TextOut(rect.left, rect.top, "��ʹ������ʱ����رձ��Ի���");
// 		rect.top += 20;
 		CvvImage cimg;
 		cimg.CopyOf( g_pTheImageWnd );					// ����ͼƬ
 		cimg.DrawToHDC( dc.GetSafeHdc(), &rect );		// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void CCameraWnd::OnDestroy() 
{
	CamStop();
	::PostMessage(GetParent()->m_hWnd, WM_USER_CCD_MSG, 0, 0 );
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CCameraWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	// �������趨�϶����ڹ����еĸ�
	// ע�⣺OnSizing�����趨���Ǵ��ڴ�С�������ͻ����ͱ߿򼰱�����
	CRect rect(0,0,400,300);	// �ٶ��ͻ�����400*300��С
	CalcWindowRect(&rect);		// ���ϱ������߿�ȵĴ�������С
	pRect->bottom = pRect->top + ( (pRect->right -pRect->left) -(rect.Width() -400) ) /4 *3 +(rect.Height() -300);
}

DWORD WINAPI CommThreadWnd(LPVOID lpParam)
{
	CCameraWnd* pWnd = (CCameraWnd*)lpParam;
	// ����1������ͷʵ��
	CCameraDS camera;
	//�򿪵�һ������ͷ
	//if(! camera.OpenCamera(0, true)) //��������ѡ�񴰿�
	if( (! camera.OpenCamera(0, false, IMAGE_WIDTH,IMAGE_HEIGHT) ) ) //����������ѡ�񴰿ڣ��ô����ƶ�ͼ���͸�
	{
		MessageBox( pWnd->m_hWnd, _T("Can not open camera.\n"), _T("Camera"), MB_OK );
		camera.CloseCamera();
		SetEvent( g_hEventWnd );
		return 0;
	}
	pWnd->m_dlgCCDCameraSetting.PostMessage(WM_SET_CAMERA_DS,(WPARAM)&camera);
	// ����һ��Iplͼ��ָ������ȡ����ͷ��֡����
	IplImage* cFrame;
	// ����һ����Ƶ���Ŵ���
//	cvNamedWindow("KYKY_Camera");
//	// ���������������С
//	cvResizeWindow("KYKY_Camera",1,1);
//	// ��ȡ���ھ��
//	HWND hWnd = (HWND) cvGetWindowHandle("KYKY_Camera");
//	// ��ȡ�ô��ڵĸ����ھ��
//	HWND hParent = ::GetParent(hWnd);
//	// ��ȡmymfc��GUI���ھ��
//	//	HWND hwnd1=::FindWindow("CPanelCamera","Test4");
//	// ��GUI��������Ϊ��Ƶ���Ŵ��ڵĸ����ڣ�ʹ��Ƶ�� GUI ��ָ�����򲥷�
////	::SetParent(hWnd, hwnd1);
//	// ��������������Ƶ���Ŵ���(ʹ��CView��������)
//	::ShowWindow(hParent, SW_HIDE);
	// ��ͼ����������
	if( g_pTheImageWnd )								
		cvZero( g_pTheImageWnd );

	pWnd->m_nCapture = 1;	
//	while( m_nCapture > 0 )
	while( pWnd->m_nCapture > 0 && WaitForSingleObject(g_hEventTeminateWnd,50) != WAIT_OBJECT_0 )
	{ 
		//��ȡһ֡
		cFrame = camera.QueryFrame();
		if(NULL == cFrame)
			continue;
		IplImage* newframe = cvCloneImage(cFrame);
		// ��ͼƬ img �������ţ������뵽 TheImage ��
		cvResize( newframe, g_pTheImageWnd );
		// ʹ��CView��ˢ��ʱ��OnDraw����ʾͼ��
		pWnd->Invalidate(FALSE);
		// ֱ��ʹ��cvNamedWindow��������ʾ����
//		cvShowImage("KYKY_Camera", TheImage);

//		if (cvWaitKey(20) == 27) break;
		if (cvWaitKey(20) == 27)
		{
			TRACE0("press ESC\n");
			break;
		}
		cvReleaseImage( &newframe );
		Sleep(1);
	}

	// ��ͼ����������
	if( g_pTheImageWnd )	
		cvZero( g_pTheImageWnd );
	camera.CloseCamera();
	cvDestroyWindow("KYKY_Camera");
	SetEvent( g_hEventWnd );
	return 0;
}

void CCameraWnd::CamInit()
{
	// ��ȡ����ͷ��Ŀ
	int nValidCam = 0;
	int cam_count = CCameraDS::CameraCount();
	if( cam_count > 0 )
	{
		for(int i=0; i < cam_count; i++)
		{
			char camera_name[1024];  
			int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
			if( retval > 0 )
			{
				// 2023.12 Ŀǰʹ����������ͷ�ͺ�
				// VID=05A3 & PID=9320������ͷ��name=HD USB Camera
				// VID=32E4 & PID=9422������ͷ��name=USB Camera
				// ���Դ�ȫ��ƥ��"HD USB Camera"��Ϊ����"USB Camera"
				char szBuf[255];
				sprintf( szBuf, "%s", camera_name );
				if( strstr( szBuf, "USB Camera") == NULL )
					continue;
				nValidCam++;
			}
		}
		if( nValidCam < 1 )
		{
//			MessageBox( _T("has no camera"), _T("Camera") );
			return;
		}
		// ��ʼ�� ���� TheImage
		CvSize ImgSize;
		ImgSize.height	= IMAGE_HEIGHT;
		ImgSize.width	= IMAGE_WIDTH;
		/////////////////////////////////////////////////////////////////
		// ��ʼ����Ƶ������ͷ��ʾѭ����ִ�к��жϣ�m_nCapture��
		g_pTheImageWnd = cvCreateImage( ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );
		m_nCapture = 0;
		ResetEvent( g_hEventWnd );
		ResetEvent( g_hEventTeminateWnd );
		
		g_hThreadWnd	= CreateThread( NULL, 0, CommThreadWnd, this, 0, &g_dwThreadWnd );
		if( g_hThreadWnd == NULL )
			return;
		CloseHandle(g_hThreadWnd);
		/////////////////////////////////////////////////////////////////
	}
}

void CCameraWnd::CamStop()
{
	if( m_nCapture > 0 )
	{
		SetEvent( g_hEventTeminateWnd );
		WaitForSingleObject( g_hEventWnd, INFINITE );
		m_nCapture = 0;
	}
}

void CCameraWnd::OnCameraSettings()
{
	// TODO: �ڴ���������������
	m_dlgCCDCameraSetting.ShowWindow(true);
}

void CCameraWnd::OnUpdateCameraSettings(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_nCapture > 0 );
}
