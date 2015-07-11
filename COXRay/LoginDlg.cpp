// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "../common/XorCode.h"

#define DEFAULT_USER _T("admin")
#define DEFAULT_PASSWORD _T("admin")

#define ROOT_USER _T("root")
#define ROOT_PASSWORD _T("root123")

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CMyDatabase *pUserDatabase,CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_pUserDatabase = pUserDatabase;
	m_nUserCount = 0;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_ComboBoxUser);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pUserDatabase);

	// 查询所有用户名
	m_nUserCount = m_pUserDatabase->GetAllUser();

	for (int i = 0; i < m_nUserCount;i++)
	{
		unsigned int len = 255;
		CString strUser = CString(m_pUserDatabase->GetStringValue(i,0,&len));

		m_ComboBoxUser.AddString(strUser);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strUserName,strPassword;
	m_ComboBoxUser.GetWindowText(strUserName);
	GetDlgItemText(IDC_EDIT_PASSWORD,strPassword);

	// 如果是超级用户直接登录
	if (strUserName == ROOT_USER && strPassword == ROOT_PASSWORD)
	{
		strPassword = CXorCode::Encode(strPassword);

		m_User.strUserName = strUserName;
		m_User.nUserType = 0;
		m_User.strPassword = strPassword;

		CDialogEx::OnOK();

		return;
	}

	// 其他用户
	if (m_nUserCount == 0)
	{
		if (strUserName == DEFAULT_USER && strPassword == DEFAULT_PASSWORD)
		{
			// 添加默认管理员用户
			strPassword = CXorCode::Encode(strPassword);

			m_User.strUserName = strUserName;
			m_User.nUserType = 0;
			m_User.strPassword = strPassword;

			if (!m_pUserDatabase->AddUser(m_User))
			{
				return;
			}
		}
		else if (strUserName == DEFAULT_USER)
		{
			AfxMessageBox(_T("用户名不存在！"));
			return;
		}
		else
		{
			AfxMessageBox(_T("密码错误！"));
			return;
		}
		
	}
	else
	{
		// 查询用户是否存在
		int nCount = m_pUserDatabase->GetUserInfo(strUserName,&m_User);

		if (nCount == 0)
		{
			AfxMessageBox(_T("用户名不存在！"));
			return;	
		}
		else
		{
			// 密码是否正确
			CString strConfirmPassword = CXorCode::Decode(m_User.strPassword);
			if (strConfirmPassword != strPassword)
			{
				AfxMessageBox(_T("密码错误！"));
				return;
			}
		}
	}
	CDialogEx::OnOK();
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
