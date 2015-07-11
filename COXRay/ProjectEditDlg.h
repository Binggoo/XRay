#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CProjectEditDlg 对话框

class CProjectEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectEditDlg)

public:
	CProjectEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectEditDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strEditProjectNumber;
	CString m_strEditProductName;
	int m_nEditLength;
	int m_nEditWidth;
	int m_nEditHeight;
	CComboBox m_ComboBoxCustomer;
	CComboBox m_ComboBoxInspectMode;
	CComboBox m_ComboBoxPos;
	CListCtrl m_ListCtrl;

	int m_nTotalPos;
	
	CMarkup *m_pXml;
	CIni m_IniCustomer;

	void IntialListCtrl();
	void UpdateListCtrl();
public:
	virtual BOOL OnInitDialog();
	void SetConfig(CMarkup *pXml) {m_pXml = pXml;}
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnInspectLevel();
};
