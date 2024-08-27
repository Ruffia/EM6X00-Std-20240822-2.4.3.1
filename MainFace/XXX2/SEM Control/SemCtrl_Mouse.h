// SemCtrl_Mouse.h : main header file for the KYKY2800B application
//======2004-5-13_屏幕鼠标控制电镜=====================
// 修改标示：2004-5-13_屏幕鼠标控制电镜
// 修改日期：2004年5月13日星期四
// 修改人：罗明华
// 修改原因：添加屏幕鼠标操作电镜控制。
// Last Modify : 2005.02.05

#if !defined(_SEMCTRL_MOUSE_INCLUDED_)
#define _SEMCTRL_MOUSE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////

void SemCtrlMouse_Init(CPoint point, UINT uAdjustorSort = 4);
void SemCtrlMouse_Update(CPoint point, UINT uAdjustorSort = 4);

//void SemCtrlMouse_Close();


/////////////////////////////////////////////////

#endif // _SEMCTRL_MOUSE_INCLUDED_
