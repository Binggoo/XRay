
// COXRayDoc.h : CCOXRayDoc 类的接口
//


#pragma once

#define MAX_UNDO_LEVELS 100

typedef CList<HImage*,HImage*> CListImage;
typedef CList<PDrawInfo,PDrawInfo> CListDrawInfo;
typedef CList<int,int> CListUndoType;

class CCOXRayDoc : public CDocument
{
protected: // 仅从序列化创建
	CCOXRayDoc();
	DECLARE_DYNCREATE(CCOXRayDoc)

// 特性
public:
	static CString GetImageType(CString strExt);

// 操作
public:
	HImage *GetImage() {return m_pHWorkImage;}
	HImage *GetOriginImage() {return m_pOriginImage;}
	void SetImage(HImage Image,BOOL bUpdateWindow = TRUE,BOOL bNew = TRUE);
	int GetChanels() {return m_nChanels;}
	double GetZoomFactor() {return m_dbZoomFactor;}

	void SetConfig(CIni *pIni) {m_pIni = pIni;}

	CListDrawInfo *GetDrawList() {return &m_UndoDraw;}

	void SubmitUndoImage();
	void SubmitUndoDraw(PDrawInfo pDrawInfo);
	void ClearUndoDraw();
	void ClearUndoImage();
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCOXRayDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CListImage m_UndoImage;
	CListDrawInfo m_UndoDraw;
	CListUndoType m_UndoType;
	HImage *m_pHWorkImage;
	HImage *m_pOriginImage;
	double m_dbZoomFactor;
	int m_nChanels;
	CStopwatch m_Stopwatch;
	CIni *m_pIni;

	void FitWindow(BOOL bUpdateWindow = TRUE);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace =TRUE);

	afx_msg void OnBtnZoomIn();
	afx_msg void OnBtnFitWindow();
	afx_msg void OnBtnZoomOut();
	afx_msg void OnBtnZoom11();
	afx_msg void OnBtnRotateLeft();
	afx_msg void OnBtnRotateRight();
	afx_msg void OnBtnRotate();
	afx_msg void OnBtnMirror();
	afx_msg void OnBtnNegative();
	afx_msg void OnBtnFlip();
	afx_msg void OnBtnMean();
	afx_msg void OnBtnMedian();
	afx_msg void OnBtnGauss();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnBtnEnhance();
	afx_msg void OnUpdateFilterMean(CCmdUI *pCmdUI);
	afx_msg void OnFilterMean();
	afx_msg void OnUpdateFilterMedian(CCmdUI *pCmdUI);
	afx_msg void OnFilterMedian();
	afx_msg void OnUpdateFilterGauss(CCmdUI *pCmdUI);
	afx_msg void OnFilterGauss();
	afx_msg void OnUpdateEnhanceContrast(CCmdUI *pCmdUI);
	afx_msg void OnEnhanceContrast();
	afx_msg void OnUpdateEnhanceEdges(CCmdUI *pCmdUI);
	afx_msg void OnEnhanceEdges();
	afx_msg void OnUpdateRotateLeft(CCmdUI *pCmdUI);
	afx_msg void OnRotateLeft();
	afx_msg void OnUpdateRotateRight(CCmdUI *pCmdUI);
	afx_msg void OnRotateRight();
	afx_msg void OnUpdateRotateAny(CCmdUI *pCmdUI);
	afx_msg void OnRotateAny();
	afx_msg void OnUpdateFlipHorz(CCmdUI *pCmdUI);
	afx_msg void OnFlipHorz();
	afx_msg void OnUpdateFlipVert(CCmdUI *pCmdUI);
	afx_msg void OnFlipVert();
	afx_msg void OnUpdateInvert(CCmdUI *pCmdUI);
	afx_msg void OnInvert();
};
