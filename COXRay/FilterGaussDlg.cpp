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
	, m_bCheckNoRemind(FALSE)
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
	DDX_Check(pDX, IDC_CHECK_NO_REMIND, m_bCheckNoRemind);
}


BEGIN_MESSAGE_MAP(CFilterGaussDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterGaussDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterGaussDlg ��Ϣ�������


BOOL CFilterGaussDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ASSERT(m_pIni);
	m_lFilterSize = m_pIni->GetInt(_T("GaussFilter"),_T("FilterSize"),5);
	m_bCheckNoRemind = m_pIni->GetBool(_T("GaussFilter"),_T("En_NoRemind"),FALSE);

	CString strFilterSize;
	strFilterSize.Format(_T("%d"),m_lFilterSize);

	m_ComboBoxFilterSize.AddString(_T("3"));
	m_ComboBoxFilterSize.AddString(_T("5"));
	m_ComboBoxFilterSize.AddString(_T("7"));
	m_ComboBoxFilterSize.AddString(_T("9"));
	m_ComboBoxFilterSize.AddString(_T("11"));

//	m_ComboBoxFilterSize.SetCurSel(1);
	m_ComboBoxFilterSize.SetWindowText(strFilterSize);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFilterGaussDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CString strFilterSize;
	m_ComboBoxFilterSize.GetWindowText(strFilterSize);

	m_lFilterSize = _ttol(strFilterSize);

	if (m_lFilterSize < 1)
	{
		AfxMessageBox(_T("�˲�����С�������0��"));
		return;
	}

	m_pIni->WriteInt(_T("GaussFilter"),_T("FilterSize"),m_lFilterSize);
	m_pIni->WriteBool(_T("GaussFilter"),_T("En_NoRemind"),m_bCheckNoRemind);

	CDialogEx::OnOK();
}
