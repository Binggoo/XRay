// FilterEnhanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterEnhanceDlg.h"
#include "afxdialogex.h"


// CFilterEnhanceDlg 对话框

IMPLEMENT_DYNAMIC(CFilterEnhanceDlg, CDialogEx)

CFilterEnhanceDlg::CFilterEnhanceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterEnhanceDlg::IDD, pParent)
{
	m_lMaskWidth = 7;
	m_lMaskHeight = 7;
	m_dbFactor = 1.0;
	m_nEnhanceTimes = 1;
	m_bCheckNoRemind = FALSE;

	m_pIni = NULL;
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
	DDX_Control(pDX, IDC_COMBO_ENHANCE_TIMES, m_ComboBoxEnhanceTimes);
	DDX_Check(pDX, IDC_CHECK_NO_REMIND, m_bCheckNoRemind);
}


BEGIN_MESSAGE_MAP(CFilterEnhanceDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFilterEnhanceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterEnhanceDlg 消息处理程序


BOOL CFilterEnhanceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT(m_pIni);

	m_lMaskHeight = m_pIni->GetInt(_T("Enhance"),_T("MaskHeight"),7);
	m_lMaskWidth = m_pIni->GetInt(_T("Enhance"),_T("MaskWidth"),7);
	m_dbFactor = m_pIni->GetDouble(_T("Enhance"),_T("Factor"),1.0);
	m_nEnhanceTimes = m_pIni->GetInt(_T("Enhance"),_T("Times"),0);
	m_bCheckNoRemind = m_pIni->GetBool(_T("Enhance"),_T("En_NoRemind"),FALSE);

	CString strMaskHeight,strMashWidth,strFactor,strTimes;
	strMashWidth.Format(_T("%d"),m_lMaskWidth);
	strMaskHeight.Format(_T("%d"),m_lMaskHeight);
	strFactor.Format(_T("%.1f"),m_dbFactor);
	strTimes.Format(_T("%d"),m_nEnhanceTimes);

	// TODO:  在此添加额外的初始化
	m_ComboBoxMaskWidth.AddString(_T("3"));
	m_ComboBoxMaskWidth.AddString(_T("5"));
	m_ComboBoxMaskWidth.AddString(_T("7"));
	m_ComboBoxMaskWidth.AddString(_T("9"));
	m_ComboBoxMaskWidth.AddString(_T("11"));

//	m_ComboBoxMaskWidth.SetCurSel(2);
	m_ComboBoxMaskWidth.SetWindowText(strMashWidth);

	m_ComboBoxMaskHeight.AddString(_T("3"));
	m_ComboBoxMaskHeight.AddString(_T("5"));
	m_ComboBoxMaskHeight.AddString(_T("7"));
	m_ComboBoxMaskHeight.AddString(_T("9"));
	m_ComboBoxMaskHeight.AddString(_T("11"));

//	m_ComboBoxMaskHeight.SetCurSel(2);
	m_ComboBoxMaskHeight.SetWindowText(strMaskHeight);

	m_ComboBoxFactor.AddString(_T("0.3"));
	m_ComboBoxFactor.AddString(_T("0.5"));
	m_ComboBoxFactor.AddString(_T("0.7"));
	m_ComboBoxFactor.AddString(_T("1.0"));
	m_ComboBoxFactor.AddString(_T("1.4"));
	m_ComboBoxFactor.AddString(_T("1.8"));
	m_ComboBoxFactor.AddString(_T("2.0"));

//	m_ComboBoxFactor.SetCurSel(3);
	m_ComboBoxFactor.SetWindowText(strFactor);

	m_ComboBoxEnhanceTimes.AddString(_T("0"));
	m_ComboBoxEnhanceTimes.AddString(_T("1"));
	m_ComboBoxEnhanceTimes.AddString(_T("2"));
	m_ComboBoxEnhanceTimes.AddString(_T("3"));

	m_ComboBoxEnhanceTimes.SetWindowText(strTimes);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFilterEnhanceDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strWidth,strHeight,strFactor,strTimes;
	m_ComboBoxMaskWidth.GetWindowText(strWidth);
	m_ComboBoxMaskHeight.GetWindowText(strHeight);
	m_ComboBoxFactor.GetWindowText(strFactor);
	m_ComboBoxEnhanceTimes.GetWindowText(strTimes);

	m_lMaskWidth = _ttol(strWidth);
	m_lMaskHeight = _ttol(strHeight);
	m_dbFactor = _ttof(strFactor);
	m_nEnhanceTimes = _ttoi(strTimes);

	if (m_lMaskWidth < 3)
	{
		AfxMessageBox(_T("Mask宽度不能小于3！"));
		return;
	}

	if (m_lMaskHeight < 3)
	{
		AfxMessageBox(_T("Mask高度不能小于3！"));
		return;
	}

	if (m_dbFactor < 0)
	{
		AfxMessageBox(_T("强度系数不能小于0！"));
		return;
	}

	if (m_nEnhanceTimes < 1)
	{
		AfxMessageBox(_T("增强次数不能小于1！"));
	}

	m_pIni->WriteInt(_T("Enhance"),_T("MaskHeight"),m_lMaskHeight);
	m_pIni->WriteInt(_T("Enhance"),_T("MaskWidth"),m_lMaskWidth);
	m_pIni->WriteDouble(_T("Enhance"),_T("Factor"),m_dbFactor);
	m_pIni->WriteInt(_T("Enhance"),_T("Times"),m_nEnhanceTimes);
	m_pIni->WriteBool(_T("Enhance"),_T("En_NoRemind"),m_bCheckNoRemind);

	CDialogEx::OnOK();
}
