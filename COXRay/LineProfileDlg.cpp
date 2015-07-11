// LineProfileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "LineProfileDlg.h"
#include "afxdialogex.h"


// CLineProfileDlg �Ի���

IMPLEMENT_DYNAMIC(CLineProfileDlg, CDialogEx)

CLineProfileDlg::CLineProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineProfileDlg::IDD, pParent)
{
	m_dbMaxGray = 0.0;
	m_dbMinGray = 0.0;
	m_dbMeanGray = 0.0;
	m_dbStd = 0.0;
}

CLineProfileDlg::~CLineProfileDlg()
{
}

void CLineProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINE_PROFILE, m_LineProfile);
	DDX_Control(pDX, IDC_LIST_POINTS, m_ListCtrl);
	DDX_Text(pDX,IDC_EDIT_MAX_GRAY,m_dbMaxGray);
	DDX_Text(pDX,IDC_EDIT_MIN_GRAY,m_dbMinGray);
	DDX_Text(pDX,IDC_EDIT_MEAN_GRAY,m_dbMeanGray);
	DDX_Text(pDX,IDC_EDIT_STD,m_dbStd);
}


BEGIN_MESSAGE_MAP(CLineProfileDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_MIN_MAX, &CLineProfileDlg::OnBnClickedCheckMinMax)
	ON_BN_CLICKED(IDC_CHECK_MEAN, &CLineProfileDlg::OnBnClickedCheckMean)
	ON_BN_CLICKED(IDC_CHECK_STD, &CLineProfileDlg::OnBnClickedCheckStd)
END_MESSAGE_MAP()


// CLineProfileDlg ��Ϣ�������


BOOL CLineProfileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ȡ��������
	HTuple htRows,htColumns,htGrayValues;
	htRows = m_hRegion.GetRegionPoints(&htColumns);

	htGrayValues = m_hImage.GetGrayval(htRows,htColumns);

	m_dbMinGray = htGrayValues.Min()[0].D();

	m_dbMaxGray = htGrayValues.Max()[0].D();

	m_dbMeanGray = htGrayValues.Mean()[0].D();

	m_dbStd = htGrayValues.Deviation()[0].D();

	// ��ʼ��LineProfile
	m_LineProfile.SetPoints(htGrayValues);
	

	m_LineProfile.SetRedraw(TRUE);

	// ��ʼ���б�
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.InsertColumn(0,_T("pos"),LVCFMT_LEFT,80);
	m_ListCtrl.InsertColumn(1,_T("value"),LVCFMT_LEFT,rect.Width() - 60);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	for (int i = 0; i < htGrayValues.Num();i++)
	{
		CString strItem,strValue;
		strItem.Format(_T("%d"),i);
		strValue.Format(_T("%f"),htGrayValues[i].D());

		m_ListCtrl.InsertItem(i,strItem);
		m_ListCtrl.SetItemText(i,1,strValue);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CLineProfileDlg::OnBnClickedCheckMinMax()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bCheck = ((CButton *)GetDlgItem(IDC_CHECK_MIN_MAX))->GetCheck();

	m_LineProfile.SetMinMaxLine(bCheck);
}


void CLineProfileDlg::OnBnClickedCheckMean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bCheck = ((CButton *)GetDlgItem(IDC_CHECK_MEAN))->GetCheck();

	m_LineProfile.SetMeanLine(bCheck);
}


void CLineProfileDlg::OnBnClickedCheckStd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bCheck = ((CButton *)GetDlgItem(IDC_CHECK_STD))->GetCheck();

	m_LineProfile.SetStdLine(bCheck);
}
