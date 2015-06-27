// FilterMedianDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterMedianDlg.h"
#include "afxdialogex.h"


// CFilterMedianDlg 对话框

IMPLEMENT_DYNAMIC(CFilterMedianDlg, CDialogEx)

CFilterMedianDlg::CFilterMedianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterMedianDlg::IDD, pParent)
	, m_bCheckNoRemind(FALSE)
{
	m_iMaskType = 0;
	m_lMaskRadius = 1;

	m_pIni = NULL;
}

CFilterMedianDlg::~CFilterMedianDlg()
{
}

void CFilterMedianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MASK_TYPE, m_ComboBoxMaskType);
	DDX_Control(pDX, IDC_COMBO_MASK_RADIUS, m_ComboBoxMaskRadius);
	DDX_Check(pDX, IDC_CHECK_NO_REMIND, m_bCheckNoRemind);
}


BEGIN_MESSAGE_MAP(CFilterMedianDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterMedianDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterMedianDlg 消息处理程序


BOOL CFilterMedianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);

	m_iMaskType = m_pIni->GetInt(_T("MedianFilter"),_T("MaskType"),0);
	m_lMaskRadius = m_pIni->GetInt(_T("MedianFilter"),_T("MaskRadius"),1);
	m_bCheckNoRemind = m_pIni->GetBool(_T("MedianFilter"),_T("En_NoRemind"),FALSE);

	CString strRadius;
	strRadius.Format(_T("%d"),m_lMaskRadius);

	m_ComboBoxMaskType.AddString(_T("circle"));
	m_ComboBoxMaskType.AddString(_T("square"));
//	m_ComboBoxMaskType.SetCurSel(0);
	m_ComboBoxMaskType.SetCurSel(m_iMaskType);

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
//	m_ComboBoxMaskRadius.SetCurSel(0);
	m_ComboBoxMaskRadius.SetWindowText(strRadius);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFilterMedianDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strMaskRadius;
	m_ComboBoxMaskRadius.GetWindowText(strMaskRadius);

	m_lMaskRadius = _ttol(strMaskRadius);
	if (m_lMaskRadius < 1)
	{
		AfxMessageBox(_T("Mask的半径必须大于0"));
		return;
	}

	m_iMaskType = m_ComboBoxMaskType.GetCurSel();

	m_pIni->WriteInt(_T("MedianFilter"),_T("MaskType"),m_iMaskType);
	m_pIni->WriteInt(_T("MedianFilter"),_T("MaskRadius"),m_lMaskRadius);
	m_pIni->WriteBool(_T("MedianFilter"),_T("En_NoRemind"),m_bCheckNoRemind);

	CDialogEx::OnOK();
}
