// SemCtrl_Mouse.h : main header file for the KYKY2800B application
//======2004-5-13_��Ļ�����Ƶ羵=====================
// �޸ı�ʾ��2004-5-13_��Ļ�����Ƶ羵
// �޸����ڣ�2004��5��13��������
// �޸��ˣ�������
// �޸�ԭ�������Ļ�������羵���ơ�
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
