// ModifyPasswordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "ModifyPasswordDlg.h"
#include "afxdialogex.h"
#include "../common/XorCode.h"

// CModifyPasswordDlg �Ի���

IMPLEMENT_DYNAMIC(CModifyPasswordDlg, CDialogEx)

CModifyPasswordDlg::CModifyPasswordDlg(CMyDatabase *pDatabase,CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyPasswordDlg::IDD, pParent)
	, m_strEditUserName(_T(""))
	, m_strEditOldPassword(_T(""))
	, m_strNewPassword(_T(""))
	, m_strEditConfirmPassword(_T(""))
{
	m_pUserDatabase = pDatabase;
}

CModifyPasswordDlg::~CModifyPasswordDlg()
{
}

void CModifyPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strEditUserName);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWORD, m_strEditOldPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_strNewPassword);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_strEditConfirmPassword);
}


BEGIN_MESSAGE_MAP(CModifyPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModifyPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyPasswordDlg ��Ϣ�������


BOOL CModifyPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CModifyPasswordDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UserInfo userInfo;

	int nCount = m_pUserDatabase->GetUserInfo(m_strEditUserName,&userInfo);

	if (nCount == 0)
	{
		AfxMessageBox(_T("���û������ڣ�"));
	}
	else
	{
		CString strPassword = CXorCode::Decode(userInfo.strPassword);
		if (strPassword != m_strEditOldPassword)
		{
			AfxMessageBox(_T("ԭʼ����������������룡"));
			GetDlgItem(IDC_EDIT_OLD_PASSWORD)->SetFocus();

			return;
		}

		if (m_strNewPassword != m_strEditConfirmPassword)
		{
			AfxMessageBox(_T("ǰ�����벻һ�£�������ȷ�ϣ�"));

			GetDlgItem(IDC_EDIT_PASSWORD_CONFIRM)->SetFocus();

			return;
		}

		m_strNewPassword = CXorCode::Encode(m_strNewPassword);

		if (m_pUserDatabase->UpdateUser(m_strEditUserName,m_strNewPassword))
		{
			AfxMessageBox(_T("�����޸ĳɹ��������µ�¼��"));

			::PostMessage(GetParent()->GetSafeHwnd(),WM_COMMAND,ID_USER_LOGIN,0);
		}
	}

	CDialogEx::OnOK();
}
