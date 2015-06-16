#pragma once


// CFullScreenViewDlg �Ի���

class CFullScreenViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenViewDlg)

public:
	CFullScreenViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFullScreenViewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FULL_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString m_strFilePath;
	HWindow *m_phWindow;
	HImage  m_hImage;
public:
	virtual BOOL OnInitDialog();
	void SetFilePath(CString strFile) {m_strFilePath = strFile;}
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
