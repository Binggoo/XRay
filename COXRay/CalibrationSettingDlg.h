#pragma once
#include "afxwin.h"


// CCalibrationSettingDlg 对话框

class CCalibrationSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrationSettingDlg)

public:
	CCalibrationSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibrationSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RULER_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
