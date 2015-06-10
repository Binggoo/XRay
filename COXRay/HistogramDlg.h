#pragma once
#include "Histogram.h"

// CHistogramDlg 对话框

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GRAY_HISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	HRegion m_hRegion;
	HTuple  m_htAbsoluteHisto;
	int    m_nMinValue;
	int    m_nMaxValue;
	HWND   m_hParent;

	CHistogram m_Histo;

	void Refresh();
public:
	int m_nEditMinGray;
	int m_nEditMaxGray;

	void SetMinMaxValue(int nMinValue,int nMaxValue);
	void SetRegion(HRegion region);
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEditMinGray();
	afx_msg void OnEnChangeEditMaxGray();
	afx_msg void OnDeltaposSpinMinGray(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMaxGray(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnDefault();
};
