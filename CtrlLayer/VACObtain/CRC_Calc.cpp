#include "StdAfx.h"
#include "CRC_Calc.h"
/*
CRC���ɷ��Ͷ˼��㣬�����ڷ�����Ϣ���ĵ�β����������Ϣ���豸�����¼�����յ���Ϣ���ĵ�CRC���Ƚϼ���õ���CRC�Ƿ�����յ��������������߲���������������
У����ļ������ʽΪ(X16 + X15 + X2 + 1)������CRC16��ļ��㷽���ǣ�
1��Ԥ��1��16λ�ļĴ���Ϊʮ������FFFF����ȫΪ1�����ƴ˼Ĵ���ΪCRC�Ĵ�����
2���ѵ�һ��8λ���������� ����ͨѶ��Ϣ֡�ĵ�һ���ֽڣ���16λ��CRC�Ĵ����ĵ�8λ����򣬰ѽ������CRC�Ĵ�����
3����CRC�Ĵ�������������һ λ������λ����0����λ����������ƺ���Ƴ�λ��
4������Ƴ�λΪ0���ظ���3�����ٴ�����һλ����
	����Ƴ�λΪ1��CRC�Ĵ��������ʽA001��1010 0000 0000 0001���������(Modbus)
5���ظ�����3��4��ֱ������8�Σ���������8λ����ȫ�������˴���
6���ظ�����2������5������ͨѶ��Ϣ֡��һ���ֽڵĴ���
7������ͨѶ��Ϣ֡�����ֽڰ��������������ɺ󣬵õ���16λCRC�Ĵ����ĸߡ����ֽڽ��н�����
8�����õ���CRC�Ĵ������ݼ�Ϊ��CRC�롣
*/
/*
CRC16�����ı�׼�����¼��֣������ڸ����淶�У����㷨ԭ�����һ�£����������ݵ����������������죬�±߰���Щ��׼�Ĳ����г���������C���Ե��㷨ʵ�֡�
CRC16_CCITT��		����ʽx16+x12+x5+1��0x1021������ʼֵ0x0000����λ��ǰ����λ�ں󣬽����0x0000���
CRC16_CCITT_FALSE��	����ʽx16+x12+x5+1��0x1021������ʼֵ0xFFFF����λ�ں󣬸�λ��ǰ�������0x0000���
CRC16_XMODEM��		����ʽx16+x12+x5+1��0x1021������ʼֵ0x0000����λ�ں󣬸�λ��ǰ�������0x0000���
CRC16_X25��			����ʽx16+x12+x5+1��0x1021������ʼֵ0x0000����λ��ǰ����λ�ں󣬽����0xFFFF���
CRC16_MODBUS��		����ʽx16+x15+x2+1��0x8005������ʼֵ0xFFFF����λ��ǰ����λ�ں󣬽����0x0000���
CRC16_IBM��			����ʽx16+x15+x2+1��0x8005������ʼֵ0x0000����λ��ǰ����λ�ں󣬽����0x0000���
CRC16_MAXIM��		����ʽx16+x15+x2+1��0x8005������ʼֵ0x0000����λ��ǰ����λ�ں󣬽����0xFFFF���
CRC16_USB��			����ʽx16+x15+x2+1��0x8005������ʼֵ0xFFFF����λ��ǰ����λ�ں󣬽����0xFFFF���
*/
/*
����ʽ������
��x16+x12+x5+1
x16��ʾ��16λΪ1��x5��ʾ��5λΪ1
(1 << 16) | (1 << 12) | (1 << 5) | (1) = 0x11021
����CRC16ֻȡ��16λ��д��16���������� 0x1021

CRC16���㷨ԭ��
1.����CRC16�ı�׼ѡ���ֵCRCIn��ֵ��
2.�����ݵĵ�һ���ֽ���CRCIn��8λ���
3.�ж����λ������λΪ 0 ����һλ����Ϊ 1 ����һλ�������ʽHex�����
4.�ظ�3ֱ��8λȫ����λ���������
5.�ظ��������������ݲ���������ϲ��裬����16λ����16λCRCУ���롣
�����㷨ԭ�����׼Ҫ����ܼ򵥵�д���������
*/

void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
{
	int i;
	unsigned char tmp[4];
	tmp[0] = 0;
	for(i=0; i<8; i++)
	{  
		if(srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (7-i);
	}
	dBuf[0] = tmp[0];
}

void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf)
{
	int i;
	unsigned short tmp[4];
	tmp[0] = 0;
	for(i=0; i<16; i++)
	{
		if(srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}

void InvertUint32(unsigned int *dBuf, unsigned int *srcBuf)
{
	int i;
	unsigned int tmp[4];
	tmp[0] = 0;
	for(i=0; i<32; i++)
	{
		if(srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}

unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x1021;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin);
}

unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen)  
{  
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x1021;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}

unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x1021;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}

unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x1021;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin ^ 0xFFFF);
}

unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin);
}

unsigned short CRC16_MODBUS2(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0xA001;
	int i;
	for(; usDataLen>0; usDataLen--)					/* Step through bytes in memory */
	{
		wCRCin ^= (*puchMsg++);						/* Fetch byte from memory, XOR into CRC top byte*/
		for(i=0; i<8; i++)							/* Prepare to rotate 8 bits */
		{
			if(wCRCin & 0x0001)						/* b01 is set... */
				wCRCin = (wCRCin >> 1) ^ wCPoly;	/* rotate and XOR with polynomic */
			else									/* b01 is clear... */
				wCRCin >>= 1;						/* just rotate */
		}											/* Loop for 8 bits */
		wCRCin &= 0xFFFF;							/* Ensure CRC remains 16-bit value */
	}												/* Loop until num=0 */
	return (wCRCin);								/* Return updated CRC */
}

unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin);
}

unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{  
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin ^ 0xFFFF);
}  

unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;

	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for(int i=0; i<8; i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin ^ 0xFFFF);
}  

//������֤ʹ��������ߣ��ں�CRC�㷨�ļ��㣬�ͺ�ߵĲ������ᵽ�������㷨�Ĺ��ߺϼ�

//���ܽ����㷨���߼�

//�����������Ҳ����CRC32 ��У���㷨

//CRC32�㷨��  
unsigned int CRC32(unsigned char *puchMsg, unsigned int usDataLen)
{
	int i;
	unsigned int wCRCin = 0xFFFFFFFF;
	unsigned int wCPoly = 0x04C11DB7;
	unsigned int wChar = 0;
	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		InvertUint8((unsigned char *)&wChar, (unsigned char *)&wChar);
		wCRCin ^= (wChar << 24);
		for(i=0; i<8; i++)
		{
			if(wCRCin & 0x80000000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint32(&wCRCin, &wCRCin);
	return (wCRCin ^ 0xFFFFFFFF);
}
//����CRC32���ܻ��������Ķ���ʽ�ͳ�ʼֵ�ͽ��ֵ�Ƿ���Ҫ����Լ����������Ƿ���Ҫλ��ת��Ҫ����Դ�����޸�
