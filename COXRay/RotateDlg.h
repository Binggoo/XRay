#pragma once


// CRotateDlg �Ի���

class CRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotateDlg)

public:
	CRotateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRotateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ROTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	double m_dbEditDegree;
public:
	afx_msg void OnBnClickedOk();
	double GetRotateDegree() {return m_dbEditDegree;}
};
