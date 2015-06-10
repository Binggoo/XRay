// CommSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "CommSettingDlg.h"
#include "afxdialogex.h"


// CCommSettingDlg �Ի���

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


// CCommSettingDlg ��Ϣ�������


BOOL CCommSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ASSERT(m_pIni);
	CString strIPAddr = m_pIni->GetString(_T("CommSetting"),_T("IPAddress"));
	m_nEditPort = m_pIni->GetUInt(_T("CommSetting"),_T("PortNum"),8000);
	m_bCheckAutoConnect = m_pIni->GetBool(_T("CommSetting"),_T("En_AutoConnect"),TRUE);

	m_IPAddressCtrl.SetWindowText(strIPAddr);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCommSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CString strIPAddr;
	m_IPAddressCtrl.GetWindowText(strIPAddr);

	m_pIni->WriteString(_T("CommSetting"),_T("IPAddress"),strIPAddr);
	m_pIni->WriteUInt(_T("CommSetting"),_T("PortNum"),m_nEditPort);
	m_pIni->WriteBool(_T("CommSetting"),_T("En_AutoConnect"),m_bCheckAutoConnect);

	CDialogEx::OnOK();
}
