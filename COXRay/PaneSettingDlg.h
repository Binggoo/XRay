#pragma once
#include "afxwin.h"

#define MAX_PANE_MODE 10
// CPaneSettingDlg 对话框

class CPaneSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneSettingDlg)

public:
	CPaneSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaneSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PANE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxPaneMode;
	UINT m_nInteralTimer;
	BOOL m_bCheckOffsetCorr;
	BOOL m_bCheckGainOffsetCorr;
	BOOL m_bCheckBadPixcelCorr;
	CString m_strEditOffsetFile;
	CString m_strEditGainOffsetFile;
	CString m_strEditBadPixcelFile;

	CIni *m_pIni;

	double m_dbPaneMode[MAX_PANE_MODE];
	int m_nPaneModeCount;
	int m_nSelectPaneMode;

	CString GetFileName(CString &strPath);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnBrowseOffset();
	afx_msg void OnBnClickedBtnBrowseGainOffset();
	afx_msg void OnBnClickedBtnBrowseBadPixcel();

	void SetConfig(CIni *pIni);
	void SetPaneMode(double *dbPaneMode,int count);
};
