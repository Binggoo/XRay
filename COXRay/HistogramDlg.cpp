// HistogramDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "COXRayDoc.h"
#include "COXRayView.h"

// CHistogramDlg �Ի���

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramDlg::IDD, pParent)
	, m_nEditMinGray(0)
	, m_nEditMaxGray(0)
{
	if (pParent)
	{
		m_hParent = pParent->GetSafeHwnd();
	}
	else
	{
		m_hParent = GetOwner()->GetSafeHwnd();
	}
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MIN_GRAY, m_nEditMinGray);
	DDX_Text(pDX, IDC_EDIT_MAX_GRAY, m_nEditMaxGray);
	DDX_Control(pDX,IDC_PIC_HISTO,m_Histo);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_MIN_GRAY, &CHistogramDlg::OnEnChangeEditMinGray)
	ON_EN_CHANGE(IDC_EDIT_MAX_GRAY, &CHistogramDlg::OnEnChangeEditMaxGray)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MIN_GRAY, &CHistogramDlg::OnDeltaposSpinMinGray)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAX_GRAY, &CHistogramDlg::OnDeltaposSpinMaxGray)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CHistogramDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_DEFAULT, &CHistogramDlg::OnBnClickedBtnDefault)
END_MESSAGE_MAP()


// CHistogramDlg ��Ϣ�������


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

}

void CHistogramDlg::OnEnChangeEditMinGray()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_nEditMinGray > m_nEditMaxGray)
	{
		m_nEditMinGray = m_nEditMaxGray;

		return;
	}

	if (m_nEditMinGray > m_nMaxValue)
	{
		m_nEditMinGray = m_nMaxValue;
		return;
	}

	if (m_nEditMinGray < m_nMinValue)
	{
		m_nEditMinGray = m_nMinValue;
		return;
	}

	//Invalidate();
	//InvalidateRect(m_RectFrame);
	m_Histo.SetBorder(m_nEditMinGray,m_nEditMaxGray);

	Refresh();
}


void CHistogramDlg::OnEnChangeEditMaxGray()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_nEditMaxGray < m_nEditMinGray)
	{
		m_nEditMaxGray = m_nEditMinGray;

		return;
	}

	if (m_nEditMaxGray < m_nMinValue)
	{
		m_nEditMaxGray = m_nMinValue;

		return;
	}

	if (m_nEditMaxGray > m_nMaxValue)
	{
		m_nEditMaxGray = m_nMaxValue;
		return;
	}

	//Invalidate();
	//InvalidateRect(m_RectFrame);
	m_Histo.SetBorder(m_nEditMinGray,m_nEditMaxGray);

	Refresh();
}


void CHistogramDlg::OnDeltaposSpinMinGray(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (pNMUpDown->iDelta == -1)
	{
		m_nEditMinGray++;

		if (m_nEditMinGray > m_nEditMaxGray)
		{
			m_nEditMinGray = m_nEditMaxGray;
		}

		if (m_nEditMinGray > m_nMaxValue)
		{
			m_nEditMinGray = m_nMaxValue;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditMinGray--;

		if (m_nEditMinGray < m_nMinValue)
		{
			m_nEditMinGray = m_nMinValue;
		}
	}

	UpdateData(FALSE);

	m_Histo.SetBorder(m_nEditMinGray,m_nEditMaxGray);

	//Invalidate();
	//InvalidateRect(m_RectFrame);

	Refresh();

	*pResult = 0;
}


void CHistogramDlg::OnDeltaposSpinMaxGray(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (pNMUpDown->iDelta == -1)
	{
		m_nEditMaxGray++;

		if (m_nEditMaxGray > m_nMaxValue)
		{
			m_nEditMaxGray = m_nMaxValue;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditMaxGray--;

		if (m_nEditMaxGray < m_nEditMinGray)
		{
			m_nEditMaxGray = m_nEditMinGray;
		}

		if (m_nEditMaxGray < m_nMinValue)
		{
			m_nEditMaxGray = m_nMinValue;
		}
	}

	UpdateData(FALSE);

	m_Histo.SetBorder(m_nEditMinGray,m_nEditMaxGray);

	//Invalidate();
	//InvalidateRect(m_RectFrame);

	Refresh();
	*pResult = 0;
}

void CHistogramDlg::Refresh()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	CCOXRayDoc *pDoc = (CCOXRayDoc *)pView->GetDocument();

	HWindow *pHWindow = pView->GetHWindow();
	HImage *pImage = pDoc->GetImage();

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	//16 λת��Ϊ8λ
	HImage hImageDst = pImage->CopyImage();

// 	if (GetImageBits(hImageDst) == 16)
// 	{
// 		hImageDst = ConvertImage(hImageDst,IPL_DEPTH_8U,3);
// 	}

	double dbZoomFactor = pDoc->GetZoomFactor();
	HImage hScaleImage = hImageDst.ZoomImageFactor(dbZoomFactor,dbZoomFactor,"constant");

	HImage hReduceImage = hScaleImage.ReduceDomain(m_hRegion);

	HRegion hRegion = hReduceImage.Threshold(m_nEditMinGray,m_nEditMaxGray);
	hRegion = hRegion.Connection();

	pHWindow->SetColor("green");
	pHWindow->SetDraw("margin");
	pHWindow->SetLineWidth(1);
	pHWindow->Display(hRegion);
}


BOOL CHistogramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHistogramDlg::SetMinMaxValue( int nMinValue,int nMaxValue )
{
	m_nEditMaxGray = m_nMaxValue = nMaxValue;
	m_nEditMinGray = m_nMinValue = nMinValue;

	if (m_hWnd)
	{
		UpdateData(FALSE);
		Invalidate();
		//InvalidateRect(m_RectFrame);
	}
}

void CHistogramDlg::SetRegion( HRegion region )
{
	m_hRegion = region;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	CCOXRayDoc *pDoc = (CCOXRayDoc *)pView->GetDocument();

	HWindow *pHWindow = pView->GetHWindow();
	HImage *pImage = pDoc->GetImage();

	//16 λת��Ϊ8λ
	HImage hImageDst = pImage->CopyImage();

// 	if (GetImageBits(hImageDst) == 16)
// 	{
// 		hImageDst = ConvertImage(hImageDst,IPL_DEPTH_8U,3);
// 	}

	double dbZoomScale = pDoc->GetZoomFactor();
	HImage hScaleImage = hImageDst.ZoomImageFactor(dbZoomScale,dbZoomScale,"constant");

	double dbMin,dbMax,dbRange;
	dbMin = m_hRegion.MinMaxGray(hScaleImage,0,&dbMax,&dbRange);

	m_nEditMinGray = m_nMinValue = (int)dbMin;
	m_nEditMaxGray = m_nMaxValue = (int)dbMax;

	m_htAbsoluteHisto = m_hRegion.GrayHistoAbs(hScaleImage,1);

	if (m_hWnd)
	{
		m_Histo.SetHisto(m_htAbsoluteHisto);
		m_Histo.SetBorder(m_nEditMinGray,m_nEditMaxGray);
		UpdateData(FALSE);

		Refresh();
	}
}


void CHistogramDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if (bShow)
	{
		
	}
}


void CHistogramDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_nEditMinGray > m_nEditMaxGray || m_nEditMinGray < m_nMinValue || m_nEditMaxGray > m_nMaxValue)
	{
		return;
	}

	::PostMessage(m_hParent,WM_ADD_GRAY_RANGE,(WPARAM)m_nEditMinGray,(LPARAM)m_nEditMaxGray);
	CDialogEx::OnOK();
}


void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CHistogramDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CHistogramDlg::OnBnClickedBtnDefault()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nEditMinGray = m_nMinValue;
	m_nEditMaxGray = m_nMaxValue;

	m_Histo.SetBorder(m_nEditMinGray,m_nEditMaxGray);

	UpdateData(FALSE);

	Refresh();
}
