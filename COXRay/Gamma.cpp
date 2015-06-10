// Gamma.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "Gamma.h"
// CGamma

#define INSET_LINE_COUNT 10

IMPLEMENT_DYNAMIC(CGamma, CStatic)

CGamma::CGamma()
{
	m_dbGamma = 1.0;
	m_colorBackground = RGB(32,32,32);
	m_colorBorder = RGB(0,255,0);
	m_colorInset = RGB(255,255,255);
	m_colorLine = RGB(255,0,0);
	m_colorText = RGB(255,255,0);

	m_LinePoints = NULL;
	m_nTotalPointCount = 0;
	m_bMove = FALSE;

	m_hNotifyWnd = NULL;
}

CGamma::~CGamma()
{
	if (m_LinePoints)
	{
		delete []m_LinePoints;
	}
}


BEGIN_MESSAGE_MAP(CGamma, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CGamma 消息处理程序




void CGamma::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	CRect rectCtrl;

	GetClientRect(&rectCtrl);

	m_rectGamma.left = rectCtrl.left + 2;
	m_rectGamma.right = rectCtrl.right - 2;
	m_rectGamma.top = rectCtrl.top + 2;
	m_rectGamma.bottom = rectCtrl.bottom - 2;

	CDC memDC;
	CBitmap memBitMap;
	memDC.CreateCompatibleDC(&dc);
	memBitMap.CreateCompatibleBitmap(&dc,rectCtrl.Width(),rectCtrl.Height());
	memDC.SelectObject(memBitMap);
	memDC.FillSolidRect(m_rectGamma,m_colorBackground);

	CBrush *pOldBrush = (CBrush *)memDC.SelectStockObject(NULL_BRUSH);

	//画外边框
	CPen drawPen,*oldPen;
	drawPen.CreatePen(PS_SOLID,2,m_colorBorder);
	oldPen = memDC.SelectObject(&drawPen);
	memDC.Rectangle(m_rectGamma);
	
	//画内边框
	drawPen.DeleteObject();
	drawPen.CreatePen(PS_DOT,1,m_colorInset);
	memDC.SelectObject(&drawPen);

	int gap = (m_rectGamma.Width() + INSET_LINE_COUNT - 1) / INSET_LINE_COUNT;

	for (int x = m_rectGamma.left + gap; x < m_rectGamma.right; x += gap)
	{
		memDC.MoveTo(x,m_rectGamma.top);
		memDC.LineTo(x,m_rectGamma.bottom);
	}

	for (int y = m_rectGamma.top + gap; y < m_rectGamma.right; y += gap)
	{
		memDC.MoveTo(m_rectGamma.left,y);
		memDC.LineTo(m_rectGamma.right,y);
	}

	// 画曲线
	drawPen.DeleteObject();
	drawPen.CreatePen(PS_SOLID,2,m_colorLine);
	memDC.SelectObject(&drawPen);

	m_nTotalPointCount = m_rectGamma.Width();
	if (m_LinePoints)
	{
		delete []m_LinePoints;
		m_LinePoints = NULL;
	}
	m_LinePoints = new CPoint[m_nTotalPointCount];
	m_LinePoints[0].x = m_rectGamma.left;
	m_LinePoints[0].y = m_rectGamma.bottom;
	m_LinePoints[m_nTotalPointCount - 1].x = m_rectGamma.right;
	m_LinePoints[m_nTotalPointCount - 1].y = m_rectGamma.top;

	for (int i = 1; i < m_nTotalPointCount - 1;i++)
	{
		double y = pow((double)i/m_nTotalPointCount,1/m_dbGamma) * m_nTotalPointCount;
		
		m_LinePoints[i].x = m_rectGamma.left + i;
		m_LinePoints[i].y = m_rectGamma.bottom - y - 0.5;
	}

	memDC.PolyBezier(m_LinePoints,m_nTotalPointCount);

	// 写文字
	CString strText;
	strText.Format(_T("Gamma=%.2f"),m_dbGamma);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(m_colorText);
	memDC.TextOut(m_rectGamma.left + 2,m_rectGamma.top + 2,strText);

	dc.BitBlt(0,0,rectCtrl.Width(),rectCtrl.Height(),&memDC,0,0,SRCCOPY);

	memDC.DeleteDC();
	memBitMap.DeleteObject();
}


void CGamma::OnSize(UINT nType, int cx, int cy)
{
	cx = cy = min(cx,cy);
	CStatic::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CGamma::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ((nFlags & MK_LBUTTON) && m_bMove)
	{
		double x = (double)(point.x - m_rectGamma.left) / m_nTotalPointCount;
		double y = (double)(point.y - m_rectGamma.top) / m_nTotalPointCount;
		m_dbGamma = log(x) / log(y);

		Invalidate();

		::SendMessage(m_hNotifyWnd,WM_GAMMA_CHANGE_PREVIEW,NULL,NULL);
	}

	CStatic::OnMouseMove(nFlags, point);
}

BOOL CGamma::IsPointInLine( CPoint point )
{
	BOOL bIn = FALSE;
	for (int i = 0; i < m_nTotalPointCount;i++)
	{
		CRect rect(m_LinePoints[i].x - 2,m_LinePoints[i].y - 2,m_LinePoints[i].x + 2,m_LinePoints[i].y + 2);
		if (rect.PtInRect(point))
		{
			bIn = TRUE;
			break;
		}
	}

	return bIn;
}


void CGamma::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bMove = IsPointInLine(point);

	if (!m_bMove)
	{
		m_dbGamma = 1.0;
		Invalidate();
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CGamma::SetGamma( double dbGamma )
{
	m_dbGamma = dbGamma;

	Invalidate();
}


void CGamma::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bMove)
	{
		m_bMove = FALSE;

		::SendMessage(m_hNotifyWnd,WM_GAMMA_CHANGE_ENSURE,NULL,NULL);
	}

	CStatic::OnLButtonUp(nFlags, point);
}
