// FilterGaussDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "FilterGaussDlg.h"
#include "afxdialogex.h"


// CFilterGaussDlg 对话框

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


// CFilterGaussDlg 消息处理程序


BOOL CFilterGaussDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBoxFilterSize.AddString(_T("3"));
	m_ComboBoxFilterSize.AddString(_T("5"));
	m_ComboBoxFilterSize.AddString(_T("7"));
	m_ComboBoxFilterSize.AddString(_T("9"));
	m_ComboBoxFilterSize.AddString(_T("11"));

	m_ComboBoxFilterSize.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFilterGaussDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilterSize;
	m_ComboBoxFilterSize.GetWindowText(strFilterSize);

	m_lFilterSize = _ttol(strFilterSize);

	if (m_lFilterSize < 1)
	{
		AfxMessageBox(_T("滤波器大小必须大于0！"));
		return;
	}

	CDialogEx::OnOK();
}
