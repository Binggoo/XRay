// CodeRuleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "CodeRuleDlg.h"
#include "afxdialogex.h"


// CCodeRuleDlg �Ի���

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


// CCodeRuleDlg ��Ϣ�������


void CCodeRuleDlg::OnEnChangeEditCodeRule()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int len = m_strEditCodeRule.GetLength();

	if (len > 0)
	{
		TCHAR ch = m_strEditCodeRule.GetAt(len - 1);
		if (ch != _T('0') && ch != _T('1'))
		{
			AfxMessageBox(_T("�����Ƕ����Ʊ��룡"));

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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_strEditDest = CodeString(m_strEditSource,m_strEditCodeRule);

	UpdateData(FALSE);
}


void CCodeRuleDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_pIni->WriteString(_T("SaveSetting"),_T("CodeRule"),m_strEditCodeRule);

	CDialogEx::OnOK();
}


BOOL CCodeRuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
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
