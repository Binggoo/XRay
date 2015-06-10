
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "RightDialogBar.h"
#include "BottomDialogBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:
	CStatusBar *GetStatusBar() {return &m_wndStatusBar;}
	CRightDialogBar *GetRightDialogBar() {return &m_wndRightDialogBar;}
	CBottomDialogBar *GetBottomDialogBar() {return &m_wndBottomDialogBar;}
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;
	CRightDialogBar   m_wndRightDialogBar;
	CBottomDialogBar  m_wndBottomDialogBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


