#pragma once
#include "afxwin.h"


// CFilterMedianDlg �Ի���

class CFilterMedianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterMedianDlg)

public:
	CFilterMedianDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFilterMedianDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MEDIAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxMaskType;
	CComboBox m_ComboBoxMaskRadius;

	long m_lMaskRadius;
	int  m_iMaskType;
	BOOL m_bCheckNoRemind;

	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	int GetMaskType() {return m_iMaskType;}
	long GetMaskRadius() {return m_lMaskRadius;}

	void SetConfig(CIni *pIni) {m_pIni = pIni;};
};
