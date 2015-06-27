// Histogram.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "Histogram.h"


// CHistogram

IMPLEMENT_DYNAMIC(CHistogram, CStatic)

CHistogram::CHistogram()
{
	m_colBackground = RGB(255,255,255);
	m_colText = RGB(0,255,0);
	m_colLine = RGB(0,0,255);
	m_colBorder = RGB(255,0,0);
}

CHistogram::~CHistogram()
{
}


BEGIN_MESSAGE_MAP(CHistogram, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CHistogram::SetBorder( int left,int right)
{
	m_nLeft = left;
	m_nRight = right;

	if (m_hWnd)
	{
		Invalidate();
	}
}


// CHistogram 消息处理程序




void CHistogram::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	CRect rect;
	GetClientRect(&rect);

	m_RectFrame.left = rect.left + 30;
	m_RectFrame.right = rect.right - 30;
	m_RectFrame.top = rect.top + 30;
	m_RectFrame.bottom = rect.bottom - 30;

	CDC memDC;
	CBitmap memBitMap;
	memDC.CreateCompatibleDC(&dc);
	memBitMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	memDC.SelectObject(memBitMap);
	memDC.FillSolidRect(rect,m_colBackground);
	CBrush *pOldBrush = (CBrush *)memDC.SelectStockObject(NULL_BRUSH);

	DrawHisto(&memDC);
	DrawBorder(&memDC);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	memDC.DeleteDC();
	memBitMap.DeleteObject();
}


void CHistogram::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Point = point;

	Invalidate();

	CStatic::OnMouseMove(nFlags, point);
}

void CHistogram::DrawHisto( CDC *pDC )
{
	double dbPixXPerGrayValue = m_RectFrame.Width() * 1.0 / (m_nRight - m_nLeft);
	double dbPixYPerFreq = m_RectFrame.Height() * 1.0 / (m_htAbsoluteHisto.Max()[0].L() - m_htAbsoluteHisto.Min()[0].L());

	CPen pen(PS_SOLID,1,m_colLine);
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);

	for (int i = m_nLeft; i <= m_nRight;i++)
	{
		int x = m_RectFrame.left + dbPixXPerGrayValue * (i - m_nLeft);
		int y = m_RectFrame.bottom - dbPixYPerFreq * m_htAbsoluteHisto[i].L();

		pDC->MoveTo(x,m_RectFrame.bottom);
		pDC->LineTo(x,y);
	}

	if (m_Point.x < m_RectFrame.left)
	{
		m_Point.x = m_RectFrame.left;
	}

	if (m_Point.x > m_RectFrame.right)
	{
		m_Point.x = m_RectFrame.right;
	}

	if (m_Point.y < m_RectFrame.bottom)
	{
		m_Point.y = m_RectFrame.bottom;
	}

	if (m_Point.y > m_RectFrame.top)
	{
		m_Point.y = m_RectFrame.top;
	}

	//if (m_RectFrame.PtInRect(m_Point))
	{
		pen.DeleteObject();
		pen.CreatePen(PS_DASH,1,RGB(0,0,0));

		pDC->SelectObject(&pen);

		pDC->MoveTo(m_Point);
		pDC->LineTo(m_Point.x,m_RectFrame.top);

		pDC->MoveTo(m_Point);
		pDC->LineTo(m_Point.x,m_RectFrame.bottom);

		CString str;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));

		str.Format(_T("%d"),(int)((m_Point.x - m_RectFrame.left) / dbPixXPerGrayValue + m_nLeft));
		pDC->TextOut(m_Point.x - 10,m_RectFrame.bottom + 5,str);
		pDC->TextOut(m_Point.x - 10,m_RectFrame.top + 5,str);
	}

	pDC->SelectObject(oldPen);
}

void CHistogram::DrawBorder( CDC *pDC )
{
	CPen pen(PS_SOLID,1,m_colBorder);
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);

	pDC->MoveTo(m_RectFrame.left,m_RectFrame.bottom);
	pDC->LineTo(m_RectFrame.right,m_RectFrame.bottom);

	pDC->MoveTo(m_RectFrame.left,m_RectFrame.bottom);
	pDC->LineTo(m_RectFrame.left,m_RectFrame.top);

	CString str;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colText);

	str.Format(_T("%d"),m_nLeft);
	pDC->TextOut(m_RectFrame.left - 10,m_RectFrame.bottom + 5,str);

	str.Format(_T("%d"),m_nRight);
	pDC->TextOut(m_RectFrame.right - 10,m_RectFrame.bottom + 5,str);

	str.Format(_T("%d"),m_htAbsoluteHisto.Min()[0].L());
	pDC->TextOut(m_RectFrame.left - 20,m_RectFrame.bottom - 5,str);

	str.Format(_T("%d"),m_htAbsoluteHisto.Max()[0].L());
	pDC->TextOut(m_RectFrame.left - 20,m_RectFrame.top - 5,str);
	pDC->SelectObject(oldPen);
}
