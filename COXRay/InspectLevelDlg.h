#pragma once


// CInspectLevelDlg 对话框

class CInspectLevelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInspectLevelDlg)

public:
	CInspectLevelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInspectLevelDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INSPECT_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCheckLevel1;
	BOOL m_bCheckLevel2;
	BOOL m_bCheckLevel3;
	BOOL m_bCheckLevel4;
	BOOL m_bCheckLevel5;
	BOOL m_bCheckLevel6;
	BOOL m_bCheckLevel7;
	BOOL m_bCheckLevel8;

	double m_dbEditSingleLevel1;
	double m_dbEditSingleLevel2;
	double m_dbEditSingleLevel3;
	double m_dbEditSingleLevel4;
	double m_dbEditSingleLevel5;
	double m_dbEditSingleLevel6;
	double m_dbEditSingleLevel7;
	double m_dbEditSingleLevel8;

	UINT m_nEditPercentLevel1;
	UINT m_nEditPercentLevel2;
	UINT m_nEditPercentLevel3;
	UINT m_nEditPercentLevel4;
	UINT m_nEditPercentLevel5;
	UINT m_nEditPercentLevel6;
	UINT m_nEditPercentLevel7;
	UINT m_nEditPercentLevel8;

	UINT m_nEditAreaWidth;
	UINT m_nEditAreaHeight;

	CMarkup *m_pXml;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void SetConfig(CMarkup *pXml) {m_pXml = pXml;}
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin12(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin13(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin14(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin15(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin16(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin17(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin18(NMHDR *pNMHDR, LRESULT *pResult);
};
