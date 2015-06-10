#pragma once
#include "afxcmn.h"


// CLightSettingDlg �Ի���

class CLightSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLightSettingDlg)

public:
	CLightSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLightSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LIGHT_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CSliderCtrl m_SliderCtrlVol;
	CSliderCtrl m_SliderCtrlCur;
	double m_dbEditVol;
	double m_dbEditCurrent;

	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void SetConfig(CIni *pIni) {m_pIni = pIni;}
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditVol();
	afx_msg void OnEnChangeEditCurrent();
};
