#pragma once
#include "afxwin.h"


// CFilterEnhanceDlg �Ի���

class CFilterEnhanceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterEnhanceDlg)

public:
	CFilterEnhanceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFilterEnhanceDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ENHANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxMaskWidth;
	CComboBox m_ComboBoxMaskHeight;
	CComboBox m_ComboBoxFactor;

	long m_lMaskWidth;
	long m_lMaskHeight;
	double m_dbFactor;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	long GetMaskWith() {return m_lMaskWidth;}
	long GetMaskHeight() {return m_lMaskHeight;}
	double GetFactor() {return m_dbFactor;}
};
