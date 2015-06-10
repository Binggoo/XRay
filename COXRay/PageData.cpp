// PageData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageData.h"
#include "afxdialogex.h"


// CPageData �Ի���

IMPLEMENT_DYNAMIC(CPageData, CDialogEx)

CPageData::CPageData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageData::IDD, pParent)
{

}

CPageData::~CPageData()
{
}

void CPageData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_ListCtrlData);
}


BEGIN_MESSAGE_MAP(CPageData, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPageData ��Ϣ�������


BOOL CPageData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rectClient;
	this->GetOwner()->GetClientRect(&rectClient);
	CTabCtrl *pTab = (CTabCtrl *)this->GetOwner();

	CRect rcHead;
	pTab->GetItemRect(0,&rcHead);
	rectClient.top += rcHead.Height() + 2;
	rectClient.left += 2;
	rectClient.right -= 2;
	rectClient.bottom -= 2;
	MoveWindow(rectClient);

	InitialListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPageData::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (nType == SIZE_MINIMIZED)
	{
		return;
	}

	if (m_ListCtrlData.m_hWnd)
	{
		m_ListCtrlData.MoveWindow(2,2,cx-4,cy-4);
	}
}

void CPageData::InitialListCtrl()
{
	int nItem = 0;
	m_ListCtrlData.InsertColumn(nItem++,_T("�ͻ�"),LVCFMT_LEFT,50);
	m_ListCtrlData.InsertColumn(nItem++,_T("����"),LVCFMT_LEFT,80);
	m_ListCtrlData.InsertColumn(nItem++,_T("����"),LVCFMT_LEFT,100);
	m_ListCtrlData.InsertColumn(nItem++,_T("���"),LVCFMT_LEFT,50);
	m_ListCtrlData.InsertColumn(nItem++,_T("��ϸ"),LVCFMT_LEFT,200);

	m_ListCtrlData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}
