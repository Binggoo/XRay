#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CBottomDialogBar 对话框

class CBottomDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CBottomDialogBar)

public:
	CBottomDialogBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBottomDialogBar();

// 对话框数据
	enum { IDD = IDD_DIALOGBAR_BOTTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CString m_strPath;

public:
	CListCtrl m_ListThumbNail;
	CEdit m_EditLog;
	CImageList m_ImageListThumbNail;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);

	void DrawThumbnail(CString strPath,CString strFilter);
	afx_msg void OnNMDblclkListThumbnail(NMHDR *pNMHDR, LRESULT *pResult);
};
