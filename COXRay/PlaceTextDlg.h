#pragma once
#include "afxwin.h"


// CPlaceTextDlg 对话框

class CPlaceTextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlaceTextDlg)

public:
	CPlaceTextDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlaceTextDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bCheckBoxBold;
	BOOL m_bCheckBoxItalic;
	BOOL m_bCheckBoxUnderLine;
	BOOL m_bCheckBoxStrikeOut;

	CComboBox m_ComboBoxFontSize;
	CIni *m_pIni;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CFont m_Font;
	COLORREF m_colorText;
	CString m_strEditText;
	afx_msg void OnStnClickedStaticColor();
	afx_msg void OnBnClickedCheckBold();
	afx_msg void OnBnClickedCheckItalic();
	afx_msg void OnBnClickedCheckUnderline();
	afx_msg void OnBnClickedCheckStrikeout();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	void SetConfig(CIni *pIni) {m_pIni = pIni;}
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboFontSize();
	afx_msg void OnCbnEditchangeComboFontSize();
};
