// PanelCamera.cpp : implementation file
//

// 1.����OpenCV��װ����1.0�汾����
//   ����Ҫ��OpenCV��װ��C:\Program Files\OpenCV���ڰ�װʱѡ��"��   \OpenCV\bin����ϵͳ����"��
//   ��װ��ɺ��ֶ���ӻ���������C:\Program Files\OpenCV\bin����
// 2.����VC++6.0���˵�Tools->Options->Directories��
//   ������lib·����ѡ��Library files�����·�����·����C:\Program Files\OpenCV\lib
//   Ȼ��ѡ��include files�����·�����·����
//   C:\Program Files\OpenCV\cxcore\include
//   C:\Program Files\OpenCV\cv\include
//   C:\Program Files\OpenCV\cvaux\include
//   C:\Program Files\OpenCV\ml\include
//   C:\Program Files\OpenCV\otherlibs\highgui
//   C:\Program Files\OpenCV\otherlibs\cvcam\include
// 3.ÿ����һ����Ҫʹ��OpenCV��VC Project������Ҫ����ָ����Ҫ��lib��
//   �˵���Project->Settings��Ȼ��Setting forѡΪAll Configurations��
//   Ȼ��ѡ���ұߵ�link��ǩ����Object/library modules�����ϣ�
//   cxcore.lib cv.lib ml.lib cvaux.lib highgui.lib cvcam.lib
//   �������Ҫ��ô��lib�������ֻ�������Ҫ��lib��
#include "stdafx.h"
#include "Options_Camera.h"
#include "PanelCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IplImage*	g_pTheImage;
DWORD WINAPI CommThread(LPVOID lpParam);

HANDLE	g_hThread;
DWORD	g_dwThread;
HANDLE	g_hEvent				= CreateEvent(NULL,FALSE, FALSE, _T("WORKcamera"));
HANDLE	g_hEventTeminate		= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEcamera"));

/////////////////////////////////////////////////////////////////////////////
// CPanelCamera dialog


CPanelCamera::CPanelCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelCamera::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelCamera)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	g_pTheImage	= NULL;
	m_nCapture	= -1;
}


void CPanelCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelCamera)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelCamera, CDialog)
	//{{AFX_MSG_MAP(CPanelCamera)
	ON_WM_DESTROY()
	ON_COMMAND(IDC_BUTTON_Run, OnRun)
	ON_COMMAND(IDC_BUTTON_Stop, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelCamera message handlers

BOOL CPanelCamera::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// ���Ի���λ�����õ�ɨ�������Ҳ�
	CRect rc;
	GetClientRect( &rc );
	ClientToScreen( &rc );
	SetWindowPos(&CWnd::wndTop,
		1280,//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
		32*3,//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
		rc.Width(), rc.Height(), SWP_NOSIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelCamera::OnDestroy() 
{
	CamStop();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CPanelCamera::OnOK() 
{
	// TODO: Add extra validation here
	CamStop();
	CDialog::OnOK();
}

void CPanelCamera::OnCancel() 
{
	// TODO: Add extra cleanup here
	CamStop();
	CDialog::OnCancel();
}

void CPanelCamera::OnRun() 
{
	// TODO: Add your command handler code here
	CamInit();
}

void CPanelCamera::OnStop() 
{
	// TODO: Add your command handler code here
	CamStop();
}

void CPanelCamera::CamStop()
{
	if( m_nCapture > 0 )
	{
		SetEvent( g_hEventTeminate );
		WaitForSingleObject( g_hEvent, INFINITE );
		m_nCapture = 0;
	}
}

DWORD WINAPI CommThread(LPVOID lpParam)
{
	CPanelCamera* pDlg = (CPanelCamera*)lpParam;
	// ����1������ͷʵ��
	CCameraDS camera;
	//�򿪵�һ������ͷ
	//if(! camera.OpenCamera(0, true)) //��������ѡ�񴰿�
	if( (! camera.OpenCamera(0, false, IMAGE_WIDTH,IMAGE_HEIGHT) ) ) //����������ѡ�񴰿ڣ��ô����ƶ�ͼ���͸�
	{
		MessageBox( pDlg->m_hWnd, _T("Can not open camera.\n"), _T("Camera"), MB_OK );
		camera.CloseCamera();
		SetEvent( g_hEvent );
		return 0;
	}
	// ����һ��Iplͼ��ָ������ȡ����ͷ��֡����
	IplImage* cFrame;
	// ����һ����Ƶ���Ŵ���
	cvNamedWindow("KYKY_Camera");
	// ���������������С
	cvResizeWindow("KYKY_Camera",1,1);
	// ��ȡ���ھ��
	HWND hWnd = (HWND) cvGetWindowHandle("KYKY_Camera");
	// ��ȡ�ô��ڵĸ����ھ��
	HWND hParent = ::GetParent(hWnd);
	// ��ȡmymfc��GUI���ھ��
//	HWND hwnd1=::FindWindow("CPanelCamera","Test4");
	// ��GUI��������Ϊ��Ƶ���Ŵ��ڵĸ����ڣ�ʹ��Ƶ�� GUI ��ָ�����򲥷�
//	::SetParent(hWnd, hwnd1);
	// ��������������Ƶ���Ŵ���
	::ShowWindow(hParent, SW_HIDE);
	// ��ͼ����������
	if( g_pTheImage )								
		cvZero( g_pTheImage );

	pDlg->m_nCapture = 1;	
//	while( m_nCapture > 0 )
	while( pDlg->m_nCapture > 0 && WaitForSingleObject(g_hEventTeminate,50) != WAIT_OBJECT_0 )
	{ 
		CWnd* pWnd = pDlg->GetDlgItem(IDC_BUTTON_Preview_Image);
		if( (pWnd == NULL) || (pWnd->m_hWnd == NULL) )
		{
			TRACE0("pointer of IDC_BUTTON_Preview_Image is NULL\n");
			break;
		}
		//��ȡһ֡
		cFrame = camera.QueryFrame();
		IplImage* newframe = cvCloneImage(cFrame);
		pDlg->ImageResize( newframe );
		pDlg->ImageShow( g_pTheImage, IDC_BUTTON_Preview_Image );

//		if (cvWaitKey(20) == 27) break;
		int nWaitKey = cvWaitKey(20);
		if (nWaitKey == 27)
		{
			TRACE0("press ESC\n");
			break;
		}
		cvReleaseImage( &newframe );
		Sleep(1);
	}

	// ��ͼ����������
	if( g_pTheImage )	
	{
		cvZero( g_pTheImage );
//		ImageShow( g_pTheImage, IDC_BUTTON_Preview_Image );
	}
	camera.CloseCamera();
	cvDestroyWindow("KYKY_Camera");
	SetEvent( g_hEvent );
	return 0;
}

void CPanelCamera::CamInit()
{
	// ��ȡ����ͷ��Ŀ
	int cam_count = CCameraDS::CameraCount();
	if( cam_count > 0 )
	{
		int nValidCam = 0;
		for(int i=0; i < cam_count; i++)
		{
			char camera_name[1024];  
			int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
			if( retval > 0 )
			{
				CString camstr;
				camstr.Format("%s",camera_name );
				if( camstr != "HD USB Camera" )
					continue;
				nValidCam++;
			}
		}
		if( nValidCam < 1 )
		{
			MessageBox( _T("has no camera"), _T("Camera") );
			return;
		}
		// ��ʼ�� ���� g_pTheImage
		CvSize ImgSize;
		ImgSize.height	= IMAGE_HEIGHT;
		ImgSize.width	= IMAGE_WIDTH;
		/////////////////////////////////////////////////////////////////
		// ��ʼ����Ƶ������ͷ��ʾѭ����ִ�к��жϣ�m_nCapture��
		g_pTheImage = cvCreateImage( ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );
		m_nCapture = 0;
		ResetEvent( g_hEvent );
		ResetEvent( g_hEventTeminate );

		g_hThread	= CreateThread( NULL, 0, CommThread, this, 0, &g_dwThread );
		if( g_hThread == NULL )
			return;
		CloseHandle(g_hThread);
		/////////////////////////////////////////////////////////////////
	}
	else
		MessageBox( _T("has no camera"), _T("Camera") );
}

void CPanelCamera::ImageShow( IplImage* img, UINT ID )	// ID ��Picture Control�ؼ���ID��
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// �����ʾ�ؼ��� DC
	HDC hDC = pDC ->GetSafeHdc();				// ��ȡ HDC(�豸���) �����л�ͼ����

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
// 	int rw = rect.right - rect.left;			// ���ͼƬ�ؼ��Ŀ�͸�
// 	int rh = rect.bottom - rect.top;
// 	int iw = img->width;						// ��ȡͼƬ�Ŀ�͸�
// 	int ih = img->height;
// 	int tx = (int)(rw - iw)/2;					// ʹͼƬ����ʾλ�������ڿؼ�������
// 	int ty = (int)(rh - ih)/2;
// 	SetRect( rect, tx, ty, tx+iw, ty+ih );

	CvvImage cimg;
	cimg.CopyOf( img );							// ����ͼƬ
	cimg.DrawToHDC( hDC, &rect );				// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	ReleaseDC( pDC );
}

void CPanelCamera::ImageResize(IplImage* img)
{
	// ���� g_pTheImage �� ROI ������������ͼƬ img
//	cvSetImageROI( g_pTheImage, cvRect( tlx, tly, nw, nh) );
	CRect rc;
	GetDlgItem(IDC_BUTTON_Preview_Image)->GetClientRect(&rc);
//	cvSetImageROI( g_pTheImage, cvRect( 0,0,w,h ) );

	// ��ͼƬ img �������ţ������뵽 g_pTheImage ��
	cvResize( img, g_pTheImage );

	// ���� g_pTheImage �� ROI ׼��������һ��ͼƬ
//	cvResetImageROI( m_gTheImage );
}

LRESULT CPanelCamera::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_EXITSIZEMOVE )
	{
		CRect rc;
		GetClientRect(&rc);
		GetDlgItem(IDC_BUTTON_Preview_Image)->SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
