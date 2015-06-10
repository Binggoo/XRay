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
	COLORREF m_colorBackground; // ����ɫ
	COLORREF m_colorText;		// ��ʾ�ı���ɫ
	COLORREF m_colorBorder;     // ��߿���ɫ
	COLORREF m_colorInset;     // �ڱ߿���ɫ
	COLORREF m_colorLine;      // ������ɫ

	CFont    m_font; //�����С
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


