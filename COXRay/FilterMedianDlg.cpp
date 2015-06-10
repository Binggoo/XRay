// FilterMedianDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterMedianDlg.h"
#include "afxdialogex.h"


// CFilterMedianDlg �Ի���

IMPLEMENT_DYNAMIC(CFilterMedianDlg, CDialogEx)

CFilterMedianDlg::CFilterMedianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterMedianDlg::IDD, pParent)
{
	m_iMaskType = 0;
	m_lMaskRadius = 1;
}

CFilterMedianDlg::~CFilterMedianDlg()
{
}

void CFilterMedianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MASK_TYPE, m_ComboBoxMaskType);
	DDX_Control(pDX, IDC_COMBO_MASK_RADIUS, m_ComboBoxMaskRadius);
}


BEGIN_MESSAGE_MAP(CFilterMedianDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterMedianDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterMedianDlg ��Ϣ�������


BOOL CFilterMedianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_ComboBoxMaskType.AddString(_T("circle"));
	m_ComboBoxMaskType.AddString(_T("square"));
	m_ComboBoxMaskType.SetCurSel(0);

	m_ComboBoxMaskRadius.AddString(_T("1"));
	m_ComboBoxMaskRadius.AddString(_T("2"));
	m_ComboBoxMaskRadius.AddString(_T("3"));
	m_ComboBoxMaskRadius.AddString(_T("4"));
	m_ComboBoxMaskRadius.AddString(_T("5"));
	m_ComboBoxMaskRadius.AddString(_T("6"));
	m_ComboBoxMaskRadius.AddString(_T("7"));
	m_ComboBoxMaskRadius.AddString(_T("8"));
	m_ComboBoxMaskRadius.AddString(_T("9"));
	m_ComboBoxMaskRadius.AddString(_T("11"));
	m_ComboBoxMaskRadius.AddString(_T("15"));
	m_ComboBoxMaskRadius.AddString(_T("19"));
	m_ComboBoxMaskRadius.AddString(_T("25"));
	m_ComboBoxMaskRadius.AddString(_T("31"));
	m_ComboBoxMaskRadius.AddString(_T("39"));
	m_ComboBoxMaskRadius.AddString(_T("47"));
	m_ComboBoxMaskRadius.AddString(_T("59"));
	m_ComboBoxMaskRadius.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFilterMedianDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMaskRadius;
	m_ComboBoxMaskRadius.GetWindowText(strMaskRadius);

	m_lMaskRadius = _ttol(strMaskRadius);
	if (m_lMaskRadius < 1)
	{
		AfxMessageBox(_T("Mask�İ뾶�������0"));
		return;
	}

	m_iMaskType = m_ComboBoxMaskType.GetCurSel();

	CDialogEx::OnOK();
}
