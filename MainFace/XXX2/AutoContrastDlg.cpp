// AutoContrastDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "AutoContrastDlg.h"
#include "afxdialogex.h"
#include "AutoContrastManager.h"
#include <opencv2/opencv.hpp>  
#include <iostream>  
#include "time.h"   
#include <map>  
#include "CvvImage.h"
#include <cstdio>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Curves.h"
#include <string>
#include <fstream>
#include <iostream>
#include "xxx2.h"

using namespace std;
using namespace cv;
// CAutoContrastDlg 对话框

Curves  curves;
static Mat curves_mat;
static Mat src;
static string curves_windows = "Adjust Curves";
int nCounth;
uchar* colorTables;
uchar ct[256];
IMPLEMENT_DYNAMIC(CAutoContrastDlg, CDialog)

CAutoContrastDlg::CAutoContrastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoContrastDlg::IDD, pParent)
	, m_min(0)
	, m_max(0)
{
	m_nTitleIndex = 0;
	
}

CAutoContrastDlg::~CAutoContrastDlg()
{
}

void CAutoContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_Brightness, m_Scrollbar_Brightness);
	DDX_Control(pDX, IDC_SCROLLBAR_Contrast, m_Scrollbar_Contrast);
	DDX_Control(pDX, IDC_SCROLLBAR_Gamma, m_Scrollbar_Gamma);
	DDX_Text(pDX, IDC_EDIT_OMi, m_min);
	DDX_Text(pDX, IDC_EDIT_OMa, m_max);
}


BEGIN_MESSAGE_MAP(CAutoContrastDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ABC, &CAutoContrastDlg::OnBnClickedButtonAbc)
	ON_BN_CLICKED(IDC_BUTTON_Reset, &CAutoContrastDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_AutoBC, &CAutoContrastDlg::OnBnClickedButtonAutobc)
	ON_BN_CLICKED(IDC_BUTTON_Gradient, &CAutoContrastDlg::OnBnClickedButtonGradient)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAutoContrastDlg 消息处理程序
void CAutoContrastDlg::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_BUTTON_AutoBC)->SetWindowText(GetResString(IDS_BUTTON_AutoBC));
	GetDlgItem(IDC_BUTTON_Gradient)->SetWindowText(GetResString(IDS_BUTTON_Gradient));
	GetDlgItem(IDC_STATIC_H)->SetWindowText(GetResString(IDS_STATIC_H));
	GetDlgItem(IDC_STATIC_G)->SetWindowText(GetResString(IDS_STATIC_G));
	GetDlgItem(IDC_STATIC_B)->SetWindowText(GetResString(IDS_STATIC_B));
	GetDlgItem(IDC_STATIC_C)->SetWindowText(GetResString(IDS_STATIC_C));
	GetDlgItem(IDC_STATIC_Ga)->SetWindowText(GetResString(IDS_STATIC_Ga));
	GetDlgItem(IDC_BUTTON_ABC)->SetWindowText(GetResString(IDS_BUTTON_ABC));
	GetDlgItem(IDC_BUTTON_Reset)->SetWindowText(GetResString(IDS_BUTTON_Reset));
	
	if( m_nTitleIndex > 0 )
		SetWindowText(GetResString( m_nTitleIndex ));
	else
		SetWindowText("");
}

BOOL CAutoContrastDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	nCounth = 1;
	colorTables = FALSE;
	bOptionstate = FALSE;
	bstateA = FALSE;
	bstate = FALSE;
	
	 // 设置水平滚动条的滚动范围为1到100   
    m_Scrollbar_Brightness.SetScrollRange(-100, 100);   
    // 设置水平滚动条的初始位置为20   
    m_Scrollbar_Brightness.SetScrollPos(0);   
    // 在编辑框中显示20   
	GetDlgItem( IDC_EDIT_Brightness )->SetWindowText("0.0");
    //SetDlgItemInt(IDC_EDIT_Brightness, 0);   
	m_Scrollbar_Contrast.SetScrollRange(-100, 100); 
	m_Scrollbar_Contrast.SetScrollPos(0);   
	GetDlgItem( IDC_EDIT_Contrast )->SetWindowText("0.0");

	m_Scrollbar_Gamma.SetScrollRange(-300, 300); 
	m_Scrollbar_Gamma.SetScrollPos(100.0);   
	GetDlgItem( IDC_EDIT_Gamma )->SetWindowText("1.0");

	GetDlgItem( IDC_EDIT_HM255 )->SetWindowText("255");
	GetDlgItem( IDC_EDIT_HM0 )->SetWindowText("0");

	GetDlgItem( IDC_EDIT_OMi )->SetWindowText("0");
	GetDlgItem( IDC_EDIT_OMa )->SetWindowText("255");

	CWnd  *pWnd1= GetDlgItem (IDC_STATIC_GD);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	
	pWnd1->GetClientRect (&rect);//GetClientRect为获得控件相自身的坐标大小
	
	namedWindow(curves_windows,CV_WINDOW_NORMAL );//设置窗口名
	
	resizeWindow(curves_windows,rect.Width(),rect.Height());
	
	HWND hWndl=(HWND )cvGetWindowHandle ("Adjust Curves");//hWnd 表示窗口句柄,获取窗口句柄（若显示cvGetWindowHandle  未定义，则添加 #include opencv2/highgui/highgui_c.h 头文件）
	
	HWND hParent1=::GetParent (hWndl );//GetParent函数一个指定子窗口的父窗口句柄
	
	::SetParent (hWndl, GetDlgItem (IDC_STATIC_GD)->m_hWnd );
	
	::ShowWindow (hParent1 ,SW_HIDE );//ShowWindow指定窗口中显示
	
	Localize();
	
    return TRUE;  // return TRUE  unless you set the focus to a control   
}

void CAutoContrastDlg::HistogramShow(float *Matrix ,int nCounts)
{
	int row = nCounts/1024;
	int col = 1024;
	unsigned char array[3][1024];
	memset(array,0,sizeof(array));
	int k=0;
	for(int i=0;i<row;i++)
	{
		for(int j =0;j<col;j++)
		{
			array[i][j] = Matrix[k++];

		}
	}		
	cv::Mat img(row,col,CV_8UC1,(unsigned char*)array);
	const int channels[1] = { 0 };
			 //直方图的每一个维度的 柱条的数目（就是将灰度级分组）  
	int histSize[] = { 256 };   //如果这里写成int histSize = 256; 那么下面调用计算直方图的函数的时候，该变量需要写 &histSize  
				 //定义一个变量用来存储 单个维度 的数值的取值范围    
	float midRanges[] = { 0, 256 };
				 //确定每个维度的取值范围，就是横坐标的总数    
	const float *ranges[] = { midRanges };
				  //输出的结果存储的 空间 ，用MatND类型来存储结果  MatND表示多为矩阵在opencv2.2后统一用Mat
	MatND dstHist;
	calcHist(&img, 1, channels, Mat(), dstHist, 1, histSize, ranges, true, false);
			//calcHist  函数调用结束后，dstHist变量中将储存了 直方图的信息  用dstHist的模版函数 at<Type>(i)得到第i个柱条的值  at<Type>(i, j)得到第i个并且第j个柱条的值    
 
			//首先先创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像    
	Mat drawImage = Mat::zeros(cv::Size(300, 256), CV_8UC3);
			//一个图像的某个灰度级的像素个数（最多为图像像素总数），可能会超过显示直方图的所定义的图像的尺寸，因此绘制直方图的时候，让直方图最高的地方只有图像高度的90%来显示  
 
			//先用minMaxLoc函数来得到计算直方图后的像素的最大个数    
	double g_dHistMaxValue;
	minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);
 
			//遍历直方图得到的数据    
	for (int i = 0; i < 256; i++)
	{
		int value = cvRound(256 * 0.9 *(dstHist.at<float>(i) / g_dHistMaxValue));
		line(drawImage, cv::Point(i, drawImage.rows - 1), cv::Point(i, drawImage.rows - 1 - value), Scalar(255, 0, 0));
	}
	IplImage* pBinary = &IplImage(drawImage);
	IplImage* image = cvCloneImage(pBinary);
		
	    
    CDC* pDC=GetDlgItem(IDC_STATIC_KJ)->GetDC();// 获得显示控件的 DC
    HDC hDC=pDC->GetSafeHdc();// 获取 HDC(设备句柄) 来进行绘图操作
 
    CRect rect;
    GetDlgItem(IDC_STATIC_KJ)->GetClientRect(&rect);
	CvvImage cimg;
    cimg.CopyOf(image,image->nChannels);    // 复制图片
    cimg.DrawToHDC(hDC,&rect);
 
    ReleaseDC(pDC);
}

static void invalidate()
{
	//imshow(curves_window, curves_mat);
	curves.draw(curves_mat);
	//resize(curves_mat,curves_mat,Size(rect.Width(),rect.Height()));
	imshow(curves_windows, curves_mat);

	int state = 0;
	do
	{
		state = CAutoContrastManager::Instance().bState;
	}while(!state);
	//一维数组->二维数组->Mat
	nCounth = CAutoContrastManager::Instance().nCounth;
	float *Matrix;
	Matrix = CAutoContrastManager::Instance().fMatrix;

	int rows;
	int cols;
	rows = nCounth/1024.+1;
	cols = 1024;
		
	uchar array[3][1024]; 
	
	for(int i=0;i<rows;i++)
		for (int j=0;j<cols;j++)
			array[i][j]=0;
		
	int k1;
	k1 = 0;
	for(int i=0;i<rows;i++)
	{
		for(int j =0;j<cols;j++)
		{
			array[i][j] = Matrix[k1];
			k1 = k1+1;
			if( k1 == nCounth)
				break;
		}
		if( k1 == nCounth)
			break;
	}	
		src = Mat(rows,cols,CV_8UC1,Scalar::all(0));
		for(int i=0;i<rows;i++)
			for(int j=0;j<cols;j++)
				src.at<uchar>(i,j) = array[i][j];	

	Mat dst;

	colorTables = curves.adjust(src, dst);
	
	for (int i = 0; i < 256; ++i ) 
	{
		ct[i] = *colorTables++;
		
	}

	CAutoContrastManager::Instance().bGrade = TRUE;
	//CAutoContrastManager::Instance().im = im;

	CAutoContrastManager::Instance().colorTable = ct;
	
	waitKey();
	
}

static void callbackMouseEvent(int mouseEvent, int x, int y, int flags, void* param)
{
	switch(mouseEvent) {
	case CV_EVENT_LBUTTONDOWN:
		curves.mouseDown(x, y);
		invalidate();
		break;
	case CV_EVENT_MOUSEMOVE:
		if ( curves.mouseMove(x, y) )
			invalidate();
		break;
	case CV_EVENT_LBUTTONUP:
		curves.mouseUp(x, y);
		invalidate();
		break;
	}
	return;
}

void CAutoContrastDlg::OnBnClickedButtonAbc()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CAutoContrastManager::Instance().fAutoBC = 1;
	if(!bOptionstate)
	{
		do
		{
			bstateA = CAutoContrastManager::Instance().bStateBC;
		}while(!bstateA);
		HistogramShow(CAutoContrastManager::Instance().fMatrix,CAutoContrastManager::Instance().nCounth);
	}
	else
	{
		do
		{
			bstate = CAutoContrastManager::Instance().bState;
		}while(!bstate);
		CString str1,str2;
		int hmin = CAutoContrastManager::Instance().m_nmin ;
		int hmax = CAutoContrastManager::Instance().m_nmax ;
		str1.Format( "%d", hmin );
		str2.Format( "%d", hmax );
		GetDlgItem( IDC_EDIT_OMi )->SetWindowText(str1);
		GetDlgItem( IDC_EDIT_OMa )->SetWindowText(str2);
		curves_mat = Mat::ones(256, 256, CV_8UC1);
		namedWindow(curves_windows);
	    setMouseCallback(curves_windows, callbackMouseEvent, NULL );
		invalidate();		
	}
}

void CAutoContrastDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CAutoContrastManager::Instance().fAutoBC = false;
	if(!bOptionstate)
	{
		int stateA = 1;
		do
		{
			stateA = CAutoContrastManager::Instance().bStateBC;
		}while(stateA);
		HistogramShow(CAutoContrastManager::Instance().fMatrix,CAutoContrastManager::Instance().nCounth);
		m_Scrollbar_Brightness.SetScrollPos(0);   
		// 在编辑框中显示20   
		GetDlgItem( IDC_EDIT_Brightness )->SetWindowText("0.0");
		CAutoContrastManager::Instance().nPosBright = 0.0;
		m_Scrollbar_Contrast.SetScrollPos(0);   
		GetDlgItem( IDC_EDIT_Contrast )->SetWindowText("0.0");
		CAutoContrastManager::Instance().nPosContrast = 0.0;
		m_Scrollbar_Gamma.SetScrollPos(1.0);   
		GetDlgItem( IDC_EDIT_Gamma )->SetWindowText("1.0");
		CAutoContrastManager::Instance().fPosGamma = 100.0;
	}
	else
	{
		int state = 1;
		do
		{
			state = CAutoContrastManager::Instance().bState;
		}while(state);
		GetDlgItem( IDC_EDIT_HM255 )->SetWindowText("255");
		GetDlgItem( IDC_EDIT_HM0 )->SetWindowText("0");
		GetDlgItem( IDC_EDIT_OMi )->SetWindowText("0");
		GetDlgItem( IDC_EDIT_OMa )->SetWindowText("255");
		curves.SemReset(curves_mat);
	}
}

void CAutoContrastDlg::OnBnClickedButtonAutobc()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_AutoBC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SCROLLBAR_Brightness)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_Brightness)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SCROLLBAR_Gamma)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_Gamma)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SCROLLBAR_Contrast)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_Contrast)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_KJ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_H)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_Ga)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_G)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_GD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_HM0)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_HM255)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_OMi)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_OMa)->ShowWindow(SW_HIDE);

	bOptionstate = FALSE;
	CAutoContrastManager::Instance().bOptionState = bOptionstate;
}

void CAutoContrastDlg::OnBnClickedButtonGradient()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Gradient)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SCROLLBAR_Brightness)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_Brightness)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SCROLLBAR_Contrast)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_Contrast)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SCROLLBAR_Gamma)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_Gamma)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_KJ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_H)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_Ga)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_G)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_GD)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_EDIT_HM0)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_HM255)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_OMi)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_OMa)->ShowWindow(SW_SHOW);

	bOptionstate = TRUE;
	CAutoContrastManager::Instance().bOptionState = bOptionstate;
}

void CAutoContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	int posA = pScrollBar->GetScrollPos();
	int id = pScrollBar->GetDlgCtrlID();	
    switch (nSBCode)   
    {   
        // 如果向左滚动一列，则pos减1  
        case SB_LINELEFT:   
            posA -= 1;   
            break;   
        // 如果向右滚动一列，则pos加1  
        case SB_LINERIGHT:   
            posA  += 1;   
            break;    
        case SB_THUMBPOSITION:   
            posA = nPos;   
            break;   
        // 下面的m_horiScrollbar.SetScrollPos(pos);执行时会第二次进入此函数，最终确定滚动块位置，并且会直接到default分支，所以在此处设置编辑框中显示数值  
        default: 
			CString str;
			float Spos = posA/100.;
			str.Format("%.2f",Spos);
			if (id == IDC_SCROLLBAR_Brightness)
				GetDlgItem(IDC_EDIT_Brightness)->SetWindowText(str);
				//SetDlgItemInt(IDC_EDIT_Brightness,str); 
			if (id == IDC_SCROLLBAR_Contrast)
				GetDlgItem(IDC_EDIT_Contrast)->SetWindowText(str);
				//SetDlgItemInt(IDC_EDIT_Contrast, str); 
			if (id == IDC_SCROLLBAR_Gamma)
				GetDlgItem(IDC_EDIT_Gamma)->SetWindowText(str);
				//SetDlgItemInt(IDC_EDIT_Gamma, str); 
            return;   
    }   
	 //m_Scrollbar_Brightness.SetScrollPos(posA);
	pScrollBar->SetScrollPos(posA);
	if (id == IDC_SCROLLBAR_Brightness)
		CAutoContrastManager::Instance().nPosBright = posA;
	if (id == IDC_SCROLLBAR_Contrast)
	    CAutoContrastManager::Instance().nPosContrast = posA;
	if (id == IDC_SCROLLBAR_Gamma)
	    CAutoContrastManager::Instance().fPosGamma = posA;
   
    HistogramShow(CAutoContrastManager::Instance().fMatrix,CAutoContrastManager::Instance().nCounth);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAutoContrastDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	destroyAllWindows();
	CDialog::OnClose();
}
