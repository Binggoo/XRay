#pragma once


// CRotateDlg 对话框

class CRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotateDlg)

public:
	CRotateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRotateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ROTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	double m_dbEditDegree;
public:
	afx_msg void OnBnClickedOk();
	double GetRotateDegree() {return m_dbEditDegree;}
};
