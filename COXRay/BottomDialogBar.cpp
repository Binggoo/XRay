// BottomDialogBar.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "BottomDialogBar.h"

#include "MainFrm.h"
#include "COXRayDoc.h"
#include "COXRayView.h"

// CBottomDialogBar 对话框

#define THUMBNAIL_WIDTH   96
#define THUMBNAIL_HEIGHT  96

IMPLEMENT_DYNAMIC(CBottomDialogBar, CDialogBar)

CBottomDialogBar::CBottomDialogBar(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CBottomDialogBar::IDD, pParent*/)
{

}

CBottomDialogBar::~CBottomDialogBar()
{
}

void CBottomDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THUMBNAIL, m_ListThumbNail);
	DDX_Control(pDX, IDC_EDIT_LOG, m_EditLog);
}


BEGIN_MESSAGE_MAP(CBottomDialogBar, CDialogBar)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_THUMBNAIL, &CBottomDialogBar::OnNMDblclkListThumbnail)
END_MESSAGE_MAP()


// CBottomDialogBar 消息处理程序


BOOL CBottomDialogBar::OnInitDialog()
{
	// TODO:  在此添加额外的初始化

	UpdateData(FALSE);

	m_ImageListThumbNail.Create(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT,ILC_COLOR24,0,1);
	m_ListThumbNail.SetImageList(&m_ImageListThumbNail,LVSIL_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBottomDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_MINIMIZED)
	{
		return;
	}

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CView *pView = pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}

	if (m_ListThumbNail.m_hWnd == NULL || m_EditLog.m_hWnd == NULL)
	{
		return;
	}

	CRect rectView,rectFrame,rectListCtrl,rectEdit;

	pView->GetClientRect(&rectView);
	pFrame->GetClientRect(&rectFrame);
	m_ListThumbNail.GetWindowRect(&rectListCtrl);
	ScreenToClient(&rectListCtrl);
	m_EditLog.GetWindowRect(&rectEdit);
	ScreenToClient(&rectEdit);

	rectListCtrl.right = rectView.Width();
	rectEdit.left = rectView.Width() + 1;
	rectEdit.right = rectFrame.Width();

// 	rectListCtrl.right = rectView.Width() * 2 / 3;
// 	rectEdit.left = rectListCtrl.right + 1;
// 	rectEdit.right = rectView.Width();

	m_ListThumbNail.MoveWindow(rectListCtrl);
	m_EditLog.MoveWindow(rectEdit);
}


BOOL CBottomDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRet = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);

	OnInitDialog();

	return bRet;
}

void CBottomDialogBar::DrawThumbnail(CString strPath,CString strFilter)
{
	strPath.TrimRight(_T("\\"));

	m_strPath = strPath;

	CString strFileFile;
	strFileFile.Format(_T("%s\\%s"),strPath,strFilter);

	// hold the window update to avoid flicking
	m_ListThumbNail.SetRedraw(FALSE);

	// reset our image list
	int nImageCount = m_ImageListThumbNail.GetImageCount();
	for (int i = 0; i < nImageCount; i++)
	{
		m_ImageListThumbNail.Remove(i);
	}

	m_ListThumbNail.DeleteAllItems();

	CFileFind ff;
	BOOL bFind = ff.FindFile(strFileFile);

	int nItem = 0;
	while (bFind)
	{
		bFind = ff.FindNextFile();

		if (ff.IsDirectory())
		{
			continue;
		}

		Bitmap img(ff.GetFilePath());

		Bitmap *pThumbNail = (Bitmap *)img.GetThumbnailImage(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT);

		CBitmap*    pImage = NULL;
		HBITMAP		hBmp = NULL;
		pThumbNail->GetHBITMAP(NULL,&hBmp);

		pImage = new CBitmap();
		pImage->Attach(hBmp);

		m_ImageListThumbNail.SetImageCount(nItem+1);
		m_ImageListThumbNail.Replace(nItem,pImage,NULL);

		m_ListThumbNail.InsertItem(nItem,ff.GetFileName(),nItem);
		nItem++;

		delete pImage;
		delete pThumbNail;
	}

	m_ListThumbNail.SetRedraw(TRUE);
}

void CBottomDialogBar::OnNMDblclkListThumbnail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nItem = pNMItemActivate->iItem;

	if (nItem != -1)
	{
		CString strFileName = m_ListThumbNail.GetItemText(nItem,0);

		CString strPath;
		strPath.Format(_T("%s\\%s"),m_strPath,strFileName);

		AfxGetApp()->OpenDocumentFile(strPath);
	}

	*pResult = 0;
}
