#pragma once


// CThresholdDlg 对话框

class CThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThresholdDlg)

public:
	CThresholdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThresholdDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_THRESHOLD };

public:
	UINT GetMinGray() {return m_nEditMinGray;}
	UINT GetMaxGray() {return m_nEditMaxGray;}
	void SetMinGrayValue(UINT nMinValue) {m_nEditActualMinGray = nMinValue;} 
	void SetMaxGrayValue(UINT nMaxValue) {m_nEditActualMaxGray = nMaxValue;} 
	void SetMeanGrayValue(UINT nMeanValue) {m_nEditActualMeanGray = nMeanValue;} 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	UINT m_nEditMinGray;
	UINT m_nEditMaxGray;
	UINT m_nEditActualMinGray;
	UINT m_nEditActualMaxGray;
	UINT m_nEditActualMeanGray;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
