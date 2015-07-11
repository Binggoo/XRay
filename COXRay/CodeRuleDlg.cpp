// CodeRuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "CodeRuleDlg.h"
#include "afxdialogex.h"


// CCodeRuleDlg 对话框

IMPLEMENT_DYNAMIC(CCodeRuleDlg, CDialogEx)

CCodeRuleDlg::CCodeRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeRuleDlg::IDD, pParent)
	, m_strEditCodeRule(_T(""))
	, m_strEditSource(_T(""))
	, m_strEditDest(_T(""))
{

}

CCodeRuleDlg::~CCodeRuleDlg()
{
}

void CCodeRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE_RULE, m_strEditCodeRule);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strEditSource);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strEditDest);
}


BEGIN_MESSAGE_MAP(CCodeRuleDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_CODE_RULE, &CCodeRuleDlg::OnEnChangeEditCodeRule)
	ON_EN_CHANGE(IDC_EDIT_SOURCE, &CCodeRuleDlg::OnEnChangeEditSource)
	ON_BN_CLICKED(IDOK, &CCodeRuleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCodeRuleDlg 消息处理程序


void CCodeRuleDlg::OnEnChangeEditCodeRule()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int len = m_strEditCodeRule.GetLength();

	if (len > 0)
	{
		TCHAR ch = m_strEditCodeRule.GetAt(len - 1);
		if (ch != _T('0') && ch != _T('1'))
		{
			AfxMessageBox(_T("必须是二进制编码！"));

			m_strEditCodeRule = m_strEditCodeRule.Left(len-1);

			UpdateData(FALSE);

			return;
		}
	}

	m_strEditSource.Empty();
	for (int i = 1; i <= len;i++)
	{
		CString strNum;
		strNum.Format(_T("%d"),i % 10);

		m_strEditSource += strNum;
	}
	

	m_strEditDest = CodeString(m_strEditSource,m_strEditCodeRule);

	UpdateData(FALSE);

}


void CCodeRuleDlg::OnEnChangeEditSource()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_strEditDest = CodeString(m_strEditSource,m_strEditCodeRule);

	UpdateData(FALSE);
}


void CCodeRuleDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_pIni->WriteString(_T("SaveSetting"),_T("CodeRule"),m_strEditCodeRule);

	CDialogEx::OnOK();
}


BOOL CCodeRuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);

	m_strEditCodeRule = m_pIni->GetString(_T("SaveSetting"),_T("CodeRule"));

	int len = m_strEditCodeRule.GetLength();

	for (int i = 1; i <= len;i++)
	{
		CString strNum;
		strNum.Format(_T("%d"),i % 10);

		m_strEditSource += strNum;
	}

	m_strEditDest = CodeString(m_strEditSource,m_strEditCodeRule);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CCodeRuleDlg::CodeString( CString strSource,CString strCodeRule )
{
	CString strDest;

	int len = min(strSource.GetLength(),strCodeRule.GetLength());

	for (int i = 0; i < len; i++)
	{
		if (strCodeRule.GetAt(i) != _T('0'))
		{
			strDest += strSource.GetAt(i);
		}
	}

	strDest += strSource.Mid(len);

	return strDest;
}
