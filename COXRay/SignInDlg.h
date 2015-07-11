#pragma once
#include "afxcmn.h"
#include "MyDatabase.h"

// CSignInDlg 对话框

class CSignInDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSignInDlg)

public:
	CSignInDlg(CMyDatabase *pUserDatabase,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSignInDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SIGNIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxUserType;
	CListCtrl m_ListCtrl;

	CMyDatabase *m_pUserDatabase;
	BOOL m_bHideListCtrl;

	void InitialListCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedTextViewUser();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNMRClickListUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUserHide();
	afx_msg void OnUserDelete();
	afx_msg void OnBnClickedOk();
};
