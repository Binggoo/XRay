#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CProjectEditDlg �Ի���

class CProjectEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectEditDlg)

public:
	CProjectEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectEditDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDIT_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strEditProjectNumber;
	CString m_strEditProductName;
	UINT m_nEditLength;
	UINT m_nEditWidth;
	UINT m_nEditHeight;
	CComboBox m_ComboBoxCustomer;
	CComboBox m_ComboBoxInspectLevel;
	CComboBox m_ComboBoxInspectMode;
	CListCtrl m_ListCtrl;
	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	void SetConfig(CIni *pIni) {m_pIni = pIni;}
};
