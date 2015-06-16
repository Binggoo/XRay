#pragma once


// CFullScreenViewDlg 对话框

class CFullScreenViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenViewDlg)

public:
	CFullScreenViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFullScreenViewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FULL_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
