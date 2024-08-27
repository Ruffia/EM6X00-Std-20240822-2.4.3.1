#ifndef __Message_H
#define __Message_H

#define WM_USER_KYKYComm_Thread_MCICard_GetRect  WM_USER+610
#define WM_USER_KYKYComm_DisplayDebugMessage     WM_USER+611
#define WM_USER_KYKYComm_AutoStageXY_FI_Init     WM_USER+612
#define WM_USER_KYKYComm_SyncFineMag             WM_USER+613
#define WM_USER_KYKYComm_CoUpdateScan            WM_USER+614
#define WM_USER_KYKYComm_UpdateBeemAlignmentX    WM_USER+615
#define WM_USER_KYKYComm_UpdateBeemAlignmentY    WM_USER+616
#define WM_USER_KYKYComm_ShowDFocusCtrl          WM_USER+617
#define WM_USER_KYKYComm_SyncContrastCtrlText    WM_USER+618

//手控盒发来的消息,自动亮度对比度
#define WM_USER_ControlPanel_AutoBrightContrast  WM_USER + 740

//手控盒发来的消息,自动聚焦
#define WM_USER_ControlPanel_AutoFocus           WM_USER + 741

//手控盒发来的消息,切换扫描速度
#define WM_USER_ControlPanel_ChangeScanSpeed     WM_USER + 742


//手控盒发来的消息,F1 快捷键，照相预览
#define WM_USER_ControlPanel_PhotoPreview        WM_USER + 743

//手控盒发来的消息,F2 快捷键，同步扫描
#define WM_USER_ControlPanel_SyncScan            WM_USER + 744

//手控盒发来的消息,F2 快捷键，冻结图像
#define WM_USER_ControlPanel_StopScan            WM_USER + 745

//手控盒发来的消息,S1 快捷键，常规扫描，选区扫描切换
#define WM_USER_ControlPanel_ChangeScan          WM_USER + 746

//SEM Log 消息，异步加载页面数据 
#define WM_MsgAsynLoadPageData                   WM_USER + 7330

//SEMLog,检查VAC是否报警的消息
#define WM_MsgCheckVACAlarm                      WM_USER + 7331

#endif
