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


// CFilterEnhanceDlg 消息处理程序


BOOL CFilterEnhanceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常: OCX 属性页应返回 FALSE
}


void CFilterEnhanceDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWidth,strHeight,strFactor;
	m_ComboBoxMaskWidth.GetWindowText(strWidth);
	m_ComboBoxMaskHeight.GetWindowText(strHeight);
	m_ComboBoxFactor.GetWindowText(strFactor);

	m_lMaskWidth = _ttol(strWidth);
	m_lMaskHeight = _ttol(strHeight);
	m_dbFactor = _ttof(strFactor);

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

	CDialogEx::OnOK();
}
