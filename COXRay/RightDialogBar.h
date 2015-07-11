#pragma once
#include "../Led/LedButton.h"
#include "../Meter/Meter.h"
#include "PageImgCapture.h"
#include "PageImageProcess.h"
#include "PageData.h"
#include "PageFilterParm.h"

// CRightDialogBar 对话框
#define PAGE_COUNT 3

class CRightDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CRightDialogBar)

public:
	CRightDialogBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRightDialogBar();

// 对话框数据
	enum { IDD = IDD_DIALOGBAR_RIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	int m_nCurSelPage;
	CRect m_rect;
	CMarkup *m_pXml;
	CIni *m_pIni;

	void ChangeSize(CWnd *pWnd, int cx,int cy,DWORD flag);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	CLedButton m_LedOnOff;
	CLedButton m_LedStat;
	CMeter     m_meterVol;
	CMeter     m_meterCurrent;
	CTabCtrl   m_tabMain;
	CDialogEx  *m_pPageDlg[PAGE_COUNT];
	CPageImgCapture m_PageImgCapture;
	CPageImageProcess m_PageImgProcess;
	//CPageData m_PageData;
	CPageFilterParm m_PageFilterParm;

	void SetConfig(CMarkup *pXml,CIni *pIni);
	void SetLightError(CString strError);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
