#pragma once


// CHistogram

class CHistogram : public CStatic
{
	DECLARE_DYNAMIC(CHistogram)

public:
	CHistogram();
	virtual ~CHistogram();

	void SetBorder(int left,int right);
	void SetBackgroudColor(COLORREF color) {m_colBackground = color;}
	void SetLineColor(COLORREF color) {m_colLine = color;}
	void SetTextColor(COLORREF color) {m_colText = color;}
	void SetBorderColor(COLORREF color) {m_colBorder = color;}
	void SetHisto(HTuple htTuple) {m_htAbsoluteHisto = htTuple;}

protected:
	DECLARE_MESSAGE_MAP()

private:
	int m_nLeft;
	int m_nRight;

	COLORREF m_colBackground;
	COLORREF m_colLine;
	COLORREF m_colText;
	COLORREF m_colBorder;

	HTuple m_htAbsoluteHisto;
	CRect m_RectFrame;
	CPoint m_Point;

	void DrawHisto(CDC *pDC);
	void DrawBorder(CDC *pDC);

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


