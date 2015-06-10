// PageImageProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageImageProcess.h"

#include "COXRayDoc.h"
#include "COXRayView.h"
#include "MainFrm.h"

// CPageImageProcess 对话框

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


// CPageImageProcess 消息处理程序


BOOL CPageImageProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	m_BtnUndo.SetTooltipText(_T("撤销"));

	m_BtnFitWindow.SubclassDlgItem(IDC_BTN_FIT_WINDOW,this);
	m_BtnFitWindow.SetTooltipText(_T("适屏"));

	m_BtnZoomIn.SubclassDlgItem(IDC_BTN_ZOOM_IN,this);
	m_BtnZoomIn.SetTooltipText(_T("放大"));

	m_BtnZoom11.SubclassDlgItem(IDC_BTN_ZOOM_11,this);
	m_BtnZoom11.SetTooltipText(_T("实际大小"));

	m_BtnZoomOut.SubclassDlgItem(IDC_BTN_ZOOM_OUT,this);
	m_BtnZoomOut.SetTooltipText(_T("缩小"));

	m_BtnRotateLeft.SubclassDlgItem(IDC_BTN_ROTATE_LEFT,this);
	m_BtnRotateLeft.SetTooltipText(_T("左转90度"));

	m_BtnRotateRight.SubclassDlgItem(IDC_BTN_ROTATE_RIGHT,this);
	m_BtnRotateRight.SetTooltipText(_T("右转90度"));

	m_BtnRotate.SubclassDlgItem(IDC_BTN_ROTATE,this);
	m_BtnRotate.SetTooltipText(_T("任意角度旋转"));

	m_BtnMirror.SubclassDlgItem(IDC_BTN_MIRROR,this);
	m_BtnMirror.SetTooltipText(_T("左右镜像"));

	m_BtnFlip.SubclassDlgItem(IDC_BTN_FLIP,this);
	m_BtnFlip.SetTooltipText(_T("上下翻转"));

	//
	m_BtnMove.SubclassDlgItem(IDC_BTN_MOVE,this);
	m_BtnMove.SetTooltipText(_T("移动"));
	m_BtnMove.SetCheckBox();

	m_BtnSelect.SubclassDlgItem(IDC_BTN_SELECT,this);
	m_BtnSelect.SetTooltipText(_T("选择"));
	m_BtnSelect.SetCheckBox();

	m_BtnZoom.SubclassDlgItem(IDC_BTN_ZOOM,this);
	m_BtnZoom.SetTooltipText(_T("缩放"));
	m_BtnZoom.SetCheckBox();

	m_BtnLine.SubclassDlgItem(IDC_BTN_LINE,this);
	m_BtnLine.SetTooltipText(_T("画直线"));
	m_BtnLine.SetCheckBox();

	m_BtnRect.SubclassDlgItem(IDC_BTN_RECT,this);
	m_BtnRect.SetTooltipText(_T("画矩形"));
	m_BtnRect.SetCheckBox();

	m_BtnEllipse.SubclassDlgItem(IDC_BTN_ELLIPSE,this);
	m_BtnEllipse.SetTooltipText(_T("画椭圆"));
	m_BtnEllipse.SetCheckBox();

	m_BtnDist.SubclassDlgItem(IDC_BTN_DIST,this);
	m_BtnDist.SetTooltipText(_T("测距"));
	m_BtnDist.SetCheckBox();

	m_BtnDegree.SubclassDlgItem(IDC_BTN_DEGREE,this);
	m_BtnDegree.SetTooltipText(_T("量角"));
	m_BtnDegree.SetCheckBox();

	m_BtnText.SubclassDlgItem(IDC_BTN_TEXT,this);
	m_BtnText.SetTooltipText(_T("文字"));
	m_BtnText.SetCheckBox();

	//
	m_BtnNegative.SubclassDlgItem(IDC_BTN_NEGATIVE,this);
	m_BtnNegative.SetTooltipText(_T("负片"));

	m_BtnThreshold.SubclassDlgItem(IDC_BTN_THRESHOLD,this);
	m_BtnThreshold.SetTooltipText(_T("阈值"));

	m_BtnMean.SubclassDlgItem(IDC_BTN_MEAN,this);
	m_BtnMean.SetTooltipText(_T("均值滤波"));

	m_BtnMedian.SubclassDlgItem(IDC_BTN_MEDIAN,this);
	m_BtnMedian.SetTooltipText(_T("中值滤波"));

	m_BtnGauss.SubclassDlgItem(IDC_BTN_GAUSS,this);
	m_BtnGauss.SetTooltipText(_T("高斯滤波"));

	m_BtnEnhance.SubclassDlgItem(IDC_BTN_ENHANCE,this);
	m_BtnEnhance.SetTooltipText(_T("增强"));

	m_SliderGamma.SetRange(0,89);
	m_SliderGamma.SetTicFreq(10);

	double angle = atan(m_dbEditGamma);
	m_SliderGamma.SetPos((int)DEGREE(angle));

	m_Gamma.SetGamma(m_dbEditGamma);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageImageProcess::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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
	if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
	{  
		CRect rectCtrl;   //获取控件变化前的大小    
		pWnd->GetWindowRect(&rectCtrl);  
		ScreenToClient(&rectCtrl);//将控件大小转换为在对话框中的区域坐标 

		int iLeft = rectCtrl.left;
		int iTop = rectCtrl.top;
		int iWidth = rectCtrl.Width();
		int iHeight = rectCtrl.Height();

		// 改变X坐标
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// 改变Y坐标
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//改变宽度
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// 改变高度
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y)
		{
			iHeight = iHeight * cy / m_rect.Height();
		}

		//改变宽度
		if ((flag & SIZE_ELASTIC_X_EX) == SIZE_ELASTIC_X_EX)
		{
			iWidth = cx - iLeft - 10;
		}

		// 改变高度
		if ((flag & SIZE_ELASTIC_Y_EX) == SIZE_ELASTIC_Y_EX)
		{
			iHeight = cy - iTop - 10;
		}

		pWnd->MoveWindow(iLeft,iTop,iWidth,iHeight);

	}  
}


LRESULT CPageImageProcess::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar; 
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();

	//多个 SLIDER 控件控制 
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	::SendMessage(pView->GetSafeHwnd(),WM_GAMMA_CHANGE_ENSURE,0,0);
}


void CPageImageProcess::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加专用代码和/或调用基类
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
