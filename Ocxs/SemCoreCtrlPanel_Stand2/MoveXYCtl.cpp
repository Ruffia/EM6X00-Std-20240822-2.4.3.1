// MoveXYCtl.cpp : implementation file
//
// Last Modify : 2009.02.12

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand2.h"
#include "MoveXYCtl.h"
#include "SemCoreCtrlPanel_Stand2Ctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoveXYCtl dialog


CMoveXYCtl::CMoveXYCtl(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveXYCtl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveXYCtl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_blMouseCtrl = FALSE;
	
	m_strCaption.Empty();

	m_MoveXYFrame.SetRect(-50,-50,50,50);
	m_MoveXYPos = m_MoveXYFrame.CenterPoint();

	m_ScrollBarsPos.x = m_MoveXYPos.x - m_MoveXYFrame.left;
//	m_ScrollBarsPos.y = m_MoveXYPos.y - m_MoveXYFrame.top;
	m_ScrollBarsPos.y = m_MoveXYFrame.bottom - m_MoveXYPos.y;

	m_blReady = FALSE;

	m_font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");
}

CMoveXYCtl::~CMoveXYCtl()
{
	m_font.DeleteObject();
}

BOOL CMoveXYCtl::Bind( CSemCoreCtrlPanel_Stand2Ctrl* pCoreCtrlPanel )
{
	if ( pCoreCtrlPanel == NULL )
		return FALSE;

	m_blReady = TRUE;
	m_pCoreCtrlPanel = pCoreCtrlPanel;
	return TRUE;
}

void CMoveXYCtl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveXYCtl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveXYCtl, CDialog)
	//{{AFX_MSG_MAP(CMoveXYCtl)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveXYCtl message handlers
void CMoveXYCtl::OnCancel() 
{
	// TODO: Add extra cleanup here
	//m_PushPin.Unpinned();

	//CDialog::OnCancel();
}

void CMoveXYCtl::OnOK() 
{
	// TODO: Add extra validation here
	//m_PushPin.Unpinned();

	//CDialog::OnOK();
}

BOOL CMoveXYCtl::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CScrollBar* pCtrl;

	m_ScrollBarsPos.x = m_MoveXYPos.x - m_MoveXYFrame.left;
//	m_ScrollBarsPos.y = m_MoveXYPos.y - m_MoveXYFrame.top;
	m_ScrollBarsPos.y = m_MoveXYFrame.bottom - m_MoveXYPos.y;

	pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_H);
	if ( pCtrl != NULL )
	{
		pCtrl->SetScrollRange( 0, m_MoveXYFrame.Width() );
		pCtrl->SetScrollPos( m_ScrollBarsPos.x );
	}
	pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_V);
	if ( pCtrl != NULL )
	{
		pCtrl->SetScrollRange( 0, m_MoveXYFrame.Height() );
		pCtrl->SetScrollPos( m_ScrollBarsPos.y );
	}

	CString csTemp;
	csTemp.Format( "X =%+3d, Y =%+3d", m_MoveXYPos.x, m_MoveXYPos.y );
	SetDlgItemText( IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Display, csTemp );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define SCROLLBAR_Width			16
#define EDIT_DISPLAY_Width		21
#define Interval_Width_Inside	1
#define Interval_Width_Outside	6

void CMoveXYCtl::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CStatic* pCtrl;
	CRect	 rectClient, rectCtrl;

	GetClientRect( (LPRECT)rectClient );

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Frame);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + Interval_Width_Outside;
		rectCtrl.right	= rectClient.right - Interval_Width_Inside
							- SCROLLBAR_Width - Interval_Width_Outside;

		rectCtrl.top	= rectClient.top + Interval_Width_Outside;
		rectCtrl.bottom = rectClient.bottom - Interval_Width_Inside
							- SCROLLBAR_Width - Interval_Width_Inside
							- EDIT_DISPLAY_Width - Interval_Width_Outside;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );

		pCtrl->GetWindowRect( (LPRECT)rectCtrl );
		ScreenToClient( (LPRECT)rectCtrl );
		rectCtrl.left ++;
		rectCtrl.right --;
		rectCtrl.top ++;
		rectCtrl.bottom --;
		//////////////////////////////////////////////////////////////
		//m_MousePos = rectCtrl.CenterPoint();
		m_MousePos.x = rectCtrl.left + 
			(int)( (double)(m_MoveXYPos.x - m_MoveXYFrame.left) / m_MoveXYFrame.Width() * rectCtrl.Width() + 0.5 );
//		m_MousePos.y = rectCtrl.top + 
//			(int)( (double)(m_MoveXYPos.y - m_MoveXYFrame.top) / m_MoveXYFrame.Height() * rectCtrl.Height() + 0.5 );
		m_MousePos.y = rectCtrl.top + 
			(int)( (double)(m_MoveXYFrame.bottom - m_MoveXYPos.y) / m_MoveXYFrame.Height() * rectCtrl.Height() + 0.5 );
		//////////////////////////////////////////////////////////////
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_V);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.right - Interval_Width_Outside
							- SCROLLBAR_Width;
		rectCtrl.right	= rectClient.right - Interval_Width_Outside;

		rectCtrl.top	= rectClient.top + Interval_Width_Outside;
		rectCtrl.bottom = rectClient.bottom - Interval_Width_Inside
							- SCROLLBAR_Width - Interval_Width_Inside
							- EDIT_DISPLAY_Width - Interval_Width_Outside;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_H);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + Interval_Width_Outside;
		rectCtrl.right	= rectClient.right - Interval_Width_Inside
							- SCROLLBAR_Width - Interval_Width_Outside;

		rectCtrl.top	= rectClient.bottom
							- SCROLLBAR_Width - Interval_Width_Inside
							- EDIT_DISPLAY_Width - Interval_Width_Outside;
		rectCtrl.bottom = rectClient.bottom	- Interval_Width_Inside
							- EDIT_DISPLAY_Width - Interval_Width_Outside;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Display);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + Interval_Width_Outside;
		rectCtrl.right	= rectClient.right - Interval_Width_Outside;

		rectCtrl.top	= rectClient.bottom + 1
							- EDIT_DISPLAY_Width - Interval_Width_Outside;
		rectCtrl.bottom = rectClient.bottom - Interval_Width_Outside;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
	}
}

void CMoveXYCtl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_blMouseCtrl && (nFlags == MK_LBUTTON) )
	{
		CStatic* pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Frame);
		if ( pCtrl != NULL )
		{
			CPoint		MousePosOld;
			CRect		rectUpdate;
			CRect		rectCtrl;
			pCtrl->GetWindowRect( (LPRECT)rectCtrl );
			ScreenToClient( (LPRECT)rectCtrl );
			rectCtrl.left ++;
			rectCtrl.right --;
			rectCtrl.top ++;
			rectCtrl.bottom --;

			MousePosOld = m_MousePos;

			///////////////////////////////////////////
			m_MousePos = point;
			if ( !rectCtrl.PtInRect( point ) )
			{
				if ( m_MousePos.x < rectCtrl.left )		m_MousePos.x = rectCtrl.left;
				if ( m_MousePos.x > rectCtrl.right )	m_MousePos.x = rectCtrl.right;

				if ( m_MousePos.y < rectCtrl.top )		m_MousePos.y = rectCtrl.top;
				if ( m_MousePos.y > rectCtrl.bottom )	m_MousePos.y = rectCtrl.bottom;
			}
			///////////////////////////////////////////

			///////////////////////////////////////////
			if ( MousePosOld.x != m_MousePos.x )
			{			
				rectUpdate = rectCtrl;
				rectUpdate.left		= MousePosOld.x - 1;
				rectUpdate.right	= MousePosOld.x + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
				rectUpdate = rectCtrl;
				rectUpdate.left		= m_MousePos.x - 1;
				rectUpdate.right	= m_MousePos.x + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
			}

			if ( MousePosOld.y != m_MousePos.y )
			{			
				rectUpdate = rectCtrl;
				rectUpdate.top		= MousePosOld.y - 1;
				rectUpdate.bottom	= MousePosOld.y + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
				rectUpdate = rectCtrl;
				rectUpdate.top		= m_MousePos.y - 1;
				rectUpdate.bottom	= m_MousePos.y + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
			}
			///////////////////////////////////////////

			CrossLinesSetScrollBarsPos( 
				(double)(m_MousePos.x - rectCtrl.left) / rectCtrl.Width(), 
				(double)(m_MousePos.y - rectCtrl.top ) / rectCtrl.Height() );
		}
	}	

	CDialog::OnMouseMove(nFlags, point);
}

void CMoveXYCtl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CStatic* pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Frame);
	if ( pCtrl != NULL )
	{
		CRect	 rectUpdate;
		CRect	 rectCtrl;
		pCtrl->GetWindowRect( (LPRECT)rectCtrl );
		ScreenToClient( (LPRECT)rectCtrl );
		rectCtrl.left ++;
		rectCtrl.right --;
		rectCtrl.top ++;
		rectCtrl.bottom --;

		if ( rectCtrl.PtInRect( point ) != 0 )
		{			
			CPoint MousePosOld = m_MousePos;
			m_MousePos = point;

			///////////////////////////////////////////
			if ( MousePosOld.x != m_MousePos.x )
			{			
				rectUpdate = rectCtrl;
				rectUpdate.left		= MousePosOld.x - 1;
				rectUpdate.right	= MousePosOld.x + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
				rectUpdate = rectCtrl;
				rectUpdate.left		= m_MousePos.x - 1;
				rectUpdate.right	= m_MousePos.x + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
			}

			if ( MousePosOld.y != m_MousePos.y )
			{			
				rectUpdate = rectCtrl;
				rectUpdate.top		= MousePosOld.y - 1;
				rectUpdate.bottom	= MousePosOld.y + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
				rectUpdate = rectCtrl;
				rectUpdate.top		= m_MousePos.y - 1;
				rectUpdate.bottom	= m_MousePos.y + 1;
				InvalidateRect( (LPRECT)rectUpdate, FALSE );
			}
			///////////////////////////////////////////

			CrossLinesSetScrollBarsPos( 
				(double)(m_MousePos.x - rectCtrl.left) / rectCtrl.Width(), 
				(double)(m_MousePos.y - rectCtrl.top ) / rectCtrl.Height() );

			m_blMouseCtrl = TRUE;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CMoveXYCtl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_blMouseCtrl = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CMoveXYCtl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CStatic * pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Frame);
	if ( pCtrl != NULL )
	{
		CPen* pOldPen;
		CPen  TempPen;

		//////////////////////////////////////////////////////
		CRect	rectCtrl;
		pCtrl->GetClientRect( (LPRECT)rectCtrl );
		pCtrl->ClientToScreen( (LPRECT)rectCtrl );
		ScreenToClient( (LPRECT)rectCtrl );
		CPoint FrameCenter = rectCtrl.CenterPoint();
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// Draw RectangleFrame
		//////////////////////////////////////////////////////
		dc.Rectangle( (LPRECT)rectCtrl );
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// FrameCenter
		//////////////////////////////////////////////////////
		TempPen.CreatePen( PS_DASHDOT, 1, RGB(255,200,100) );
		pOldPen = dc.SelectObject( &TempPen );
		dc.MoveTo(rectCtrl.left, FrameCenter.y);
		dc.LineTo(rectCtrl.right,FrameCenter.y);
		dc.MoveTo(FrameCenter.x, rectCtrl.top);
		dc.LineTo(FrameCenter.x, rectCtrl.bottom);
		dc.SelectObject( pOldPen );
		TempPen.DeleteObject();
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// 调试时显示面板显示值和实际控制值
		//////////////////////////////////////////////////////
		CString str = m_pCoreCtrlPanel->m_strY;
		CSize TitleSize = dc.GetTextExtent( str );
		int posY = str.Find( '[' );
		str = m_pCoreCtrlPanel->m_strX;
		TitleSize = dc.GetTextExtent( str );
		int posX = str.Find( '[' );
		if( posX > 0 &&  posY > 0 )	// '['肯定不是第一个字符
		{
			CString strDisValue = str.Left( str.GetLength() - posX -1 );
			dc.TextOut( rectCtrl.left +5, rectCtrl.bottom -TitleSize.cy*2 -1, strDisValue );
			CString strCtlValue = str.Right( str.GetLength() -posX +1 );
			dc.TextOut( FrameCenter.x +5, rectCtrl.bottom -TitleSize.cy*2 -1, strCtlValue );

			str = m_pCoreCtrlPanel->m_strY;
			strDisValue = str.Left( str.GetLength() - posY -1 );
			dc.TextOut( rectCtrl.left +5, rectCtrl.bottom -TitleSize.cy -1, strDisValue );
			strCtlValue = str.Right( str.GetLength() -posY +1 );
			dc.TextOut( FrameCenter.x +5, rectCtrl.bottom -TitleSize.cy -1, strCtlValue );
		}
		//////////////////////////////////////////////////////

		int		 nBkMode;
		COLORREF BkColor, TextColor;

		BkColor		= dc.GetBkColor();
		TextColor	= dc.GetTextColor();
		//////////////////////////////////////////////////////
		// Draw Title
		//////////////////////////////////////////////////////
		const CString& m_Title = m_strCaption;
		if ( m_bCaptionVisible )
		{
			CFont* poldFont = dc.SelectObject(&m_font);
			dc.SetBkColor( GetSysColor( COLOR_WINDOW ) );
			dc.SetTextColor( m_pCoreCtrlPanel->TranslateColor(m_pCoreCtrlPanel->GetForeColor()) );//RGB(0,0,255) );
			nBkMode		= dc.SetBkMode( TRANSPARENT );

			int TitleX, TitleY;
			CSize TitleSize = dc.GetTextExtent( m_Title );
			//======2009.02.12_标签超长的显示======================
			// 修改标示：2009.02.12_标签超长的显示
			// 修改日期：2009.02.12
			// 修改人  ：J.Y.Ma
			// 修改原因：当标签超长时显示不全
			//			现分多行显示，以空格符为换行标志

			// 本次修改前代码副本
			/*----------------------------------------------------- 
			TitleX = FrameCenter.x - TitleSize.cx / 2;
			TitleY = FrameCenter.y - TitleSize.cy / 2;		
			dc.TextOut( TitleX, TitleY, m_Title );
			-----------------------------------------------------*/

			// 本次修改后代码
			//-----------------------------------------------------
			if( TitleSize.cx > rectCtrl.Width() )	//标签超长
			{
				CString strTemp, str = m_Title;
				// 计算标签中共有多少个空格符
				int nLength = str.GetLength();
				int nPos = str.Find( ' ' );
				int nCount = 0;
				while( nPos >= 0 )
				{
					nCount++;
					str = str.Right( nLength -nPos -1 );
					nLength = str.GetLength();
					nPos = str.Find( ' ' );
				}

				str = m_Title;
				nLength = str.GetLength();
				// 计算分行显示时第一行的上边界值
				TitleY = (int)(FrameCenter.y -(nCount +1) /2.0 * TitleSize.cy);
				// 分多行显示标签
				int i;
				for( i=0; i<=nCount; i++ )
				{
					nPos = str.Find( ' ' );
					if( nPos >= 0 )
					{
						strTemp = str.Left( nPos );
						str = str.Right( nLength -nPos -1 );
						nLength = str.GetLength();
					}
					else
						strTemp = str;	//最后一行所显示的字符

					TitleSize = dc.GetTextExtent( strTemp );
					// 调整本行的横向位置
					TitleX = FrameCenter.x - TitleSize.cx / 2;
					// 输出本行字符
					dc.TextOut( TitleX, TitleY, strTemp );
					// 调整下一行的纵向位置
					TitleY += TitleSize.cy;
				}
			}
			else	// 一行即可显示的标签
			{
				TitleX = FrameCenter.x - TitleSize.cx / 2;
				TitleY = FrameCenter.y - TitleSize.cy / 2;		
				dc.TextOut( TitleX, TitleY, m_Title );
			}
			//-----------------------------------------------------
			//======2009.02.12_标签超长的显示======================

			dc.SetBkColor( BkColor );
			dc.SetTextColor( TextColor ); 
			dc.SetBkMode( nBkMode );
			dc.SelectObject( poldFont );
		}
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// Draw XMin & XMax String
		//////////////////////////////////////////////////////
		CString csTextOut;
		nBkMode		= dc.SetBkMode( TRANSPARENT );
		dc.SetBkColor( GetSysColor( COLOR_WINDOW ) );

		csTextOut = m_csXMinString;
		dc.SetTextAlign(TA_LEFT | TA_TOP);
		dc.ExtTextOut(rectCtrl.left+Interval_Width_Inside, FrameCenter.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		csTextOut = m_csXMaxString;
		dc.SetTextAlign(TA_RIGHT | TA_BOTTOM);
		dc.ExtTextOut(rectCtrl.right-Interval_Width_Inside, FrameCenter.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		csTextOut = m_csYMinString;
		dc.SetTextAlign(TA_LEFT | TA_BOTTOM);
		dc.ExtTextOut(FrameCenter.x, rectCtrl.bottom-Interval_Width_Inside,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		csTextOut = m_csYMaxString;
		dc.SetTextAlign(TA_RIGHT | TA_TOP);
		dc.ExtTextOut(FrameCenter.x, rectCtrl.top+Interval_Width_Inside,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		dc.SetBkColor( BkColor );
		dc.SetBkMode( nBkMode );
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// MousePos
		//////////////////////////////////////////////////////
		TempPen.CreatePen( PS_SOLID, 1, RGB(0,0,255) );
		pOldPen = dc.SelectObject( &TempPen );
		dc.MoveTo(rectCtrl.left, m_MousePos.y);
		dc.LineTo(rectCtrl.right,m_MousePos.y);
		dc.MoveTo(m_MousePos.x, rectCtrl.top);
		dc.LineTo(m_MousePos.x, rectCtrl.bottom);
		dc.SelectObject( pOldPen );
		TempPen.DeleteObject();
		//////////////////////////////////////////////////////
	}

	// Do not call CDialog::OnPaint() for painting messages
}


void CMoveXYCtl::CrossLinesSetScrollBarsPos( double x, double y )
{
	CPoint ScrollBarsPosOld	= m_ScrollBarsPos;

	m_ScrollBarsPos.x = (int)(x * m_MoveXYFrame.Width());
	m_ScrollBarsPos.y = (int)(y * m_MoveXYFrame.Height());

	CScrollBar* pCtrl;

	if ( ScrollBarsPosOld.x != m_ScrollBarsPos.x )
	{			
		pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_H);
		if ( pCtrl != NULL )
		{
			pCtrl->SetScrollPos( m_ScrollBarsPos.x );
		}
	}
	if ( ScrollBarsPosOld.y != m_ScrollBarsPos.y )
	{			
		pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_V);
		if ( pCtrl != NULL )
		{
			pCtrl->SetScrollPos( m_ScrollBarsPos.y );
		}
	}

	if (ScrollBarsPosOld != m_ScrollBarsPos )
	{
		m_MoveXYPos.x = m_ScrollBarsPos.x + m_MoveXYFrame.left;
//		m_MoveXYPos.y = m_ScrollBarsPos.y + m_MoveXYFrame.top;
		m_MoveXYPos.y = m_MoveXYFrame.bottom - m_ScrollBarsPos.y;

		CString csTemp;
		csTemp.Format( "X=%+3d,Y=%+3d", m_MoveXYPos.x, m_MoveXYPos.y );
		SetDlgItemText( IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Display, csTemp );

		ReportPos();
	}
}

void CMoveXYCtl::ScrollBarsSetCrossLinesPos( int nScrollH, int nScrollV, BOOL blReportPos )
{
	CStatic* pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Frame);
	if ( pCtrl != NULL )
	{
		CPoint		MousePosOld;
		CRect		rectUpdate;
		CRect		rectCtrl;
		pCtrl->GetWindowRect( (LPRECT)rectCtrl );
		ScreenToClient( (LPRECT)rectCtrl );
		rectCtrl.left ++;
		rectCtrl.right --;
		rectCtrl.top ++;
		rectCtrl.bottom --;

		MousePosOld = m_MousePos;

		///////////////////////////////////////////
		if ( nScrollH != -1 )
		{
			m_MousePos.x = rectCtrl.left + 
				(int)( (double)nScrollH / m_MoveXYFrame.Width() * rectCtrl.Width() + 0.5 );
		}
		if ( nScrollV != -1 )
		{
			m_MousePos.y = rectCtrl.top + 
				(int)( (double)nScrollV / m_MoveXYFrame.Height() * rectCtrl.Height() + 0.5 );
		}
		///////////////////////////////////////////

		///////////////////////////////////////////
		if ( nScrollH != -1 )
		{
			rectUpdate = rectCtrl;
			rectUpdate.left		= MousePosOld.x - 1;
			rectUpdate.right	= MousePosOld.x + 1;
			InvalidateRect( (LPRECT)rectUpdate, FALSE );
			rectUpdate = rectCtrl;
			rectUpdate.left		= m_MousePos.x - 1;
			rectUpdate.right	= m_MousePos.x + 1;
			InvalidateRect( (LPRECT)rectUpdate, FALSE );
		}

		if ( nScrollV != -1 )
		{
			rectUpdate = rectCtrl;
			rectUpdate.top		= MousePosOld.y - 1;
			rectUpdate.bottom	= MousePosOld.y + 1;
			InvalidateRect( (LPRECT)rectUpdate, FALSE );
			rectUpdate = rectCtrl;
			rectUpdate.top		= m_MousePos.y - 1;
			rectUpdate.bottom	= m_MousePos.y + 1;
			InvalidateRect( (LPRECT)rectUpdate, FALSE );
		}
		///////////////////////////////////////////
	}

	if ( ( nScrollH != -1 ) || ( nScrollV != -1 ) )
	{
		if ( nScrollH != -1 ) m_MoveXYPos.x = nScrollH + m_MoveXYFrame.left;
//		if ( nScrollV != -1 ) m_MoveXYPos.y = nScrollV + m_MoveXYFrame.top;
		if ( nScrollV != -1 ) m_MoveXYPos.y = m_MoveXYFrame.bottom - nScrollV;

		CString csTemp;
		csTemp.Format( "X =%+3d, Y =%+3d", m_MoveXYPos.x, m_MoveXYPos.y );
		SetDlgItemText( IDC_DIALOG_SEMCORECTRLPANEL_STAND2_STATIC_Display, csTemp );

		if ( blReportPos )
			ReportPos();
	}
}

void CMoveXYCtl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nSBCode)
	{
	case SB_LINEUP:
		if ( m_ScrollBarsPos.y > 0 )
			m_ScrollBarsPos.y --;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.y );
		ScrollBarsSetCrossLinesPos( -1, m_ScrollBarsPos.y );
		break;
	case SB_PAGEUP:
		if ( m_ScrollBarsPos.y > 10 )
			m_ScrollBarsPos.y -= 10;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.y );
		ScrollBarsSetCrossLinesPos( -1, m_ScrollBarsPos.y );
		break;

	case SB_LINEDOWN:
		if ( m_ScrollBarsPos.y < m_MoveXYFrame.Height() )
			m_ScrollBarsPos.y ++;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.y );
		ScrollBarsSetCrossLinesPos( -1, m_ScrollBarsPos.y );
		break;
	case SB_PAGEDOWN:
		if ( m_ScrollBarsPos.y < m_MoveXYFrame.Height()-10 )
			m_ScrollBarsPos.y += 10;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.y );
		ScrollBarsSetCrossLinesPos( -1, m_ScrollBarsPos.y );
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_ScrollBarsPos.y = nPos;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.y );
		ScrollBarsSetCrossLinesPos( -1, m_ScrollBarsPos.y );
		break;

	default:
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMoveXYCtl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nSBCode)
	{
	case SB_LINELEFT:
		if ( m_ScrollBarsPos.x > 0 )
			m_ScrollBarsPos.x --;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.x );
		ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, -1 );
		break;
	case SB_PAGELEFT:
		if ( m_ScrollBarsPos.x > 10 )
			m_ScrollBarsPos.x -= 10;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.x );
		ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, -1 );
		break;

	case SB_LINERIGHT:
		if ( m_ScrollBarsPos.x < m_MoveXYFrame.Width() )
			m_ScrollBarsPos.x ++;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.x );
		ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, -1 );
		break;
	case SB_PAGERIGHT:
		if ( m_ScrollBarsPos.x < m_MoveXYFrame.Width() -10 )
			m_ScrollBarsPos.x += 10;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.x );
		ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, -1 );
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_ScrollBarsPos.x = nPos;
		pScrollBar->SetScrollPos( m_ScrollBarsPos.x );
		ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, -1 );
		break;

	default:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMoveXYCtl::SetPos( int nXpos, int nYpos )
{
	m_MoveXYPos.x = nXpos;
	m_MoveXYPos.y = nYpos;
	m_ScrollBarsPos.x = m_MoveXYPos.x - m_MoveXYFrame.left;
//	m_ScrollBarsPos.y = m_MoveXYPos.y - m_MoveXYFrame.top;
	m_ScrollBarsPos.y = m_MoveXYFrame.bottom - m_MoveXYPos.y;

	if ( m_hWnd == NULL )
		return;

	ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, m_ScrollBarsPos.y, FALSE );

	CScrollBar* pCtrl;
	pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_H);
	if ( pCtrl != NULL )
	{
		pCtrl->SetScrollPos( m_ScrollBarsPos.x );
	}
	pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_V);
	if ( pCtrl != NULL )
	{
		pCtrl->SetScrollPos( m_ScrollBarsPos.y );
	}
}

void CMoveXYCtl::SetRange( int nXmin, int nXmax, int nYmin, int nYmax )
{
	m_MoveXYFrame.left		= nXmin;
	m_MoveXYFrame.right		= nXmax;
	m_MoveXYFrame.top		= nYmin;
	m_MoveXYFrame.bottom	= nYmax;
	m_ScrollBarsPos.x = m_MoveXYPos.x - m_MoveXYFrame.left;
//	m_ScrollBarsPos.y = m_MoveXYPos.y - m_MoveXYFrame.top;
	m_ScrollBarsPos.y = m_MoveXYFrame.bottom - m_MoveXYPos.y;

	if ( m_hWnd == NULL )
		return;

	ScrollBarsSetCrossLinesPos( m_ScrollBarsPos.x, m_ScrollBarsPos.y, FALSE  );

	CScrollBar* pCtrl;
	pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_H);
	if ( pCtrl != NULL )
	{
		pCtrl->SetScrollRange( 0, m_MoveXYFrame.Width() );
		pCtrl->SetScrollPos( m_ScrollBarsPos.x );
	}
	pCtrl = (CScrollBar *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND2_SCROLLBAR_V);
	if ( pCtrl != NULL )
	{
		pCtrl->SetScrollRange( 0, m_MoveXYFrame.Height() );
		pCtrl->SetScrollPos( m_ScrollBarsPos.y );
	}
}

void CMoveXYCtl::ReportPos()
{
	if ( !m_blReady )
		return;

	m_pCoreCtrlPanel->ReportPos(m_MoveXYPos.x, m_MoveXYPos.y);
}

void CMoveXYCtl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetPos( 0, 0 );
	ReportPos();
	CDialog::OnRButtonDown(nFlags, point);
}

void CMoveXYCtl::OnSetFont(CFont* pFont) 
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);

/*	m_ctrlRoundSlider.SetFontName(lf.lfFaceName);
	m_ctrlRoundSlider.SetFontSize(-lf.lfHeight);
	m_ctrlRoundSlider.SetFontBold(lf.lfWeight);
	m_ctrlRoundSlider.SetFontItalic(lf.lfItalic);
	m_ctrlRoundSlider.SetFontUnderline(lf.lfUnderline);
*/
	CDialog::OnSetFont(pFont);
}
