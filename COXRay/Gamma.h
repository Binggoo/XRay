#pragma once


// CGamma

class CGamma : public CStatic
{
	DECLARE_DYNAMIC(CGamma)

public:
	CGamma();
	virtual ~CGamma();

	double GetGamma() {return m_dbGamma;}
	void SetGamma(double dbGamma);
	void SetNotifyWnd(HWND hWnd) {m_hNotifyWnd = hWnd;}
protected:
	COLORREF m_colorBackground; // 背景色
	COLORREF m_colorText;		// 显示文本颜色
	COLORREF m_colorBorder;     // 外边框颜色
	COLORREF m_colorInset;     // 内边框颜色
	COLORREF m_colorLine;      // 曲线颜色

	CFont    m_font; //字体大小
	double   m_dbGamma;
	CPoint   *m_LinePoints;
	int      m_nTotalPointCount;
	BOOL     m_bMove;
	CRect    m_rectGamma;
	HWND     m_hNotifyWnd;

	BOOL IsPointInLine(CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


