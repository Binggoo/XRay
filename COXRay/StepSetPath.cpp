// StepSetPath.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "StepSetPath.h"
#include "afxdialogex.h"


// CStepSetPath 对话框

IMPLEMENT_DYNAMIC(CStepSetPath, CPropertyPage)

CStepSetPath::CStepSetPath(BOOL bOffset)
	: CPropertyPage(CStepSetPath::IDD)
	, m_strEditOffsetPath(_T(""))
	, m_strEditOffsetFileName(_T(""))
	, m_strEditGainPath(_T(""))
	, m_strEditGainFileName(_T(""))
{
	m_bOffset = bOffset;
	m_pIni = NULL;
}

CStepSetPath::~CStepSetPath()
{
}

void CStepSetPath::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_PATH, m_strEditOffsetPath);
	DDX_Text(pDX, IDC_EDIT_OFFSET_FILE_NAME, m_strEditOffsetFileName);
	DDX_Text(pDX, IDC_EDIT_GAIN_PATH, m_strEditGainPath);
	DDX_Text(pDX, IDC_EDIT_GAIN_FILE_NAME, m_strEditGainFileName);
}


BEGIN_MESSAGE_MAP(CStepSetPath, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_OFFSET_BROWSE, &CStepSetPath::OnBnClickedBtnOffsetBrowse)
	ON_BN_CLICKED(IDC_BTN_GAIN_BROWSE, &CStepSetPath::OnBnClickedBtnGainBrowse)
END_MESSAGE_MAP()


// CStepSetPath 消息处理程序


BOOL CStepSetPath::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);

	CString strOffsetPath = m_pIni->GetString(_T("PaneSetting"),_T("OffsetPath"));
	CString strGainPath = m_pIni->GetString(_T("PaneSetting"),_T("GainPath"));

	m_strEditOffsetFileName = CUtils::GetFileName(strOffsetPath);
	m_strEditOffsetPath = CUtils::GetFilePathWithoutName(strOffsetPath);

	m_strEditGainFileName = CUtils::GetFileName(strGainPath);
	m_strEditGainPath = CUtils::GetFilePathWithoutName(strGainPath);

	if (m_bOffset)
	{
		GetDlgItem(IDC_GROUP_GAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_GAIN_PATH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN_PATH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_GAIN_FILE_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAIN_FILE_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_GAIN_BROWSE)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CStepSetPath::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	CString strOffsetPath,strGainPath;

	if (m_strEditOffsetPath.IsEmpty() || m_strEditOffsetFileName.IsEmpty()
		|| !PathFileExists(m_strEditOffsetPath))
	{
		AfxMessageBox(_T("路径不存在或为空"));
		return -1;
	}

	m_strEditOffsetPath.TrimRight(_T("\\"));

	strOffsetPath.Format(_T("%s\\%s"),m_strEditOffsetPath,m_strEditOffsetFileName);

	m_pIni->WriteString(_T("PaneSetting"),_T("OffsetPath"),strOffsetPath);

	if (!m_bOffset)
	{

		if (m_strEditGainPath.IsEmpty() || m_strEditGainFileName.IsEmpty()
			|| !PathFileExists(m_strEditGainPath))
		{
			AfxMessageBox(_T("路径不存在或为空"));
			return -1;
		}

		m_strEditGainPath.TrimRight(_T("\\"));

		strGainPath.Format(_T("%s\\%s"),m_strEditGainPath,m_strEditGainFileName);

		m_pIni->WriteString(_T("PaneSetting"),_T("GainPath"),strGainPath);
	}

	return CPropertyPage::OnWizardNext();
}


BOOL CStepSetPath::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent=(CPropertySheet*)GetParent(); 
	pParent->SetWizardButtons(PSWIZB_NEXT); 

	return CPropertyPage::OnSetActive();
}


void CStepSetPath::OnBnClickedBtnOffsetBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CFileDialog dlg(FALSE,_T("his"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("HIS Files(*.his)|*.his|All Files(*.*)|*.*||"));

	if (dlg.DoModal() != IDCANCEL)
	{
		m_strEditOffsetPath = CUtils::GetFilePathWithoutName(dlg.GetPathName());
		m_strEditOffsetFileName = dlg.GetFileName();

		UpdateData(FALSE);
	}
}


void CStepSetPath::OnBnClickedBtnGainBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CFileDialog dlg(FALSE,_T("his"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("HIS Files(*.his)|*.his|All Files(*.*)|*.*||"));

	if (dlg.DoModal() != IDCANCEL)
	{
		m_strEditGainPath = CUtils::GetFilePathWithoutName(dlg.GetPathName());
		m_strEditGainFileName = dlg.GetFileName();

		UpdateData(FALSE);
	}
}
