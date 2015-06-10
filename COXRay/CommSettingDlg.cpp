// CommSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "CommSettingDlg.h"
#include "afxdialogex.h"


// CCommSettingDlg 对话框

IMPLEMENT_DYNAMIC(CCommSettingDlg, CDialogEx)

CCommSettingDlg::CCommSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommSettingDlg::IDD, pParent)
	, m_nEditPort(0)
	, m_bCheckAutoConnect(FALSE)
{
	m_pIni = NULL;
}

CCommSettingDlg::~CCommSettingDlg()
{
}

void CCommSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddressCtrl);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nEditPort);
	DDX_Check(pDX, IDC_CHECK_AUTO_CONNECT, m_bCheckAutoConnect);
}


BEGIN_MESSAGE_MAP(CCommSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCommSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCommSettingDlg 消息处理程序


BOOL CCommSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);
	CString strIPAddr = m_pIni->GetString(_T("CommSetting"),_T("IPAddress"));
	m_nEditPort = m_pIni->GetUInt(_T("CommSetting"),_T("PortNum"),8000);
	m_bCheckAutoConnect = m_pIni->GetBool(_T("CommSetting"),_T("En_AutoConnect"),TRUE);

	m_IPAddressCtrl.SetWindowText(strIPAddr);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCommSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strIPAddr;
	m_IPAddressCtrl.GetWindowText(strIPAddr);

	m_pIni->WriteString(_T("CommSetting"),_T("IPAddress"),strIPAddr);
	m_pIni->WriteUInt(_T("CommSetting"),_T("PortNum"),m_nEditPort);
	m_pIni->WriteBool(_T("CommSetting"),_T("En_AutoConnect"),m_bCheckAutoConnect);

	CDialogEx::OnOK();
}
