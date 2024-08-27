#include "StdAfx.h"
#include "CRC_Calc.h"
/*
CRC码由发送端计算，放置于发送信息报文的尾部。接收信息的设备再重新计算接收到信息报文的CRC，比较计算得到的CRC是否与接收到的相符，如果两者不相符，则表明出错。
校验码的计算多项式为(X16 + X15 + X2 + 1)。具体CRC16码的计算方法是：
1．预置1个16位的寄存器为十六进制FFFF（即全为1）；称此寄存器为CRC寄存器；
2．把第一个8位二进制数据 （既通讯信息帧的第一个字节）与16位的CRC寄存器的低8位相异或，把结果放于CRC寄存器；
3．把CRC寄存器的内容右移一 位（朝低位）用0填补最高位，并检查右移后的移出位；
4．如果移出位为0：重复第3步（再次右移一位）；
	如果移出位为1：CRC寄存器与多项式A001（1010 0000 0000 0001）进行异或；(Modbus)
5．重复步骤3和4，直到右移8次，这样整个8位数据全部进行了处理；
6．重复步骤2到步骤5，进行通讯信息帧下一个字节的处理；
7．将该通讯信息帧所有字节按上述步骤计算完成后，得到的16位CRC寄存器的高、低字节进行交换；
8．最后得到的CRC寄存器内容即为：CRC码。
*/
/*
CRC16常见的标准有以下几种，被用在各个规范中，其算法原理基本一致，就是在数据的输入和输出有所差异，下边把这些标准的差异列出，并给出C语言的算法实现。
CRC16_CCITT：		多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0x0000异或
CRC16_CCITT_FALSE：	多项式x16+x12+x5+1（0x1021），初始值0xFFFF，低位在后，高位在前，结果与0x0000异或
CRC16_XMODEM：		多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在后，高位在前，结果与0x0000异或
CRC16_X25：			多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或
CRC16_MODBUS：		多项式x16+x15+x2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0x0000异或
CRC16_IBM：			多项式x16+x15+x2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0x0000异或
CRC16_MAXIM：		多项式x16+x15+x2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或
CRC16_USB：			多项式x16+x15+x2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0xFFFF异或
*/
/*
多项式产生：
如x16+x12+x5+1
x16表示第16位为1，x5表示第5位为1
(1 << 16) | (1 << 12) | (1 << 5) | (1) = 0x11021
但是CRC16只取低16位，写成16进制数就是 0x1021

CRC16的算法原理：
1.根据CRC16的标准选择初值CRCIn的值。
2.将数据的第一个字节与CRCIn高8位异或。
3.判断最高位，若该位为 0 左移一位，若为 1 左移一位再与多项式Hex码异或。
4.重复3直至8位全部移位计算结束。
5.重复将所有输入数据操作完成以上步骤，所得16位数即16位CRC校验码。
根据算法原理与标准要求就能简单的写出具体程序：
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

//具体验证使用这个工具，内含CRC算法的计算，和后边的博客中提到的其他算法的工具合集

//加密解密算法工具集

//在这个基础上也加入CRC32 的校验算法

//CRC32算法：  
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
//对于CRC32可能还有其他的多项式和初始值和结果值是否需要异或以及输入数据是否需要位序倒转等要求在源码中修改
