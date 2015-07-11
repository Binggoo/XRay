// LoginDlg.cpp : ʵ���ļ�
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

// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ASSERT(m_pUserDatabase);

	// ��ѯ�����û���
	m_nUserCount = m_pUserDatabase->GetAllUser();

	for (int i = 0; i < m_nUserCount;i++)
	{
		unsigned int len = 255;
		CString strUser = CString(m_pUserDatabase->GetStringValue(i,0,&len));

		m_ComboBoxUser.AddString(strUser);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strUserName,strPassword;
	m_ComboBoxUser.GetWindowText(strUserName);
	GetDlgItemText(IDC_EDIT_PASSWORD,strPassword);

	// ����ǳ����û�ֱ�ӵ�¼
	if (strUserName == ROOT_USER && strPassword == ROOT_PASSWORD)
	{
		strPassword = CXorCode::Encode(strPassword);

		m_User.strUserName = strUserName;
		m_User.nUserType = 0;
		m_User.strPassword = strPassword;

		CDialogEx::OnOK();

		return;
	}

	// �����û�
	if (m_nUserCount == 0)
	{
		if (strUserName == DEFAULT_USER && strPassword == DEFAULT_PASSWORD)
		{
			// ���Ĭ�Ϲ���Ա�û�
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
			AfxMessageBox(_T("�û��������ڣ�"));
			return;
		}
		else
		{
			AfxMessageBox(_T("�������"));
			return;
		}
		
	}
	else
	{
		// ��ѯ�û��Ƿ����
		int nCount = m_pUserDatabase->GetUserInfo(strUserName,&m_User);

		if (nCount == 0)
		{
			AfxMessageBox(_T("�û��������ڣ�"));
			return;	
		}
		else
		{
			// �����Ƿ���ȷ
			CString strConfirmPassword = CXorCode::Decode(m_User.strPassword);
			if (strConfirmPassword != strPassword)
			{
				AfxMessageBox(_T("�������"));
				return;
			}
		}
	}
	CDialogEx::OnOK();
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
