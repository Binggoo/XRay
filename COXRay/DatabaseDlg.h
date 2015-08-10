#pragma once
#include "MyDatabase.h"
#include "afxwin.h"
#include "../common/CEditList/EditList.h"
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
	CComboBox m_ComboBoxDataBase;
	CListCtrl m_ListCtrlQueryCondition;
	CEditList m_ListCtrlQueryResult;
	CMyDatabase *m_pMyDatabase;

	int m_nCurSelectItem;

	CStatic m_ThumbnailPic;

	void InitialListCtrl();

public:
	void SetDatabase(CMyDatabase *pDatabase) {m_pMyDatabase = pDatabase;};
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnNMClickListQueryResult(NMHDR *pNMHDR, LRESULT *pResult);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkListQueryResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnView();
	afx_msg void OnLvnKeydownListQueryResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListQueryResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatabaseDel();
	afx_msg void OnDatabaseClear();
	afx_msg void OnCbnSelchangeComboDatabase();
	afx_msg void OnDatabaseDrop();
	afx_msg void OnDatabaseClearAll();
	afx_msg void OnDatabaseExport();
	afx_msg void OnDatabaseOpenPath();
	afx_msg void OnDatabaseModify();
protected:
	afx_msg LRESULT OnUserClick(WPARAM wParam, LPARAM lParam);
};
