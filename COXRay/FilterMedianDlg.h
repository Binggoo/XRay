#pragma once
#include "afxwin.h"


// CFilterMedianDlg 对话框

class CFilterMedianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterMedianDlg)

public:
	CFilterMedianDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilterMedianDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MEDIAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxMaskType;
	CComboBox m_ComboBoxMaskRadius;

	long m_lMaskRadius;
	int  m_iMaskType;
	BOOL m_bCheckNoRemind;

	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	int GetMaskType() {return m_iMaskType;}
	long GetMaskRadius() {return m_lMaskRadius;}

	void SetConfig(CIni *pIni) {m_pIni = pIni;};
};
