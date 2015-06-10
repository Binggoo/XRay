// FilterGaussDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterGaussDlg.h"
#include "afxdialogex.h"


// CFilterGaussDlg �Ի���

IMPLEMENT_DYNAMIC(CFilterGaussDlg, CDialogEx)

CFilterGaussDlg::CFilterGaussDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterGaussDlg::IDD, pParent)
{
	m_lFilterSize = 5;
}

CFilterGaussDlg::~CFilterGaussDlg()
{
}

void CFilterGaussDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FILTER_SIZE, m_ComboBoxFilterSize);
}


BEGIN_MESSAGE_MAP(CFilterGaussDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterGaussDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterGaussDlg ��Ϣ�������


BOOL CFilterGaussDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboBoxFilterSize.AddString(_T("3"));
	m_ComboBoxFilterSize.AddString(_T("5"));
	m_ComboBoxFilterSize.AddString(_T("7"));
	m_ComboBoxFilterSize.AddString(_T("9"));
	m_ComboBoxFilterSize.AddString(_T("11"));

	m_ComboBoxFilterSize.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFilterGaussDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilterSize;
	m_ComboBoxFilterSize.GetWindowText(strFilterSize);

	m_lFilterSize = _ttol(strFilterSize);

	if (m_lFilterSize < 1)
	{
		AfxMessageBox(_T("�˲�����С�������0��"));
		return;
	}

	CDialogEx::OnOK();
}
