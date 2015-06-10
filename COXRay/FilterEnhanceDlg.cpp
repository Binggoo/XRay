// FilterEnhanceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterEnhanceDlg.h"
#include "afxdialogex.h"


// CFilterEnhanceDlg �Ի���

IMPLEMENT_DYNAMIC(CFilterEnhanceDlg, CDialogEx)

CFilterEnhanceDlg::CFilterEnhanceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterEnhanceDlg::IDD, pParent)
{
	m_lMaskWidth = 7;
	m_lMaskHeight = 7;
	m_dbFactor = 1.0;
}

CFilterEnhanceDlg::~CFilterEnhanceDlg()
{
}

void CFilterEnhanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MASK_WIDTH, m_ComboBoxMaskWidth);
	DDX_Control(pDX, IDC_COMBO_MASK_HEIGHT, m_ComboBoxMaskHeight);
	DDX_Control(pDX, IDC_COMBO_FACTOR, m_ComboBoxFactor);
}


BEGIN_MESSAGE_MAP(CFilterEnhanceDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterEnhanceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterEnhanceDlg ��Ϣ�������


BOOL CFilterEnhanceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboBoxMaskWidth.AddString(_T("3"));
	m_ComboBoxMaskWidth.AddString(_T("5"));
	m_ComboBoxMaskWidth.AddString(_T("7"));
	m_ComboBoxMaskWidth.AddString(_T("9"));
	m_ComboBoxMaskWidth.AddString(_T("11"));

	m_ComboBoxMaskWidth.SetCurSel(2);

	m_ComboBoxMaskHeight.AddString(_T("3"));
	m_ComboBoxMaskHeight.AddString(_T("5"));
	m_ComboBoxMaskHeight.AddString(_T("7"));
	m_ComboBoxMaskHeight.AddString(_T("9"));
	m_ComboBoxMaskHeight.AddString(_T("11"));

	m_ComboBoxMaskHeight.SetCurSel(2);

	m_ComboBoxFactor.AddString(_T("0.3"));
	m_ComboBoxFactor.AddString(_T("0.5"));
	m_ComboBoxFactor.AddString(_T("0.7"));
	m_ComboBoxFactor.AddString(_T("1.0"));
	m_ComboBoxFactor.AddString(_T("1.4"));
	m_ComboBoxFactor.AddString(_T("1.8"));
	m_ComboBoxFactor.AddString(_T("2.0"));

	m_ComboBoxFactor.SetCurSel(3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFilterEnhanceDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strWidth,strHeight,strFactor;
	m_ComboBoxMaskWidth.GetWindowText(strWidth);
	m_ComboBoxMaskHeight.GetWindowText(strHeight);
	m_ComboBoxFactor.GetWindowText(strFactor);

	m_lMaskWidth = _ttol(strWidth);
	m_lMaskHeight = _ttol(strHeight);
	m_dbFactor = _ttof(strFactor);

	if (m_lMaskWidth < 3)
	{
		AfxMessageBox(_T("Mask��Ȳ���С��3��"));
		return;
	}

	if (m_lMaskHeight < 3)
	{
		AfxMessageBox(_T("Mask�߶Ȳ���С��3��"));
		return;
	}

	if (m_dbFactor < 0)
	{
		AfxMessageBox(_T("ǿ��ϵ������С��0��"));
		return;
	}

	CDialogEx::OnOK();
}
