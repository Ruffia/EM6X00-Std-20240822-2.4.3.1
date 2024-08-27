#ifndef __CommMC6600_H
#define __CommMC6600_H

#include <afxtempl.h>

#define WM_USER_MC6600_UpdateValue	(WM_USER+98)

/*
typedef struct
{
}KYKYCommMsg_Ver, *PKYKYCommMsg_Ver;
*/
typedef struct _MC6600_Params_Speed{
	float	fSpeed[5];		// �������ٶ�
	float	fSpeedInit[5];	// ��������ٶ�
	float	fSpeedAcc[5];	// ��������ٶ�
	float	fSpeedHome[5];	// �������ԭ���ٶ�
	float   fSpeedTrack[5];
} MC6600_Params_Speed;

typedef struct _MC6600_Params_Fault{
	BOOL	bCommunicationFault[5];
	BOOL    bMotorFault[5];		//������� 0Ϊ����
} MC6600_Params_Fault;
typedef struct _MC6600_Params_Current{
	float	fSteps[5];		// ��ǰλ��
	float	fOrigin[5];		// �û�ԭ��λ��
	int		nStatus[5];		// ��ǰ״̬
} MC6600_Params_Current;

typedef struct _MC600_Params_Bl{
	short	nDir[5];		// -1��ʾ������1��ʾ������
	float	fCount[5];		// ��϶������λ����
	int     nCoeff[5];		// �س�ϵ��
} MC6600_Params_Bl;

typedef struct _MC6600_Params_AxialAddr{
	char    cAxial[5];
	BYTE	byteAddr[5];	// ��վ��ַ
} MC6600_Params_AxialAddr;

typedef struct _MC6600_Params_LimitDir{
	float   fA[5];
	float   fB[5];			// ���޷���
}MC6600_Params_LimitDir;

typedef struct _MC6600_Params_DriveDir{
	short   nDir[5];		// ��������-1��ʾ������1��ʾ������
}MC6600_Params_DriveDir;

typedef struct _MC6600_Params_SetupWizard{
	int		nSetMotorType[5];	//�������
	int		nEncoderLines[5];	//����������
	int		nControlMode[5];	//���Ʒ�ʽ
	int		nControlSignal[5];	//�����ź�
	int		nMaxCurrent[5];		//������
}MC6600_Params_SetupWizard;	// ������

typedef struct _MC6600_Params_PID{
	float    fP[5];
	float    fI[5];
	float    fD[5];
}MC6600_Params_PID;


struct structMC6600Pos{
	CString	strName;		// ����
	float	fPos[5];		// λ��
};
typedef CTypedPtrList< CPtrList, structMC6600Pos* > ListMC6600Pos;

typedef struct _MC6600_Params_ZRes{
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

} MC6600_Params_ZRes;


typedef struct _MC6600_Params_Style{
	unsigned char ucAxis;	// �Ͽ������ַ�
	short	nAxis;			// �Ͽ���������
	BOOL	bOld;			// �Ƿ����Ͽ�����

	short	nType;			// 0��KYKY����̨��1��KYKY����̨��2��Klein����̨��3��Klein����̨
} MC6600_Params_Style;

#endif
