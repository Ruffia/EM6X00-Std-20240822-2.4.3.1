// ScenarioManageDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "resource.h"
#include "ScenarioManageDlg.h"
#include "afxdialogex.h"
#include "ControlLayer.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
using namespace pugi;
#include "ControlLayer.h"
#include "SemCtrl.h"
#include "CCPS3603tatusDetector.h"
#include "SpellmanStatusDetector.h"
#include "SemCoreAssemble.h"
#include "XXX2.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"


bool Save2Scenario(const CString& fileName, const CScenario& o)
{
	xml_document doc;
	//xml声明部分
	xml_node declaration_node = doc.append_child(pugi::node_declaration);
	declaration_node.append_attribute("version") = "1.0";
	declaration_node.append_attribute("encoding") = "UTF-8";

	pugi::xml_node root = doc.append_child("root"); //根节点
	//root.append_attribute("id") = "S1"; //根节点属性

	xml_node node = root.append_child("Settings");
	node.append_attribute("SampleName") = o.szSampleName;
	node.append_attribute("CreateTime") = o.szCreateTime;
	node.append_attribute("AuthorName") = o.szAuthorName;
	node.append_attribute("Description") = o.szDescription;

	{
		xml_node ChildNode = node.append_child("HVType");
		ChildNode.append_attribute("Value") = o.HVType;
		ChildNode.append_attribute("Unit") = "";
	}

	{
		xml_node ChildNode = node.append_child("HV");
		ChildNode.append_attribute("Value") = o.HV;
		ChildNode.append_attribute("Unit") = "V";
	}

	{
		xml_node ChildNode = node.append_child("Bias");
		ChildNode.append_attribute("Value") = o.Bias;
		ChildNode.append_attribute("Unit") = o.HVType == 1?"V":"mA";
	}

	{
		xml_node ChildNode = node.append_child("FilamentCurrent");
		ChildNode.append_attribute("Value") = o.FilamentCurrent;
		ChildNode.append_attribute("Unit") = "A";
	}


	{
		xml_node ChildNode = node.append_child("Contrast");
		ChildNode.append_attribute("Value") = o.Contrast;
		ChildNode.append_attribute("Unit") = "";
	}

	{
		xml_node ChildNode = node.append_child("Brightness");
		ChildNode.append_attribute("Value") = o.Brightness;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("CondenserLens");
		ChildNode.append_attribute("Value") = o.CondenserLens;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("ObjectiveLens");
		ChildNode.append_attribute("Value") = o.ObjectiveLens;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("Magnifier");
		ChildNode.append_attribute("Value") = o.Magnifier;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("BeemAlignmentX");
		ChildNode.append_attribute("Value") = o.BeemAlignmentX;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("BeemAlignmentY");
		ChildNode.append_attribute("Value") = o.BeemAlignmentY;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("StigmatorX");
		ChildNode.append_attribute("Value") = o.StigmatorX;
		ChildNode.append_attribute("Unit") = "";
	}


	{
		xml_node ChildNode = node.append_child("StigmatorY");
		ChildNode.append_attribute("Value") = o.StigmatorY;
		ChildNode.append_attribute("Unit") = "";
	}

	doc.save_file((LPSTR)(LPCTSTR)fileName);
	return true;
}


// ScenarioManageDlg 对话框
bool Read2Scenario(const CString& fileName, CScenario& o)
{
	DWORD dwErr = 0;
	xml_document doc;
	if (!doc.load_file((LPSTR)(LPCTSTR)fileName)) { //加载xml文件
		dwErr = GetLastError();
		return false;
	}

	xml_node root = doc.child("root");  //根节点
	xml_node nodeSettings = root.child("Settings");

	sprintf_s(o.szSampleName,"%s",nodeSettings.attribute("SampleName").as_string(""));
	sprintf_s(o.szCreateTime,"%s",nodeSettings.attribute("CreateTime").as_string(""));
	sprintf_s(o.szAuthorName,"%s",nodeSettings.attribute("AuthorName").as_string(""));
	sprintf_s(o.szDescription,"%s",nodeSettings.attribute("Description").as_string(""));

	{
		xml_node ChildNode = nodeSettings.child("HVType");
		o.HVType = ChildNode.attribute("Value").as_int(0);
	}

	{
		xml_node ChildNode = nodeSettings.child("HV");
		o.HV = ChildNode.attribute("Value").as_double(0.0);
	}

	{
		xml_node ChildNode = nodeSettings.child("Bias");
		o.Bias = ChildNode.attribute("Value").as_double(0.0);
	}

	{
		xml_node ChildNode = nodeSettings.child("FilamentCurrent");
		o.FilamentCurrent = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("Contrast");
		o.Contrast = ChildNode.attribute("Value").as_double(0.0);
	}

	{
		xml_node ChildNode = nodeSettings.child("Brightness");
		o.Brightness = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("CondenserLens");
		o.CondenserLens = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("ObjectiveLens");
		o.ObjectiveLens = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("Magnifier");
		o.Magnifier = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("BeemAlignmentX");
		o.BeemAlignmentX = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("BeemAlignmentY");
		o.BeemAlignmentY = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("StigmatorX");
		o.StigmatorX = ChildNode.attribute("Value").as_double(0.0);
	}


	{
		xml_node ChildNode = nodeSettings.child("StigmatorY");
		o.StigmatorY = ChildNode.attribute("Value").as_double(0.0);
	}

	return true;
}


IMPLEMENT_DYNAMIC(ScenarioManageDlg, CDialog)
ScenarioManageDlg::ScenarioManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ScenarioManageDlg::IDD, pParent)
	, m_strSampleName(_T(""))
	, m_strAuthorName(_T(""))
	, m_strCreateTime(_T(""))
	, m_strDescription(_T(""))
	, m_strHVType(_T(""))
	, m_dHV(0)
	, m_dFilamentCurrent(0)
	, m_dBias(0)
	, m_dBrightness(0)
	, m_dContrast(0)
	, m_dConsenderLens(0)
	, m_ObjectiveLens(0)
	, m_dMagnifier(0)
	, m_dBeemAlignmentX(0)
	, m_dBeemAlignmentY(0)
	, m_dStigmatorX(0)
	, m_dStigmatorY(0)
{

}

ScenarioManageDlg::~ScenarioManageDlg()
{
}

void ScenarioManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SmapleName, m_strSampleName);
	DDX_Text(pDX, IDC_EDIT_AuthorName, m_strAuthorName);
	DDX_Text(pDX, IDC_EDIT_Description, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_HVType, m_strHVType);
	DDX_Text(pDX, IDC_EDIT_HV, m_dHV);
	DDX_Text(pDX, IDC_EDIT_GunFilament, m_dFilamentCurrent);
	DDX_Text(pDX, IDC_EDIT_BIAS, m_dBias);
	DDX_Text(pDX, IDC_EDIT_Brightness, m_dBrightness);
	DDX_Text(pDX, IDC_EDIT_Contrast, m_dContrast);
	DDX_Text(pDX, IDC_EDIT_ConsenderLens, m_dConsenderLens);
	DDX_Text(pDX, IDC_EDIT_ObjectiveLens, m_ObjectiveLens);
	DDX_Text(pDX, IDC_EDIT_Magnifier, m_dMagnifier);
	DDX_Text(pDX, IDC_EDIT_BeemAlignmentX, m_dBeemAlignmentX);
	DDX_Text(pDX, IDC_EDIT_Contrast2, m_dBeemAlignmentY);
	DDX_Text(pDX, IDC_EDIT_ConsenderLens2, m_dStigmatorX);
	DDX_Text(pDX, IDC_EDIT_ObjectiveLens2, m_dStigmatorY);
}


BEGIN_MESSAGE_MAP(ScenarioManageDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_Scenario_Load, &ScenarioManageDlg::OnBnClickedBtnScenarioLoad)
	ON_BN_CLICKED(ID_BTN_Scenario_Save, &ScenarioManageDlg::OnBnClickedBtnScenarioSave)
	ON_BN_CLICKED(ID_BTN_Scenario_Apply, &ScenarioManageDlg::OnBnClickedBtnScenarioApply)
	ON_BN_CLICKED(ID_BTN_Scenario_OK, &ScenarioManageDlg::OnBnClickedDlgOk)
END_MESSAGE_MAP()

BOOL ScenarioManageDlg::OnInitDialog()
{
	Localize();
	Initialize();

	if (CControlLayer::Instance().m_pScenario->HVType < 0)
	{
	}
	else
	{
		GetDlgItem(IDC_STATIC_BIAS)->SetWindowTextA("束流");
		GetDlgItem(IDC_STATIC_BIAS_UNIT)->SetWindowTextA("uA");	
	}
	UpdateData(FALSE);
	return TRUE;
}

// ScenarioManageDlg 消息处理程序
void ScenarioManageDlg::Initialize()
{
	CScenario* pScenario = CControlLayer::Instance().m_pScenario;
	if(!pScenario) return;

	m_strHVType = CControlLayer::Instance().m_pScenario->HVType?"Spellman":"CPS_3603";
	m_dHV = pScenario->HV;
	m_dBias = pScenario->Bias;
	m_dFilamentCurrent = pScenario->FilamentCurrent;

	m_dBrightness = pScenario->Brightness;
	m_dContrast = pScenario->Contrast;
	m_dMagnifier = pScenario->Magnifier;

	m_dConsenderLens = pScenario->CondenserLens;
	m_ObjectiveLens = pScenario->ObjectiveLens;	

	m_dBeemAlignmentX = pScenario->BeemAlignmentX;
	m_dBeemAlignmentY = pScenario->BeemAlignmentY;

	m_dStigmatorX = pScenario->StigmatorX;
	m_dStigmatorY = pScenario->StigmatorY;

	m_Scenario = *pScenario;
}

void ScenarioManageDlg::OnBnClickedBtnScenarioLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile = _T("");
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.xml)|*.xml|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		m_Scenario.Reset();
		Read2Scenario(strFile,m_Scenario);
		_UpdateUIFromScenario();
		UpdateData(FALSE);
	}
}


void ScenarioManageDlg::OnBnClickedBtnScenarioSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if ("" == m_strSampleName)
	{
		MessageBox("样品名称不能为空");
		return;
	}

	sprintf_s(m_Scenario.szSampleName,256,"%s",m_strSampleName.GetBuffer());
	sprintf_s(m_Scenario.szAuthorName,64,"%s",m_strAuthorName.GetBuffer());
	m_strCreateTime = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	sprintf_s(m_Scenario.szCreateTime,64,"%s",m_strCreateTime.GetBuffer());
	sprintf_s(m_Scenario.szDescription,64,"%s",m_strDescription.GetBuffer());	 

	char exeFullPath[255] = {0};
	GetModuleFileNameA(NULL,exeFullPath,MAX_PATH);
	char path[255] = {0};
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';

	CString sPath(path);
	CString defaultDir = sPath + "\\Scenario";
	CString fileName = m_strSampleName + ".xml";
	CString filter = "文件 (*.xml)|*.xml||";
	CFileDialog openFileDlg(FALSE, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if(result == IDOK) 
	{
		filePath = openFileDlg.GetPathName();
		Save2Scenario(filePath,m_Scenario);
	}
}


void ScenarioManageDlg::_UpdateUIFromScenario()
{
	m_strSampleName = m_Scenario.szSampleName;
	m_strAuthorName = m_Scenario.szAuthorName;
	m_strCreateTime = m_Scenario.szCreateTime;
	m_strDescription = m_Scenario.szDescription;

	m_strHVType = m_Scenario.HVType?"Spellman":"CPS_3603";
	m_dHV = m_Scenario.HV;
	m_dBias = m_Scenario.Bias;
	m_dFilamentCurrent = m_Scenario.FilamentCurrent;

	m_dBrightness = m_Scenario.Brightness;
	m_dContrast = m_Scenario.Contrast;
	m_dMagnifier = m_Scenario.Magnifier;

	m_dConsenderLens = m_Scenario.CondenserLens;
	m_ObjectiveLens = m_Scenario.ObjectiveLens;	

	m_dBeemAlignmentX = m_Scenario.BeemAlignmentX;
	m_dBeemAlignmentY = m_Scenario.BeemAlignmentY;

	m_dStigmatorX = m_Scenario.StigmatorX;
	m_dStigmatorY = m_Scenario.StigmatorY;
}

void ScenarioManageDlg::OnBnClickedBtnScenarioApply()
{
	// TODO: 在此添加控件通知处理程序代码
	_Apply_HV();
	_Apply_Bias();
	_Apply_FilamentCurrent();
	_Apply_Brightness();
	_Apply_Contrast();
	_Apply_ConsenderLens();
	_Apply_ObjectiveLens();
	_Apply_Magnifier();
	_Apply_BeemAlignment();
	_Apply_Stigmator();
	OnOK();
}


void ScenarioManageDlg::_Apply_HV() 
{
	if (CControlLayer::Instance().m_pScenario->HVType < 0)
	{
		CCPS3603tatusDetector::Instance().SetParam(10,(int)m_dHV * 1000);
		CCPS3603tatusDetector::Instance().SetParam(13,TRUE);
	}
	else
	{
		CSpellmanStatusDetector::Instance().SetParam( 72, (int)m_dHV * 10);
	}
}


void ScenarioManageDlg::_Apply_Bias() 
{
	if (CControlLayer::Instance().m_pScenario->HVType < 0)
	{
		long lPos = CSemCoreAssemble::Instance().m_CPS_Bias.GetPosByValue(m_dBias);

		VARIANT ClassID;
		ClassID.lVal = SemControlPanelClass_CPS_Bias;

		VARIANT SerialNumber;
		SerialNumber.lVal = -1;

		VARIANT Xpos;
		Xpos.lVal = lPos;

		VARIANT Ypos;
		Ypos.lVal = 0;

		VARIANT Flag;
		Flag.lVal = 1;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
	}
	else
	{
		long lPos = CSemCoreAssemble::Instance().m_SP_Bias.GetPosByValue(m_dBias);
		CSpellmanStatusDetector::Instance().SetParam( 33, lPos *10 );

		VARIANT ClassID;
		ClassID.lVal = SemControlPanelClass_SP_Bias;

		VARIANT SerialNumber;
		SerialNumber.lVal = -1;

		VARIANT Xpos;
		Xpos.lVal = lPos;

		VARIANT Ypos;
		Ypos.lVal = 0;

		VARIANT Flag;
		Flag.lVal = 1;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
	}
}


void ScenarioManageDlg::_Apply_FilamentCurrent() 
{
	if (CControlLayer::Instance().m_pScenario->HVType < 0)
	{
		CCPS3603tatusDetector::Instance().SetParam(12,int(m_dFilamentCurrent * 1000));
		CCPS3603tatusDetector::Instance().SetParam(14,TRUE);
	}
	else
	{
		CSpellmanStatusDetector::Instance().SetParam(74, m_dFilamentCurrent * 1000);
	}
}


void ScenarioManageDlg::_Apply_Brightness() 
{
	long lPos = CSemCoreAssemble::Instance().m_Brightness.GetPosByValue(m_dBrightness);

	VARIANT ClassID;
	ClassID.lVal = SemControlPanelClass_Brightness;

	VARIANT SerialNumber;
	SerialNumber.lVal = -1;

	VARIANT Xpos;
	Xpos.lVal = lPos;

	VARIANT Ypos;
	Ypos.lVal = 0;

	VARIANT Flag;
	Flag.lVal = 1;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


void ScenarioManageDlg::_Apply_Contrast() 
{
	if (CControlLayer::Instance().m_pScenario->HVType < 0)
	{
		long lPos = CSemCoreAssemble::Instance().m_Contrast.GetPosByValue(m_dContrast);

		VARIANT ClassID;
		ClassID.lVal = SemControlPanelClass_Contrast;

		VARIANT SerialNumber;
		SerialNumber.lVal = -1;

		VARIANT Xpos;
		Xpos.lVal = lPos;

		VARIANT Ypos;
		Ypos.lVal = 0;

		VARIANT Flag;
		Flag.lVal = 1;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
	}
	else
	{
		long lPos = CSemCoreAssemble::Instance().m_SP_PMT.GetPosByValue(m_dContrast);
		CSpellmanStatusDetector::Instance().SetParam( 35, lPos *10 );

		VARIANT ClassID;
		ClassID.lVal = SemControlPanelClass_SP_PMT;

		VARIANT SerialNumber;
		SerialNumber.lVal = -1;

		VARIANT Xpos;
		Xpos.lVal = lPos;

		VARIANT Ypos;
		Ypos.lVal = 0;

		VARIANT Flag;
		Flag.lVal = 1;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
	}
}


void ScenarioManageDlg::_Apply_ConsenderLens() 
{
	long lPos = CSemCoreAssemble::Instance().m_CondenserLens.GetPosByValue(m_dConsenderLens);

	VARIANT ClassID;
	ClassID.lVal = SemControlPanelClass_CondenserLens;

	VARIANT SerialNumber;
	SerialNumber.lVal = -1;

	VARIANT Xpos;
	Xpos.lVal = lPos;

	VARIANT Ypos;
	Ypos.lVal = 0;

	VARIANT Flag;
	Flag.lVal = 1;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


void ScenarioManageDlg::_Apply_ObjectiveLens() 
{
	long lPos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPosByValue(m_ObjectiveLens);

	VARIANT ClassID;
	ClassID.lVal = SemControlPanelClass_ObjectiveLens;

	VARIANT SerialNumber;
	SerialNumber.lVal = -1;

	VARIANT Xpos;
	Xpos.lVal	= lPos;

	VARIANT Ypos;
	Ypos.lVal = 0;

	VARIANT Flag;
	Flag.lVal = 1;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


void ScenarioManageDlg::_Apply_Magnifier() 
{
	VARIANT ClassID;
	ClassID.lVal = SemControlPanelClass_Magnifier;

	VARIANT SerialNumber;
	SerialNumber.lVal = -1;

	VARIANT Xpos;
	Xpos.dblVal	= m_dMagnifier;

	VARIANT Ypos;
	Ypos.lVal = 0;

	VARIANT Flag;
	Flag.lVal = 1;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


void ScenarioManageDlg::_Apply_BeemAlignment() 
{
	VARIANT ClassID;
	ClassID.lVal = SemControlPanelClass_BeemAlignment;

	VARIANT SerialNumber;
	SerialNumber.lVal = -1;

	VARIANT Xpos;
	Xpos.lVal	= (long)m_dBeemAlignmentX;

	VARIANT Ypos;
	Ypos.lVal = (long)m_dBeemAlignmentY;

	VARIANT Flag;
	Flag.lVal = 0x01|0x02;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


void ScenarioManageDlg::_Apply_Stigmator() 
{
	VARIANT ClassID;
	ClassID.lVal = SemControlPanelClass_Stigmator;

	VARIANT SerialNumber;
	SerialNumber.lVal = -1;

	VARIANT Xpos;
	Xpos.lVal	= m_dStigmatorX;

	VARIANT Ypos;
	Ypos.lVal = m_dStigmatorY;

	VARIANT Flag;
	Flag.lVal = 0x01|0x02;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}

void ScenarioManageDlg::OnBnClickedDlgOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void ScenarioManageDlg::Localize()
{
	//SetWindowText(GetResString(IDS_DLG_Scenario_Caption));
	GetDlgItem(IDC_DLG_STATIC_SampleName)->SetWindowText(GetResString(IDS_DLG_STATIC_SampleName));
	GetDlgItem(IDC_DLG_STATIC_AuthorName)->SetWindowText(GetResString(IDS_DLG_STATIC_AuthorName));
	GetDlgItem(IDC_DLG_STATIC_Description)->SetWindowText(GetResString(IDS_DLG_STATIC_Description));
	GetDlgItem(IDC_DLG_STATIC_PowerType)->SetWindowText(GetResString(IDS_DLG_STATIC_PowerType));
	GetDlgItem(IDC_DLG_STATIC_HV)->SetWindowText(GetResString(IDS_DLG_STATIC_HV));
	GetDlgItem(IDC_DLG_STATIC_Filament)->SetWindowText(GetResString(IDS_DLG_STATIC_Filament));
	GetDlgItem(IDC_DLG_STATIC_Contrast)->SetWindowText(GetResString(IDS_DLG_STATIC_Contrast));
	GetDlgItem(IDC_DLG_STATIC_Brightness)->SetWindowText(GetResString(IDS_DLG_STATIC_Brightness));
	GetDlgItem(IDC_DLG_STATIC_ConsenderLens)->SetWindowText(GetResString(IDS_DLG_STATIC_ConsenderLens));
	GetDlgItem(IDC_DLG_STATIC_ObjectiveLens)->SetWindowText(GetResString(IDS_DLG_STATIC_ObjectiveLens));
	GetDlgItem(IDC_DLG_STATIC_Magnifier)->SetWindowText(GetResString(IDS_DLG_STATIC_Magnifier));
	GetDlgItem(IDC_DLG_STATIC_BeamAligmentX)->SetWindowText(GetResString(IDS_DLG_STATIC_BeamAligmentX));
	GetDlgItem(IDC_DLG_STATIC_BeamAligmentY)->SetWindowText(GetResString(IDS_DLG_STATIC_BeamAligmentY));
	GetDlgItem(IDC_DLG_STATIC_StigmatorX)->SetWindowText(GetResString(IDS_DLG_STATIC_StigmatorX));
	GetDlgItem(IDC_DLG_STATIC_StigmatorY)->SetWindowText(GetResString(IDS_DLG_STATIC_StigmatorY));

	GetDlgItem(ID_BTN_Scenario_Load)->SetWindowText(GetResString(IDS_BTN_Scenario_Load));
	GetDlgItem(ID_BTN_Scenario_Save)->SetWindowText(GetResString(IDS_BTN_Scenario_Save));
	GetDlgItem(ID_BTN_Scenario_Apply)->SetWindowText(GetResString(IDS_BTN_Scenario_Apply));
	GetDlgItem(ID_BTN_Scenario_OK)->SetWindowText(GetResString(IDS_BTN_Scenario_OK));
}