// ProjectEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "ProjectEditDlg.h"
#include "afxdialogex.h"


// CProjectEditDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectEditDlg, CDialogEx)

CProjectEditDlg::CProjectEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectEditDlg::IDD, pParent)
	, m_strEditProjectNumber(_T(""))
	, m_strEditProductName(_T(""))
	, m_nEditLength(0)
	, m_nEditWidth(0)
	, m_nEditHeight(0)
{

}

CProjectEditDlg::~CProjectEditDlg()
{
}

void CProjectEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROJECT_NUMBER, m_strEditProjectNumber);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_strEditProductName);
	DDX_Text(pDX, IDC_EDIT_LENGHT, m_nEditLength);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nEditWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nEditHeight);
	DDX_Control(pDX, IDC_COMBO_CUSTOMER, m_ComboBoxCustomer);
	DDX_Control(pDX, IDC_COMBO_INSPECT_LEVEL, m_ComboBoxInspectLevel);
	DDX_Control(pDX, IDC_COMBO_INSPECT_MODE, m_ComboBoxInspectMode);
	DDX_Control(pDX, IDC_LIST_POS, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CProjectEditDlg, CDialogEx)
END_MESSAGE_MAP()


// CProjectEditDlg ��Ϣ�������


BOOL CProjectEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
