#pragma once
#include "afxwin.h"

// CPenSettingDlg 对话框

class CPenSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPenSettingDlg)

public:
	CPenSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPenSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PEN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxFontSize;
	CComboBox m_ComboBoxLineWidth;
	BOOL m_bCheckNoTips;
	CIni *m_pIni;
	CFont m_Font;
	COLORREF m_colorText;
	int      m_nFontSize;
	int      m_nLineWidth;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStnClickedStaticColor();

	void SetConfig(CIni *pIni);
	int GetFontSize() {return m_nFontSize;}
	int GetLineWidth() {return m_nLineWidth;}
	COLORREF GetTextColor() {return m_colorText;}
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCbnSelchangeComboFontSize();
	afx_msg void OnCbnSelchangeComboLineWidth();
	afx_msg void OnCbnEditchangeComboFontSize();
	afx_msg void OnCbnEditchangeComboLineWidth();
	afx_msg void OnPaint();
};
