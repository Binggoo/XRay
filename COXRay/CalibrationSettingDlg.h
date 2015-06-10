#pragma once
#include "afxwin.h"


// CCalibrationSettingDlg �Ի���

class CCalibrationSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrationSettingDlg)

public:
	CCalibrationSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalibrationSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RULER_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	double m_dbEditPixel;
	double m_dbEditPhysicalDistance;
	CComboBox m_ComboBoxUnit;
	int    m_nSelectIndex;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	double GetPerPixel();
	int GetSelectUnit() {return m_nSelectIndex;}
	void SetPixel(double dbPixel) {m_dbEditPixel = dbPixel;}
};
