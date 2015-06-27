// FullScreenViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "FullScreenViewDlg.h"
#include "afxdialogex.h"


// CFullScreenViewDlg 对话框

IMPLEMENT_DYNAMIC(CFullScreenViewDlg, CDialogEx)

CFullScreenViewDlg::CFullScreenViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFullScreenViewDlg::IDD, pParent)
{
	m_phWindow = NULL;
}

CFullScreenViewDlg::~CFullScreenViewDlg()
{
}

void CFullScreenViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFullScreenViewDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CFullScreenViewDlg 消息处理程序


BOOL CFullScreenViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);

	USES_CONVERSION;
	char *file = W2A(m_strFilePath);

	try
	{
		m_hImage = HImage::ReadImage(file);
	}
	catch (HException &)
	{
		AfxMessageBox(_T("图像不存在或格式不正确！"));

		CDialogEx::OnOK();

		return TRUE;
	}

	double dbScaleWidth = 0.0,dbScaleHeight = 0.0,dbZoomScale;
	dbScaleWidth = (double)cx / m_hImage.Width();
	dbScaleHeight = (double)cy / m_hImage.Height();
	dbZoomScale = min(dbScaleWidth,dbScaleHeight);

	m_hImage = m_hImage.ZoomImageFactor(dbZoomScale,dbZoomScale,"constant");

	if (m_phWindow == NULL)
	{
		m_phWindow = new HWindow(0,0,-1,-1,(Hlong)m_hWnd);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFullScreenViewDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);

	int cx = 0, cy = 0;

	int nWidth = m_hImage.Width();
	int nHeight = m_hImage.Height();

	// 居中显示
	if (rect.Width() > nWidth)
	{
		cx = (rect.Width() - nWidth) / 2;
	}


	if (rect.Height() > nHeight)
	{
		cy = (rect.Height() - nHeight) / 2;
	}


	// 注意宽和高是反的
	m_phWindow->SetWindowExtents(cy,cx,nWidth,nHeight);
	m_phWindow->SetPart(0,0,nHeight,nWidth);


	m_phWindow->Display(m_hImage);

}
