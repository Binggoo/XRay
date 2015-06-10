#pragma once


// CStepSetPath 对话框

class CStepSetPath : public CPropertyPage
{
	DECLARE_DYNAMIC(CStepSetPath)

public:
	CStepSetPath(BOOL bOffset);
	virtual ~CStepSetPath();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_STEP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strEditOffsetPath;
	CString m_strEditOffsetFileName;
	CString m_strEditGainPath;
	CString m_strEditGainFileName;

	CIni *m_pIni;
	BOOL m_bOffset;
public:
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();

	void SetConfig(CIni *pIni) {m_pIni = pIni;}
	afx_msg void OnBnClickedBtnOffsetBrowse();
	afx_msg void OnBnClickedBtnGainBrowse();
};
