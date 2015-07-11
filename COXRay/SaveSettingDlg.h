#pragma once
#include "afxwin.h"
#include "MyDatabase.h"

// CSaveSettingDlg �Ի���

class CSaveSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveSettingDlg)

public:
	CSaveSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSaveSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SAVE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCheckDate;
	BOOL m_bCheckVoltage;
	BOOL m_bCheckCurrent;
	BOOL m_bCheckCustomer;
	BOOL m_bCheckProductName;
	BOOL m_bCheckPos;
	BOOL m_bCheckNoRemind;
	BOOL m_bCheckAutoSave;
	BOOL m_bCheckSaveImgEdit;

	CString m_strEditDate;
	double m_dbEditVoltage;
	double m_dbEditCurrent;
	CString m_strEditCustomer;
	CString m_strEditProduct;
	CString m_strEditPos;

	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void SetConfig(CIni *pIni) {m_pIni = pIni;}
	void SetImgInfo(IMG_INFO imgInfo);
};
