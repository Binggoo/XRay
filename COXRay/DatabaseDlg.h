#pragma once
#include "MyDatabase.h"
#include "afxwin.h"


// CDatabaseDlg �Ի���

class CDatabaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseDlg)

public:
	CDatabaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDatabaseDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CDateTimeCtrl m_DataTimeCtrlStart;
	CDateTimeCtrl m_DataTimeCtrlEnd;
	CComboBox m_ComboBoxUnion;
	CComboBox m_ComboBoxFields;
	CListCtrl m_ListCtrlQueryCondition;
	CListCtrl m_ListCtrlQueryResult;
	CMyDatabase *m_pMyDatabase;

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
};
