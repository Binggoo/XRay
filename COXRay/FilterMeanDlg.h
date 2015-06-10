#pragma once
#include "afxwin.h"


// CFilterMeanDlg 对话框

class CFilterMeanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterMeanDlg)

public:
	CFilterMeanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilterMeanDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxWidth;
	CComboBox m_ComboBoxHeight;

	long m_lMaskWidth;
	long m_lMaskHeight;
public:
	virtual BOOL OnInitDialog();
	long GetMaskWidth() {return m_lMaskWidth;}
	long GetMaskHeight() {return m_lMaskHeight;}
	afx_msg void OnBnClickedOk();
};
