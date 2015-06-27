#pragma once
#include "afxwin.h"
#include "../common/BtnST.h"

// CPageImgCapture 对话框

class CPageImgCapture : public CDialogEx
{
	DECLARE_DYNAMIC(CPageImgCapture)

public:
	CPageImgCapture(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageImgCapture();

// 对话框数据
	enum { IDD = IDD_PAGE_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void ChangeSize(CWnd *pWnd,int cx,int cy,DWORD flag);

	CRect m_rect;

public:
	CComboBox m_ComboBoxCtlMode;
	CComboBox m_ComboBoxFrames;
	CComboBox m_ComboBoxPosNum;

	CButtonST m_BtnStaticCap;
	CButtonST m_BtnContinueCap;
	CButtonST m_BtnPlcStart;
	CButtonST m_BtnPlcStop;
	CButtonST m_BtnView;
	CButtonST m_BtnCheck;
	CButtonST m_BtnBrowse;
	CButtonST m_BtnOpen;
	CButtonST m_BtnSave;

	CButton m_CheckAutoProcess;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnCbnSelchangeComboCtlMode();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CEdit m_EditSavePath;
	CEdit m_EditPN;
};
