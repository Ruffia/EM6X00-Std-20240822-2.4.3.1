// AIOUSB.h : Header file for AIOUSB.dll
//
//For Borland C++ Builder, use CBAIOUSB.lib
//For Microsoft Visual C++, use VCAIOUSB.lib

#ifdef __cplusplus
extern "C" {
#endif



#define diFirst 0xFFFFFFFE
#define diOnly 0xFFFFFFFD

#define TIME_METHOD_NOW 0
#define TIME_METHOD_WAIT_INPUT_ENABLE 86
#define TIME_METHOD_NOW_AND_ABORT 5
#define TIME_METHOD_WHEN_INPUT_ENABLE 1



__declspec(dllimport) unsigned long GetDevices(void);
__declspec(dllimport) unsigned long QueryDeviceInfo(unsigned long DeviceIndex, unsigned long *pPID, unsigned long *pNameSize, char *pName, unsigned long *pDIOBytes, unsigned long *pCounters);
__declspec(dllimport) unsigned long ClearDevices(void);
__declspec(dllimport) unsigned long GetDeviceUniqueStr(unsigned long DeviceIndex, unsigned long *pUIDSize, char *pUID);
__declspec(dllimport) unsigned long GetDeviceSerialNumber(unsigned long DeviceIndex, unsigned __int64 *pSerialNumber);
__declspec(dllimport) unsigned long ResolveDeviceIndex(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long GetDeviceHandle(unsigned long DeviceIndex, unsigned long *pHandle);


__declspec(dllimport) unsigned long AIOUSB_ClearFIFO(unsigned long DeviceIndex, unsigned long TimeMethod);
__declspec(dllimport) unsigned long AIOUSB_GetStreamStatus(unsigned long DeviceIndex, unsigned long *Status);
__declspec(dllimport) unsigned long AIOUSB_SetStreamingBlockSize(unsigned long DeviceIndex, unsigned long BlockSize);

__declspec(dllimport) unsigned long CustomEEPROMRead(unsigned long DeviceIndex, unsigned long StartAddress, unsigned long *DataSize, void *pData);
__declspec(dllimport) unsigned long CustomEEPROMWrite(unsigned long DeviceIndex, unsigned long StartAddress, unsigned long DataSize, void *pData);

__declspec(dllimport) unsigned long GenericVendorRead(unsigned long DeviceIndex, unsigned char Request, unsigned short Value, unsigned short Index, unsigned long *pDataSize, void *pData);
__declspec(dllimport) unsigned long GenericVendorWrite(unsigned long DeviceIndex, unsigned char Request, unsigned short Value, unsigned short Index, unsigned long DataSize, void *pData);



__declspec(dllimport) unsigned long ADC_GetImmediate(unsigned long DeviceIndex, unsigned long Channel, unsigned short *pBuf);
__declspec(dllimport) unsigned long ADC_GetScan(unsigned long DeviceIndex, unsigned short *pBuf);
__declspec(dllimport) unsigned long ADC_GetScanV(unsigned long DeviceIndex, double *pBuf);
__declspec(dllimport) unsigned long ADC_GetChannelV(unsigned long DeviceIndex, unsigned long ChannelIndex, double *pBuf);

__declspec(dllimport) unsigned long ADC_GetConfig(unsigned long DeviceIndex, void *pConfigBuf, unsigned long *ConfigBufSize);
__declspec(dllimport) unsigned long ADC_SetConfig(unsigned long DeviceIndex, void *pConfigBuf, unsigned long *ConfigBufSize);
__declspec(dllimport) unsigned long ADC_ADMode(unsigned long DeviceIndex, unsigned char TriggerMode, unsigned char CalMode);
__declspec(dllimport) unsigned long ADC_Range1(unsigned long DeviceIndex, unsigned long ADChannel, unsigned char GainCode, unsigned long bSingleEnded);
__declspec(dllimport) unsigned long ADC_RangeAll(unsigned long DeviceIndex, unsigned char *pGainCodes, unsigned long bSingleEnded);
__declspec(dllimport) unsigned long ADC_SetScanLimits(unsigned long DeviceIndex, unsigned long StartChannel, unsigned long EndChannel);
__declspec(dllimport) unsigned long ADC_SetOversample(unsigned long DeviceIndex, unsigned char Oversample);
__declspec(dllimport) unsigned long ADC_QueryCal(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long ADC_SetCal(unsigned long DeviceIndex, char *CalFileName);
__declspec(dllimport) unsigned long ADC_Initialize(unsigned long DeviceIndex, void *pConfigBuf, unsigned long *ConfigBufSize, char *CalFileName);

__declspec(dllimport) unsigned long ADC_BulkAcquire(unsigned long DeviceIndex, unsigned long BufSize, void *pBuf);
__declspec(dllimport) unsigned long ADC_BulkPoll(unsigned long DeviceIndex, unsigned long *BytesLeft);



__declspec(dllimport) unsigned long CTR_8254Load(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned short LoadValue);
__declspec(dllimport) unsigned long CTR_8254Mode(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned long Mode);
__declspec(dllimport) unsigned long CTR_8254ModeLoad(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned long Mode, unsigned short LoadValue);
__declspec(dllimport) unsigned long CTR_8254Read(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned short *pReadValue);
__declspec(dllimport) unsigned long CTR_8254ReadStatus(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned short *pReadValue, unsigned char *pStatus);
__declspec(dllimport) unsigned long CTR_8254ReadModeLoad(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned long Mode, unsigned short LoadValue, unsigned short *pReadValue);

__declspec(dllimport) unsigned long CTR_StartOutputFreq(unsigned long DeviceIndex, unsigned long CounterIndex, double *pHz);

__declspec(dllimport) unsigned long CTR_8254ReadAll(unsigned long DeviceIndex, unsigned short *pData);

__declspec(dllimport) unsigned long CTR_8254SelectGate(unsigned long DeviceIndex, unsigned long GateIndex);
__declspec(dllimport) unsigned long CTR_8254ReadLatched(unsigned long DeviceIndex, unsigned short *pData);
__declspec(dllimport) unsigned long CTR_SetWaitGates(unsigned long DeviceIndex, unsigned char A, unsigned char B);
__declspec(dllimport) unsigned long CTR_EndWaitGates(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long CTR_WaitForGate(unsigned long DeviceIndex, unsigned char GateIndex, unsigned short *pContent);

__declspec(dllimport) unsigned long CTR_StartMeasuringPulseWidth(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long CTR_StopMeasuringPulseWidth(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long CTR_GetPulseWidthMeasurement(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned short *pReadValue);



__declspec(dllimport) unsigned long DIO_Configure(unsigned long DeviceIndex, unsigned char bTristate, void *pOutMask, void *pData);
__declspec(dllimport) unsigned long DIO_ConfigureEx(unsigned long DeviceIndex, void *pOutMask, void *pData, void *pTristateMask);
__declspec(dllimport) unsigned long DIO_ConfigurationQuery(unsigned long DeviceIndex, void *pOutMask, void *pTristateMask);

__declspec(dllimport) unsigned long DIO_Read1(unsigned long DeviceIndex, unsigned long BitIndex, unsigned char *Buffer);
__declspec(dllimport) unsigned long DIO_Read8(unsigned long DeviceIndex, unsigned long ByteIndex, unsigned char *pBuffer);
__declspec(dllimport) unsigned long DIO_ReadAll(unsigned long DeviceIndex, void *Buffer);

__declspec(dllimport) unsigned long DIO_Write1(unsigned long DeviceIndex, unsigned long BitIndex, unsigned char bData);
__declspec(dllimport) unsigned long DIO_Write8(unsigned long DeviceIndex, unsigned long ByteIndex, unsigned char Data);
__declspec(dllimport) unsigned long DIO_WriteAll(unsigned long DeviceIndex, void *pData);



__declspec(dllimport) unsigned long DIO_StreamOpen(unsigned long DeviceIndex, unsigned long bIsRead);
__declspec(dllimport) unsigned long DIO_StreamClose(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long DIO_StreamFrame(unsigned long DeviceIndex, unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred);
__declspec(dllimport) unsigned long DIO_StreamSetClocks(unsigned long DeviceIndex, double *ReadClockHz, double *WriteClockHz);

__declspec(dllimport) unsigned long DIO_SPI_Read(unsigned long DeviceIndex, unsigned char Address, unsigned char Reg, unsigned char * pValue);
__declspec(dllimport) unsigned long DIO_SPI_Write(unsigned long DeviceIndex, unsigned char Address, unsigned char Reg, unsigned char Value);



__declspec(dllimport) unsigned long DACDirect(unsigned long DeviceIndex, unsigned long Channel, unsigned short Counts);
__declspec(dllimport) unsigned long DACMultiDirect(unsigned long DeviceIndex, void *pDACData, unsigned long DACDataCount);

__declspec(dllimport) unsigned long DACOutputOpen(unsigned long DeviceIndex, double *ClockHz);
__declspec(dllimport) unsigned long DACOutputClose(unsigned long DeviceIndex, unsigned long bWait);
__declspec(dllimport) unsigned long DACOutputCloseNoEnd(unsigned long DeviceIndex, unsigned long bWait);
__declspec(dllimport) unsigned long DACOutputFrame(unsigned long DeviceIndex, unsigned long FramePoints, unsigned short *FrameData);
__declspec(dllimport) unsigned long DACOutputFrameRaw(unsigned long DeviceIndex, unsigned long FramePoints, unsigned short *FrameData);
__declspec(dllimport) unsigned long DACOutputSetCount(unsigned long DeviceIndex, unsigned long NewCount);
__declspec(dllimport) unsigned long DACOutputStart(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long DACOutputSetInterlock(unsigned long DeviceIndex, unsigned long bInterlock);

__declspec(dllimport) unsigned long DACOutputAbort(unsigned long DeviceIndex);
__declspec(dllimport) unsigned long DACOutputStatus(unsigned long DeviceIndex);



#ifdef __cplusplus
}
#endif
