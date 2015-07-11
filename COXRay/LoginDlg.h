#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
// CLoginDlg �Ի���

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CMyDatabase *pUserData,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxUser;

	CMyDatabase *m_pUserDatabase;

	UserInfo m_User;

	int m_nUserCount;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	UserInfo GetUser() {return m_User;};
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
