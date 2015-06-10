// ThresholdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "ThresholdDlg.h"
#include "afxdialogex.h"


// CThresholdDlg 对话框

IMPLEMENT_DYNAMIC(CThresholdDlg, CDialogEx)

CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThresholdDlg::IDD, pParent)
	, m_nEditMinGray(0)
	, m_nEditMaxGray(0)
	, m_nEditActualMinGray(0)
	, m_nEditActualMaxGray(0)
	, m_nEditActualMeanGray(0)
{

}

CThresholdDlg::~CThresholdDlg()
{
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MIN_GRAY, m_nEditMinGray);
	DDX_Text(pDX, IDC_EDIT_MAX_GRAY, m_nEditMaxGray);
	DDX_Text(pDX,IDC_EDIT_MIN_GRAY2,m_nEditActualMinGray);
	DDX_Text(pDX,IDC_EDIT_MAX_GRAY2,m_nEditActualMaxGray);
	DDX_Text(pDX,IDC_EDIT_MEAN_GRAY,m_nEditActualMeanGray);
 	DDV_MinMaxUInt(pDX,m_nEditMinGray,m_nEditActualMinGray,m_nEditActualMaxGray);
 	DDV_MinMaxUInt(pDX,m_nEditMaxGray,m_nEditActualMinGray,m_nEditActualMaxGray);
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CThresholdDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CThresholdDlg 消息处理程序


void CThresholdDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_nEditMinGray > m_nEditMaxGray)
	{
		AfxMessageBox(_T("下限值超过上限值！"));

		((CEdit *)GetDlgItem(IDC_EDIT_MAX_GRAY))->SetSel(0,-1);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_MAX_GRAY));
		return;
	}

	CDialogEx::OnOK();
}


BOOL CThresholdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
