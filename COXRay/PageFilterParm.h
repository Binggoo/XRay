#pragma once
#include "afxcmn.h"
#include "HistogramDlg.h"

// CPageFilterParm 对话框

class CPageFilterParm : public CDialogEx
{
	DECLARE_DYNAMIC(CPageFilterParm)

public:
	CPageFilterParm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageFilterParm();

// 对话框数据
	enum { IDD = IDD_PAGE_FILTER_PARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CRect m_rect;
	CMarkup *m_pXml;
	DWORD m_dwCurrentPos;
	void ChangeSize(CWnd *pWnd,int cx,int cy,DWORD flag);
	void InitialListCtrl();
	void UpdateListCtrl();
	void Refresh();

	CHistogramDlg *m_pHistoDlg;
	BOOL m_bLoacte;
	HRegion m_hRegionLoacte;

public:
	CListCtrl m_ListCtrlInpectAreas;
	double m_dbEditVolKV;
	double m_dbEditCurrentmA;
	UINT m_nEditErrorValue;
	double m_dbPerPixel;
	CComboBox m_ComboBoxUnits;

	void SetConfig(CMarkup *pXml) {m_pXml = pXml;}
	void SetCurrentPos(DWORD dwCurrentPos) {m_dwCurrentPos = dwCurrentPos;}

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeltaposSpinVoltage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCurrent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnLast();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnDeleteArea();
	afx_msg void OnBnClickedBtnAddRange();
	afx_msg void OnBnClickedBtnSetLight();
	afx_msg void OnBnClickedBtnViewArea();
	afx_msg void OnBnClickedBtnAddArea();
	afx_msg void OnBnClickedBtnInspect();
protected:
	afx_msg LRESULT OnAddGrayRange(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnLocate();
	afx_msg void OnDeltaposSpinError(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListAreas(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSnap();
	afx_msg void OnBnClickedBtnCalibration();
};
