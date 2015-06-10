// PageData.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageData.h"
#include "afxdialogex.h"


// CPageData 对话框

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


// CPageData 消息处理程序


BOOL CPageData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常: OCX 属性页应返回 FALSE
}


void CPageData::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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
	m_ListCtrlData.InsertColumn(nItem++,_T("客户"),LVCFMT_LEFT,50);
	m_ListCtrlData.InsertColumn(nItem++,_T("工件"),LVCFMT_LEFT,80);
	m_ListCtrlData.InsertColumn(nItem++,_T("日期"),LVCFMT_LEFT,100);
	m_ListCtrlData.InsertColumn(nItem++,_T("结果"),LVCFMT_LEFT,50);
	m_ListCtrlData.InsertColumn(nItem++,_T("明细"),LVCFMT_LEFT,200);

	m_ListCtrlData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}
