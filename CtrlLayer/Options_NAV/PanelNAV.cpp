// PanelNAV.cpp : implementation file
#include "stdafx.h"
#include "PanelNAV.h"
#include "stdio.h"
#include <fstream>
#include <string>
#include <vector>
//#include "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <atlconv.h>
#include "DlgProgress.h"
#include "DlgInputSet.h"
#include "Calculator.h"
#include "Options_NAV.h"
#include "DlgPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HHOOK g_hook = NULL;

using namespace std;
using namespace cv;

/////////////////////////////////////////////////////////////////

double	g_CameraDist = 155.0;		//相机到样品杯距离 mm  暂时没用

int g_X=0;							//像素位置
int g_Y=0;							//这里等下进行一定量的变换,

double g_X1=0;						//坐标位置
double g_Y1=0;

cv::Mat g_srcImage;					//原始图像 
cv::Mat g_srcImageUndistort;		//原始图像校正后

cv::Mat	g_SmallImage;				//小图像用来显示
cv::Mat g_SmallImageUndistort;      //小图像用来显示校正后
cv::Mat	g_ShowImage;				//用于缩放，加十字等操作显示的图像
cv::Mat	g_NoCrossImage;				//一张没有标十字的图像

CPoint g_ptZoomPos;					//放大后显示区域左上角在放大图像中的位置

//用于选取坐标点的位置
int  cross_x = 0, cross_y = 0;		//十字中心位置
BOOL showCross = FALSE;				//是否显示十字

CRect g_DisplayPicRect;				//图片控件的尺寸

cv::Mat g_cameraMatrix = cv::Mat::eye(3,3,CV_64F);	//相机内参矩阵
cv::Mat g_distCoeffs = cv::Mat::zeros(5, 1, CV_64F);//畸变系数 k1 k2 p1 p2 k3
cv::Mat rvecs = Mat::zeros(3,1,CV_64FC1);			//旋转向量
cv::Mat tvecs = Mat::zeros(3,1,CV_64FC1);			//平移向量
cv::Mat rotM = Mat::eye(3,3,CV_64F);				//世界坐标下的相机旋转矩阵
cv::Mat rotT = Mat::eye(3,3,CV_64F);				//世界坐标下的相机平移矩阵

/////////////////////////////////////////////////////////////////

struct ParamData{
	CPanelNAV* m_pDemodlg;
	cv::Mat* m_Image;
};
ParamData* g_paramData;

// CPanelNAV dialog
CPanelNAV::CPanelNAV(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PANEL_NAV, pParent)
	, m_pcMyCamera(NULL)
	, m_bGrabbing(FALSE)
	, m_pGrabThread(NULL)	//m_hGrabThread(NULL)
	, m_bThreadState(FALSE)
	, m_pSaveImageBuf(NULL)
	, m_nSaveImageBufSize(0)	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
	m_pSaveImageBuf = nullptr;
	m_bSaveJpg = FALSE;
	m_bInitCamera = FALSE;
//	parent_wnd = pParent->m_hWnd;
//	_px = _py = 0;
//	_pz = 0;
	m_navPos.fPosX = m_navPos.fPosY = m_navPos.fPosZ = 0.0f;
	g_paramData			= NULL;
	m_brushPowerON.CreateSolidBrush(RGB(0,192,0));
	m_brushPowerOFF.CreateSolidBrush(RGB(128,128,64));

	m_bShowselfImg		= FALSE;

	m_bIsShowCenter		= FALSE;
	m_bNavPowerOn		= FALSE;
	m_bStageIsMoving	= FALSE;

	m_nMultiple = 1;
	m_bFirstGetStageMoved = FALSE;
}

CPanelNAV::~CPanelNAV()
{
	m_brushPowerON.DeleteObject();
	m_brushPowerOFF.DeleteObject();

	if( m_pdlgProgress != NULL )
	{
		delete m_pdlgProgress;
		m_pdlgProgress = NULL;
	}
	if( g_paramData != NULL )
	{
		delete g_paramData;
		g_paramData = NULL;
	}

	if(m_pGrabThread != NULL)
	{
		if(m_hExitEvent != NULL)
			SetEvent(m_hExitEvent);
		m_bThreadState = FALSE;
		WaitForSingleObject(m_pGrabThread->m_hThread, INFINITE);

		delete m_pGrabThread;
		m_pGrabThread = NULL;
	}
	if(m_hExitEvent != NULL)
	{
		::CloseHandle(m_hExitEvent);
	}
}

void LeftRotate(cv::Mat & image)
{
	cv::Mat temp;
	cv::transpose(image, temp);
	cv::flip(temp,image, 0);
}

void RightRotate(cv::Mat & image)
{
	cv::Mat temp;
	cv::transpose(image, temp);
	cv::flip(temp,image, 1);
}

void HorTrans(cv::Mat & image)
{
	cv::Mat temp(image);
	cv::flip(temp, image, 1);
}

void VerTrans(cv::Mat & image)
{
	cv::Mat temp(image);
	cv::flip(temp, image, 0);
}

void CPanelNAV::ShowImageFromPath(std::string path)
{
	cv::Mat img = cv::imread(path, 1);//cvLoadImage
	int tmp = 0;
	tmp = img.type();
	g_srcImage = img;
	cv::resize(img, g_SmallImage, cv::Size(m_rcImageInit.Width(), m_rcImageInit.Height()));
	//cv::imshow("src1", g_SmallImage);
	img.release();

	char exe_file[1024];
	GetModuleFileName(NULL, exe_file, 1024);
	(strrchr(exe_file, '\\'))[1] = 0;
	std::string config_path = exe_file;
	config_path += "NAV\\Config.xml";

	FileStorage fs1(config_path, FileStorage::READ);
	fs1["rotMMatrix"] >> rotM;
	fs1["rotTMatrix"] >> rotT;
	fs1["rvecs"] >> rvecs;
	fs1["tvecs"] >> tvecs;
	fs1["cameraMatrix"] >> g_cameraMatrix;
	fs1["distCoeffs"] >> g_distCoeffs;
	fs1.release();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//MessageBox(_T("读取参数完成!"), _T("提示"), MB_OK);

	cv::Size image_size = g_srcImage.size();
	cv::Mat view, rview, mapx, mapy;
	cv::Size imageSize;
	imageSize = g_srcImage.size();
	initUndistortRectifyMap(g_cameraMatrix, g_distCoeffs, cv::Mat(),
		getOptimalNewCameraMatrix(g_cameraMatrix, g_distCoeffs, imageSize, 1, imageSize, 0),
		imageSize, CV_32FC1, mapx, mapy);
	remap(g_srcImage, g_srcImageUndistort, mapx, mapy, cv::INTER_LINEAR);
	cv::resize(g_srcImageUndistort, g_SmallImageUndistort, cv::Size(m_rcImageInit.Width(), m_rcImageInit.Height()));

	//这里对图像进行旋转变化（2023.10 如图片与电镜所得一致，则屏蔽以下两句）
	//旋转
	//cv::transpose(g_SmallImageUndistort, g_SmallImageUndistort);
	//镜像
	//cv::flip(g_SmallImageUndistort,g_SmallImageUndistort, 1);

	/////////////////////////////////////////////
	//2024.02 读取设置选项文件
	{
		//char exe_file[1024];
		//GetModuleFileName(NULL, exe_file, 1024);
		//(strrchr(exe_file, '\\'))[1] = 0; 
		//std::string sexe_path = exe_file ;
		//sexe_path +=  "NAV\\nav.ini";

		int i;
		int nLeftRotateCount = 0;
		int nRightRotateCount = 0;
		int nHor = 0;
		int nVer = 0;
		nHor = m_dlgCameraSetting.m_navSets.imgShow.nHorizontal;
		nVer = m_dlgCameraSetting.m_navSets.imgShow.nVertical;
		nLeftRotateCount = m_dlgCameraSetting.m_navSets.imgShow.nLeftRotateCount;
		nRightRotateCount = m_dlgCameraSetting.m_navSets.imgShow.nRightRotateCount;
		if(nHor != 0)
		{
			//cv::Mat temp(g_SmallImageUndistort);
			//cv::flip(temp, g_SmallImageUndistort, 1);
			HorTrans(g_SmallImageUndistort);
		}
		if(nVer != 0)
		{
			//cv::Mat temp(g_SmallImageUndistort);
			//cv::flip(temp, g_SmallImageUndistort, 0);
			VerTrans(g_SmallImageUndistort);
		}
		nLeftRotateCount	= nLeftRotateCount % 4;
		nRightRotateCount	= nRightRotateCount % 4;
		GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
		ScreenToClient(&g_DisplayPicRect); 
		for(i = 0; i < nLeftRotateCount; i++)
		{
			LeftRotate(g_SmallImageUndistort);
			int width = g_DisplayPicRect.Width();
			int height = g_DisplayPicRect.Height();
			g_DisplayPicRect.right = g_DisplayPicRect.left + height;
			g_DisplayPicRect.bottom = g_DisplayPicRect.top + width;
			GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(g_DisplayPicRect);
		}
		for(i = 0; i < nRightRotateCount; i++)
		{
			RightRotate(g_SmallImageUndistort);
			int width = g_DisplayPicRect.Width();
			int height = g_DisplayPicRect.Height();
			g_DisplayPicRect.right = g_DisplayPicRect.left + height;
			g_DisplayPicRect.bottom = g_DisplayPicRect.top + width;
			GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(g_DisplayPicRect);
		}
	}
	/////////////////////////////////////////////

	cv::imshow("src1", g_SmallImageUndistort);

	std::string sexe_path = exe_file;
	sexe_path += "NAV\\undisworld.bmp";
	g_SmallImage.release();

	g_SmallImage = g_SmallImageUndistort;
	cv::imwrite(sexe_path, g_srcImageUndistort);

	g_ShowImage = g_SmallImage.clone();
	g_NoCrossImage = g_SmallImage.clone();
	m_bShowselfImg = TRUE;
}

void CPanelNAV::SaveImageToPath(std::string path, unsigned char* SaveBuffer)
{
	int nRet;
	nRet = SaveImage(MV_Image_Jpeg, path.c_str(), SaveBuffer);
	if (MV_OK != nRet)
	{
		ShowErrorMsg(TEXT("Save jpg fail"), nRet);
		return;
	}
}

void CPanelNAV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NAV_ShowMag, m_comboShowMag);
}

BEGIN_MESSAGE_MAP(CPanelNAV, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_NAV_GrabbingStart, OnBnClickedStartGrabbingButton)
	ON_BN_CLICKED(IDC_BUTTON_NAV_GrabbingStop, OnBnClickedStopGrabbingButton)
	ON_BN_CLICKED(IDC_BUTTON_NAV_SaveJPG, OnBnClickedSaveJpgButton)
	ON_BN_CLICKED(IDC_BUTTON_NAV_Navigate, OnBnClickedButtonNavigate)
	ON_BN_CLICKED(IDC_BUTTON_NAV_InitImage, OnBnClickedInitImage)
	ON_BN_CLICKED(IDC_BUTTON_NAV_ADV, OnBnClickedAdvanced)			
	ON_BN_CLICKED(IDC_BUTTON_NAV_OpenPower, OnBnClickedOpenPower)
	ON_BN_CLICKED(IDC_CHECK_TestStage, OnTestStage)
	// }}AFX_MSG_MAP
	// ON_BN_CLICKED(IDC_ENUM_BUTTON, &CPanelNAV::OnBnClickedEnumButton)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_SET_OPENIMAGE, OnSetOpenImage)
	ON_MESSAGE(WM_SET_READARGUMENT, OnSetReadArgument)
	ON_MESSAGE(WM_SET_WRITEWORLD, OnSetUndistor)
	ON_MESSAGE(WM_SET_CREATEWORLD, OnSetCreateWorld)
END_MESSAGE_MAP()

// ch:取流线程 | en:Grabbing thread
UINT  GrabThread(LPVOID lpram)
{
	return -1;
}

void GetPosition( double pixelx,double pixely)
{
	//从像素坐标系转换到世界坐标系
	cv::Mat uvPoint = cv::Mat::ones(3,1,cv::DataType<double>::type); //u,v,1

	// image point
	uvPoint.at<double>(0,0) = pixelx; 
	uvPoint.at<double>(1,0) = pixely;

	cv::Mat tempMat, tempMat2;
	double s, zConst = 0;
	tempMat = rotM.inv() * g_cameraMatrix.inv() * uvPoint;
	tempMat2 = rotM.inv() * tvecs;
	s = zConst + tempMat2.at<double>(2,0);
	s /= tempMat.at<double>(2,0);
	cv::Mat wcPoint = rotM.inv() * (s * g_cameraMatrix.inv() * uvPoint - tvecs);

	g_X1 = wcPoint.at<double>(0, 0);
	g_Y1 = wcPoint.at<double>(1, 0);
	//Point3f realPoint(wcPoint.at<double>(0, 0), wcPoint.at<double>(1, 0), wcPoint.at<double>(2, 0)); // point in world coordinates
}


void DrawCross(cv::Mat& image, cv::Point2i ptCenter)
{
	int length_r = 5;
	int size = 1;

	line(image, cvPoint(ptCenter.x,   ptCenter.y-length_r),cvPoint(ptCenter.x, ptCenter.y+length_r),CV_RGB(255,0,0),size,CV_AA,0);
	line(image, cvPoint(ptCenter.x-length_r,  ptCenter.y),cvPoint(ptCenter.x+length_r,ptCenter.y),CV_RGB(255,0,0),size,CV_AA,0);
}


void On_Mouse(int e, int x, int y, int flags, void* param)
{
	if( g_NoCrossImage.empty() )
		return;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	static BOOL  firstClick = TRUE;
	static int64 firstTime = 0; 
	ParamData* m_fParamData = (ParamData*)param;
	CPanelNAV *pdlg = m_fParamData->m_pDemodlg;
	if(pdlg->m_bShowselfImg)
	{
		if (e == cv::EVENT_LBUTTONUP)
		{
			cv::Mat& image = *m_fParamData->m_Image;

			//在选中点处绘制十字交叉线
			g_ShowImage.release();
			g_ShowImage = g_NoCrossImage.clone();
			showCross = TRUE;
			DrawCross(g_ShowImage, cv::Point2i(x, y));

			//在这里计算放大前的十字坐标
			CRect rect = pdlg->m_rcImageInit;
			if(pdlg->m_nMultiple > 1)
			{
				int zoomW = rect.Width() * pdlg->m_nMultiple;
				int zoomH = rect.Height() * pdlg->m_nMultiple;
				int xTemp = g_ptZoomPos.x + x;
				int yTemp = g_ptZoomPos.y + y;
				x = xTemp / pdlg->m_nMultiple;
				y = yTemp / pdlg->m_nMultiple;
			}

			cross_x = x;
			cross_y = y;

			///////////////////////////////////////////////
			//2024.03.17修改 旋转后坐标系计算
			//char exe_file[1024];
			//GetModuleFileName(NULL, exe_file, 1024);
			//(strrchr(exe_file, '\\'))[1] = 0; 
			//std::string sexe_path = exe_file ;
			//sexe_path +=  "NAV\\nav.ini";

			int i;
			int nLeftRotateCount = 0;
			int nRightRotateCount = 0;
			int nHor = 0;
			int nVer = 0;
			nHor = pdlg->m_dlgCameraSetting.m_navSets.imgShow.nHorizontal;
			nVer = pdlg->m_dlgCameraSetting.m_navSets.imgShow.nVertical;
			nLeftRotateCount = pdlg->m_dlgCameraSetting.m_navSets.imgShow.nLeftRotateCount;
			nRightRotateCount = pdlg->m_dlgCameraSetting.m_navSets.imgShow.nRightRotateCount;

			nLeftRotateCount	= nLeftRotateCount % 4;
			nRightRotateCount	= nRightRotateCount % 4;

			//因为m_rcShow一直是没旋转的数值，所以这里先计算当前的宽度和高度的数值
			int curWidth = rect.Width();
			int curHeight = rect.Height();
			for(i = 0; i < nLeftRotateCount; i++)
			{
				int newWidth = curHeight;
				int newHeight = curWidth;
				curWidth = newWidth;
				curHeight = newHeight;
			}
			for(i = 0; i < nRightRotateCount; i++)
			{
				int newWidth = curHeight;
				int newHeight = curWidth;
				curWidth = newWidth;
				curHeight = newHeight;
			}
			int tempWidth = curWidth;
			int tempHeight = curHeight;
			if(nHor != 0)
			{
				x = tempWidth - x;
			}
			if(nVer != 0)
			{
				y = tempHeight - y;
			}
			for(i = 0; i < nLeftRotateCount; i++)
			{
				//左旋后坐标的计算需要右旋来还原xy
				int srcX = x;
				int srcY = y;
				x = tempHeight - srcY;
				y = srcX;

				int newWidth = tempHeight;
				int newHeight = tempWidth;
				tempWidth = newWidth;
				tempHeight = newHeight;
			}
			for(i = 0; i < nRightRotateCount; i++)
			{
				//右旋后坐标的计算需要左旋来还原xy
				int srcX = x;
				int srcY = y;
				x = srcY;
				y = tempWidth - srcX;
				int newWidth = tempHeight;
				int newHeight = tempWidth;
				tempWidth = newWidth;
				tempHeight = newHeight;
			}
			///////////////////////////////////////////////
			g_X = x;
			g_Y = y;
 
			int iImgWidth = pdlg->m_dlgCameraSetting.m_navSets.camera.nCameraW;       
			int iImgZoomWidth1 =  (float)pdlg->m_rcDesk.Width()/(float)pdlg->m_dlgCameraSetting.m_navSets.camera.fZoom; 
			float zoom = (float)iImgWidth/(float)iImgZoomWidth1;

			// 2023.10
//			GetPosition( ( g_DisplayPicRect.Height()-g_Y)*zoom, (g_X) *zoom ); 
			GetPosition(g_X*zoom, g_Y* zoom); 

			g_X1 = g_X1*1000000;
			g_Y1 = g_Y1*1000000;
			g_X1 = (int)g_X1/1000000.0;
			g_Y1 = (int)g_Y1/1000000.0;
			imshow("src1",g_ShowImage);

			//判断是否双击
			if(pdlg->_GetCurrentTimeStamp()- firstTime > 500)
			{
				firstClick = TRUE;
			}
			if(firstClick)
			{
				firstTime =  pdlg->_GetCurrentTimeStamp();
				firstClick = FALSE;
			}
			else if(!firstClick)
			{
				if(pdlg->_GetCurrentTimeStamp()- firstTime <= 500 )
				{
					int nRet = 0;
					if(g_X1 < -41000 || g_X1 > 100000 || g_Y1 < -25700 || g_Y1 >60000)
					{
						pdlg->ShowErrorMsg(TEXT("移动超出范围"), nRet);
					}
					else
					{
						pdlg->ClickStageMove();
					}
				}
				firstClick = TRUE;
			}
		}
		pdlg->UpdateData(FALSE);
	}
}

LRESULT CALLBACK GetMsgHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG lpMsg = (LPMSG)lParam;

	if( nCode >= 0 && PM_REMOVE == wParam )
	{
		// Don't translate non-input events.
//		if( (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST) )
		if( lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_RETURN )
		{
//			if( IsDialogMessage(m_hWnd, lpMsg) )
			if( theApp.m_pPanelNAV != NULL && theApp.m_pPanelNAV->m_hWnd != NULL )
			{
				if( IsDialogMessage(theApp.m_pPanelNAV->m_hWnd, lpMsg) )
				{
					theApp.m_pPanelNAV->PreTranslateMessage(lpMsg);
					// The value returned from this hookproc is ignored, 
					// and it cannot be used to tell Windows the message
					// has been handled. To avoid further processing,
					// convert the message to WM_NULL before returning.
					lpMsg->message	= WM_NULL;
					lpMsg->lParam	= 0;
					lpMsg->wParam	= 0;
				}
			}
		}
	}
	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

// CPanelNAV message handlers
BOOL CPanelNAV::OnInitDialog()
{
	CDialog::OnInitDialog();	
	//pDemodlg=this;

	m_bInitCamera = FALSE;
	m_nCameraFlag = 0;
	m_bInitCameraRunning = FALSE;
	
	::GetWindowRect(::GetDesktopWindow(), &m_rcDesk);

	//ReSize();

	// ch:显示框初始化 | en:Display Window Initialization
	DisplayWindowInitial();            

	InitializeCriticalSection(&m_hSaveImageMux);
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_dlgCameraSetting.Create(IDD_CAMERA_SETTINGS, this);
	m_dlgCameraSetting.ConfigRead();
	char szBuf[255] = {0};
	sprintf_s( szBuf, "NAV: finish read OpenMove Params: [%.2f, %.2f, %.2f]\n",
		m_dlgCameraSetting.m_navSets.move.posStart.fPosX, m_dlgCameraSetting.m_navSets.move.posStart.fPosY, m_dlgCameraSetting.m_navSets.move.posStart.fPosZ );
	OutputDebugString(szBuf);
	::PostMessage(theApp.m_pPanelNAV->GetParent()->m_hWnd, WM_USER_NAV_Update, 15, (LPARAM)(&(m_dlgCameraSetting.m_navSets.move)));

	//创建进度条和输入框
	m_pdlgProgress = new CDlgProgress;
	m_pdlgProgress->Create(IDD_PROGRESS_CTRL, this);

	//初始化阶段调整图像显示框大小
	int iImgWidth	= m_dlgCameraSetting.m_navSets.camera.nCameraW;
	int iImgHeight	= m_dlgCameraSetting.m_navSets.camera.nCameraH;
	int iImgZoomWidth	= (float)m_rcDesk.Width()/m_dlgCameraSetting.m_navSets.camera.fZoom;	//m_rcDesk代表桌面大小
	int iImgZoomHeight	= ((float)iImgHeight/iImgWidth)*iImgZoomWidth;

	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect); // IDC_PICTURE 是 picture control 的 ID 
	ScreenToClient(&g_DisplayPicRect); 
	m_nDisplayLeft = g_DisplayPicRect.left;
	GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(m_nDisplayLeft, g_DisplayPicRect.top, iImgZoomWidth, iImgZoomHeight, TRUE); 
	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);                                                 
	m_rcImageInit = CRect(0,0,g_DisplayPicRect.Width(),g_DisplayPicRect.Height());

	//使用opencv嵌入式窗口
	cv::namedWindow("src1",CV_WINDOW_AUTOSIZE);//设置窗口名
	HWND hWndl = (HWND)cvGetWindowHandle("src1");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent1 = ::GetParent(hWndl);//GetParent函数一个指定子窗口的父窗口句柄
	::SetParent(hWndl, GetDlgItem(IDC_STATIC_NAV_Display)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);

	//读取一张背景图片
	char exe_file[1024];
	GetModuleFileName(NULL, exe_file, 1024);
	(strrchr(exe_file, '\\'))[1] = 0; 
	std::string bk_path = exe_file ;
	bk_path +=  "NAV\\bk.jpg";
	cv::Mat srcImg = cv::imread(bk_path);
	resize(srcImg, g_SmallImage, cv::Size(g_DisplayPicRect.Width(), g_DisplayPicRect.Height()));
	imshow("src1", g_SmallImage);

	g_paramData = new ParamData;
	g_paramData->m_Image = &g_SmallImage;
	g_paramData->m_pDemodlg = this;
	cv::setMouseCallback("src1", On_Mouse, (void*)g_paramData);	//关键内置函数
	srcImg.release();
	//m_rcShow = CRect(0,0, g_DisplayPicRect.Width(), g_DisplayPicRect.Height() );

	//确定下拉条的倍数选择
	m_nMultiple = 1;
	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	m_comboShowMag.AddString(TEXT("4x"));
	m_comboShowMag.AddString(TEXT("9x"));
	m_comboShowMag.AddString(TEXT("16x"));
	m_comboShowMag.AddString(TEXT("25x"));
	m_comboShowMag.AddString(TEXT("36x"));
	m_comboShowMag.AddString(TEXT("64x"));
	m_comboShowMag.AddString(TEXT("100x"));
	m_comboShowMag.SetCurSel(1);

	//创建相机进行图像采集工作
	m_hExitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);  
	m_bThreadState = TRUE;
	m_pGrabThread = AfxBeginThread(GrabThreadProcess, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL );	
	if (NULL == m_pGrabThread)
	{
		m_bThreadState = FALSE;
		ShowErrorMsg(TEXT("Create thread fail"), 0);   
	}

	// Place this code inside an initialization method in your implementation file (.cpp)
	g_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgHookProc, NULL, GetCurrentThreadId());
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CPanelNAV::ClearImageRect()
{
	//将图片显示区域清空为黑色
	char exe_file[1024];
	GetModuleFileName(NULL, exe_file, 1024);
	(strrchr(exe_file, '\\'))[1] = 0; 
	std::string bk_path = exe_file ;
	bk_path +=  "NAV\\bk.jpg";
	cv::Mat srcImg = cv::imread(bk_path);
	cv::Mat dstImg;
	resize(srcImg, dstImg, cv::Size(g_DisplayPicRect.Width(), g_DisplayPicRect.Height()));
	imshow("src1", dstImg);
	srcImg.release();
	dstImg.release();
}

void CPanelNAV::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CPanelNAV::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		{
			CDialog::OnPaint();			
		}

	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPanelNAV::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ch:按钮使能 | en:Enable control
void CPanelNAV::EnableControls(BOOL bIsCameraReady)
{
	//GetDlgItem(IDC_BUTTON_NAV_GrabbingStart)->EnableWindow(bIsCameraReady);
	//GetDlgItem(IDC_BUTTON_NAV_GrabbingStop)->EnableWindow(!bIsCameraReady);
// 	GetDlgItem(IDC_BUTTON_NAV_GrabbingStart)->EnableWindow( m_bNavPowerOn && !m_bGrabbing );
// 	GetDlgItem(IDC_BUTTON_NAV_GrabbingStop)->EnableWindow( m_bNavPowerOn && m_bGrabbing );
	GetDlgItem(IDC_BUTTON_NAV_GrabbingStart)->EnableWindow( !m_bGrabbing );
	GetDlgItem(IDC_BUTTON_NAV_GrabbingStop)->EnableWindow( m_bGrabbing );
	
	if(m_dlgCameraSetting.m_navSets.camera.nTriggerMode == 1)
	{
		GetDlgItem(IDC_BUTTON_NAV_SaveJPG)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NAV_SaveJPG)->EnableWindow(m_bGrabbing ? TRUE : FALSE);
	}

}

// ch:最开始时的窗口初始化 | en:Initial window initialization
void CPanelNAV::DisplayWindowInitial()
{
	CWnd *pWnd = GetDlgItem(IDC_STATIC_NAV_Display);
	if (pWnd)
	{
		m_hwndDisplay = pWnd->GetSafeHwnd();
		if (m_hwndDisplay)
			EnableControls(TRUE);
	}
}

// ch:显示错误信息 | en:Show error message
void CPanelNAV::ShowErrorMsg(CString csMessage, int nErrorNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString errorMsg;
	if (nErrorNum == 0)
	{
		errorMsg.Format(_T("%s"), csMessage);
	}
	else
	{
		errorMsg.Format(_T("%s: Error = %x: "), csMessage, nErrorNum);
	}

	switch(nErrorNum)
	{
	case MV_E_HANDLE:           errorMsg += "Error or invalid handle";	break;
	case MV_E_SUPPORT:          errorMsg += "Not supported function";	break;
	case MV_E_BUFOVER:          errorMsg += "Buffer overflow";		break;
	case MV_E_CALLORDER:        errorMsg += "Function calling order error"; break;
	case MV_E_PARAMETER:        errorMsg += "Incorrect parameter";	break;
	case MV_E_RESOURCE:         errorMsg += "Applying resource failed"; break;
	case MV_E_NODATA:           errorMsg += "No data";				break;
	case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed"; break;
	case MV_E_VERSION:          errorMsg += "Version mismatches";	break;
	case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory";	break;
	case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet"; break;
	case MV_E_UNKNOW:           errorMsg += "Unknown error";		break;
	case MV_E_GC_GENERIC:       errorMsg += "General error";		break;
//	case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";	break;
	case MV_E_ACCESS_DENIED:	errorMsg += "No permission";		break;
	case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected"; break;
	case MV_E_NETER:            errorMsg += "Network error";		break;
	}

//	MessageBox(errorMsg, TEXT("提示"), MB_OK | MB_ICONWARNING);
	CDlgPrompt dlg;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	dlg.m_byteType = 0x01;
	dlg.m_strMsg = "\r\n\r\n"+errorMsg;
	dlg.DoModal();
}

void  CPanelNAV::ChangeShowRectSize(BOOL islateral)
{
	//图像显示框大小
	int iImgWidth	= m_dlgCameraSetting.m_navSets.camera.nCameraW;
	int iImgHeight	= m_dlgCameraSetting.m_navSets.camera.nCameraH;
	int iImgZoomWidth	= (float)m_rcDesk.Width()/m_dlgCameraSetting.m_navSets.camera.fZoom; 
	int iImgZoomHeight	= ((float)iImgHeight/iImgWidth)*iImgZoomWidth;

	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect); // IDC_PICTURE 是 picture control 的 ID 
	ScreenToClient(&g_DisplayPicRect); 

	if(islateral)
	{
		GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(m_nDisplayLeft, g_DisplayPicRect.top, iImgZoomWidth, iImgZoomHeight, TRUE); 
		GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(m_nDisplayLeft + (iImgZoomWidth - iImgZoomHeight)/2, g_DisplayPicRect.top, iImgZoomHeight, iImgZoomWidth, TRUE); 
		GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
		//m_rcShow = CRect(0,0, g_DisplayPicRect.Width(), g_DisplayPicRect.Height() );
	}
}

 void  CPanelNAV::ReSize()
 {
	CRect recta;
	GetClientRect(&recta);     //取客户区大小  
	m_ptOld.x = recta.right - recta.left;  //m_ptOld是成员变量，用于保存上一次的界面尺寸
	m_ptOld.y = recta.bottom - recta.top;

	float fsp[2];
	POINT Newp; //现在对话框的大小

	GetClientRect(&recta);     //取客户区大小  
	Newp.x = (recta.right - recta.left)*((float)m_rcDesk.Width()/1920.0);
	Newp.y = (recta.bottom - recta.top)*((float)m_rcDesk.Height()/1080.0);

	SetWindowPos(NULL, recta.left,recta.top, Newp.x,Newp.y, SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOMOVE );

	fsp[0] = (float)Newp.x / m_ptOld.x;
	fsp[1] = (float)Newp.y / m_ptOld.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角
	CPoint OldBRPoint, BRPoint; //右下角
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件  
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
 
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x * fsp[0]);
		TLPoint.y = long(OldTLPoint.y * fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x * fsp[0]);
		BRPoint.y = long(OldBRPoint.y * fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
 
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);  //控件自适应
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	m_ptOld = Newp;
}



int CPanelNAV::SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType, CString imagePath, unsigned char* img_data)
{
	MV_SAVE_IMG_TO_FILE_PARAM stSaveFileParam;
	memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMG_TO_FILE_PARAM));
	EnterCriticalSection(&m_hSaveImageMux);
	if ( m_stImageInfo.enPixelType == 0)
	{
		LeaveCriticalSection(&m_hSaveImageMux);
		return MV_E_NODATA;
	}
	if(RemoveCustomPixelFormats(m_stImageInfo.enPixelType))
	{
		LeaveCriticalSection(&m_hSaveImageMux);
		return MV_E_SUPPORT;
	}

	stSaveFileParam.enImageType = enSaveImageType;				// ch:需要保存的图像类型 | en:Image format to save
	stSaveFileParam.enPixelType = m_stImageInfo.enPixelType;	// ch:相机对应的像素格式 | en:Camera pixel type
	stSaveFileParam.nWidth      = m_stImageInfo.nWidth;			// ch:相机对应的宽 | en:Width
	stSaveFileParam.nHeight     = m_stImageInfo.nHeight;		// ch:相机对应的高 | en:Height
	stSaveFileParam.nDataLen    = m_stImageInfo.nFrameLen;
	stSaveFileParam.pData = img_data;
	stSaveFileParam.iMethodValue = 0;

	// ch:jpg图像质量范围为(50-99], png图像质量范围为[0-9] | en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
	if (MV_Image_Bmp == stSaveFileParam.enImageType)
	{
		//sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.bmp", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
	}
	else if (MV_Image_Jpeg == stSaveFileParam.enImageType)
	{
		stSaveFileParam.nQuality = 80;
		//sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.jpg", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
	}
	else if (MV_Image_Tif == stSaveFileParam.enImageType)
	{
		//sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.tif", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
	}
	else if (MV_Image_Png == stSaveFileParam.enImageType)
	{
		stSaveFileParam.nQuality = 8;
		//sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.png", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
	}
	USES_CONVERSION;
	int len = WideCharToMultiByte(CP_ACP, 0,  A2W(imagePath), imagePath.GetLength(),NULL,0, NULL,NULL);
	WideCharToMultiByte(CP_ACP, 0, A2W(imagePath), imagePath.GetLength(),stSaveFileParam.pImagePath,len, NULL, NULL);
	int nRet = m_pcMyCamera->SaveImageToFile(&stSaveFileParam);
	LeaveCriticalSection(&m_hSaveImageMux);
	return nRet;
}

__int64 CPanelNAV::_GetCurrentTimeStamp()
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);	
	CTime curTime(sysTime);
	__int64	timeStamp = curTime.GetTime();
	__int64 curTimeStamp = timeStamp*1000 + sysTime.wMilliseconds;
	return curTimeStamp;
}

UINT CPanelNAV::GrabThreadProcess(LPVOID lpram)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPanelNAV* pThis = (CPanelNAV*)lpram;
	MV_FRAME_OUT stImageInfo = {0};
	MV_DISPLAY_FRAME_INFO stDisplayInfo = {0};
	int nRet = MV_OK;
	int nImageTick = 0;
	while(pThis->m_bThreadState)
	{	
		if(pThis->m_bGrabbing)
		{			
			nRet = pThis->m_pcMyCamera->GetImageBuffer(&stImageInfo, 1000);
			if (nRet == MV_OK)
			{			
				memcpy(&pThis->m_stImageInfo, &(stImageInfo.stFrameInfo), sizeof(MV_FRAME_OUT_INFO_EX));
				LeaveCriticalSection(&pThis->m_hSaveImageMux);
				//自定义格式不支持显示
				if(pThis->RemoveCustomPixelFormats(stImageInfo.stFrameInfo.enPixelType))
				{
					pThis->m_pcMyCamera->FreeImageBuffer(&stImageInfo);
					continue;
				}		

				nImageTick++;
				if( pThis->m_bSaveJpg)
				{
					if(pThis->m_pSaveImageBuf)
					{
						free(pThis->m_pSaveImageBuf);
					}
					pThis->m_pSaveImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * stImageInfo.stFrameInfo.nFrameLen);
					memcpy(pThis->m_pSaveImageBuf, stImageInfo.pBufAddr, stImageInfo.stFrameInfo.nFrameLen);
					pThis->m_bSaveJpg = FALSE;
				}
				else if ( (pThis->m_dlgCameraSetting.m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_ON)
						&& (pThis->m_dlgCameraSetting.m_navSets.camera.nTriggerSource == MV_TRIGGER_SOURCE_LINE0)
						&& ( (nImageTick % pThis->m_dlgCameraSetting.m_navSets.imgStorage.nTick) == 0))
				{
					if (pThis->m_pSaveImageBuf)
					{
						free(pThis->m_pSaveImageBuf);
						pThis->m_pSaveImageBuf = NULL;
					}
					pThis->m_pSaveImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * stImageInfo.stFrameInfo.nFrameLen);
					memcpy(pThis->m_pSaveImageBuf, stImageInfo.pBufAddr, stImageInfo.stFrameInfo.nFrameLen);
					pThis->m_pcMyCamera->FreeImageBuffer(&stImageInfo);

					CString str; //获取系统时间
					CTime tm;
					tm = CTime::GetCurrentTime();
					str = tm.Format("%Y%m%d%H%M%S");
					if (!PathIsDirectory(pThis->m_dlgCameraSetting.m_navSets.imgStorage.strPath))
					{
						CreateDirectory(pThis->m_dlgCameraSetting.m_navSets.imgStorage.strPath, 0);
					}
					CString save_jpg_path = pThis->m_dlgCameraSetting.m_navSets.imgStorage.strPath;
					save_jpg_path += "\\";
					save_jpg_path += str;
					save_jpg_path += ".jpg";
					pThis->SaveImageToPath(save_jpg_path.GetBuffer(), pThis->m_pSaveImageBuf);

					// 2023.10
//					pThis->ChangeShowRectSize(0);
					pThis->ChangeShowRectSize(1);
					pThis->ClearImageRect();

					//pThis->OnBnClickedStopGrabbingButton();
					pThis->ShowImageFromPath(save_jpg_path.GetBuffer());
					pThis->StopGrabbingFunc( FALSE );	// 自动完成导航图片获取
					
					goto StopGrab;
				}
				else if ( (pThis->m_dlgCameraSetting.m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_ON)
						&& (pThis->m_dlgCameraSetting.m_navSets.camera.nTriggerSource == MV_TRIGGER_SOURCE_SOFTWARE) )
				{
					if (pThis->m_pSaveImageBuf)
					{
						free(pThis->m_pSaveImageBuf);
						pThis->m_pSaveImageBuf = NULL;
					}
					pThis->m_pSaveImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * stImageInfo.stFrameInfo.nFrameLen);
					memcpy(pThis->m_pSaveImageBuf, stImageInfo.pBufAddr, stImageInfo.stFrameInfo.nFrameLen);
					pThis->m_pcMyCamera->FreeImageBuffer(&stImageInfo);

					CString str; //获取系统时间
					CTime tm;
					tm = CTime::GetCurrentTime();
					str = tm.Format("%Y%m%d%H%M%S");
					if (!PathIsDirectory(pThis->m_dlgCameraSetting.m_navSets.imgStorage.strPath))
					{
						CreateDirectory(pThis->m_dlgCameraSetting.m_navSets.imgStorage.strPath, 0);
					}
					CString save_jpg_path = pThis->m_dlgCameraSetting.m_navSets.imgStorage.strPath;
					save_jpg_path += "\\";
					save_jpg_path += str;
					save_jpg_path += ".jpg";
					pThis->SaveImageToPath(save_jpg_path.GetBuffer(), pThis->m_pSaveImageBuf);

					// 2023.10
//					pThis->ChangeShowRectSize(0);
					pThis->ChangeShowRectSize(1);
					pThis->ClearImageRect();

					//pThis->OnBnClickedStopGrabbingButton();
					pThis->ShowImageFromPath(save_jpg_path.GetBuffer());
					pThis->StopGrabbingFunc( FALSE );	// 自动完成导航图片获取
					
					goto StopGrab;
/*
					stDisplayInfo.hWnd = pThis->m_hwndDisplay;		
					stDisplayInfo.pData = stImageInfo.pBufAddr;
					stDisplayInfo.nDataLen = stImageInfo.stFrameInfo.nFrameLen;
					stDisplayInfo.nWidth = stImageInfo.stFrameInfo.nWidth;
					stDisplayInfo.nHeight = stImageInfo.stFrameInfo.nHeight;
					stDisplayInfo.enPixelType = stImageInfo.stFrameInfo.enPixelType;
					pThis->m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);*/
				}
				if(pThis->m_dlgCameraSetting.m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_OFF)
				{
					// MV_TRIGGER_MODE_OFF：内触发模式（见用户手册7.1.1触发模式，相机通过设备内部给出的信号采集图像）
					//			实际为连续触发的视频方式，用于校准。在图片中间画十字
					stDisplayInfo.hWnd = pThis->m_hwndDisplay;		
					stDisplayInfo.pData = stImageInfo.pBufAddr;
					stDisplayInfo.nDataLen = stImageInfo.stFrameInfo.nFrameLen;
					stDisplayInfo.nWidth = stImageInfo.stFrameInfo.nWidth;
					stDisplayInfo.nHeight = stImageInfo.stFrameInfo.nHeight;
					stDisplayInfo.enPixelType = stImageInfo.stFrameInfo.enPixelType;
					pThis->m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);
					if(pThis->m_bIsShowCenter)
					{	
						CRect _rect;
						pThis->GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&_rect);
						pThis->ScreenToClient( &_rect);
						CWnd *m_pDrawWnd = pThis->GetDlgItem(IDC_STATIC_NAV_Display);			
						CClientDC pDC (m_pDrawWnd);
						CPen pen(PS_SOLID, 2, RGB(255,0,0)); //ÔÚÕâÀïÉèÖÃÏßÐÍ,Ïß¿í¼°ÑÕÉ«
						CPen* pOldPen;	
						pOldPen = pDC.SelectObject (&pen);
						CPoint _center((_rect.right - _rect.left)/2, (_rect.bottom - _rect.top)/2);
						int len_r = 5;
						pDC.MoveTo(_center.x - len_r,  _center.y );
						pDC.LineTo(_center.x + len_r,  _center.y);				
						pDC.MoveTo(_center.x ,  _center.y - len_r );
						pDC.LineTo(_center.x,  _center.y + len_r);
						pDC.SelectObject (&pOldPen);
					}
				}
				pThis->m_pcMyCamera->FreeImageBuffer(&stImageInfo);
			}
		}
		StopGrab:

		Sleep(10);
	}
	return 0;
}


UINT _cdecl InitThreadProgress(LPVOID lpvoid)
{
	CPanelNAV* pDlg = (CPanelNAV*) lpvoid;
	pDlg->m_bInitCameraRunning = TRUE;

	BOOL bFlag = pDlg->CameraPrepare();
	pDlg->m_pdlgProgress->ShowWindow(SW_HIDE);

	if(!bFlag)
	{
//		::PostMessage(theApp.m_pPanelNAV->GetParent()->m_hWnd, WM_USER_NAV_Update, 14, 1);
		pDlg->UpdateNAVSwitch(FALSE);
	}

	pDlg->m_bInitCameraRunning = FALSE;
	return 0;
}

void CPanelNAV::CallCameraInit()
{
	if(m_bInitCameraRunning)
	{
//		AfxMessageBox("camera is running");
		CDlgPrompt dlg;
		strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n\r\n\r\n";
		char szBuf[255];
		if(GetPrivateProfileString("Message", "CameraIsRun", "", szBuf, 255, m_cPathLocalize) != 0)
			dlg.m_strMsg += szBuf;
		dlg.DoModal();
		return;
	}
	m_nCameraFlag = 1;
	m_bInitCamera = FALSE;
	m_bGrabbing = FALSE;
	m_bStageIsMoving = TRUE;
//	::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 14, 0);
	NotifyStageMove(m_dlgCameraSetting.m_navSets.move.posStart.fPosX, m_dlgCameraSetting.m_navSets.move.posStart.fPosY, m_dlgCameraSetting.m_navSets.move.posStart.fPosZ);
	//m_bStageIsMoving = TRUE;
	CWinThread *pThread = AfxBeginThread(InitThreadProgress, (LPVOID)this, THREAD_PRIORITY_NORMAL , 0 , 0 , NULL);
}

BOOL CPanelNAV::CameraPrepare()
{
	if(!m_bInitCamera)
	{
		if( !m_bNavPowerOn )
		{
			//开启灯板电源
			UpdateNAVSwitch(TRUE);
		}

		memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
		int i, nRet = 0;

		char szBuf[255];
		CString strCap = "Prompt";
		if(GetPrivateProfileString("PromptDlg", "Cap", "", szBuf, 255, m_cPathLocalize) != 0)
			strCap = szBuf;
		CString strPrompt = _T("The Camera is Preparing..., please don't move stage");
		if(GetPrivateProfileString("Message", "CameraIsInit", "", szBuf, 255, m_cPathLocalize) != 0)
			strPrompt = szBuf;

		m_pdlgProgress->ShowWindow(SW_SHOW);
		m_pdlgProgress->m_progressCtrl.ShowWindow(SW_SHOW);
		m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
		m_pdlgProgress->SetWindowText(strCap);
		m_pdlgProgress->m_progressCtrl.SetRange(0, 100);
		m_pdlgProgress->m_progressCtrl.SetPos(0);
		m_pdlgProgress->m_bIsCancelled = FALSE;
		//这里设置相机初始化时间,为100*200ms
		for (i = 0; i < 100; i++)
		{
			if(m_pdlgProgress->m_bIsCancelled)
				return FALSE;

			// ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
			nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
			if (MV_OK == nRet && 0 != m_stDevList.nDeviceNum ){
				if(i < 50)
					m_pdlgProgress->m_progressCtrl.SetPos(50);
				strPrompt = _T("The Camera has been found");
				if(GetPrivateProfileString("Message", "CameraFound", "", szBuf, 255, m_cPathLocalize) != 0)
					strPrompt = szBuf;
				m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
				break;
			}

			m_pdlgProgress->m_progressCtrl.OffsetPos(1);
			Sleep(290);
		}

		//初始化相机
		CString strMsg;
		if (MV_OK != nRet)
		{
			if(MV_E_RESOURCE == nRet)
				ShowErrorMsg(TEXT("Resource allocation failure"), 0);
			return FALSE;
		}
		if (0 == m_stDevList.nDeviceNum)
		{
			ShowErrorMsg(TEXT("No device"), 0);
			return FALSE;
		}

		if (NULL != m_pcMyCamera)
		{
			delete m_pcMyCamera;
			m_pcMyCamera = NULL;
			//return FALSE;
		}

		// ch:由设备信息创建设备实例 | en:Device instance created by device information
		if (NULL == m_stDevList.pDeviceInfo[0])
		{
			ShowErrorMsg(TEXT("Device does not exist"), 0);
			return FALSE;
		}

		m_pcMyCamera = new CMvCamera;
		if (NULL == m_pcMyCamera)
		{
			ShowErrorMsg(TEXT("create camera object failed"), 0);
			return FALSE;
		}

		nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[0]);
		if (MV_OK != nRet)
		{
			delete m_pcMyCamera;
			m_pcMyCamera = NULL;
			ShowErrorMsg(TEXT("Open Fail"), nRet);
			return FALSE;
		}
		strPrompt = _T("Successfully opened the camera");
		if(GetPrivateProfileString("Message", "CameraIsOpen", "", szBuf, 255, m_cPathLocalize) != 0)
			strPrompt = szBuf;
		m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
		m_pdlgProgress->m_progressCtrl.OffsetPos(10);
		m_dlgCameraSetting.m_pcMyCamera = m_pcMyCamera;
		m_dlgCameraSetting.dlg = this;
		m_dlgCameraSetting.InitCameraSet();
		m_pdlgProgress->m_progressCtrl.OffsetPos(10);
		// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
		if (m_stDevList.pDeviceInfo[0]->nTLayerType == MV_GIGE_DEVICE)
		{
			unsigned int nPacketSize = 0;
			nRet = m_pcMyCamera->GetOptimalPacketSize(&nPacketSize);
			if (nRet == MV_OK)
			{
				nRet = m_pcMyCamera->SetIntValue("GevSCPSPacketSize", nPacketSize);
				if (nRet != MV_OK)
				{
					ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
				}
			}
			else
			{
				ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nRet);
			}
		}
		if (MV_OK != nRet)
		{
			//return;
		}
		strPrompt = _T("Camera parameters have been set");
		if(GetPrivateProfileString("Message", "CameraSetParams", "", szBuf, 255, m_cPathLocalize) != 0)
			strPrompt = szBuf;
		m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
		m_pdlgProgress->m_progressCtrl.OffsetPos(10);
		memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
		nRet = m_pcMyCamera->StartGrabbing();
		if (MV_OK != nRet)
		{
			// m_bThreadState = FALSE;
			ShowErrorMsg(TEXT("Start grabbing fail"), nRet);
			return FALSE;
		}

		//在这里判断位置移动是否完成
		strPrompt = _T("Waiting for the stage to be in place...");
		if(GetPrivateProfileString("Message", "CameraWaitForStage", "", szBuf, 255, m_cPathLocalize) != 0)
			strPrompt = szBuf;
		m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
		DWORD dwTickCount = GetTickCount();
		while(m_bStageIsMoving)
		{
			if( GetTickCount() -dwTickCount > 1000*60 )	// 如果1分钟台子仍未到位，跳出
				break;
			Sleep(100);
		}

		if( m_bStageIsMoving )
		{
			// 超时台子仍未到位，则弹窗提示
			CDlgPrompt dlg;
			strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
			dlg.m_byteType = 0x01;
			dlg.m_strMsg = "\r\n\r\n\r\n";
			char szBuf[255];
			if(GetPrivateProfileString("Message", "StageNotInPlace", "", szBuf, 255, m_cPathLocalize) != 0)
				dlg.m_strMsg += szBuf;
			dlg.DoModal();
		}
		else
		{
			strPrompt = _T("The Stage is in place now.");
			if(GetPrivateProfileString("Message", "StageInPlace", "", szBuf, 255, m_cPathLocalize) != 0)
				strPrompt = szBuf;
			m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
			m_bInitCamera = TRUE;
		}

		m_pdlgProgress->m_progressCtrl.SetPos(100);
		m_pdlgProgress->m_progressCtrl.ShowWindow(SW_HIDE);
		m_pdlgProgress->ShowWindow(SW_HIDE);
	}
	if(m_nCameraFlag == 0 || m_bInitCamera)
	{
/*		//设置成外触发模式
		//MV_TRIGGER_SOURCE_LINE0, 0
		int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_dlgCameraSetting.m_navSets.camera.nTriggerSource);
		//0：Rising Edge  1：Falling Edge 2：LevelHigh 3：LevelLow
		m_pcMyCamera->SetEnumValue("TriggerActivation", m_dlgCameraSetting.m_navSets.camera.nTriggerActivate);

		m_bGrabbing = TRUE;
	
		EnableControls(FALSE);
		GetDlgItem(IDC_BUTTON_NAV_InitImage)->EnableWindow(FALSE);
*/
		int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_dlgCameraSetting.m_navSets.camera.nTriggerSource);
		if (MV_OK != nRet)
		{
			ShowErrorMsg(TEXT("Set TriggerSource Fail"), nRet);
			return FALSE;
		}
		nRet = m_pcMyCamera->SetEnumValue("TriggerMode", m_dlgCameraSetting.m_navSets.camera.nTriggerMode);
		if (MV_OK != nRet)
		{
			ShowErrorMsg(TEXT("Set TriggerMode Fail"), nRet);
			return FALSE;
		}
		if( m_dlgCameraSetting.m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_ON
			&& m_dlgCameraSetting.m_navSets.camera.nTriggerSource == MV_TRIGGER_SOURCE_LINE0 )
		{
			//0：Rising Edge  1：Falling Edge 2：LevelHigh 3：LevelLow
			nRet = m_pcMyCamera->SetEnumValue("TriggerActivation", m_dlgCameraSetting.m_navSets.camera.nTriggerActivate);
		}

		Sleep(200);
		m_bGrabbing = TRUE;
		Sleep(200);
		EnableControls(FALSE);
		GetDlgItem(IDC_BUTTON_NAV_InitImage)->EnableWindow(FALSE);

		if( m_dlgCameraSetting.m_navSets.camera.nTriggerMode == MV_TRIGGER_MODE_ON
			&& m_dlgCameraSetting.m_navSets.camera.nTriggerSource == MV_TRIGGER_SOURCE_SOFTWARE )
		{
			// 软触发一次
			nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");
		}
	}
	m_bInitCamera = TRUE;
	return TRUE;
}



// ch:按下开始采集按钮 | en:Click Start button
void CPanelNAV::OnBnClickedStartGrabbingButton()
{
	if(m_bInitCameraRunning)
	{
//		AfxMessageBox("camera is running");
		CDlgPrompt dlg;
		strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n\r\n\r\n";
		char szBuf[255];
		if(GetPrivateProfileString("Message", "CameraIsRun", "", szBuf, 255, m_cPathLocalize) != 0)
			dlg.m_strMsg += szBuf;
		dlg.DoModal();
		return;
	}

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( m_bV1Status )
	{
//		MessageBox("请先关闭V1阀!","");
		CDlgPrompt dlg;
		strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n\r\n\r\n";
		char szBuf[255];
		if(GetPrivateProfileString("Message", "CloseV1First", "", szBuf, 255, m_cPathLocalize) != 0)
			dlg.m_strMsg += szBuf;
		dlg.DoModal();
		return;
	}

	if(!m_bNavPowerOn)
	{
		m_bGrabbing = FALSE;
		m_nCameraFlag = 0;
		m_bInitCamera = FALSE;
		m_bStageIsMoving = TRUE;
		UpdateNAVSwitch(TRUE);
		NotifyStageMove(m_dlgCameraSetting.m_navSets.move.posStart.fPosX, m_dlgCameraSetting.m_navSets.move.posStart.fPosY, m_dlgCameraSetting.m_navSets.move.posStart.fPosZ);
	}
	CWinThread *pThread = AfxBeginThread(InitThreadProgress, (LPVOID)this, THREAD_PRIORITY_NORMAL , 0 , 0 , NULL);

	int iImgWidth	= m_dlgCameraSetting.m_navSets.camera.nCameraW;
	int iImgHeight	= m_dlgCameraSetting.m_navSets.camera.nCameraH;
	int iImgZoomWidth	= (float)m_rcDesk.Width()/m_dlgCameraSetting.m_navSets.camera.fZoom; 
	int iImgZoomHeight	= ((float)iImgHeight/iImgWidth)*iImgZoomWidth;

	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect); // IDC_PICTURE 是 picture control 的 ID
	ScreenToClient(&g_DisplayPicRect);
	// 2023.10
	GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(m_nDisplayLeft, g_DisplayPicRect.top, iImgZoomWidth, iImgZoomHeight, TRUE); 
	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
	ClearImageRect();
}

// ch:按下结束采集按钮 | en:Click Stop button
void CPanelNAV::OnBnClickedStopGrabbingButton()
{
	StopGrabbingFunc( TRUE );
}

void CPanelNAV::StopGrabbingFunc( BOOL bClear )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_bInitCamera)
	{
		m_bGrabbing = FALSE;
		int nRet = m_pcMyCamera->StopGrabbing();
		m_pcMyCamera->Close();
		delete m_pcMyCamera;
		m_pcMyCamera = NULL;
		m_dlgCameraSetting.m_pcMyCamera = m_pcMyCamera;

		m_nSaveImageBufSize = 0;
		m_bInitCamera = FALSE;
		if( bClear )	// Clear为TRUE，即主动点击“结束采集”按钮，需清除图像显示区域
			ClearImageRect();
	}
	BOOL bClose = FALSE;
	if( bClear )
	{
		::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 16, 0);
		bClose = TRUE;
	}
	else
	{
		if( m_dlgCameraSetting.m_navSets.move.nProcess < 1 )
		{
			CDlgPrompt dlg;
			strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
			dlg.m_byteType = 0x04;
			dlg.m_strMsg = "\r\n\r\n\r\n";
			char szBuf[255];
			if(GetPrivateProfileString("Message", "ReturnToCenter", "", szBuf, 255, m_cPathLocalize) != 0)
				dlg.m_strMsg += szBuf;
			if(dlg.DoModal() == IDYES)
			{
				bClose = TRUE;
				::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 16, 0);
			}
		}
		else if( m_dlgCameraSetting.m_navSets.move.nProcess < 2 )
		{
			// 弹出是否回扫描原点进度条（按钮为“取消”），10秒钟进度条走完；没有操作则回中心
			//							用户点击了“取消”，可再次单击导航面板上的“开始采集”按钮，重新拍导航图片
			char szBuf[255];
			CString strCap = "Prompt";
			if(GetPrivateProfileString("PromptDlg", "Cap", "", szBuf, 255, m_cPathLocalize) != 0)
				strCap = szBuf;
			CString strPrompt = _T("return to center?");
			if(GetPrivateProfileString("Message", "ReturnToCenter", "", szBuf, 255, m_cPathLocalize) != 0)
				strPrompt = szBuf;

			m_pdlgProgress->ShowWindow(SW_SHOW);
			m_pdlgProgress->m_progressCtrl.ShowWindow(SW_SHOW);
			m_pdlgProgress->m_staticPrompt.SetWindowText(strPrompt);
			m_pdlgProgress->SetWindowText(strCap);
			m_pdlgProgress->m_progressCtrl.SetRange(0, 1000);
			m_pdlgProgress->m_progressCtrl.SetPos(0);
			m_pdlgProgress->m_bIsCancelled = FALSE;
			for( int i=0; i<1000; i++ )
			{
				if( m_pdlgProgress->m_bIsCancelled )
					break;
				Sleep(10);
				m_pdlgProgress->m_progressCtrl.OffsetPos(1);
			}
			if( !(m_pdlgProgress->m_bIsCancelled) )
			{
				bClose = TRUE;
				::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 16, 0);
			}
			m_pdlgProgress->m_progressCtrl.SetPos(100);
			m_pdlgProgress->m_progressCtrl.ShowWindow(SW_HIDE);
			m_pdlgProgress->ShowWindow(SW_HIDE);
		}
	}

	if( bClose )
	{
		//关闭灯板电源
		UpdateNAVSwitch(FALSE);
	}
	EnableControls(TRUE);
	GetDlgItem(IDC_BUTTON_NAV_InitImage)->EnableWindow(TRUE);
}


// ch:按下保存jpg图片按钮 | en:Click Save JPG button
void CPanelNAV::OnBnClickedSaveJpgButton()
{
	int iImgWidth	= m_dlgCameraSetting.m_navSets.camera.nCameraW;
	int iImgHeight	= m_dlgCameraSetting.m_navSets.camera.nCameraH;
	int iImgZoomWidth	= (float)m_rcDesk.Width()/m_dlgCameraSetting.m_navSets.camera.fZoom; 
	int iImgZoomHeight	= ((float)iImgHeight/iImgWidth)*iImgZoomWidth;

	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect); // IDC_PICTURE 是 picture control 的 ID
	ScreenToClient(&g_DisplayPicRect); 
	//GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(g_DisplayPicRect.left + (iImgZoomWidth - iImgZoomHeight)/2, g_DisplayPicRect.top, iImgZoomHeight,  iImgZoomWidth, TRUE); 
	//GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
	//m_rcShow = CRect(0,0, g_DisplayPicRect.Width(), g_DisplayPicRect.Height() );
	ClearImageRect();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_bSaveJpg = TRUE;
	char szFilter[] = "JPG file|*.jpg||";
	CFileDialog openFileDlg(FALSE, "JPG", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, NULL);
	if( openFileDlg.DoModal() != IDOK )
		return;
	CString filePath = openFileDlg.GetPathName();
	SaveImageToPath(filePath.GetBuffer(), m_pSaveImageBuf);
	ShowImageFromPath(filePath.GetBuffer());
	OnBnClickedStopGrabbingButton();
}

// ch:右上角退出 | en:Exit from upper right corner
void CPanelNAV::OnClose()
{ 
//	OnBnClickedStopGrabbingButton();
	// Finally, when your application is shutting down or when you no longer need the hook, unhook the procedure.
	UnhookWindowsHookEx(g_hook);
	CDialog::OnClose();
}

BOOL CPanelNAV::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPanelNAV::RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat)
{
	int nResult = enPixelFormat & MV_GVSP_PIX_CUSTOM;
	if(MV_GVSP_PIX_CUSTOM == nResult)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CPanelNAV::UpdateNAVSwitch( BOOL bHVOn, BOOL bAction )
{
	m_bNavPowerOn = bHVOn;
	GetDlgItem(IDC_STATIC_POWER_STATUS)->Invalidate();
	EnableControls( m_bNavPowerOn );
	if( bAction )
		BOOL success  = ::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 11, m_bNavPowerOn);
}

void CPanelNAV::NotifyStageMove(float fX, float fY, float fZ)
{
/*	_px = _x;
	_py = _y;
	if(_px > 85000000 || _px < -8500000 || _py > 8500000 || _py < -8500000)
	{
		ShowErrorMsg(TEXT("move  is out range"), 0);
		return;
	}
	BOOL success  = ::PostMessage(parent_wnd, WM_CONTROL_STAGE_MOVE, 0, 0);*/
/*	BOOL b = ::SendMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 18, (LPARAM)(&m_navPosRecv));
	float fDeltaX = abs(m_navPosRecv.fPosX - m_navPosLast.fPosX);
	float fDeltaY = abs(m_navPosRecv.fPosY - m_navPosLast.fPosY);
	if(m_bFirstGetStageMoved)
	{
		if (fDeltaX > m_dlgCameraSetting.m_fDiffPos || fDeltaY > m_dlgCameraSetting.m_fDiffPos)
		{
			CDlgPrompt dlg;
			strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
			dlg.m_byteType = 0x01;
			dlg.m_strMsg = "\r\n\r\n\r\n";
			char szBuf[255];
			if(GetPrivateProfileString("Message", "WaitForNext", "", szBuf, 255, m_cPathLocalize) != 0)
				dlg.m_strMsg += szBuf;
			dlg.DoModal();
			return;
		}
	}*/
	m_navPos.fPosX = fX;
	m_navPos.fPosY = fY;
	m_navPos.fPosZ = fZ;
	if(m_navPos.fPosX > 85000000 || m_navPos.fPosX < -8500000 || m_navPos.fPosY > 8500000 || m_navPos.fPosY < -8500000)
	{
		ShowErrorMsg(TEXT("move is out range"), 0);
		return;
	}
	char szBuf[255] = {0};
	sprintf_s( szBuf, "NAV: NotifyStageMove: [%.2f, %.2f, %.2f]",	m_navPos.fPosX, m_navPos.fPosY, m_navPos.fPosZ );
	OutputDebugString(szBuf);
	BOOL success  = ::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 12, (LPARAM)(&m_navPos));
	m_bFirstGetStageMoved = TRUE;
	m_navPosLast.fPosX = m_navPos.fPosX;
	m_navPosLast.fPosY = m_navPos.fPosY;
	m_navPosLast.fPosZ = m_navPos.fPosZ;
}

void  CPanelNAV::ClickStageMove()
{
	char szBuf[255] = {0};
	sprintf_s( szBuf, "NAV: ClickStageMove: ExpX = %s, ExpY = %s", m_dlgCameraSetting.m_navSets.move.strExpressionX, m_dlgCameraSetting.m_navSets.move.strExpressionY );
	OutputDebugString(szBuf);

	m_navPosLast = m_navPos;
	//ÕâÀïÉèÖÃÒ»Ôª¶þ´Î·½³Ì£¬ÊÇÓÃ»§µã»÷Í¼Æ¬×øÑ¡ÔñÎ»ÖÃÒÆ¶¯µÄÎ»ÖÃ£¬µ¥Î»ÊÇmm
	string m_strExpressionX = m_dlgCameraSetting.m_navSets.move.strExpressionX;
	while (m_strExpressionX.find("g_X1") != string::npos)
	{
		int pos = m_strExpressionX.find("g_X1");	
		m_strExpressionX.replace(m_strExpressionX.begin() + pos, m_strExpressionX.begin() + pos + 4, "(" + to_string(g_X1) + ")");
		
	}
	while (m_strExpressionX.find("g_Y1") != string::npos)
	{
		int pos = m_strExpressionX.find("g_Y1");
		m_strExpressionX.replace(m_strExpressionX.begin() + pos, m_strExpressionX.begin() + pos + 4, "(" +to_string(g_Y1) + ")");
	}

	string m_strExpressionY = m_dlgCameraSetting.m_navSets.move.strExpressionY;
	while (m_strExpressionY.find("g_Y1") != string::npos)
	{
		int pos = m_strExpressionY.find("g_Y1");
		m_strExpressionY.replace(m_strExpressionY.begin() + pos, m_strExpressionY.begin() + pos + 4, "(" +to_string(g_Y1) + ")");
	}
	while (m_strExpressionY.find("g_X1") != string::npos)
	{
		int pos = m_strExpressionY.find("g_X1");
		m_strExpressionY.replace(m_strExpressionY.begin() + pos, m_strExpressionY.begin() + pos + 4, "(" +to_string(g_X1) + ")");
	}
	sprintf_s( szBuf, "NAV: ClickStageMove: g_X1 = %.6f, g_Y1 = %.6f, ExpX2 = %s, ExpY2 = %s", g_X1, g_Y1, m_strExpressionX.c_str(), m_strExpressionY.c_str());
	OutputDebugString(szBuf);

	Calculator calX;
	calX.infix = m_strExpressionX;
	calX.calculate();

	Calculator calY;
	calY.infix = m_strExpressionY;
	calY.calculate();

	NotifyStageMove( calX.getResult(), -calY.getResult(), -1000 );	//默认Y反号
}

void CPanelNAV::NotifyPos( int nIndex )
{
	// 通知主程序获取当前位置值，再传递给导航模块，作为导航原点的位置值
	// 0为XY，1为Z
//	BOOL success  = ::PostMessage(parent_wnd, WM_WRITE_STARTXY_TOPATH, 0, 0);
	BOOL success  = ::PostMessage(GetParent()->m_hWnd, WM_USER_NAV_Update, 13, nIndex );
}

void CPanelNAV::UpdatePos( float fX, float fY )
{
	// 将主程序传递过来的位置值，更新导航模块的参数，并写入ini
	if( fX < -200.0 )
	{
		// 此时，入参实际是样品台Z参数值
		m_dlgCameraSetting.m_navSets.move.posStart.fPosZ = (int)fY;
		m_dlgCameraSetting.ConfigSave(2);
	}
	else
	{
		m_dlgCameraSetting.m_navSets.move.posStart.fPosX = fX;
		m_dlgCameraSetting.m_navSets.move.posStart.fPosY = fY;
		m_dlgCameraSetting.ConfigSave(1);
	}
}

BOOL CPanelNAV::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_bShowselfImg)
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&rect);
		cv::Rect imageShowRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		cv::Point p(pt.x, pt.y);	
		if (imageShowRect.contains(p) && !g_SmallImage.empty())
		{
			int lx = pt.x - rect.left;
			int ty = pt.y - rect.top;	// 得到鼠标相对相框左上角坐标
			int rx = rect.Width() - lx;
			int by = rect.Height() - ty;
			/*if(m_comboShowMag.GetCurSel() == 0)
			{
				m_nMultiple = 2;
			}
			else if(m_comboShowMag.GetCurSel() == 1)
			{
				m_nMultiple = 3;
			}
			else if(m_comboShowMag.GetCurSel() == 2)
			{
				m_nMultiple = 4;
			}
			else if(m_comboShowMag.GetCurSel() == 3)
			{
				m_nMultiple = 5;
			}
			else if(m_comboShowMag.GetCurSel() == 4)
			{
				m_nMultiple = 6;
			}
			else if(m_comboShowMag.GetCurSel() == 5)
			{
				m_nMultiple = 8;
			}
			else if(m_comboShowMag.GetCurSel() == 6)
			{
				m_nMultiple =10;
			}*/
			if (zDelta > 0)
			{
				m_nMultiple++;
			}
			else
			{
				if(m_nMultiple > 1)
					m_nMultiple--;
			}

			//if (zDelta > 0)			//放大
			{
				//计算放大后需要显示的矩形区域，此处比较巧妙
				//以放大后的图像的左上角为原点计算需要显示的图像区域在放大后的图像中的坐标范围
				int x, y, w, h;			
				x = lx * m_nMultiple - lx;
				y = ty * m_nMultiple - ty;	 
				w = rect.Width();
				h = rect.Height();

				g_ptZoomPos.x = x;
				g_ptZoomPos.y = y;

				cv::Mat _image;
				if(!g_ShowImage.empty())
				{
					g_ShowImage.release();
				}
				//g_SmallImage是打开图片的时候存储的原始图像
				g_ShowImage = g_SmallImage.clone();
				g_NoCrossImage.release();
				g_NoCrossImage = g_SmallImage.clone();
				//首先将图像按照放大倍数进行放大，放大后的图像存储在_image中
				cv::resize(g_NoCrossImage, _image, cv::Size(rect.Width()*m_nMultiple, rect.Height()*m_nMultiple));
				g_NoCrossImage.release();
				//将放大后的图像的显示区域进行截图，得到需要显示给用户的图像
				g_NoCrossImage = _image(Rect(x, y, w, h));
				_image.release();
				//在原始图像上绘制导航十字，因此在鼠标单击时,必须根据放大倍数以及旋转，
				//翻转等逻辑计算出原始的十字坐标
				if(showCross)
				{
					DrawCross(g_ShowImage, cv::Point2i(cross_x, cross_y) );
				}
				//将绘制了导航十字的原始图像进行放大并截取显示区域
				cv::resize(g_ShowImage, _image, cv::Size(rect.Width()*m_nMultiple, rect.Height()*m_nMultiple));	//ÏÈ½«Ô­Í¼ÏñµÄ³¤ºÍ¿í·Å´ó4±¶.
				g_ShowImage.release();
				g_ShowImage = _image(Rect(x, y, w, h));
				//m_rcShow = CRect(x, y, w, h);	
				cv::imshow("src1", g_ShowImage);
				_image.release();
			}
/*			else
			{
				if(!g_ShowImage.empty())
				{
					g_ShowImage.release();
				}
				g_ShowImage = g_SmallImage.clone();
				if(showCross)
				{
					DrawCross(g_ShowImage, cv::Point2i(cross_x, cross_y) );
				}
				cv::imshow("src1", g_ShowImage);
				m_rcShow = CRect(0, 0, rect.Width(),rect.Height());
				m_nMultiple = 1;
				g_NoCrossImage.release();
				g_NoCrossImage = g_SmallImage.clone();
			}
*/
		}
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CPanelNAV::OnBnClickedButtonNavigate()
{
	int nRet = 0;	
	ClickStageMove();		
}

void CPanelNAV::OnBnClickedInitImage()
{
	//int iImgWidth	= m_dlgCameraSetting.m_navSets.camera.nCameraW;
	//int iImgHeight	= m_dlgCameraSetting.m_navSets.camera.nCameraH;
	//int iImgZoomWidth	= (float)m_rcDesk.Width()/m_dlgCameraSetting.m_navSets.camera.fZoom; 
	//int iImgZoomHeight	= ((float)iImgHeight/iImgWidth)*iImgZoomWidth;

	//GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect); // IDC_PICTURE 是 picture control 的 ID
	//ScreenToClient(&g_DisplayPicRect); 
	//// 2023.10
	//GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(m_nDisplayLeft + (iImgZoomWidth - iImgZoomHeight)/2, g_DisplayPicRect.top, iImgZoomWidth, iImgZoomHeight, TRUE); 
	//GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
	//m_rcShow = CRect(0,0, g_DisplayPicRect.Width(), g_DisplayPicRect.Height() );

	ClearImageRect();

	//Bmp File(*.bmp)|*.bmp|Jpg File(*.JPG)|*.JPG|Png File(*.PNG)|*.PNG|
	// 拿到图片路径
//	CFileDialog fileDlg(TRUE, _T("Bmp File(*.bmp)|*.bmp|Jpg File(*.jpg)|*.jpg|Png File(*.png)|*.png||"), NULL, 0, _T("All Files (*.*) |*.*|"), this);
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, _T("All Files |*.bmp;*.jpg;*.png|Bmp File(*.bmp)|*.bmp|Jpg File(*.jpg)|*.jpg|Png File(*.png)|*.png||"), this);
	if( fileDlg.DoModal() != IDOK )
		return;
	CString strFilePath = fileDlg.GetPathName();	//文件路径
	CString strFileName = fileDlg.GetFileName();	//文件名
	if (strFilePath == _T(""))
		return;
	USES_CONVERSION;
	std::string strPath = strFilePath.GetBuffer(0);
	
	// 展示图片
	ShowImageFromPath(strPath);
}

void  CPanelNAV::ShowCameraSeting()
{
	m_dlgCameraSetting.dlg = this;
	m_dlgCameraSetting.m_pcMyCamera = m_pcMyCamera;
	strcpy_s( m_dlgCameraSetting.m_cPathLocalize, m_cPathLocalize );
	m_dlgCameraSetting.Localize();
	m_dlgCameraSetting.ShowWindow(1);	
}

void CPanelNAV::OnBnClickedAdvanced()
{
	CDlgInputSet dlg;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	if( (dlg.DoModal() != IDOK) || dlg.m_strInput != "kx-sem" )
		return;
	ShowCameraSeting();
}

LRESULT CPanelNAV::OnSetOpenImage(WPARAM wParam, LPARAM lParam)
{
	int iImgWidth	= m_dlgCameraSetting.m_navSets.camera.nCameraW;
	int iImgHeight	= m_dlgCameraSetting.m_navSets.camera.nCameraH;
	int iImgZoomWidth	= (float)m_rcDesk.Width()/m_dlgCameraSetting.m_navSets.camera.fZoom; 
	int iImgZoomHeight	= ((float)iImgHeight/iImgWidth)*iImgZoomWidth;

	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect); // IDC_PICTURE 是 picture control 的 ID
	ScreenToClient(&g_DisplayPicRect); 
	GetDlgItem(IDC_STATIC_NAV_Display)->MoveWindow(m_nDisplayLeft, g_DisplayPicRect.top, iImgZoomWidth, iImgZoomHeight, TRUE); 
	GetDlgItem(IDC_STATIC_NAV_Display)->GetWindowRect(&g_DisplayPicRect);
	//m_rcShow = CRect( 0, 0, g_DisplayPicRect.Width(), g_DisplayPicRect.Height() );

	CFileDialog fileDlg(TRUE, _T("Bmp File(*.bmp)|*.bmp|Jpg File(*.jpg)|*.jpg|Png File(*.png)|*.png||"), NULL, 0, _T("All Files (*.*) |*.*|"), this);
	if( fileDlg.DoModal() != IDOK )
		return 0;
	CString strFilePath = fileDlg.GetPathName();	//文件路径
	CString strFileName = fileDlg.GetFileName();	//文件名
	if (strFilePath == _T(""))
		return 0;
	USES_CONVERSION;
	std::string strPath = strFilePath.GetBuffer(0);
	cv::Mat img=cv::imread(strPath,1);//cvLoadImage
	int tmp=0;
	tmp = img.type();

	g_srcImage=img;
	cv::resize(img, g_SmallImage, cv::Size(g_DisplayPicRect.Width(), g_DisplayPicRect.Height()));
	cv::imshow("src1", g_SmallImage);
	img.release();

	return 0;
}

LRESULT CPanelNAV::OnSetReadArgument(WPARAM wParam, LPARAM lParam)
{
	char exe_file[1024];
	GetModuleFileName(NULL, exe_file, 1024);
	(strrchr(exe_file, '\\'))[1] = 0; 
	std::string config_path = exe_file ;
	config_path +=  "NAV\\Config.xml";

	FileStorage fs1(config_path, FileStorage::READ); 
	fs1["rotMMatrix"] >> rotM;
	fs1["rotTMatrix"]>>rotT;
	fs1["rvecs"]>>rvecs;
	fs1["tvecs"]>>tvecs;
	fs1["cameraMatrix"]>>g_cameraMatrix;
	fs1["distCoeffs"]>>g_distCoeffs;
	fs1.release();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	MessageBox(_T("读取参数完成!"), _T("提示"),MB_OK);
	CDlgPrompt dlg;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	dlg.m_byteType = 0x01;
	dlg.m_strMsg = "\r\n\r\n\r\n";
	char szBuf[255];
	if(GetPrivateProfileString("Message", "CompleteReadParams", "", szBuf, 255, m_cPathLocalize) != 0)
		dlg.m_strMsg += szBuf;
	dlg.DoModal();
	return 0;
}

LRESULT CPanelNAV::OnSetUndistor(WPARAM wParam, LPARAM lParam)
{
	char exe_file[1024];
	GetModuleFileName(NULL, exe_file, 1024);
	(strrchr(exe_file, '\\'))[1] = 0; 
	std::string sexe_path = exe_file ;
	sexe_path +=  "NAV\\undisworld.bmp";

	cv::Size image_size = g_srcImage.size();
	cv::Mat view, rview, mapx, mapy;
	cv::Size imageSize;
	imageSize = g_srcImage.size();
	initUndistortRectifyMap(g_cameraMatrix, g_distCoeffs, cv::Mat(),
		getOptimalNewCameraMatrix(g_cameraMatrix, g_distCoeffs, imageSize, 1, imageSize, 0),
		imageSize, CV_32FC1, mapx, mapy);
	remap(g_srcImage, g_srcImageUndistort, mapx, mapy, cv::INTER_LINEAR);
	cv::resize(g_srcImageUndistort, g_SmallImageUndistort, cv::Size(g_DisplayPicRect.Width(), g_DisplayPicRect.Height()));
	cv::imshow("src1", g_SmallImageUndistort);	
	g_SmallImage.release();
	g_SmallImage = g_SmallImageUndistort;
	cv::imwrite(sexe_path,g_srcImageUndistort);
	g_ShowImage = g_SmallImage.clone();
	g_NoCrossImage = g_SmallImage.clone();
	m_bShowselfImg = TRUE;
	return 0;
}

LRESULT CPanelNAV::OnSetCreateWorld(WPARAM wParam, LPARAM lParam)
{
	CFileDialog fileDlg(TRUE, _T("BMP File(*.bmp)|*.bmp"), NULL, 0, _T("Image Files(.bmp; .jpg;.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*|"), this);
	fileDlg.DoModal();
	CString strFilePath = fileDlg.GetPathName();	//文件路径
	CString strFileName = fileDlg.GetFileName();	//文件名
	if (strFilePath == _T(""))
	{
		return 0;
	}
	USES_CONVERSION;
	cv::Mat img = cv::imread(strFilePath.GetBuffer(),1);
	cv::Mat img_gray = cv::Mat::zeros(img.size(), CV_64F);
	int BOARDSIZE[2]={8,11};					//棋盘格内角点个数 水平，垂直
	vector<Point3f> obj_world_pts;				//三维世界坐标  从左至右  从上至下
	vector<Point2f> img_corner_points;			//检测到的角点  从左至右  从上至下
	//世界坐标系 
	for (int i = 0; i < BOARDSIZE[1]; i++)
	{
		for (int j = 0; j < BOARDSIZE[0]; j++)
		{   //(-40 50)  (30  50)    从左至右  从上至下
			//          (30 -50)
			obj_world_pts.push_back(Point3f(-10.5+j*3, 15-i*3, 0));
		}
	}
	cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
	bool found_success = findChessboardCorners(img_gray, cv::Size(BOARDSIZE[0], BOARDSIZE[1]),
		img_corner_points,
		CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
	if(found_success)
	{
		//迭代终止条件
		TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);

		//进一步提取亚像素角点
		cornerSubPix(img_gray, img_corner_points, Size(11, 11),
			Size(-1, -1), criteria);

		//角点排序 与世界坐标对应  从左至右  从上至下
		if(img_corner_points[0].x > img_corner_points.back().x)
		{
			reverse(img_corner_points.begin(),img_corner_points.end());
		}

		//绘制角点 显示
		drawChessboardCorners(img, Size(BOARDSIZE[0], BOARDSIZE[1]), img_corner_points,	found_success);
		resize(img,img,Size(4032/2,3036/2));
		imshow("ddd",img);

		//已获取 角点像素坐标   世界坐标  
		//创建旋转矩阵和平移矩阵
		solvePnP(obj_world_pts,img_corner_points,g_cameraMatrix,g_distCoeffs,rvecs,tvecs,FALSE,0);
		Rodrigues(rvecs, rotM);  //将旋转向量变换成旋转矩阵
		Rodrigues(tvecs, rotT);

		//计算深度
		Mat P;
		P = (rotM.t()) * tvecs;   //Ïà»ú×ø±êC=-R^(-1)*T

		//保存参数
		char exe_file[1024];
		GetModuleFileName(NULL, exe_file, 1024);
		(strrchr(exe_file, '\\'))[1] = 0; 
		std::string config_path = exe_file ;
		config_path +=  "NAV\\Config.xml";
		FileStorage fs(config_path, FileStorage::WRITE); 
		fs<<"rotMMatrix"<<rotM; 
		fs<<"rotTMatrix"<<rotT;
		fs<<"rvecs"<<rvecs;
		fs<<"tvecs"<<tvecs;
		fs<<"cameraMatrix"<<g_cameraMatrix;
		fs<<"distCoeffs"<<g_distCoeffs;
		fs.release();			
//		MessageBox(_T("建立世界坐标系完成，参数已经保存!"), _T("提示"), MB_OK);
		CDlgPrompt dlg;
		strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
		dlg.m_byteType = 0x01;
		dlg.m_strMsg = "\r\n";
		char szBuf[255];
		if(GetPrivateProfileString("Message", "WorldCoordinateSystem", "", szBuf, 255, m_cPathLocalize) != 0)
		{
			// 提示字符串太长，以逗号为分隔，多行显示
			if( strchr( szBuf, ',') != NULL )
			{
				char szBuf1[255];
				strcpy_s( szBuf1, strrchr(szBuf, ',') +2 );
				*strchr(szBuf, ',') = '\0';
				dlg.m_strMsg += szBuf;
				dlg.m_strMsg += "\r\n";
				dlg.m_strMsg += szBuf1;
			}
			else
				dlg.m_strMsg += szBuf;
		}
		dlg.DoModal();
	}
	img.release();
	img_gray.release();
	return 0;
}

void CPanelNAV::SetLanguageID( WORD wID )
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	if( wID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED) )
		strcat_s(path, "\\NAV\\Locale\\zh\\Camera_zh_CN.ini");
	else if( wID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT) )
		strcat_s(path, "\\NAV\\Locale\\en\\Camera_en_US.ini");
	else if( wID == MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT) )
		strcat_s(path, "\\NAV\\Locale\\ru\\Camera_ru_RU.ini");
	strcpy_s( m_cPathLocalize, path );

	char szBuf[255];
	int nLen = 50;
	if(GetPrivateProfileString("Label", "Cap", "", szBuf, nLen, path) != 0)
		SetWindowText( szBuf );

	if(GetPrivateProfileString("Label", "StartGrab", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_BUTTON_NAV_GrabbingStart)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("Label", "StopGrab", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_BUTTON_NAV_GrabbingStop)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("Label", "OpenImage", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_BUTTON_NAV_InitImage)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("Label", "SaveImage", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_BUTTON_NAV_SaveJPG)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("Label", "AmplificationFactor", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_EDIT_NAV_Factor)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("Label", "Navigation", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_BUTTON_NAV_Navigate)->SetWindowText( szBuf );	
	if(GetPrivateProfileString("Label", "Power", "", szBuf, nLen, path) != 0)
		GetDlgItem(IDC_BUTTON_NAV_OpenPower)->SetWindowText( szBuf );	
}

void CPanelNAV::OnBnClickedOpenPower()
{
	UpdateNAVSwitch(!m_bNavPowerOn);
}

HBRUSH CPanelNAV::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		if(pWnd->GetDlgCtrlID() == IDC_STATIC_POWER_STATUS)
		{
			pDC->SetBkColor(TRANSPARENT);
			if(m_bNavPowerOn)
				return m_brushPowerON;
			else
				return m_brushPowerOFF;
		}
	}
	return hbr;
}

void CPanelNAV::OnTestStage()
{
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_TestStage)))->GetCheck();
	if( nCheck )
		m_bStageIsMoving = TRUE;
	else
		m_bStageIsMoving = FALSE;
}