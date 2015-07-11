#pragma once


// CLineProfile

class CLineProfile : public CStatic
{
	DECLARE_DYNAMIC(CLineProfile)

public:
	CLineProfile();
	virtual ~CLineProfile();

// 	void SetMinMaxGray(int nMin,int nMax);
// 	void SetMeanStd(double dbMean,double dbStd);
	void SetPoints(HTuple htPoint);

	void SetBackgroudColor(COLORREF color);
	void SetLineColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void SetBorderColor(COLORREF color);
	void SetMinMaxLine(BOOL bDraw);
	void SetMeanLine(BOOL bDraw);
	void SetStdLine(BOOL bDraw);

protected:
	DECLARE_MESSAGE_MAP()

	COLORREF m_colBackground;
	COLORREF m_colLine;
	COLORREF m_colText;
	COLORREF m_colBorder;

	HTuple m_htPoints;
	int m_nMinGray;
	int m_nMaxGray;
	int m_nBottom;
	int m_nTop;
	double m_dbMeanGray;
	double m_dbStd;

	CRect m_RectFrame;
	BOOL m_bDrawMinMaxLine;
	BOOL m_bDrawMeanLine;
	BOOL m_bDrawStdLine;
	int  m_nXgap;
	int  m_nYgap;
	double m_dbYPixPerGrayValue;
	double m_dbXPixPerPoint;

	void DrawBorder(CDC *pDC);
	void DrawCurve(CDC *pDC);
	void DrawMinMaxLine(CDC *pDC);
	void DrawMeanLine(CDC *pDC);
	void DrawStdLine(CDC *pDC);
public:
	afx_msg void OnPaint();
};


