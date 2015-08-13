
// COXRayView.h : CCOXRayView 类的接口
//

#pragma once
#include "../xis/pane.h"
#include "PLCCommand.h"
#include "LightControl.h"
#include "MyDatabase.h"

#include "COXRayDoc.h"
#include "MainFrm.h"
#include "HistogramDlg.h"

typedef struct _STRUCT_VOID_PARM
{
	LPVOID lpVoid1;
	LPVOID lpVoid2;
}VOID_PARM,*PVOID_PARM;

class CCOXRayView : public CScrollView
{
protected: // 仅从序列化创建
	CCOXRayView();
	DECLARE_DYNCREATE(CCOXRayView)

// 特性
public:
	CCOXRayDoc* GetDocument() const;
	BOOL GetLisence() {return m_bIsLisence;}
	HWindow *GetHWindow() {return m_pHWindow;}
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCOXRayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HWindow *m_pHWindow;
	CStopwatch m_Stopwatch;
	CRect m_RectImage;
	BOOL  m_bGammaPreview;
	int   m_nOptionType;
	BOOL  m_bFilterParmSnap;

	CPoint m_ptStart;
	CRectTracker m_tracker;

	CXISLFunction m_XISLFunction;
	CIni m_Ini;
	UINT m_nCaptureFrames;
	CString m_strAppPath; //程序路径
	BOOL m_bAcqConnected; //平板是否连接
	BOOL m_bIsLisence;    //是否认证注册
	BOOL m_bContinueCapture; //连续采集
	BOOL m_bStopSnap; //静态采集是否结束

	CWnd *m_pDistanceDlg;

	CPLCCommand m_PLCommand;
	BOOL m_bPLCConnected;
	BOOL m_bPLCStarted;
	BOOL m_bPLCRunning;

	DWORD m_dwCurrentLocation;
	UINT  m_nTimeOutTimes;

	CLightControl m_LightControl;
	BOOL m_bLightOn;

	CRightDialogBar *m_pRightDialogBar;
	CStatusBar *m_pStatusBar;
	CBottomDialogBar *m_pBottomDialogBar;
	CWinThread *m_pWatchDogThread;

	// OpenCV 
	cv::VideoWriter m_VideoWriter;

	// database
	CMyDatabase m_MyDatabase;

	UserInfo m_CurUser;

	BOOL m_bDatabaseConnected;
	int m_nCurrentId;

	// project
	CMarkup m_ProjectXml;
	BOOL m_bLoadProject;
	int m_nInpectMode;
	CTime m_Pos1Time; // 记录采集第一个位置的时间

	CHistogramDlg *m_pHistoDlg;

	BOOL m_bWindowOK;

public:
	void InitAcq();

	BOOL IsLisence();

	void SetOptionType(int nType);

	int GetOptionType() {return m_nOptionType;}

	void DrawHistory();
	void DrawGraphics(PDrawInfo pDraw,double dbZoomScale);

	// 线程
	static DWORD WINAPI ConnectPLCThreadProc(LPVOID lpParm);
	void ConnectPLC();

	static DWORD WINAPI LightWatchDogThreadProc(LPVOID lpParm);
	void LightWatchDog();

	// 判断缺陷等级
	int AdjustDefectLevel(CMarkup *pXml,HTuple htRadius,HRegion hRegion,double dbScale,double dbPerPixel,int *plevel);

	HImage AutoProcess(const HImage& Image);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBtnBrowse();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDeviceConnect();
	afx_msg void OnUpdateDeviceConnect(CCmdUI *pCmdUI);
	afx_msg void OnSettingPane();
	afx_msg void OnUpdateSettingPane(CCmdUI *pCmdUI);
	afx_msg void OnBtnStaticCap();
	afx_msg void OnBtnContCap();
protected:
	afx_msg LRESULT OnEndAcqMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndFrameMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGammaChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGammaChangeEnsure(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBtnMove();
	afx_msg void OnBtnSelect();
	afx_msg void OnBtnZoom();
	afx_msg void OnBtnLine();
	afx_msg void OnBtnRect();
	afx_msg void OnBtnEllipse();
	afx_msg void OnBtnDist();
	afx_msg void OnBtnText();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSettingPen();
	afx_msg void OnUpdateSettingPen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingCalibration(CCmdUI *pCmdUI);
	afx_msg void OnSettingCalibration();
	afx_msg void OnBtnDegree();
	afx_msg void OnBtnSave();
	afx_msg void OnUpdateDeviceOffset(CCmdUI *pCmdUI);
	afx_msg void OnDeviceOffset();
	afx_msg void OnUpdateDeviceGainOffset(CCmdUI *pCmdUI);
	afx_msg void OnDeviceGainOffset();
	afx_msg void OnBtnCheck();
	afx_msg void OnBtnView();
	afx_msg void OnUpdateSettingComm(CCmdUI *pCmdUI);
	afx_msg void OnSettingComm();
protected:
	afx_msg LRESULT OnSocketMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnCheckOnOff();
	afx_msg void OnCheckStat();
	afx_msg void OnUpdateDeviceLight(CCmdUI *pCmdUI);
	afx_msg void OnDeviceLight();
	afx_msg void OnUpdateSettingLight(CCmdUI *pCmdUI);
	afx_msg void OnSettingLight();
	afx_msg void OnSettingSave();
	afx_msg void OnProjectNew();
	afx_msg void OnUpdateProjectNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProjectLoad(CCmdUI *pCmdUI);
	afx_msg void OnProjectLoad();
	afx_msg void OnUpdateProjectEdit(CCmdUI *pCmdUI);
	afx_msg void OnProjectEdit();
	afx_msg void OnBtnSnap();
	afx_msg void OnBtnThreshold();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingInspectLevel(CCmdUI *pCmdUI);
	afx_msg void OnSettingInspectLevel();
	afx_msg void OnUpdateFileDatabase(CCmdUI *pCmdUI);
	afx_msg void OnFileDatabase();
	afx_msg void OnUpdateThreshold(CCmdUI *pCmdUI);
	afx_msg void OnThreshold();
	afx_msg void OnUpdateProjectRecord(CCmdUI *pCmdUI);
	afx_msg void OnProjectRecord();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateUserLogin(CCmdUI *pCmdUI);
	afx_msg void OnUserLogin();
	afx_msg void OnUpdateUserSignin(CCmdUI *pCmdUI);
	afx_msg void OnUserSignin();
	afx_msg void OnUpdateUserModifyPassword(CCmdUI *pCmdUI);
	afx_msg void OnUserModifyPassword();
	afx_msg void OnUpdateSettingCodeRule(CCmdUI *pCmdUI);
	afx_msg void OnSettingCodeRule();
	afx_msg void OnUpdateLineProfile(CCmdUI *pCmdUI);
	afx_msg void OnLineProfile();
	afx_msg void OnFileLoadIni();
	afx_msg void OnFileSaveIni();
	afx_msg void OnUpdateAutoProcessAlg(CCmdUI *pCmdUI);
	afx_msg void OnAutoProcessAlg();
};

#ifndef _DEBUG  // COXRayView.cpp 中的调试版本
inline CCOXRayDoc* CCOXRayView::GetDocument() const
   { return reinterpret_cast<CCOXRayDoc*>(m_pDocument); }
#endif

