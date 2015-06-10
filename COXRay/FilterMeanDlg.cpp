// FilterMeanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterMeanDlg.h"
#include "afxdialogex.h"


// CFilterMeanDlg �Ի���

IMPLEMENT_DYNAMIC(CFilterMeanDlg, CDialogEx)

CFilterMeanDlg::CFilterMeanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterMeanDlg::IDD, pParent)
{
	m_lMaskWidth = 9;
	m_lMaskHeight = 9;
}

CFilterMeanDlg::~CFilterMeanDlg()
{
}

void CFilterMeanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WIDTH, m_ComboBoxWidth);
	DDX_Control(pDX, IDC_COMBO_HEIGHT, m_ComboBoxHeight);
}


BEGIN_MESSAGE_MAP(CFilterMeanDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterMeanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterMeanDlg ��Ϣ�������


BOOL CFilterMeanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboBoxWidth.AddString(_T("3"));
	m_ComboBoxWidth.AddString(_T("5"));
	m_ComboBoxWidth.AddString(_T("7"));
	m_ComboBoxWidth.AddString(_T("9"));
	m_ComboBoxWidth.AddString(_T("11"));
	m_ComboBoxWidth.AddString(_T("15"));
	m_ComboBoxWidth.AddString(_T("23"));
	m_ComboBoxWidth.AddString(_T("31"));
	m_ComboBoxWidth.AddString(_T("43"));
	m_ComboBoxWidth.AddString(_T("61")); 
	m_ComboBoxWidth.AddString(_T("101"));

	m_ComboBoxHeight.AddString(_T("3"));
	m_ComboBoxHeight.AddString(_T("5"));
	m_ComboBoxHeight.AddString(_T("7"));
	m_ComboBoxHeight.AddString(_T("9"));
	m_ComboBoxHeight.AddString(_T("11"));
	m_ComboBoxHeight.AddString(_T("15"));
	m_ComboBoxHeight.AddString(_T("23"));
	m_ComboBoxHeight.AddString(_T("31"));
	m_ComboBoxHeight.AddString(_T("43"));
	m_ComboBoxHeight.AddString(_T("61")); 
	m_ComboBoxHeight.AddString(_T("101"));


	m_ComboBoxWidth.SetCurSel(3);
	m_ComboBoxHeight.SetCurSel(3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFilterMeanDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strWidth,strHeight;
	m_ComboBoxWidth.GetWindowText(strWidth);
	m_ComboBoxHeight.GetWindowText(strHeight);

	m_lMaskWidth = _ttol(strWidth);
	m_lMaskHeight = _ttol(strHeight);

	if (m_lMaskWidth < 1 || m_lMaskHeight < 1)
	{
		AfxMessageBox(_T("�������0"));
		return;
	}

	CDialogEx::OnOK();
}
