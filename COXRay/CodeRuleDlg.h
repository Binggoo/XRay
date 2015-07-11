#pragma once


// CCodeRuleDlg 对话框

class CCodeRuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCodeRuleDlg)

public:
	CCodeRuleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCodeRuleDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CODE_RULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CIni *m_pIni;

	CString m_strEditCodeRule;
	CString m_strEditSource;
	CString m_strEditDest;

public:
	afx_msg void OnEnChangeEditCodeRule();
	afx_msg void OnEnChangeEditSource();
	afx_msg void OnBnClickedOk();

	void SetConfig(CIni *pIni) {m_pIni = pIni;};

	virtual BOOL OnInitDialog();

	static CString CodeString(CString strSource,CString strCodeRule);
};
