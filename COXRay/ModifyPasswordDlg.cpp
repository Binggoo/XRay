// ModifyPasswordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "ModifyPasswordDlg.h"
#include "afxdialogex.h"
#include "../common/XorCode.h"

// CModifyPasswordDlg 对话框

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


// CModifyPasswordDlg 消息处理程序


BOOL CModifyPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CModifyPasswordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UserInfo userInfo;

	int nCount = m_pUserDatabase->GetUserInfo(m_strEditUserName,&userInfo);

	if (nCount == 0)
	{
		AfxMessageBox(_T("此用户不存在！"));
	}
	else
	{
		CString strPassword = CXorCode::Decode(userInfo.strPassword);
		if (strPassword != m_strEditOldPassword)
		{
			AfxMessageBox(_T("原始密码错误，请重新输入！"));
			GetDlgItem(IDC_EDIT_OLD_PASSWORD)->SetFocus();

			return;
		}

		if (m_strNewPassword != m_strEditConfirmPassword)
		{
			AfxMessageBox(_T("前后密码不一致，请重新确认！"));

			GetDlgItem(IDC_EDIT_PASSWORD_CONFIRM)->SetFocus();

			return;
		}

		m_strNewPassword = CXorCode::Encode(m_strNewPassword);

		if (m_pUserDatabase->UpdateUser(m_strEditUserName,m_strNewPassword))
		{
			AfxMessageBox(_T("密码修改成功，请重新登录！"));

			::PostMessage(GetParent()->GetSafeHwnd(),WM_COMMAND,ID_USER_LOGIN,0);
		}
	}

	CDialogEx::OnOK();
}
