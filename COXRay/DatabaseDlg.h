#pragma once
#include "MyDatabase.h"
#include "afxwin.h"


// CDatabaseDlg 对话框

class CDatabaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseDlg)

public:
	CDatabaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatabaseDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CDateTimeCtrl m_DataTimeCtrlStart;
	CDateTimeCtrl m_DataTimeCtrlEnd;
	CComboBox m_ComboBoxUnion;
	CComboBox m_ComboBoxFields;
	CListCtrl m_ListCtrlQueryCondition;
	CListCtrl m_ListCtrlQueryResult;
	CMyDatabase *m_pMyDatabase;

	CBitmap m_Bitmap;

	void InitialListCtrl();

public:
	void SetDatabase(CMyDatabase *pDatabase) {m_pMyDatabase = pDatabase;};
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnNMClickListQueryResult(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_ThumbnailPic;
};
