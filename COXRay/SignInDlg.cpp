// SignInDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "SignInDlg.h"
#include "afxdialogex.h"
#include "../common/XorCode.h"

// CSignInDlg �Ի���

IMPLEMENT_DYNAMIC(CSignInDlg, CDialogEx)

CSignInDlg::CSignInDlg(CMyDatabase *pUserDatabase,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignInDlg::IDD, pParent)
{
	m_pUserDatabase = pUserDatabase;
	m_bHideListCtrl = TRUE;
}

CSignInDlg::~CSignInDlg()
{
}

void CSignInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USER_TYPE, m_ComboBoxUserType);
	DDX_Control(pDX, IDC_LIST_USER, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CSignInDlg, CDialogEx)
	ON_STN_CLICKED(IDC_TEXT_VIEW_USER, &CSignInDlg::OnStnClickedTextViewUser)
	ON_WM_SETCURSOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USER, &CSignInDlg::OnNMRClickListUser)
	ON_COMMAND(ID_USER_HIDE, &CSignInDlg::OnUserHide)
	ON_COMMAND(ID_USER_DELETE, &CSignInDlg::OnUserDelete)
	ON_BN_CLICKED(IDOK, &CSignInDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSignInDlg ��Ϣ�������


BOOL CSignInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ASSERT(m_pUserDatabase);

	m_ComboBoxUserType.AddString(_T("����Ա�û�"));
	m_ComboBoxUserType.AddString(_T("��ͨ�û�"));

	m_ComboBoxUserType.SetCurSel(1);

	InitialListCtrl();

	// ����ListCtrl
	CRect rectWnd,rectListCtrl;
	GetWindowRect(&rectWnd);
	m_ListCtrl.GetWindowRect(&rectListCtrl);

	m_ListCtrl.ShowWindow(SW_HIDE);

	SetWindowPos(NULL,0,0,rectWnd.Width(),rectWnd.Height() - rectListCtrl.Height(),SWP_NOMOVE | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSignInDlg::InitialListCtrl()
{
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);

	int nWidth = rect.Width() - 120;

	m_ListCtrl.InsertColumn(0,_T("user_name"),LVCFMT_LEFT,nWidth / 2);
	m_ListCtrl.InsertColumn(1,_T("user_type"),LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(2,_T("password"),LVCFMT_LEFT,nWidth / 2);

	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	// �������
	int nCount = m_pUserDatabase->GetAllUser();

	CString strUserName,strUserType,strPassword;
	unsigned int len = 255;
	for (int i = 0; i < nCount; i++)
	{
		strUserName = CString(m_pUserDatabase->GetStringValue(i,0,&len));

		int nIndex = m_pUserDatabase->GetIntValue(i,1);
		m_ComboBoxUserType.GetLBText(nIndex,strUserType);

		strPassword = CString(m_pUserDatabase->GetStringValue(i,2,&len));

		strPassword = CXorCode::Decode(strPassword);

		m_ListCtrl.InsertItem(i,strUserName);
		m_ListCtrl.SetItemText(i,1,strUserType);
		m_ListCtrl.SetItemText(i,2,strPassword);
	}
}


void CSignInDlg::OnStnClickedTextViewUser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_bHideListCtrl)
	{
		CRect rectWnd,rectListCtrl;
		GetWindowRect(&rectWnd);
		m_ListCtrl.GetWindowRect(&rectListCtrl);

		rectWnd.bottom += rectListCtrl.Height();

		MoveWindow(rectWnd);

		m_ListCtrl.ShowWindow(SW_SHOW);

		m_bHideListCtrl = FALSE;
	}
	

}


BOOL CSignInDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pWnd)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_TEXT_VIEW_USER:
			SetCursor(LoadCursor(NULL,IDC_HAND));  
			return TRUE; 
		}
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CSignInDlg::OnNMRClickListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos),HIWORD(dwPos));

	CMenu menu;

	menu.LoadMenu(IDR_MENU_USER);

	CMenu *popue = menu.GetSubMenu(0);

	if (pNMItemActivate->iItem == -1)
	{
		popue->DeleteMenu(ID_USER_DELETE,MF_BYCOMMAND);
	}

	popue->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
	*pResult = 0;
}


void CSignInDlg::OnUserHide()
{
	// TODO: �ڴ���������������
	if (!m_bHideListCtrl)
	{
		CRect rectWnd,rectListCtrl;
		GetWindowRect(&rectWnd);
		m_ListCtrl.GetWindowRect(&rectListCtrl);

		rectWnd.bottom -= rectListCtrl.Height();

		MoveWindow(rectWnd);

		m_ListCtrl.ShowWindow(SW_HIDE);

		m_bHideListCtrl = TRUE;
	}
	
}

void CSignInDlg::OnUserDelete()
{
	// TODO: �ڴ���������������
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		int nItem = m_ListCtrl.GetNextSelectedItem(pos);

		CString strUserName = m_ListCtrl.GetItemText(nItem,0);

		if (m_pUserDatabase->DeleteUser(strUserName))
		{
			m_ListCtrl.DeleteItem(nItem);
		}
	}
}


void CSignInDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strUserName,strPassword,strConfirmPassword;
	GetDlgItemText(IDC_EDIT_USERNAME,strUserName);
	GetDlgItemText(IDC_EDIT_PASSWORD,strPassword);
	GetDlgItemText(IDC_EDIT_PASSWORD_CONFIRM,strConfirmPassword);

	strUserName.Trim();

	if (strUserName.IsEmpty())
	{
		AfxMessageBox(_T("�û�������Ϊ�գ�"));

		GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();

		return;
	}

	if (strPassword != strConfirmPassword)
	{
		AfxMessageBox(_T("���벻��ͬ��������ȷ�ϣ�"));
		GetDlgItem(IDC_EDIT_PASSWORD_CONFIRM)->SetFocus();

		return;
	}

	strPassword = CXorCode::Encode(strPassword);

	UserInfo user;
	user.strUserName = strUserName;
	user.nUserType = m_ComboBoxUserType.GetCurSel();
	user.strPassword = strPassword;

	if (m_pUserDatabase->AddUser(user))
	{
		AfxMessageBox(_T("ע���û��ɹ���"));
	}
	else
	{
		AfxMessageBox(_T("ע���û�ʧ�ܣ�"));
	}

	CDialogEx::OnOK();
}
