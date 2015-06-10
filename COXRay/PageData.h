#pragma once
#include "afxcmn.h"


// CPageData 对话框

class CPageData : public CDialogEx
{
	DECLARE_DYNAMIC(CPageData)

public:
	CPageData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageData();

// 对话框数据
	enum { IDD = IDD_PAGE_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_ListCtrlData;

	void InitialListCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
