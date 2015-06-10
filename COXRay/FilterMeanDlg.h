#pragma once
#include "afxwin.h"


// CFilterMeanDlg �Ի���

class CFilterMeanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterMeanDlg)

public:
	CFilterMeanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFilterMeanDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ComboBoxWidth;
	CComboBox m_ComboBoxHeight;

	long m_lMaskWidth;
	long m_lMaskHeight;
public:
	virtual BOOL OnInitDialog();
	long GetMaskWidth() {return m_lMaskWidth;}
	long GetMaskHeight() {return m_lMaskHeight;}
	afx_msg void OnBnClickedOk();
};
