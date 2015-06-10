// PageImageProcess.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageImageProcess.h"

#include "COXRayDoc.h"
#include "COXRayView.h"
#include "MainFrm.h"

// CPageImageProcess �Ի���

IMPLEMENT_DYNAMIC(CPageImageProcess, CDialogEx)

CPageImageProcess::CPageImageProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageImageProcess::IDD, pParent)
{
	m_dbEditGamma = 1.0;
	m_bLBDown = FALSE;
}

CPageImageProcess::~CPageImageProcess()
{
}

void CPageImageProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GAMMA, m_Gamma);
	DDX_Control(pDX,IDC_SLIDER_GAMMA,m_SliderGamma);
	//DDX_Text(pDX,IDC_EDIT_GAMMA,m_dbEditGamma);
}


BEGIN_MESSAGE_MAP(CPageImageProcess, CDialogEx)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CPageImageProcess ��Ϣ�������


BOOL CPageImageProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetClientRect(&m_rect);

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

	// Button
	m_BtnUndo.SubclassDlgItem(ID_EDIT_UNDO,this);
	m_BtnUndo.SetTooltipText(_T("����"));

	m_BtnFitWindow.SubclassDlgItem(IDC_BTN_FIT_WINDOW,this);
	m_BtnFitWindow.SetTooltipText(_T("����"));

	m_BtnZoomIn.SubclassDlgItem(IDC_BTN_ZOOM_IN,this);
	m_BtnZoomIn.SetTooltipText(_T("�Ŵ�"));

	m_BtnZoom11.SubclassDlgItem(IDC_BTN_ZOOM_11,this);
	m_BtnZoom11.SetTooltipText(_T("ʵ�ʴ�С"));

	m_BtnZoomOut.SubclassDlgItem(IDC_BTN_ZOOM_OUT,this);
	m_BtnZoomOut.SetTooltipText(_T("��С"));

	m_BtnRotateLeft.SubclassDlgItem(IDC_BTN_ROTATE_LEFT,this);
	m_BtnRotateLeft.SetTooltipText(_T("��ת90��"));

	m_BtnRotateRight.SubclassDlgItem(IDC_BTN_ROTATE_RIGHT,this);
	m_BtnRotateRight.SetTooltipText(_T("��ת90��"));

	m_BtnRotate.SubclassDlgItem(IDC_BTN_ROTATE,this);
	m_BtnRotate.SetTooltipText(_T("����Ƕ���ת"));

	m_BtnMirror.SubclassDlgItem(IDC_BTN_MIRROR,this);
	m_BtnMirror.SetTooltipText(_T("���Ҿ���"));

	m_BtnFlip.SubclassDlgItem(IDC_BTN_FLIP,this);
	m_BtnFlip.SetTooltipText(_T("���·�ת"));

	//
	m_BtnMove.SubclassDlgItem(IDC_BTN_MOVE,this);
	m_BtnMove.SetTooltipText(_T("�ƶ�"));
	m_BtnMove.SetCheckBox();

	m_BtnSelect.SubclassDlgItem(IDC_BTN_SELECT,this);
	m_BtnSelect.SetTooltipText(_T("ѡ��"));
	m_BtnSelect.SetCheckBox();

	m_BtnZoom.SubclassDlgItem(IDC_BTN_ZOOM,this);
	m_BtnZoom.SetTooltipText(_T("����"));
	m_BtnZoom.SetCheckBox();

	m_BtnLine.SubclassDlgItem(IDC_BTN_LINE,this);
	m_BtnLine.SetTooltipText(_T("��ֱ��"));
	m_BtnLine.SetCheckBox();

	m_BtnRect.SubclassDlgItem(IDC_BTN_RECT,this);
	m_BtnRect.SetTooltipText(_T("������"));
	m_BtnRect.SetCheckBox();

	m_BtnEllipse.SubclassDlgItem(IDC_BTN_ELLIPSE,this);
	m_BtnEllipse.SetTooltipText(_T("����Բ"));
	m_BtnEllipse.SetCheckBox();

	m_BtnDist.SubclassDlgItem(IDC_BTN_DIST,this);
	m_BtnDist.SetTooltipText(_T("���"));
	m_BtnDist.SetCheckBox();

	m_BtnDegree.SubclassDlgItem(IDC_BTN_DEGREE,this);
	m_BtnDegree.SetTooltipText(_T("����"));
	m_BtnDegree.SetCheckBox();

	m_BtnText.SubclassDlgItem(IDC_BTN_TEXT,this);
	m_BtnText.SetTooltipText(_T("����"));
	m_BtnText.SetCheckBox();

	//
	m_BtnNegative.SubclassDlgItem(IDC_BTN_NEGATIVE,this);
	m_BtnNegative.SetTooltipText(_T("��Ƭ"));

	m_BtnThreshold.SubclassDlgItem(IDC_BTN_THRESHOLD,this);
	m_BtnThreshold.SetTooltipText(_T("��ֵ"));

	m_BtnMean.SubclassDlgItem(IDC_BTN_MEAN,this);
	m_BtnMean.SetTooltipText(_T("��ֵ�˲�"));

	m_BtnMedian.SubclassDlgItem(IDC_BTN_MEDIAN,this);
	m_BtnMedian.SetTooltipText(_T("��ֵ�˲�"));

	m_BtnGauss.SubclassDlgItem(IDC_BTN_GAUSS,this);
	m_BtnGauss.SetTooltipText(_T("��˹�˲�"));

	m_BtnEnhance.SubclassDlgItem(IDC_BTN_ENHANCE,this);
	m_BtnEnhance.SetTooltipText(_T("��ǿ"));

	m_SliderGamma.SetRange(0,89);
	m_SliderGamma.SetTicFreq(10);

	double angle = atan(m_dbEditGamma);
	m_SliderGamma.SetPos((int)DEGREE(angle));

	m_Gamma.SetGamma(m_dbEditGamma);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPageImageProcess::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (nType == SIZE_MINIMIZED)
	{
		return;
	}

	CWnd *pWnd = NULL;
	pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		DWORD dwFlag = SIZE_MOVE_X | SIZE_MOVE_Y;
		ChangeSize(pWnd,cx,cy,dwFlag);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	GetClientRect(&m_rect);
}

void CPageImageProcess::ChangeSize( CWnd *pWnd,int cx,int cy,DWORD flag )
{
	if(pWnd)  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����   
	{  
		CRect rectCtrl;   //��ȡ�ؼ��仯ǰ�Ĵ�С    
		pWnd->GetWindowRect(&rectCtrl);  
		ScreenToClient(&rectCtrl);//���ؼ���Сת��Ϊ�ڶԻ����е��������� 

		int iLeft = rectCtrl.left;
		int iTop = rectCtrl.top;
		int iWidth = rectCtrl.Width();
		int iHeight = rectCtrl.Height();

		// �ı�X����
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// �ı�Y����
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//�ı���
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// �ı�߶�
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y)
		{
			iHeight = iHeight * cy / m_rect.Height();
		}

		//�ı���
		if ((flag & SIZE_ELASTIC_X_EX) == SIZE_ELASTIC_X_EX)
		{
			iWidth = cx - iLeft - 10;
		}

		// �ı�߶�
		if ((flag & SIZE_ELASTIC_Y_EX) == SIZE_ELASTIC_Y_EX)
		{
			iHeight = cy - iTop - 10;
		}

		pWnd->MoveWindow(iLeft,iTop,iWidth,iHeight);

	}  
}


LRESULT CPageImageProcess::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) != IDC_SLIDER_GAMMA)
		{
			return ::SendMessage(GetParent()->GetParent()->GetSafeHwnd(),message,wParam,lParam);
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CPageImageProcess::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar; 
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();

	//��� SLIDER �ؼ����� 
	switch( pSlider->GetDlgCtrlID() ) 
	{ 
	case IDC_SLIDER_GAMMA:
		{
			m_dbEditGamma = tan(RAD(pSlider->GetPos()));

			m_Gamma.SetGamma(m_dbEditGamma);
			
			if (m_bLBDown)
			{
				::SendMessage(pView->GetSafeHwnd(),WM_GAMMA_CHANGE_PREVIEW,0,0);
			}
			
		}
		
		break;
	} 

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPageImageProcess::OnBnClickedBtnGamma()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	::SendMessage(pView->GetSafeHwnd(),WM_GAMMA_CHANGE_ENSURE,0,0);
}


void CPageImageProcess::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	m_SliderGamma.GetWindowRect(&rect);
	ScreenToClient(&rect);

	if (rect.PtInRect(point))
	{
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
		::SendMessage(pView->GetSafeHwnd(),WM_GAMMA_CHANGE_ENSURE,0,0);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CPageImageProcess::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		CRect rect;
		m_SliderGamma.GetClientRect(&rect);
		m_SliderGamma.ClientToScreen(&rect);

		if (rect.PtInRect(pMsg->pt))
		{
			m_bLBDown = TRUE;
		}

	}

	if (pMsg->message == WM_LBUTTONUP)
	{

		if (m_bLBDown)
		{
			m_bLBDown = FALSE;
			CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
			CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
			::SendMessage(pView->GetSafeHwnd(),WM_GAMMA_CHANGE_ENSURE,0,0);
		}
		
	}

	if (pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
