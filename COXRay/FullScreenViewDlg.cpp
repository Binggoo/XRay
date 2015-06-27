// FullScreenViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "FullScreenViewDlg.h"
#include "afxdialogex.h"


// CFullScreenViewDlg �Ի���

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


// CFullScreenViewDlg ��Ϣ�������


BOOL CFullScreenViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
		AfxMessageBox(_T("ͼ�񲻴��ڻ��ʽ����ȷ��"));

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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFullScreenViewDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);

	int cx = 0, cy = 0;

	int nWidth = m_hImage.Width();
	int nHeight = m_hImage.Height();

	// ������ʾ
	if (rect.Width() > nWidth)
	{
		cx = (rect.Width() - nWidth) / 2;
	}


	if (rect.Height() > nHeight)
	{
		cy = (rect.Height() - nHeight) / 2;
	}


	// ע���͸��Ƿ���
	m_phWindow->SetWindowExtents(cy,cx,nWidth,nHeight);
	m_phWindow->SetPart(0,0,nHeight,nWidth);


	m_phWindow->Display(m_hImage);

}
