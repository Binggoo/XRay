#pragma once
#include "afxwin.h"


// CFilterEnhanceDlg 对话框

class CFilterEnhanceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterEnhanceDlg)

public:
	CFilterEnhanceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilterEnhanceDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENHANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxMaskWidth;
	CComboBox m_ComboBoxMaskHeight;
	CComboBox m_ComboBoxFactor;

	long m_lMaskWidth;
	long m_lMaskHeight;
	double m_dbFactor;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	long GetMaskWith() {return m_lMaskWidth;}
	long GetMaskHeight() {return m_lMaskHeight;}
	double GetFactor() {return m_dbFactor;}
};
