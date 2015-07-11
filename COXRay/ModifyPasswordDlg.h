#pragma once
#include "MyDatabase.h"

// CModifyPasswordDlg �Ի���

class CModifyPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPasswordDlg)

public:
	CModifyPasswordDlg(CMyDatabase *pDatabase,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyPasswordDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MODIFY_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strEditUserName;
	CString m_strEditOldPassword;
	CString m_strNewPassword;
	CString m_strEditConfirmPassword;

	CMyDatabase *m_pUserDatabase;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	void SetUserName(CString strUserName) {m_strEditUserName = strUserName;};
};
