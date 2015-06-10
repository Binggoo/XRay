#pragma once


// CProjectNewDlg �Ի���

class CProjectNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectNewDlg)

public:
	CProjectNewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectNewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NEW_PROJECT };

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
	CComboBox m_ComboBoxInspectPos;
	CIni m_IniCustomer;

	CString m_strProjectFile;
	CString m_strWorkerName;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinLenght(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	void SetWorkerName(CString strWorkName) {m_strWorkerName = strWorkName;}

	CString GetProjectFile() {return m_strProjectFile;}
};
