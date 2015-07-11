#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CMyDatabase *pUserData,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
