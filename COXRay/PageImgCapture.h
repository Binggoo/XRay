#pragma once
#include "afxwin.h"
#include "../common/BtnST.h"

// CPageImgCapture 对话框
typedef enum _ENUM_RECORD_MODE
{
	Record_PN,
	Record_Date,
	Record_NO
}RecordMode;

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

	CIni *m_pIni;

public:
	CComboBox m_ComboBoxCtlMode;
	CComboBox m_ComboBoxFrames;
	CComboBox m_ComboBoxPosNum;
	CComboBox m_ComboBoxLevel;
	CComboBox m_ComboBoxRecordMode;

	CButtonST m_BtnStaticCap;
	CButtonST m_BtnContinueCap;
	CButtonST m_BtnPlcStart;
	CButtonST m_BtnPlcStop;
	CButtonST m_BtnView;
	CButtonST m_BtnCheck;
	CButtonST m_BtnBrowse;
//	CButtonST m_BtnOpen;
	CButtonST m_BtnSave;

	CButton m_CheckAutoProcess;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnCbnSelchangeComboCtlMode();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CEdit m_EditSavePath;
	CEdit m_EditPN;
	CEdit m_EditDate;
	CEdit m_EditModuleNo;
	CEdit m_EditDefectLen;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetConfig(CIni *pIni) {m_pIni = pIni;};
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnCbnSelchangeComboRecordMode();
	afx_msg void OnEnChangeEditDefectLen();
	afx_msg void OnEnChangeEditDate();
};
