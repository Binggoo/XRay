#pragma once
#include "afxwin.h"


// CFilterGaussDlg 对话框

class CFilterGaussDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterGaussDlg)

public:
	CFilterGaussDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilterGaussDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GAUSS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxFilterSize;
	long m_lFilterSize;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	long GetFilterSize() {return m_lFilterSize;}
};
