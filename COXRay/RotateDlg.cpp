// RotateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "RotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg �Ի���

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRotateDlg::IDD, pParent)
	, m_dbEditDegree(0)
{

}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROTATE, m_dbEditDegree);
	DDV_MinMaxDouble(pDX, m_dbEditDegree, 0, 360);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRotateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRotateDlg ��Ϣ�������


void CRotateDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
