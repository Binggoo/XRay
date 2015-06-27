// FilterMeanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterMeanDlg.h"
#include "afxdialogex.h"


// CFilterMeanDlg 对话框

IMPLEMENT_DYNAMIC(CFilterMeanDlg, CDialogEx)

CFilterMeanDlg::CFilterMeanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterMeanDlg::IDD, pParent)
	, m_bCheckNoRemind(FALSE)
{
	m_lMaskWidth = 9;
	m_lMaskHeight = 9;
	
	m_pIni = NULL;
}

CFilterMeanDlg::~CFilterMeanDlg()
{
}

void CFilterMeanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WIDTH, m_ComboBoxWidth);
	DDX_Control(pDX, IDC_COMBO_HEIGHT, m_ComboBoxHeight);
	DDX_Check(pDX, IDC_CHECK_NO_REMIND, m_bCheckNoRemind);
}


BEGIN_MESSAGE_MAP(CFilterMeanDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterMeanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterMeanDlg 消息处理程序


BOOL CFilterMeanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);

	m_lMaskHeight = m_pIni->GetInt(_T("MeanFilter"),_T("MaskHeight"),9);
	m_lMaskWidth = m_pIni->GetInt(_T("MeanFilter"),_T("MaskWidth"),9);
	m_bCheckNoRemind = m_pIni->GetBool(_T("MeanFilter"),_T("En_NoRemind"),FALSE);

	CString strHeight,strWidth;
	strHeight.Format(_T("%d"),m_lMaskHeight);
	strWidth.Format(_T("%d"),m_lMaskWidth);

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


// 	m_ComboBoxWidth.SetCurSel(3);
// 	m_ComboBoxHeight.SetCurSel(3);
	m_ComboBoxWidth.SetWindowText(strWidth);
	m_ComboBoxHeight.SetWindowText(strHeight);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFilterMeanDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strWidth,strHeight;
	m_ComboBoxWidth.GetWindowText(strWidth);
	m_ComboBoxHeight.GetWindowText(strHeight);

	m_lMaskWidth = _ttol(strWidth);
	m_lMaskHeight = _ttol(strHeight);

	if (m_lMaskWidth < 1 || m_lMaskHeight < 1)
	{
		AfxMessageBox(_T("必须大于0"));
		return;
	}

	m_pIni->WriteInt(_T("MeanFilter"),_T("MaskHeight"),m_lMaskHeight);
	m_pIni->WriteInt(_T("MeanFilter"),_T("MaskWidth"),m_lMaskWidth);
	m_pIni->WriteBool(_T("MeanFilter"),_T("En_NoRemind"),m_bCheckNoRemind);

	CDialogEx::OnOK();
}
