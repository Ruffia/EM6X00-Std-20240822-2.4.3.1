#pragma once

class CCommonVACFunction
{
public:
	static CCommonVACFunction& Instance();

	int ConvertComData22( CString strIn, char* data );
	int ConvertCommData_Win2( CString strWin, char cType, CString strData, char* data );
	int AnalyzeRecvData( CString strIn, BOOL bSameChn, CString& strOut, double& dValue, int& nExp );

	double ScientificToDouble(CString strVal);
	double ScientificToDouble(CString strVal, double& dValue, int& nExp);

	int ConvertReceiveData( CString strData, CString& strOut );
	int ConvertReceiveData2( CString strIn, CString& strOut );

	int ConvertReceiveData_Win( CString strIn, CString& strOut );
	int GetFieldLength_Win( CString strWin );

protected:
	CCommonVACFunction();

};