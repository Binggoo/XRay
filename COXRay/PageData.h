#pragma once
#include "afxcmn.h"


// CPageData �Ի���

class CPageData : public CDialogEx
{
	DECLARE_DYNAMIC(CPageData)

public:
	CPageData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageData();

// �Ի�������
	enum { IDD = IDD_PAGE_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_ListCtrlData;

	void InitialListCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
