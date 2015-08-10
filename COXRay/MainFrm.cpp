
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "COXRay.h"

#include "MainFrm.h"

#include "CodeRuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_PROJECT,
	ID_INDICATOR_USER,
	ID_INDICATOR_ZOOM,
	ID_INDICATOR_TIME,
	ID_INDICATOR_IMG_INFO,
	ID_INDICATOR_PLC_CONNECT,
	ID_INDICATOR_PLC_RUN,
	ID_INDICATOR_PLC_LOCATION,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_pIni = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	// 创建底部的DialogBar
	if (!m_wndBottomDialogBar.Create(this,IDD_DIALOGBAR_BOTTOM,WS_CHILD | WS_VISIBLE |
		CBRS_BOTTOM,IDD_DIALOGBAR_BOTTOM))
	{
		TRACE0("未能创建Bottom DialogBar\n");
		return -1;      // 未能创建
	}

	// 创建右边的DialogBar
	if (!m_wndRightDialogBar.Create(this,IDD_DIALOGBAR_RIGHT,WS_CHILD | WS_VISIBLE |
		CBRS_RIGHT,IDD_DIALOGBAR_RIGHT))
	{
		TRACE0("未能创建Right DialogBar\n");
		return -1;      // 未能创建
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if (pMsg->message == WM_KEYDOWN)
// 	{
// 		if (pMsg->wParam == VK_RETURN)
// 		{
// 			return TRUE;
// 		}
// 	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}
