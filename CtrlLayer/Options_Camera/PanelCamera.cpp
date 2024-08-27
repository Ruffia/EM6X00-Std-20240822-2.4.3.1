// PanelCamera.cpp : implementation file
//

// 1.下载OpenCV安装程序（1.0版本）。
//   假如要将OpenCV安装到C:\Program Files\OpenCV，在安装时选择"将   \OpenCV\bin加入系统变量"。
//   或安装完成后手动添加环境变量“C:\Program Files\OpenCV\bin”。
// 2.启动VC++6.0，菜单Tools->Options->Directories：
//   先设置lib路径，选择Library files，在下方填入路径：C:\Program Files\OpenCV\lib
//   然后选择include files，在下方填入路径：
//   C:\Program Files\OpenCV\cxcore\include
//   C:\Program Files\OpenCV\cv\include
//   C:\Program Files\OpenCV\cvaux\include
//   C:\Program Files\OpenCV\ml\include
//   C:\Program Files\OpenCV\otherlibs\highgui
//   C:\Program Files\OpenCV\otherlibs\cvcam\include
// 3.每创建一个将要使用OpenCV的VC Project，都需要给它指定需要的lib。
//   菜单：Project->Settings，然后将Setting for选为All Configurations，
//   然后选择右边的link标签，在Object/library modules附加上：
//   cxcore.lib cv.lib ml.lib cvaux.lib highgui.lib cvcam.lib
//   如果不需要这么多lib，你可以只添加你需要的lib。
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
	// 将对话框位置设置到扫描区域右侧
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
	// 创建1个摄像头实例
	CCameraDS camera;
	//打开第一个摄像头
	//if(! camera.OpenCamera(0, true)) //弹出属性选择窗口
	if( (! camera.OpenCamera(0, false, IMAGE_WIDTH,IMAGE_HEIGHT) ) ) //不弹出属性选择窗口，用代码制定图像宽和高
	{
		MessageBox( pDlg->m_hWnd, _T("Can not open camera.\n"), _T("Camera"), MB_OK );
		camera.CloseCamera();
		SetEvent( g_hEvent );
		return 0;
	}
	// 创建一个Ipl图像指针来读取摄像头的帧画面
	IplImage* cFrame;
	// 创建一个视频播放窗口
	cvNamedWindow("KYKY_Camera");
	// 将这个窗口缩至最小
	cvResizeWindow("KYKY_Camera",1,1);
	// 获取窗口句柄
	HWND hWnd = (HWND) cvGetWindowHandle("KYKY_Camera");
	// 获取该窗口的父窗口句柄
	HWND hParent = ::GetParent(hWnd);
	// 获取mymfc的GUI窗口句柄
//	HWND hwnd1=::FindWindow("CPanelCamera","Test4");
	// 将GUI窗口设置为视频播放窗口的父窗口，使视频在 GUI 的指定区域播放
//	::SetParent(hWnd, hwnd1);
	// 隐藏所创建的视频播放窗口
	::ShowWindow(hParent, SW_HIDE);
	// 对图像数据清零
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
		//获取一帧
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

	// 对图像数据清零
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
	// 获取摄像头数目
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
		// 初始化 创建 g_pTheImage
		CvSize ImgSize;
		ImgSize.height	= IMAGE_HEIGHT;
		ImgSize.width	= IMAGE_WIDTH;
		/////////////////////////////////////////////////////////////////
		// 初始化视频、摄像头显示循环的执行和中断（m_nCapture）
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

void CPanelCamera::ImageShow( IplImage* img, UINT ID )	// ID 是Picture Control控件的ID号
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC ->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
// 	int rw = rect.right - rect.left;			// 求出图片控件的宽和高
// 	int rh = rect.bottom - rect.top;
// 	int iw = img->width;						// 读取图片的宽和高
// 	int ih = img->height;
// 	int tx = (int)(rw - iw)/2;					// 使图片的显示位置正好在控件的正中
// 	int ty = (int)(rh - ih)/2;
// 	SetRect( rect, tx, ty, tx+iw, ty+ih );

	CvvImage cimg;
	cimg.CopyOf( img );							// 复制图片
	cimg.DrawToHDC( hDC, &rect );				// 将图片绘制到显示控件的指定区域内

	ReleaseDC( pDC );
}

void CPanelCamera::ImageResize(IplImage* img)
{
	// 设置 g_pTheImage 的 ROI 区域，用来存入图片 img
//	cvSetImageROI( g_pTheImage, cvRect( tlx, tly, nw, nh) );
	CRect rc;
	GetDlgItem(IDC_BUTTON_Preview_Image)->GetClientRect(&rc);
//	cvSetImageROI( g_pTheImage, cvRect( 0,0,w,h ) );

	// 对图片 img 进行缩放，并存入到 g_pTheImage 中
	cvResize( img, g_pTheImage );

	// 重置 g_pTheImage 的 ROI 准备读入下一幅图片
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
