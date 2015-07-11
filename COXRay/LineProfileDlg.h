#pragma once
#include "lineprofile.h"
#include "afxcmn.h"


// CLineProfileDlg �Ի���

class CLineProfileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineProfileDlg)

public:
	CLineProfileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLineProfileDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LINE_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CLineProfile m_LineProfile;
	CListCtrl m_ListCtrl;
	double m_dbMinGray;
	double m_dbMaxGray;
	double m_dbMeanGray;
	double m_dbStd;

	HRegion m_hRegion;
	HImage  m_hImage;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckMinMax();
	afx_msg void OnBnClickedCheckMean();
	afx_msg void OnBnClickedCheckStd();

	void SetImageRegion(HImage hImage,HRegion hRegion) {m_hImage = hImage;m_hRegion = hRegion;};
};
