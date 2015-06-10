#pragma once
#include "afxwin.h"


// CFilterGaussDlg �Ի���

class CFilterGaussDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterGaussDlg)

public:
	CFilterGaussDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFilterGaussDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GAUSS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxFilterSize;
	long m_lFilterSize;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	long GetFilterSize() {return m_lFilterSize;}
};
