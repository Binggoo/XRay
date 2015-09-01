#pragma once
#include "afxcmn.h"


// CCommSettingDlg �Ի���

class CCommSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCommSettingDlg)

public:
	CCommSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_COMM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CIPAddressCtrl m_IPAddressCtrl;
	UINT m_nEditPort;
	BOOL m_bCheckAutoConnect;
	UINT m_nEditLocNums;

	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	void SetConfig(CIni *pIni) {m_pIni = pIni;}
	afx_msg void OnBnClickedOk();
private:
	UINT m_nEditSleepBeforeSnap;
};
