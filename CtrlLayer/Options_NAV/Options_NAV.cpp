// Options_NAV.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Options_NAV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// COptions_NAVApp

BEGIN_MESSAGE_MAP(COptions_NAVApp, CWinApp)
END_MESSAGE_MAP()


// COptions_NAVApp ����

COptions_NAVApp::COptions_NAVApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pPanelNAV	= NULL;
}


// Ψһ��һ�� COptions_NAVApp ����

COptions_NAVApp theApp;


// COptions_NAVApp ��ʼ��

BOOL COptions_NAVApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int COptions_NAVApp::ExitInstance() 
{
	if( m_pPanelNAV != NULL )
	{
		delete m_pPanelNAV;
		m_pPanelNAV = NULL;
	}
	return CWinApp::ExitInstance();
}
