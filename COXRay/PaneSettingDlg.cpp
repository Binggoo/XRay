// PaneSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PaneSettingDlg.h"
#include "afxdialogex.h"


// CPaneSettingDlg 对话框

IMPLEMENT_DYNAMIC(CPaneSettingDlg, CDialogEx)

CPaneSettingDlg::CPaneSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPaneSettingDlg::IDD, pParent)
	, m_nInteralTimer(0)
	, m_bCheckOffsetCorr(FALSE)
	, m_bCheckGainOffsetCorr(FALSE)
	, m_bCheckBadPixcelCorr(FALSE)
	, m_strEditOffsetFile(_T(""))
	, m_strEditGainOffsetFile(_T(""))
	, m_strEditBadPixcelFile(_T(""))
{
	m_pIni = NULL;
}

CPaneSettingDlg::~CPaneSettingDlg()
{
}

void CPaneSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PANE_MODE, m_ComboBoxPaneMode);
	DDX_Text(pDX, IDC_EDIT_INTERAL_TIME, m_nInteralTimer);
	DDX_Check(pDX, IDC_CHECK_OFFSET, m_bCheckOffsetCorr);
	DDX_Check(pDX, IDC_CHECK_GAIN_OFFSET, m_bCheckGainOffsetCorr);
	DDX_Check(pDX, IDC_CHECK_BAD_PIXCEL, m_bCheckBadPixcelCorr);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_strEditOffsetFile);
	DDX_Text(pDX, IDC_EDIT_GAIN_OFFSET, m_strEditGainOffsetFile);
	DDX_Text(pDX, IDC_EDIT_BAD_PIXCEL, m_strEditBadPixcelFile);
}


BEGIN_MESSAGE_MAP(CPaneSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPaneSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BROWSE_OFFSET, &CPaneSettingDlg::OnBnClickedBtnBrowseOffset)
	ON_BN_CLICKED(IDC_BTN_BROWSE_GAIN_OFFSET, &CPaneSettingDlg::OnBnClickedBtnBrowseGainOffset)
	ON_BN_CLICKED(IDC_BTN_BROWSE_BAD_PIXCEL, &CPaneSettingDlg::OnBnClickedBtnBrowseBadPixcel)
END_MESSAGE_MAP()


// CPaneSettingDlg 消息处理程序


BOOL CPaneSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);

	m_nSelectPaneMode = m_pIni->GetInt(_T("PaneSetting"),_T("PaneMode"),0);

	if (m_nSelectPaneMode < 0 || m_nSelectPaneMode > m_nPaneModeCount)
	{
		m_nSelectPaneMode = 0;
	}

	m_nInteralTimer = m_pIni->GetUInt(_T("PaneSetting"),_T("InteralTimer"),0);

	if (m_nInteralTimer == 0)
	{
		m_nInteralTimer = (UINT)(m_dbPaneMode[m_nSelectPaneMode]/1000);
	}

	m_bCheckOffsetCorr = m_pIni->GetBool(_T("PaneSetting"),_T("En_OffsetCorr"),FALSE);
	m_bCheckGainOffsetCorr = m_pIni->GetBool(_T("PaneSetting"),_T("En_GainCorr"),FALSE);
	m_bCheckBadPixcelCorr = m_pIni->GetBool(_T("PaneSetting"),_T("En_PixelCorr"),FALSE);

	m_strEditOffsetFile = m_pIni->GetString(_T("PaneSetting"),_T("OffsetPath"));
	m_strEditGainOffsetFile= m_pIni->GetString(_T("PaneSetting"),_T("GainPath"));
	m_strEditBadPixcelFile = m_pIni->GetString(_T("PaneSetting"),_T("PixelPath"));

	CString strCombobox;
	for (int i = 0; i < m_nPaneModeCount;i++)
	{
		strCombobox.Format(_T("Timing%i %dms"),i,(UINT)(m_dbPaneMode[i]/1000));
		m_ComboBoxPaneMode.AddString(strCombobox);
	}

	if (m_ComboBoxPaneMode.GetCount() > 0)
	{
		m_ComboBoxPaneMode.SetCurSel(m_nSelectPaneMode);
	}

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPaneSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_nSelectPaneMode = m_ComboBoxPaneMode.GetCurSel();
	m_pIni->WriteInt(_T("PaneSetting"),_T("PaneMode"),m_nSelectPaneMode);
	 m_pIni->WriteUInt(_T("PaneSetting"),_T("InteralTimer"),m_nInteralTimer);

	m_pIni->WriteBool(_T("PaneSetting"),_T("En_OffsetCorr"),m_bCheckOffsetCorr);
	m_pIni->WriteBool(_T("PaneSetting"),_T("En_GainCorr"),m_bCheckGainOffsetCorr);
	m_pIni->WriteBool(_T("PaneSetting"),_T("En_PixelCorr"),m_bCheckBadPixcelCorr);

	m_pIni->WriteString(_T("PaneSetting"),_T("OffsetPath"),m_strEditOffsetFile);
	m_pIni->WriteString(_T("PaneSetting"),_T("GainPath"),m_strEditGainOffsetFile);
	m_pIni->WriteString(_T("PaneSetting"),_T("PixelPath"),m_strEditBadPixcelFile);
	CDialogEx::OnOK();
}


void CPaneSettingDlg::OnBnClickedBtnBrowseOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetFileName(m_strEditOffsetFile);
	UpdateData(FALSE);
}


void CPaneSettingDlg::OnBnClickedBtnBrowseGainOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetFileName(m_strEditGainOffsetFile);
	UpdateData(FALSE);
}


void CPaneSettingDlg::OnBnClickedBtnBrowseBadPixcel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetFileName(m_strEditBadPixcelFile);
	UpdateData(FALSE);
}

void CPaneSettingDlg::SetConfig( CIni *pIni )
{
	m_pIni = pIni;
}

void CPaneSettingDlg::SetPaneMode( double *dbPaneMode,int count )
{
	ASSERT(count <= MAX_PANE_MODE);

	for (int i = 0;i < count;i++)
	{
		m_dbPaneMode[i] = dbPaneMode[i];
	}

	m_nPaneModeCount = count;
}

CString CPaneSettingDlg::GetFileName( CString &strPath )
{
	CString strInitialDir = CUtils::GetFilePathWithoutName(strPath);

	CFileDialog dlg(TRUE,_T("his"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("HIS File(*.his)|*.his||"));

	dlg.m_ofn.lpstrInitialDir = strInitialDir.GetBuffer();

	if (dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetPathName();

		return strPath;
	}

	return _T("");
}
