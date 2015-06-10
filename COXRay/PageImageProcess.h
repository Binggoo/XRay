#pragma once
#include "../common/BtnST.h"
#include "gamma.h"

// CPageImageProcess 对话框

class CPageImageProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CPageImageProcess)

public:
	CPageImageProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageImageProcess();

// 对话框数据
	enum { IDD = IDD_PAGE_IMG_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CRect m_rect;
	void ChangeSize(CWnd *pWnd,int cx,int cy,DWORD flag);
	BOOL m_bLBDown;

public:
	CButtonST m_BtnUndo;
	CButtonST m_BtnFitWindow;
	CButtonST m_BtnZoomIn;
	CButtonST m_BtnZoom11;
	CButtonST m_BtnZoomOut;
	CButtonST m_BtnRotateLeft;
	CButtonST m_BtnRotateRight;
	CButtonST m_BtnRotate;
	CButtonST m_BtnMirror;
	CButtonST m_BtnFlip;
	CButtonST m_BtnMove;
	CButtonST m_BtnSelect;
	CButtonST m_BtnZoom;
	CButtonST m_BtnLine;
	CButtonST m_BtnRect;
	CButtonST m_BtnEllipse;
	CButtonST m_BtnDist;
	CButtonST m_BtnDegree;
	CButtonST m_BtnText;
	CButtonST m_BtnNegative;
	CButtonST m_BtnThreshold;
	CButtonST m_BtnMean;
	CButtonST m_BtnMedian;
	CButtonST m_BtnGauss;
	CButtonST m_BtnEnhance;

	CGamma m_Gamma;
	CSliderCtrl m_SliderGamma;
	double m_dbEditGamma;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnGamma();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
