
#ifndef __BoardVAC_H
#define __BoardVAC_H

#define WM_USER_VAC_UpdateCCGValue		(WM_USER	+61)
#define WM_USER_BoardVAC_UpdateStatus	(WM_USER	+69)

typedef struct tagVACcfg_Ports{
	int				nPort;			// COM�˿ں�
	BOOL			bReady;			// �˿��Ƿ�������
} CfgVAC_Ports;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 6000��Ŀ��տ��ư��ͨѶ(�������)

const	BYTE	byte_BrdHead	= 0xF2;
const	BYTE	byte_BrdTail	= 0x2F;
const	BYTE	byte_BrdLenSnd1	= 0x05;
const	BYTE	byte_BrdLenSnd2	= 0x06;		// ���������ֵ��6�ֽ�
const	BYTE	byte_BrdLenRec	= 0x06;

// 1. PC������տ��ư��ͨ��֡��ʽ
//	֡ͷByte1	��������Byte2	������Byte3		У��(XOR)Byte4		֡βByte5
//	F2			opcXXX			XX				Byte2 XOR Byte3		2F

// PC������տ��ư������
const	BYTE	byte_opcVentChamb	= 0x01;		// oprand = 0x01����Ʒ�ҷ���
const	BYTE	byte_opcVentGun		= 0x02;		// oprand = 0x02������ǹ����

const	BYTE	byte_opcPumpChamb	= 0x03;		// oprand = 0x03����Ʒ�ҳ����
const	BYTE	byte_opcPumpGun		= 0x04;		// oprand = 0x04������ǹ�����

const	BYTE	byte_opcV1		= 0x05;		// oprand = 0x01������V1��oprand = 0x02���ر�V1
const	BYTE	byte_opcCCD		= 0x06;		// oprand = 01��CCD����oprand = 0x02��CCD��

const	BYTE	byte_opcStatus1	= 0x11;		// oprand = 0x11����ȡ���״̬
const	BYTE	byte_opcStatus2	= 0x21;		// oprand = 0x21����ȡ����״̬

const	BYTE	byte_opcCCG		= 0x31;		// oprand = 0x31����ȡCCGֵ
const	BYTE	byte_opcTC3		= 0x41;		// oprand = 0x41����ȡTC3ֵ
const	BYTE	byte_opcTC4		= 0x51;		// oprand = 0x51����ȡTC4ֵ

// 2. PC������տ��ư�ĸ���ֵ��ͨ��֡��ʽ
//	֡ͷByte1	��������Byte2	������Byte3		������Byte4		У��(XOR)Byte5				֡βByte6
//	F2			opcXXX			XX				XX				Byte2 XOR Byte3	XOR Byte4	2F

// PC������տ��ư�ĸ���ֵ����
const	BYTE	byte_opcTsTC3	= 0x07;		// oprand = ��ֵ����ֵXX XX(��8λ��ǰ)������TC3��ѹ��ֵ
const	BYTE	byte_opcTsTC4	= 0x08;		// oprand = ��ֵ����ֵXX XX(��8λ��ǰ)������TC4��ѹ��ֵ
const	BYTE	byte_opcTsCCG1	= 0x09;		// oprand = ��ֵ����ֵXX XX(��8λ��ǰ)������CCG��ѹ����ֵ(���ڸ�ֵ��ձ仵)
const	BYTE	byte_opcTsCCG2	= 0x0A;		// oprand = ��ֵ����ֵXX XX(��8λ��ǰ)������CCG��ѹ����ֵ(���ڸ�ֵ��մ��)
const	BYTE	byte_opcInterval= 0x0B;		// oprand = ���������ʱ��XX XX����λ�룬��Χ60-90��
// ע��10V��Ӧ0xFFFF, ���õ���ֵ�������Ӧ��ʮ�����ƴ���
// ���磺
// TC��ת4.0E-01�൱�ڵ�ѹ0.4V���ң�������TC��ѹ��ֵΪ0x0A3D(65535*0.4/10)
// CCG��ת5.0E-05�൱�ڵ�ѹ4.3V���ң�������CCG����ֵΪ0x6E14(65535*4.3/10)

// 3. ��տ��ư��ϴ�����Ӧ֡��ʽ
//	֡ͷByte1	��������Byte2	����1 Byte3		����2 Byte4		У��(XOR)Byte5				֡βByte6
//	F2			opcXXX			XX				XX				Byte2 XOR Byte3 XOR Byte4	2F

// ��տ��ư����͵�����
//const	BYTE	byte_opcStatus1	= 0x11;		// �������״̬
// ����1��0xXY
//	X��	0 ��ʼ��״̬��1 ��Ʒ�Ҵ���״̬��2 ����ǹ����״̬
//		3 ��Ʒ�ҳ����״̬��4 ����ǹ�����״̬
//		5 ��Ʒ�Ҵ����״̬��6 ����ǹ�����״̬��
//		7 TMP����״̬��8 TMP���д���״̬
//	Y��	0bMKQP
//		M��1 ��Ʒ����մ���״̬��0 ��Ʒ���������״̬
//		KQP ����
//	����2��0x00

//const	BYTE	byte_opcStatus2	= 0x21;		// ���ط���״̬
//	����1��0b ABCD EFGH ��1Ϊ����0Ϊ�أ�
//		A:CCD��B:TMP��C:CCG��D:V5��E:V4��F:V3��G:V2��H:V1
//	����2��0b ABCD EFGH ��1Ϊ����0Ϊ�أ�
//		H:V1�ض�
//		ABCDEFG����

// const	BYTE	byte_opcCCG		= 0x31;		// ����CCGֵ
// const	BYTE	byte_opcTC3		= 0x41;		// ����TC3ֵ
// const	BYTE	byte_opcTC4		= 0x51;		// ����TC1ֵ
// ����1��0xXX������2��0xXX
// ����1��2��Ӧ16λADC��ֵ

// ����տ���ģ���յ�PC������opcVentChamb��opcVentGun��opcPumpChamb��opcPumpGun��opcStatus1�����ִ�в�Ӧ��opcStatus1
// ����տ���ģ���յ�PC�����Ļ�ȡCCG��TCֵ����ʱ��Ӧ�𷵻���Ӧֵ



// 4. ��տ��ư��ϴ��ı���֡��ʽ
//	֡ͷByte1	��������Byte2	����1 Byte3	����2 Byte4		У��(XOR)Byte5				֡βByte6
//	F2			opcXXX			XX			XX				Byte2 XOR Byte3 XOR Byte4	2F

// ��տ��ư��ϴ��ı���֡����
const	BYTE	byte_opcWarning	= 0x92;		// ����1��0x92;
											// ����2��
											//		  0x00 TMP���б���
											//		  0x01 ��Ʒ����մ��󱨾�
// ��λ����������PC����Ӧֵ



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//const	BYTE	byte_opcStatus1	= 0x11;		// �������״̬
// ����1��0xXY
//	X��	0 ��ʼ��״̬��1 ��Ʒ�Ҵ���״̬��2 ����ǹ����״̬
//		3 ��Ʒ�ҳ����״̬��4 ����ǹ�����״̬
//		5 ��Ʒ�Ҵ����״̬��6 ����ǹ�����״̬��
//		7 TMP����״̬��8 TMP���д���״̬
//	Y��	0bMKQP
//		M��1 ��Ʒ����մ���״̬��0 ��Ʒ���������״̬
//		KQP ����
//	����2��0x00

typedef struct tagStatusVac6K{
	BYTE	Yrev	: 3;
	BYTE	YM		: 1;
	BYTE	X		: 4;
}StatusVac6K;

typedef union tagStatusParam16K{
	BYTE		bStatus;
	StatusVac6K	status;
}StatusParam16K;


//const	BYTE	byte_opcStatus2	= 0x21;		// ���ط���״̬
//	����1��0b ABCD EFGH ��1Ϊ����0Ϊ�أ�
//		A:CCD��B:TMP��C:CCG��D:V5��E:V4��F:V3��G:V2��H:V1
//	����2��0b ABCD EFGH ��1Ϊ����0Ϊ�أ�
//		H:V1�ض�
//		ABCDEFG����

typedef struct tagStatusValve6K{
	WORD	V1		: 1;	// 00000000 0000000?
	WORD	V2		: 1;	// 00000000 000000?0
	WORD	V3		: 1;	// 00000000 00000?00
	WORD	V4		: 1;	// 00000000 0000?000
	WORD	V5		: 1;	// 00000000 000?0000
	WORD	CCG		: 1;	// 00000000 00?00000
	WORD	TMP		: 1;	// 00000000 0?000000
	WORD	CCD		: 1;	// 00000000 ?0000000
	WORD	V1Rec	: 1;	// 0000000? 00000000
	WORD	Rev		: 7;	// ???????0 00000000
}StatusValve6K;

typedef union tagStatusParam26K
{
	WORD			wStatus;
	StatusValve6K	status;	
}StatusParam26K;

typedef struct tagVACstatus6K_Board{
	BYTE	byteVent;	// 0x01����Ʒ�ҷ�����	0x02������ǹ����
	BYTE	bytePump;	// 0x01�������գ�		0x02��������
	BYTE	byteV1;		// 0x01���򿪣�			0x02���ر�

	StatusParam16K	Vac;
	StatusParam26K	Valve;
}VACstatus6K_Board;

typedef struct tagVACstatus6K_CCG{
	double	dVac;
}VACvalue6K_CCG;

typedef struct tagVACstatus6K_TC{
	float	fTC3;	// ��ѹ�ض�ֵ
	float	fTC4;	// ��ѹ�ض�ֵ
}VACvalue6K_TC;

typedef struct tagVACstatus6K_Thresholds{
	float	fCCG1;	// ���ֵTorr
	float	fCCG2;	// ���ֵTorr
	float	fTC3;	// ��ѹֵV
	float	fTC4;	// ��ѹֵV
	float	fTC3Min;
	float	fTC3Max;
	float	fTC4Min;
	float	fTC4Max;
}VACvalue6K_Thresholds;

#endif
