// LineProfile.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "LineProfile.h"

#define DEFAULT_COLS 5
#define DEFAULT_ROWS 5
// CLineProfile

IMPLEMENT_DYNAMIC(CLineProfile, CStatic)

CLineProfile::CLineProfile()
{
	m_nMinGray = m_nMaxGray = 0;

	m_nBottom = m_nTop = 0;

	m_dbStd = m_dbMeanGray = 0.0;

	m_bDrawMinMaxLine = m_bDrawMeanLine = m_bDrawStdLine = FALSE;

	m_colBackground = RGB(255,255,255);
	m_colText = RGB(0,0,0);
	m_colLine = RGB(255,0,0);
	m_colBorder = RGB(0,0,0);

	m_dbYPixPerGrayValue = m_dbXPixPerPoint = 0.0;

}

CLineProfile::~CLineProfile()
{
}


BEGIN_MESSAGE_MAP(CLineProfile, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CLineProfile::DrawBorder( CDC *pDC )
{
	CPen pen(PS_SOLID,2,m_colBorder);
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);

	// 水平线和垂直线
	pDC->MoveTo(m_RectFrame.left,m_RectFrame.bottom);
	pDC->LineTo(m_RectFrame.right,m_RectFrame.bottom);

	pDC->MoveTo(m_RectFrame.left,m_RectFrame.bottom);
	pDC->LineTo(m_RectFrame.left,m_RectFrame.top);

	CString str;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colText);
	// 画刻度
	for (int i = 0; i <= m_nTop - m_nBottom; i += m_nYgap )
	{
		int y = m_RectFrame.bottom - m_dbYPixPerGrayValue * i;

		pDC->MoveTo(m_RectFrame.left - 10,y);
		pDC->LineTo(m_RectFrame.left,y);

		str.Format(_T("%d"),m_nBottom + i);

		CSize sz = pDC->GetTextExtent(str);

		pDC->TextOut(m_RectFrame.left - sz.cx - 10,y - sz.cy / 2,str);
	}

	for (int i = 0; i <= m_htPoints.Num(); i+= m_nXgap)
	{
		int x = m_RectFrame.left + m_dbXPixPerPoint * i;

		pDC->MoveTo(x,m_RectFrame.bottom + 10);
		pDC->LineTo(x,m_RectFrame.bottom);

		str.Format(_T("%d"),i);

		CSize sz = pDC->GetTextExtent(str);

		pDC->TextOut(x - sz.cx / 2,m_RectFrame.bottom + sz.cy,str);
	}

	pDC->SelectObject(oldPen);
}

void CLineProfile::DrawCurve( CDC *pDC )
{
	// 先画背景格子
	CPen pen(PS_SOLID,1,RGB(128,128,128));
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);
	for (int i = 0; i <= m_nTop - m_nBottom; i += m_nYgap )
	{
		int y = m_RectFrame.bottom - m_dbYPixPerGrayValue * i;

		pDC->MoveTo(m_RectFrame.left,y);
		pDC->LineTo(m_RectFrame.right,y);
	}

	for (int i = 0; i <= m_htPoints.Num(); i+= m_nXgap)
	{
		int x = m_RectFrame.left + m_dbXPixPerPoint * i;

		pDC->MoveTo(x,m_RectFrame.top);
		pDC->LineTo(x,m_RectFrame.bottom);
	}

	// 画曲线
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID,1,m_colLine);
	pDC->SelectObject(&pen);

	// 3n + 1
	int nCount = (m_htPoints.Num() - 1) / 3 * 3 + 1;

	CPoint *pp = new CPoint[nCount];

	for (int i = 0; i < nCount;i++)
	{
		pp[i].x = m_RectFrame.left + m_dbXPixPerPoint * i;
		pp[i].y = m_RectFrame.bottom - (int)((m_htPoints[i].D() - m_nBottom) * m_dbYPixPerGrayValue);
	}
	
	pDC->PolyBezier(pp,nCount);

	delete []pp;

	pDC->SelectObject(oldPen);
}

void CLineProfile::DrawMinMaxLine( CDC *pDC )
{
	CPen pen(PS_DASH,1,RGB(0,0,255));
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);

	int y = m_RectFrame.bottom - (int)((m_nMinGray - m_nBottom) * m_dbYPixPerGrayValue);

	pDC->MoveTo(m_RectFrame.left,y);
	pDC->LineTo(m_RectFrame.right,y);

	y = m_RectFrame.bottom - (int)((m_nMaxGray - m_nBottom) * m_dbYPixPerGrayValue);

	pDC->MoveTo(m_RectFrame.left,y);
	pDC->LineTo(m_RectFrame.right,y);

	pDC->SelectObject(oldPen);
}

void CLineProfile::DrawMeanLine( CDC *pDC )
{
	CPen pen(PS_DASH,1,RGB(255,0,255));
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);

	int y = m_RectFrame.bottom - (int)((m_dbMeanGray - m_nBottom) * m_dbYPixPerGrayValue);

	pDC->MoveTo(m_RectFrame.left,y);
	pDC->LineTo(m_RectFrame.right,y);

	pDC->SelectObject(oldPen);
}

void CLineProfile::DrawStdLine( CDC *pDC )
{
	CPen pen(PS_DASH,1,RGB(0,255,255));
	CPen *oldPen = (CPen *)pDC->SelectObject(&pen);

	int y = m_RectFrame.bottom - (int)((m_dbMeanGray + m_dbStd - m_nBottom) * m_dbYPixPerGrayValue);

	pDC->MoveTo(m_RectFrame.left,y);
	pDC->LineTo(m_RectFrame.right,y);

	y = m_RectFrame.bottom - (int)((m_dbMeanGray - m_dbStd - m_nBottom) * m_dbYPixPerGrayValue);

	pDC->MoveTo(m_RectFrame.left,y);
	pDC->LineTo(m_RectFrame.right,y);

	pDC->SelectObject(oldPen);
}

// CLineProfile 消息处理程序




void CLineProfile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rect;
	GetClientRect(&rect);

	m_RectFrame.left = rect.left + 60;
	m_RectFrame.right = rect.right - 30;
	m_RectFrame.top = rect.top + 30;
	m_RectFrame.bottom = rect.bottom - 30;

	// 垂直方向
	int range = m_nMaxGray - m_nMinGray;
	m_nYgap = (range + DEFAULT_ROWS - 1) / DEFAULT_ROWS;

	if (m_nYgap == 0)
	{
		return;
	}

	if (m_nYgap < 5 && m_nYgap > 1)
	{
		m_nYgap = 5;
	}
	else if (m_nYgap < 10)
	{
		m_nYgap = 10;
	}
	else if (m_nYgap < 20)
	{
		m_nYgap = 20;
	}
	else if (m_nYgap < 50)
	{
		m_nYgap = 50;
	}
	else if (m_nYgap < 100)
	{
		m_nYgap = 100;
	}
	else if (m_nYgap < 200)
	{
		m_nYgap = 200;
	}
	else if (m_nYgap < 500)
	{
		m_nYgap = 500;
	}
	else if (m_nYgap < 1000)
	{
		m_nYgap = 1000; 
	}
	else if (m_nYgap < 2000)
	{
		m_nYgap = 2000;
	}
	else if (m_nYgap < 5000)
	{
		m_nYgap = 5000;
	}
	else if (m_nYgap < 10000)
	{
		m_nYgap = 10000;
	}
	else if (m_nYgap < 20000)
	{
		m_nYgap = 20000;
	}
	else if (m_nYgap < 50000)
	{
		m_nYgap = 50000;
	}

	// 求接近最小值最大值的整数值
	m_nTop = (m_nMaxGray + m_nYgap - 1) / m_nYgap * m_nYgap;
	m_nBottom = (m_nMinGray - 1) / m_nYgap * m_nYgap;

	m_dbYPixPerGrayValue = m_RectFrame.Height() * 1.0 / (m_nTop - m_nBottom);
	// 水平方向
	range = m_htPoints.Num();
	m_nXgap = (range + DEFAULT_COLS - 1) / DEFAULT_COLS;

	if (m_nXgap == 0)
	{
		return;
	}

	if (m_nXgap < 5 && m_nXgap > 1)
	{
		m_nXgap = 5;
	}
	else if (m_nXgap < 10)
	{
		m_nXgap = 10;
	}
	else if (m_nXgap < 20)
	{
		m_nXgap = 20;
	}
	else if (m_nXgap < 50)
	{
		m_nXgap = 50;
	}
	else if (m_nXgap < 100)
	{
		m_nXgap = 100;
	}
	else if (m_nXgap < 200)
	{
		m_nXgap = 200;
	}
	else if (m_nXgap < 500)
	{
		m_nXgap = 500;
	}
	else if (m_nXgap < 1000)
	{
		m_nXgap = 1000; 
	}
	else if (m_nXgap < 2000)
	{
		m_nXgap = 2000;
	}
	else if (m_nXgap < 5000)
	{
		m_nXgap = 5000;
	}
	else if (m_nXgap < 10000)
	{
		m_nXgap = 10000;
	}
	else if (m_nXgap < 20000)
	{
		m_nXgap = 20000;
	}
	else if (m_nXgap < 50000)
	{
		m_nXgap = 50000;
	}

	m_dbXPixPerPoint = m_RectFrame.Width() * 1.0 / m_htPoints.Num();

	CDC memDC;
	CBitmap memBitMap;
	memDC.CreateCompatibleDC(&dc);
	memBitMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	memDC.SelectObject(memBitMap);
	memDC.FillSolidRect(rect,m_colBackground);
	CBrush *pOldBrush = (CBrush *)memDC.SelectStockObject(NULL_BRUSH);

	DrawCurve(&memDC);
	if (m_bDrawMinMaxLine)
	{
		DrawMinMaxLine(&memDC);
	}

	if (m_bDrawMeanLine)
	{
		DrawMeanLine(&memDC);
	}

	if (m_bDrawStdLine)
	{
		DrawStdLine(&memDC);
	}

	DrawBorder(&memDC);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	memDC.DeleteDC();
	memBitMap.DeleteObject();
}

void CLineProfile::SetPoints( HTuple htPoint )
{
	m_htPoints = htPoint;

	m_nMinGray = (int)htPoint.Min()[0].D();
	m_nMaxGray = (int)htPoint.Max()[0].D();
	m_dbMeanGray = htPoint.Mean()[0].D();
	m_dbStd = htPoint.Deviation()[0].D();

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetBackgroudColor( COLORREF color )
{
	m_colBackground = color;

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetLineColor( COLORREF color )
{
	m_colLine = color;

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetTextColor( COLORREF color )
{
	m_colText = color;

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetBorderColor( COLORREF color )
{
	m_colBorder = color;

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetMinMaxLine(BOOL bDraw)
{
	m_bDrawMinMaxLine = bDraw;

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetMeanLine(BOOL bDraw)
{
	m_bDrawMeanLine = bDraw;

	if (m_hWnd)
	{
		Invalidate();
	}
}

void CLineProfile::SetStdLine( BOOL bDraw )
{
	m_bDrawStdLine = bDraw;

	if (m_hWnd)
	{
		Invalidate();
	}
}
