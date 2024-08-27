#ifndef __CommTMCM_H
#define __CommTMCM_H

#include <afxtempl.h>

#define WM_USER_TMCM_UpdateValue	(WM_USER+98)

/*
typedef struct
{
}KYKYCommMsg_Ver, *PKYKYCommMsg_Ver;
*/
typedef struct _TMCM_Params_Speed{
	float	fSpeedPos[5];		// �����ᶨλ�ٶ�
	float	fSpeedDec[5];		// ��������ٶ�
	float	fSpeedAcc[5];		// ��������ٶ�
	float	fMaxSpeedPos[5];	// �����ᶨλ�ٶ�-���
	float	fMaxSpeedDec[5];	// ��������ٶ�-���
	float	fMaxSpeedAcc[5];	// ��������ٶ�-���
	float	fMinSpeedPos[5];	// �����ᶨλ�ٶ�-��С
	float	fMinSpeedDec[5];	// ��������ٶ�-��С
	float	fMinSpeedAcc[5];	// ��������ٶ�-��С
	float	fSpeedHome[5];		// �������ԭ���ٶ�
	float   fSpeedTrack[5];
} TMCM_Params_Speed;


typedef struct _TMCM_Params_Fault{
	BOOL	bCommunicationFault[5];
	BOOL    bMotorFault[5];		// ������� 0Ϊ����
} TMCM_Params_Fault;

typedef struct _TMCM_Params_Elec{
	float   fAxisCurSpeed[5];       // ���е���
	float   fAxisStandbySpeed[5];	// ���е���
	
} TMCM_Params_Elec;

typedef struct _TMCM_Params_Current{
	float	fSteps[5];		// ��ǰλ��
	float	fOrigin[5];		// �û�ԭ��λ��
} TMCM_Params_Current;

typedef struct _TMCM_Params_Bl{
	short	nDir[5];		// -1��ʾ������1��ʾ������
	float	fCount[5];		// ��϶������λ����
	int     nCoeff[5];		// �س�ϵ��
} TMCM_Params_Bl;


typedef struct _TMCM_Params_LimitStatus{
	int		nL[5];
	int		nR[5];			// ����\�ӵ�
	int		nGL;
	int		nGR;
}TMCM_Params_LimitStatus;

typedef struct _TMCM_Params_DriveDir{
	short   nDir[5];		// ��������-1��ʾ������1��ʾ������
}TMCM_Params_DriveDir;

struct structTMCMPos{
	CString	strName;		// ����
	float	fPos[5];		// λ��
};
typedef CTypedPtrList< CPtrList, structTMCMPos* > ListTMCMPos;

typedef struct _TMCM_Params_Sys{
	float   fPitch[5];			// ˿�ܵ���
	float	fRat[5];			// ������
	float   fStepAngle[5];		// �����
	int 	nAxisSubDivide[5];	// ϸ��
	float   fStepsRev[5];		// ÿת������
} TMCM_Params_Sys;

typedef struct _TMCM_Params_Pointpos{
	float   fPointPosX[7];		
	float   fPointPosY[7];	
} TMCM_Params_Pointpos;

typedef struct _TMCM_Params_ZRes{
//	float   fCentrCoor[3],fCentrCur[3];     //ɨ����������X/Y/Z -�����̶�/��ǰ
	float   fProbeCenCoor[3];	// ̽ͷ��������X/Y/Z
	float   fTangCircent[3];	// ����Բ��������
	float   fRightArcjudg[3];	// ��Բ��λ���ж�
	float   fLeftCylinleft[3];	// Բ������Ʒ��
	float   fLeftObjectlow[3];	// �ﾵ�¶˵�
	float   fLeftSquareleft[3];	// ������Ʒ����
	float   fTo;				// ��ת������̽ͷ���������Z�����б�ǣ��̶���
	float   fInclina;			// Բ�ε������϶��뷽�ε������϶�������б��
	float   fTa;				// T����ת�Ƕ�
	float	fHy;				// ��Ʒ�߶�H���û�[5-30)��������Ա[3,30)��ȱʡΪ16
	float   fHb;				// ���Ķ��깤��
	float   fHs;				// ��Ʒ̨����ߴ�
	float   fHt;				// T����ת���׹�������
	float   fRy;				// Բ������Ʒ�뾶
	float   fRtb;				// ̽ͷ�뾶
	float   fRqy;				// ��ǰZ��Ӧ����Բ�뾶
	float   fDis;				// ��Ʒ����������Բ���ľ��루��R��+R��Բ�Ƚϣ�
	float	fZ0wd;				// Z������Ӧ�Ĺ�������Z0wd
	float	fZsink;				// ���ε���̽ͷ��Ե���ﾵ��Z�³�
	float	fZbr[2];			// �����棺br1ΪZ�����λ��������ʼ��λ��λ��
								//			Zbr1 = fT0 -Hy
								//			Zbr0 = fZ0wd -Hy -Zsink
	float	fSafeWD;			// ��ȫ���
	float	fCali[3];			// X/Y/Z �������Ʊ궨λ�ã�����X�궨��Y�궨�ɱ߽��߼���
								//							Z�궨 = Z0wd -safeWD -Hy
	float	fRx0;				// 24.5
	float	fRy0;				// 16.0

	float	fLimitPos[5];		// X/Y/Z/R/T ������
	float	fLimitNeg[5];		// X/Y/Z/R/T ������

} TMCM_Params_ZRes;


typedef struct _TMCM_Params_Style{
	short	nVersion;
	short	nType;				// 0��KYKY����̨��1��KYKY����̨��

} TMCM_Params_Style;

typedef struct _TMCM_Params_LimitParam{
	int		nLeftLimit[5];			// 0����λ���ã�1����λ����
	int		nRightLimit[5];	
	int		nExchangeLimit[5];		// 0����λ��������1����λ����
	int     nLeftLevelReversal[5];
	int     nRightLevelReversal[5];	// 0���͵�ƽ��1���ߵ�ƽ

} TMCM_Params_LimitParam;
#endif
