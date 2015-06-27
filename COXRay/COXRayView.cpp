
// COXRayView.cpp : CCOXRayView ���ʵ��
//
// 2015-06-11 Binggoo 1.�޸ķŴ�ʱͼ����ʾ��������λ�ò��䡣
//                    2.����ͼ��궨�������ȱ�ݺ�����ܵ�ȱ�ݴ�С��
//                    3.�޸�GammaУ����
//                    4.�޸�ģ��ͼ���ʽΪBMP
// 2015-06-12 Binggoo 1.��Ӽ���׼��
//                    2.��ӱ༭��Ŀ��
// 2015-06-14 Binggoo 1.������ݿ��ѯ��
// 2015-06-15 Binggoo 1.�������ݿ��ѯ��
// 2015-06-16 Binggoo 1.�����ݿ��ѯ������ɾ������ղ�ѯ���ݿ���Ҽ��˵���
//                    2.��ȱ�ݵȼ���������������С�
//                    3.���û�д�ͼ��ʱGammaУ����������⡣
//                    4.����ɼ�ͼ����Զ����GammaУ����ˢ��ͼ������⡣
// 2015-06-19 Binggoo 1.������������ȱ�ݻ��ǰ�ȱ�ݣ������׺ͼ�����
//                    2.�޸Ķ�λģ��ƥ�䣬֮ǰ�Ƚ�ROI�ϳ���(�������⣬��ת֮��ȽϽ�����ԣ��������
// 2015-06-24 Binggoo 1.16λͼ�����ģ��ƥ��ʱ�����ܴ󣬷���ÿ�βɼ���ͼ��Ҷȶ���һ����ģ��ƥ��ʱת����8λͼ����д���
// 2015-06-25 Binggoo 1.����ͼ����˵���
//                    2.��¼ƫ�����á�
// 2015-06-26 Binggoo 1.�����Զ���ǿ����
//                    2.��������Ŀ�ɼ���ͼ��Ҳ�������ݿ⡣
// 2015-06-27 Binggoo 1.����Ctrl + Enter�ɼ�ͼ���ݼ���Ctrl + A �����ݼ���
//                    2.�����ԭʼͼƬ��Ҳ��¼PN�š�


#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "COXRay.h"
#endif

//include "COXRayDoc.h"
#include "COXRayView.h"
//#include "MainFrm.h"
#include "../Lisence/Lisence.h"

#include "PaneSettingDlg.h"
#include "PenSettingDlg.h"
#include "CalibrationSettingDlg.h"
#include "PlaceTextDlg.h"
#include "StepSetPath.h"
#include "StepTips.h"
#include "CommSettingDlg.h"
#include "LightSettingDlg.h"
#include "SaveSettingDlg.h"
#include "ProjectNewDlg.h"
#include "ProjectEditDlg.h"
#include "InspectLevelDlg.h"
#include "DatabaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_WIDTH  800
#define MAX_HEIGHT 800

#define TIME_OUT_S   60

#define TIMER_LISENCE     1
#define TIMER_PLC         2
#define TIMER_PLC_LISTEN  3

// CCOXRayView

IMPLEMENT_DYNCREATE(CCOXRayView, CScrollView)

BEGIN_MESSAGE_MAP(CCOXRayView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDC_BTN_BROWSE, &CCOXRayView::OnBtnBrowse)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_DEVICE_CONNECT, &CCOXRayView::OnDeviceConnect)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_CONNECT, &CCOXRayView::OnUpdateDeviceConnect)
	ON_COMMAND(ID_SETTING_PANE, &CCOXRayView::OnSettingPane)
	ON_UPDATE_COMMAND_UI(ID_SETTING_PANE, &CCOXRayView::OnUpdateSettingPane)
	ON_COMMAND(IDC_BTN_STATIC_CAP, &CCOXRayView::OnBtnStaticCap)
	ON_COMMAND(IDC_BTN_CONT_CAP, &CCOXRayView::OnBtnContCap)

	ON_MESSAGE(WM_ACQ_SNAP_END_ACQ, &CCOXRayView::OnEndAcqMessage)
	ON_MESSAGE(WM_ACQ_CONT_END_ACQ, &CCOXRayView::OnEndAcqMessage)
	ON_MESSAGE(WM_ACQ_GAIN_END_ACQ, &CCOXRayView::OnEndAcqMessage)
	ON_MESSAGE(WM_ACQ_OFFSET_END_ACQ, &CCOXRayView::OnEndAcqMessage)

	ON_MESSAGE(WM_ACQ_CONT_END_FRAME, &CCOXRayView::OnEndFrameMessage)
	ON_MESSAGE(WM_ACQ_GAIN_END_FRAME, &CCOXRayView::OnEndFrameMessage)
	ON_MESSAGE(WM_ACQ_OFFSET_END_FRAME, &CCOXRayView::OnEndFrameMessage)

	ON_MESSAGE(WM_GAMMA_CHANGE_PREVIEW, &CCOXRayView::OnGammaChange)
	ON_MESSAGE(WM_GAMMA_CHANGE_ENSURE, &CCOXRayView::OnGammaChangeEnsure)

	ON_COMMAND(IDC_BTN_MOVE, &CCOXRayView::OnBtnMove)
	ON_COMMAND(IDC_BTN_SELECT, &CCOXRayView::OnBtnSelect)
	ON_COMMAND(IDC_BTN_ZOOM, &CCOXRayView::OnBtnZoom)
	ON_COMMAND(IDC_BTN_LINE, &CCOXRayView::OnBtnLine)
	ON_COMMAND(IDC_BTN_RECT, &CCOXRayView::OnBtnRect)
	ON_COMMAND(IDC_BTN_ELLIPSE, &CCOXRayView::OnBtnEllipse)
	ON_COMMAND(IDC_BTN_DIST, &CCOXRayView::OnBtnDist)
	ON_COMMAND(IDC_BTN_TEXT, &CCOXRayView::OnBtnText)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SETTING_PEN, &CCOXRayView::OnSettingPen)
	ON_UPDATE_COMMAND_UI(ID_SETTING_PEN, &CCOXRayView::OnUpdateSettingPen)
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_SETTING_CALIBRATION, &CCOXRayView::OnUpdateSettingCalibration)
	ON_COMMAND(ID_SETTING_CALIBRATION, &CCOXRayView::OnSettingCalibration)
	ON_COMMAND(IDC_BTN_DEGREE, &CCOXRayView::OnBtnDegree)
	ON_COMMAND(IDC_BTN_SAVE, &CCOXRayView::OnBtnSave)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_OFFSET, &CCOXRayView::OnUpdateDeviceOffset)
	ON_COMMAND(ID_DEVICE_OFFSET, &CCOXRayView::OnDeviceOffset)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_GAIN_OFFSET, &CCOXRayView::OnUpdateDeviceGainOffset)
	ON_COMMAND(ID_DEVICE_GAIN_OFFSET, &CCOXRayView::OnDeviceGainOffset)
	ON_COMMAND(IDC_BTN_CHECK, &CCOXRayView::OnBtnCheck)
	ON_COMMAND(IDC_BTN_VIEW, &CCOXRayView::OnBtnView)
	ON_UPDATE_COMMAND_UI(ID_SETTING_COMM, &CCOXRayView::OnUpdateSettingComm)
	ON_COMMAND(ID_SETTING_COMM, &CCOXRayView::OnSettingComm)
	ON_MESSAGE(WM_SOCKET_MSG, &CCOXRayView::OnSocketMsg)
	ON_COMMAND(IDC_BTN_START, &CCOXRayView::OnBtnStart)
	ON_COMMAND(IDC_BTN_STOP, &CCOXRayView::OnBtnStop)
	ON_COMMAND(IDC_CHECK_ON_OFF, &CCOXRayView::OnCheckOnOff)
	ON_COMMAND(IDC_CHECK_STAT, &CCOXRayView::OnCheckStat)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_LIGHT, &CCOXRayView::OnUpdateDeviceLight)
	ON_COMMAND(ID_DEVICE_LIGHT, &CCOXRayView::OnDeviceLight)
	ON_UPDATE_COMMAND_UI(ID_SETTING_LIGHT, &CCOXRayView::OnUpdateSettingLight)
	ON_COMMAND(ID_SETTING_LIGHT, &CCOXRayView::OnSettingLight)
	ON_COMMAND(ID_SETTING_SAVE, &CCOXRayView::OnSettingSave)
	ON_COMMAND(ID_PROJECT_NEW, &CCOXRayView::OnProjectNew)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW, &CCOXRayView::OnUpdateProjectNew)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_LOAD, &CCOXRayView::OnUpdateProjectLoad)
	ON_COMMAND(ID_PROJECT_LOAD, &CCOXRayView::OnProjectLoad)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_EDIT, &CCOXRayView::OnUpdateProjectEdit)
	ON_COMMAND(ID_PROJECT_EDIT, &CCOXRayView::OnProjectEdit)
	ON_COMMAND(IDC_BTN_SNAP, &CCOXRayView::OnBtnSnap)
	ON_COMMAND(IDC_BTN_THRESHOLD, &CCOXRayView::OnBtnThreshold)
// 	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CCOXRayView::OnUpdateFileSave)
// 	ON_COMMAND(ID_FILE_SAVE, &CCOXRayView::OnFileSave)
// 	ON_COMMAND(ID_FILE_SAVE_AS, &CCOXRayView::OnFileSaveAs)
// 	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CCOXRayView::OnUpdateFileSaveAs)
ON_UPDATE_COMMAND_UI(ID_SETTING_INSPECT_LEVEL, &CCOXRayView::OnUpdateSettingInspectLevel)
ON_COMMAND(ID_SETTING_INSPECT_LEVEL, &CCOXRayView::OnSettingInspectLevel)
ON_UPDATE_COMMAND_UI(ID_FILE_DATABASE, &CCOXRayView::OnUpdateFileDatabase)
ON_COMMAND(ID_FILE_DATABASE, &CCOXRayView::OnFileDatabase)
ON_UPDATE_COMMAND_UI(ID_THRESHOLD, &CCOXRayView::OnUpdateThreshold)
ON_COMMAND(ID_THRESHOLD, &CCOXRayView::OnThreshold)
ON_UPDATE_COMMAND_UI(ID_PROJECT_RECORD, &CCOXRayView::OnUpdateProjectRecord)
ON_COMMAND(ID_PROJECT_RECORD, &CCOXRayView::OnProjectRecord)
END_MESSAGE_MAP()

// CCOXRayView ����/����

CCOXRayView::CCOXRayView()
{
	// TODO: �ڴ˴���ӹ������
	m_pHWindow = NULL;
	m_nCaptureFrames = 8;
	m_bAcqConnected = FALSE;
	m_bIsLisence = FALSE;
	m_bContinueCapture = FALSE;
	m_bStopSnap = TRUE;
	m_bGammaPreview = FALSE;

	m_nOptionType = OP_SELECT;

	m_tracker.m_rect = CRect(0,0,0,0);
	m_tracker.m_nStyle = 0;
	m_tracker.m_nStyle |= CRectTracker::dottedLine;
	m_tracker.m_nStyle |= CRectTracker::resizeOutside;

	m_bPLCConnected = FALSE;
	m_bPLCStarted = FALSE;
	m_bPLCRunning = FALSE;
	m_dwCurrentLocation = 1;

	m_nTimeOutTimes = 0;

	m_bLightOn = FALSE;

	m_pRightDialogBar = NULL;
	m_pStatusBar = NULL;
	m_pBottomDialogBar = NULL;

	m_pWatchDogThread = NULL;

	m_bDatabaseConnected = FALSE;
	m_nCurrentId = 0;

	m_bLoadProject = FALSE;
	m_nInpectMode = MANUL_MODE;

	m_bFilterParmSnap = FALSE;

	m_pHistoDlg = NULL;

	m_bWindowOK = FALSE;
}

CCOXRayView::~CCOXRayView()
{
	if (m_pHWindow != NULL)
	{
		m_pHWindow->CloseWindow();
		delete m_pHWindow;
		m_pHWindow = NULL;
	}

	if (m_pHistoDlg)
	{
		m_pHistoDlg->DestroyWindow();
		delete m_pHistoDlg;
	}

	if (m_LightControl.IsLightOn())
	{
		m_LightControl.CloseLight();
	}
	m_LightControl.Close();

	if (m_pWatchDogThread && m_pWatchDogThread->m_hThread)
	{
		WaitForSingleObject(m_pWatchDogThread->m_hThread,INFINITE);
		delete m_pWatchDogThread;
	}
}

BOOL CCOXRayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CCOXRayView ����

void CCOXRayView::OnDraw(CDC* pDC)
{
	CCOXRayDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CRect rect;
	GetClientRect(&rect);
	pDC->DPtoLP(&rect);
	pDC->FillSolidRect(rect,RGB(32,32,32));
}

void CCOXRayView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CString strSavePath,strCaptureFrames;
	m_nCaptureFrames = m_Ini.GetUInt(_T("PaneSetting"),_T("CaptureFrames"),16);
	strCaptureFrames.Format(_T("%d"),m_nCaptureFrames);
	strSavePath = m_Ini.GetString(_T("SaveSetting"),_T("SavePath"),m_strAppPath + DEFAULT_SAVE_PATH);

	if (m_pRightDialogBar == NULL)
	{
		m_pRightDialogBar = ((CMainFrame *)AfxGetMainWnd())->GetRightDialogBar();

		if (m_pRightDialogBar)
		{
			m_pRightDialogBar->SetConfig(&m_ProjectXml);
		}
	}

	if (m_pStatusBar == NULL)
	{
		m_pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();
	}

	if (m_pBottomDialogBar == NULL)
	{
		m_pBottomDialogBar = ((CMainFrame *)AfxGetMainWnd())->GetBottomDialogBar();
	}

	m_pRightDialogBar->m_PageImgCapture.m_ComboBoxFrames.SetWindowText(strCaptureFrames);
	m_pRightDialogBar->m_PageImgCapture.m_EditSavePath.SetWindowText(strSavePath);
	m_pRightDialogBar->m_PageImgProcess.m_Gamma.SetNotifyWnd(m_hWnd);

	double dbGamma = m_Ini.GetDouble(_T("ImageProcess"),_T("Gamma"),1.0);
	double angle = atan(dbGamma);

	BOOL bAutoProcess = m_Ini.GetBool(_T("ImageProcess"),_T("En_Auto"),FALSE);

	m_pRightDialogBar->m_PageImgCapture.m_CheckAutoProcess.SetCheck(bAutoProcess);
	m_pRightDialogBar->m_PageImgProcess.m_SliderGamma.SetPos((int)DEGREE(angle));
	m_pRightDialogBar->m_PageImgProcess.m_Gamma.SetGamma(dbGamma);

//	m_pBottomDialogBar->DrawThumbnail(strSavePath,_T("*.*"));

	SetOptionType(OP_SELECT);

	CCOXRayDoc* pDoc = GetDocument();
	if (pDoc)
	{
		pDoc->SetConfig(&m_Ini);
	}
}


// CCOXRayView ��ӡ

BOOL CCOXRayView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCOXRayView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCOXRayView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CCOXRayView ���

#ifdef _DEBUG
void CCOXRayView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCOXRayView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCOXRayDoc* CCOXRayView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCOXRayDoc)));
	return (CCOXRayDoc*)m_pDocument;
}
#endif //_DEBUG


// CCOXRayView ��Ϣ�������


BOOL CCOXRayView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CScrollView::OnEraseBkgnd(pDC);
}


void CCOXRayView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: �ڴ����ר�ô����/����û���
	CClientDC dc(this);
	switch (lHint)
	{
	case WM_USER_NEWIMAGE:
		{
			CRect rect;
			GetClientRect(&rect);

			CCOXRayDoc *pDoc = GetDocument();
			HImage *pImage = pDoc->GetImage();

			if (pImage)
			{
				HImage hImageTemp = pImage->CopyImage();

				double dbGamma = m_Ini.GetDouble(_T("ImageProcess"),_T("Gamma"),1.0);
				if (dbGamma != 1.0 && m_bGammaPreview)
				{
					hImageTemp = GammaImage(hImageTemp,dbGamma);
				}

				int cx = 0,cy = 0,nWidth = 0,nHeight = 0;
				double dbZoomFactor = pDoc->GetZoomFactor();
				hImageTemp = hImageTemp.ZoomImageFactor(dbZoomFactor,dbZoomFactor,"constant");


				nWidth = hImageTemp.Width();
				nHeight = hImageTemp.Height();

				CSize sizeTotal(nWidth,nHeight);
				SetScrollSizes(MM_TEXT, sizeTotal);

// 				int nHorzMin,nHorzMax,nVertMin,nVertMax;
// 				GetScrollRange(SB_HORZ,&nHorzMin,&nHorzMax);
// 				GetScrollRange(SB_VERT,&nVertMin,&nVertMax);
// 				SetScrollPos(SB_HORZ,(nHorzMax - nHorzMin)/2);
// 				SetScrollPos(SB_VERT,(nVertMax - nVertMin)/2);

				CPoint pt(0,0);
				OnPrepareDC(&dc);
				dc.LPtoDP(&pt);

				cx = pt.x;
				cy = pt.y;

				m_RectImage.left = 0;
				m_RectImage.top = 0;
				m_RectImage.right = nWidth;
				m_RectImage.bottom = nHeight;

				// ������ʾ
				if (rect.Width() > nWidth)
				{
					cx = (rect.Width() -nWidth) / 2;
					m_RectImage.left = cx;
					m_RectImage.right += cx;
				}

				
				if (rect.Height() > nHeight)
				{
					cy = (rect.Height() - nHeight) / 2;
					m_RectImage.top = cy;
					m_RectImage.bottom += cy;
				}
				
				

				// ������Ļ��ͣˢ��
				if ((!m_bGammaPreview && !m_bContinueCapture) || !m_bWindowOK)
				{
					m_pHWindow->ClearWindow();

					// ע���͸��Ƿ���
					m_pHWindow->SetWindowExtents(cy,cx,nWidth,nHeight);
					m_pHWindow->SetPart(0,0,nHeight,nWidth);

					m_bWindowOK = TRUE;
				}

				m_pHWindow->Display(hImageTemp);
				
				if ( m_tracker.m_rect.Width() > 0 && m_tracker.m_rect.Height() > 0)
				{
					m_tracker.Draw(&dc);
				}

				// draw
				DrawHistory();
			}
			else
			{
				m_pHWindow->ClearWindow();
				m_pHWindow->SetWindowExtents(0,0,1,1);
			}
		
		}
		break;
	}
	//CScrollView::OnUpdate(pSender, lHint, pHint);
}

void CCOXRayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	CPoint tempPoint = point;

	// �豸����ת��Ϊ�߼�����
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&tempPoint);

	if (m_RectImage.PtInRect(tempPoint))
	{
		m_ptStart = tempPoint;

		switch (m_nOptionType)
		{
		case OP_SELECT:
			if (m_tracker.HitTest(point) < 0)
			{
				CRectTracker track;
				if (track.TrackRubberBand(this, point, true)) 
				{
					track.m_rect.NormalizeRect();
					m_tracker.m_rect = track.m_rect;
				} 
				
				m_tracker.m_rect = CRect(0,0,0,0);
				
			} 
			break;

		case OP_ZOOM:
			PostMessage(WM_COMMAND,IDC_BTN_ZOOM_IN);
			break;
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CCOXRayView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	CPoint tempPoint = point;
	// �豸����ת��Ϊ�߼�����
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&tempPoint);

	if (m_RectImage.PtInRect(tempPoint))
	{
		switch (m_nOptionType)
		{
		case OP_ZOOM:
			PostMessage(WM_COMMAND,IDC_BTN_ZOOM_OUT);
			break;
		}
	}

	CScrollView::OnRButtonDown(nFlags, point);
}

void CCOXRayView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	CPoint tempPoint = point;
	// �豸����ת��Ϊ�߼�����
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&tempPoint);

	CRect rectClient;
	GetClientRect(&rectClient);

	if (m_RectImage.PtInRect(tempPoint))
	{
		long cx = tempPoint.x - m_RectImage.left;
		long cy = tempPoint.y - m_RectImage.top;

		cx /= pDoc->GetZoomFactor();
		cy /= pDoc->GetZoomFactor();

		CString strPixInfo;
		int nChanels = pImage->CountChannels();
		switch (nChanels)
		{
		case 1:
			{
				HPixVal pix = pImage->GetPixVal(cx,cy);

				strPixInfo.Format(_T("(X=%d,Y=%d) = (%d)"),cx,cy,(int)pix);
			}
			break;

		case 3:
			{
				HTuple htPtrRed,htPtrGreen,htPtrBlue,htType,htWidth,htHeight;

				htPtrRed = pImage->GetImagePointer3(&htPtrGreen,&htPtrBlue,&htType,&htWidth,&htHeight);

				long red = *((unsigned char *)htPtrRed[0].L() + cy * htWidth[0].L() + cx);
				long green = *((unsigned char *)htPtrGreen[0].L() + cy * htWidth[0].L() + cx);
				long blue = *((unsigned char *)htPtrBlue[0].L() + cy * htWidth[0].L() + cx);

// 				HImage hiRed,hiGreen,hiBlue;
// 				hiRed = pImage->Decompose3(&hiGreen,&hiBlue);
// 
// 				HPixVal pixRed = hiRed.GetPixVal(cx,cy);
// 				HPixVal pixGreen = hiGreen.GetPixVal(cx,cy);
// 				HPixVal pixBlue = hiBlue.GetPixVal(cx,cy);

				strPixInfo.Format(_T("(X=%d,Y=%d) = RGB(%d,%d,%d)")
					,cx,cy,red,green,blue);
			}
			break;
		}
		

		m_pStatusBar->SetPaneText(0,strPixInfo);

		if (nFlags & MK_LBUTTON)
		{
			switch (m_nOptionType)
			{
			case OP_MOVE:
				{
					SetCursor(LoadCursor(0,IDC_SIZEALL));

					CSize sz(m_ptStart - tempPoint);

					if (m_RectImage.Width() > rectClient.Width() 
						|| m_RectImage.Height() > rectClient.Height())
					{
						OnScrollBy(sz);
					}
				}
				break;	
			}
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CCOXRayView::InitAcq()
{
	if (!m_bAcqConnected)
	{
		UINT nNumOfSensor = m_XISLFunction.GetSensorsCount();

		if (nNumOfSensor < 1)
		{
			m_pStatusBar->SetPaneText(0,_T("û�м�⵽����ƽ��!"));
			AfxMessageBox(_T("û�м�⵽����ƽ��!"),MB_ICONSTOP | MB_SETFOREGROUND);
			return;
		}

		UINT nRet = m_XISLFunction.InitDetector(0);

		if (nRet != HIS_ALL_OK)
		{
			m_pStatusBar->SetPaneText(0,_T("��ʼ��ƽ��ʧ��!"));
			AfxMessageBox(_T("��ʼ��ƽ��ʧ��!"),MB_ICONSTOP | MB_SETFOREGROUND);
			return;
		}

		m_bAcqConnected = TRUE;
	}

	double dbIntTimes[8];
	int nIntTimes = 8;

	if (m_XISLFunction.GetFrameTimes(dbIntTimes,&nIntTimes) != HIS_ALL_OK)
	{
		m_pStatusBar->SetPaneText(0,_T("��ȡƽ��ģʽʧ��!"));
		AfxMessageBox(_T("��ȡƽ��ģʽʧ��!"),MB_ICONSTOP | MB_SETFOREGROUND);

		return;
	}

	DWORD dwPaneMode = m_Ini.GetInt(_T("PaneSetting"),_T("PaneMode"),0);
	DWORD dwTimes = m_Ini.GetUInt(_T("PaneSetting"),_T("InteralTimer"),0);

	if (dwTimes == 0)
	{
		dwTimes = (DWORD)(dbIntTimes[dwPaneMode] / 1000);
	}

	m_XISLFunction.SetCameraSeting(dwPaneMode,dwTimes);

	BOOL bCheckOffsetCorr = m_Ini.GetBool(_T("PaneSetting"),_T("En_OffsetCorr"),FALSE);
	BOOL bCheckGainCorr = m_Ini.GetBool(_T("PaneSetting"),_T("En_GainCorr"),FALSE);
	BOOL bCheckPixelCorr = m_Ini.GetBool(_T("PaneSetting"),_T("En_PixelCorr"),FALSE);

	CString strOffsetPath = m_Ini.GetString(_T("PaneSetting"),_T("OffsetPath"));
	CString strGainPath = m_Ini.GetString(_T("PaneSetting"),_T("GainPath"));
	CString strPixelPath = m_Ini.GetString(_T("PaneSetting"),_T("PixelPath"));

	if (bCheckOffsetCorr || bCheckGainCorr)
	{
		HANDLE hFile = CreateFile(strOffsetPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			m_pStatusBar->SetPaneText(0,_T("�򿪻�׼У���ļ�ʧ��!"));
			AfxMessageBox(_T("�򿪻�׼У���ļ�ʧ��!"));

			return;
		}

		WinHeaderType FileHeader;
		DWORD dwSize = 0;
		ReadFile(hFile,&FileHeader,sizeof(WinHeaderType),&dwSize,NULL);

		DWORD dwOffset = FileHeader.ImageHeaderSize + FileHeader.HeaderSize;

		SetFilePointer(hFile,dwOffset,NULL,FILE_BEGIN);

		DWORD dwLen = FileHeader.FileSize - dwOffset;
		PVOID pBuffer = ALLOC(dwLen);
		ReadFile(hFile,pBuffer,dwLen,&dwSize,NULL);

		CloseHandle(hFile);

		m_XISLFunction.SetOffsetBuffer(pBuffer,dwLen);

		FREE(pBuffer);

	}
	else
	{
		m_XISLFunction.SetOffsetBuffer(NULL,0);
	}

	if (bCheckGainCorr)
	{
		HANDLE hFile = CreateFile(strGainPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			m_pStatusBar->SetPaneText(0,_T("������У���ļ�ʧ��!"));
			AfxMessageBox(_T("������У���ļ�ʧ��!"));
			return;
		}

		WinHeaderType FileHeader;
		DWORD dwSize = 0;
		ReadFile(hFile,&FileHeader,sizeof(WinHeaderType),&dwSize,NULL);

		DWORD dwOffset = FileHeader.ImageHeaderSize + FileHeader.HeaderSize;

		SetFilePointer(hFile,dwOffset,NULL,FILE_BEGIN);

		DWORD dwLen = FileHeader.FileSize - dwOffset;
		PVOID pBuffer = ALLOC(dwLen);
		ReadFile(hFile,pBuffer,dwLen,&dwSize,NULL);

		CloseHandle(hFile);

		m_XISLFunction.SetGainBuffer(pBuffer,dwLen);

		FREE(pBuffer);
	}
	else
	{
		m_XISLFunction.SetGainBuffer(NULL,0);
	}

	if (bCheckPixelCorr)
	{
		HANDLE hFile = CreateFile(strPixelPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			m_pStatusBar->SetPaneText(0,_T("�򿪻�����У���ļ�ʧ��!"));
			AfxMessageBox(_T("�򿪻�����У���ļ�ʧ��!"));

			return;
		}

		WinHeaderType FileHeader;
		DWORD dwSize = 0;
		ReadFile(hFile,&FileHeader,sizeof(WinHeaderType),&dwSize,NULL);

		DWORD dwOffset = FileHeader.ImageHeaderSize + FileHeader.HeaderSize;

		SetFilePointer(hFile,dwOffset,NULL,FILE_BEGIN);

		DWORD dwLen = FileHeader.FileSize - dwOffset;
		PVOID pBuffer = ALLOC(dwLen);
		ReadFile(hFile,pBuffer,dwLen,&dwSize,NULL);

		CloseHandle(hFile);

		m_XISLFunction.SetPixelBuffer(pBuffer,dwLen);

		FREE(pBuffer);
	}
	else
	{
		m_XISLFunction.SetPixelBuffer(NULL,0);
	}
}


void CCOXRayView::OnBtnBrowse()
{
	// TODO: �ڴ���������������
	BROWSEINFO broInfo = {0};
	TCHAR       szDisName[MAX_PATH] = {0};

	broInfo.hwndOwner = this->m_hWnd;
	broInfo.pidlRoot  = NULL;
	broInfo.pszDisplayName = szDisName;
	broInfo.ulFlags   = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN
		| BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	broInfo.lpfn      = NULL;
	broInfo.lParam    = NULL;
	broInfo.iImage    = IDR_MAINFRAME;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&broInfo);
	if (pIDList != NULL)
	{
		memset(szDisName, 0, sizeof(szDisName));
		SHGetPathFromIDList(pIDList, szDisName);

		m_pRightDialogBar->m_PageImgCapture.m_EditSavePath.SetWindowText(szDisName);

		m_Ini.WriteString(_T("SaveSetting"),_T("SavePath"),szDisName);
	}
}


int CCOXRayView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_XISLFunction.SetNotify(m_hWnd);

	m_PLCommand.SetNotify(m_hWnd);

	if (m_pHWindow == NULL)
	{
		set_window_attr("border_width",0); //���ô�������
		m_pHWindow = new HWindow(0,0,1,1,(int)m_hWnd,"visible");
	}
	SetScrollSizes(MM_TEXT, CSize(0, 0));

	m_strAppPath = CUtils::GetFilePathWithoutName(CUtils::GetAppPath());

	m_Ini.SetPathName(m_strAppPath + CONFIG_NAME);

	m_bDatabaseConnected = m_MyDatabase.Connect("localhost","root","root","db_cox",3306);
	if (m_bDatabaseConnected)
	{
		m_MyDatabase.CreateTable();
	}
	else
	{
		AfxMessageBox(_T("�������ݿ�ʧ�ܣ�"));
	}

	SetTimer(TIMER_LISENCE,100,NULL);

	return 0;
}


void CCOXRayView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pRightDialogBar == NULL)
	{
		m_pRightDialogBar = ((CMainFrame *)AfxGetMainWnd())->GetRightDialogBar();

		if (m_pRightDialogBar)
		{
			m_pRightDialogBar->SetConfig(&m_ProjectXml);
		}
	}

	if (m_pStatusBar == NULL)
	{
		m_pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();
	}

	if (m_pBottomDialogBar == NULL)
	{
		m_pBottomDialogBar = ((CMainFrame *)AfxGetMainWnd())->GetBottomDialogBar();
	}

	switch (nIDEvent)
	{
	case TIMER_LISENCE:
		{
			KillTimer(nIDEvent);

			BeginWaitCursor();
			m_bIsLisence = IsLisence();

			if (m_bIsLisence)
			{
				//AfxBeginThread((AFX_THREADPROC)ConnectPLCThreadProc,this);
				// ������Դ
				if (!m_LightControl.EnumLight())
				{
					AfxMessageBox(_T("û���ҵ����ʵĹ�Դ��"));
				}
				else
				{
					if (m_LightControl.IsLightOn())
					{
						m_LightControl.CloseLight();
					}

					m_LightControl.SelCommand();

					m_LightControl.SetVoltage(0.0);
					m_LightControl.SetCurrent(0.0);
					
					WORD wFault = 0;
					double dbVol = 0.0,dbCur = 0.0,dbTemp = 0.0;
					int nFilement = 0;
					m_LightControl.ReportFault(&wFault);

					m_LightControl.Monitor(&dbVol,&dbCur,&dbTemp,&nFilement);

					m_pRightDialogBar->m_LedStat.SetLedState(1);
					
					m_pWatchDogThread = AfxBeginThread((AFX_THREADPROC)LightWatchDogThreadProc,this,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED);

					if (m_pWatchDogThread == NULL)
					{
						AfxMessageBox(_T("������Դ�����߳�ʧ�ܣ�"));
					}
					else
					{
						m_pWatchDogThread->m_bAutoDelete = FALSE;
						m_pWatchDogThread->ResumeThread();
					}
					

				}

				InitAcq();

				ConnectPLC();
			}
			else
			{
				CCOXRayDoc *pDoc = GetDocument();
				
				if (pDoc)
				{
					pDoc->SetTitle(_T("δע����֤������ϵ��Ӧ��!"));
				}

				m_pStatusBar->SetPaneText(0,_T("���û��ע����֤������ϵ��Ӧ��!"));
				AfxMessageBox(_T("���û��ע����֤������ϵ��Ӧ��!"),MB_ICONSTOP | MB_SETFOREGROUND);
			}
			EndWaitCursor();
		}
		break;

	case TIMER_PLC:
		{
			KillTimer(TIMER_PLC);

			// ��PLC״̬
			DWORD dwD2000 = 0;
			int nLen = 1;
			if (!m_PLCommand.ReadPLCD(D2000,nLen,&dwD2000))
			{
				CString strMsg;
				if (m_PLCommand.m_ErrorType == Error_System)
				{
					strMsg.Format(_T("��PLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
				}
				else
				{
					strMsg.Format(_T("��PLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
				}

				m_pStatusBar->SetPaneText(0,strMsg);

				AfxMessageBox(strMsg);
			}

			if ((dwD2000 & Mask_PLC_Run) == Mask_PLC_Run)
			{
				m_bPLCRunning = TRUE;
				int nIndex = m_pStatusBar->CommandToIndex(ID_INDICATOR_PLC_RUN);
				m_pStatusBar->SetPaneText(nIndex,_T("Running"));
			}
			else
			{
				m_bPLCRunning = FALSE;

				int nIndex = m_pStatusBar->CommandToIndex(ID_INDICATOR_PLC_RUN);
				m_pStatusBar->SetPaneText(nIndex,_T("Stop"));
			}

			if ((dwD2000 & Mask_PLC_Alarm) == Mask_PLC_Alarm)
			{
				// PLC����
				m_LightControl.CloseLight();
				m_LightControl.Close();

				PostMessage(WM_COMMAND,IDC_BTN_STOP,0);

				m_pStatusBar->SetPaneText(0,_T("PLC���ϣ�����"));
				AfxMessageBox(_T("PLC���ϣ�����"));

				break;
			}
			else if ((dwD2000 & (Mask_PLC_Run | Mask_PLC_Ready)) == (Mask_PLC_Run | Mask_PLC_Ready))
			{
				// PLC��λ��ʼ�ɼ�
				if (m_bStopSnap)
				{
					// ��ȡ��ǰλ��
					DWORD dwD2001 = 0;
					nLen = 1;
					if (!m_PLCommand.ReadPLCD(D2001,nLen,&dwD2001))
					{
						CString strMsg;
						if (m_PLCommand.m_ErrorType == Error_System)
						{
							strMsg.Format(_T("��PLCλ��ʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
						}
						else
						{
							strMsg.Format(_T("��PLCλ�÷��ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
						}

						m_pStatusBar->SetPaneText(0,strMsg);

						AfxMessageBox(strMsg);
					}
					else
					{
						m_dwCurrentLocation = dwD2001;

						int nIndex = m_pStatusBar->CommandToIndex(ID_INDICATOR_PLC_LOCATION);

						CString strLocation;
						strLocation.Format(_T("λ��%d"),m_dwCurrentLocation);

						m_pStatusBar->SetPaneText(nIndex,strLocation);
					}

					// �������λ��־
					dwD2000 &= ~Mask_PLC_Ready;

					if (!m_PLCommand.WritePLCD(D2000,1,&dwD2000))
					{
						CString strMsg;
						if (m_PLCommand.m_ErrorType == Error_System)
						{
							strMsg.Format(_T("дPLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
						}
						else
						{
							strMsg.Format(_T("дPLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
						}

						m_pStatusBar->SetPaneText(0,strMsg);

						AfxMessageBox(strMsg);
					}
					else
					{
						OnBtnStaticCap();
					}
				}
				
			}

			SetTimer(TIMER_PLC,500,NULL);
		}
		break;

	case TIMER_PLC_LISTEN:
		{
			KillTimer(TIMER_PLC_LISTEN);
			// ���ó����������б�־���������PLC�Ƿ�����
			DWORD dwD2100 = 0;
			int nLen = 1;
			if (!m_PLCommand.ReadPLCD(D2100,nLen,&dwD2100))
			{
				CString strMsg;
				if (m_PLCommand.m_ErrorType == Error_System)
				{
					strMsg.Format(_T("��PLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
				}
				else
				{
					strMsg.Format(_T("��PLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
				}

				m_pStatusBar->SetPaneText(0,strMsg);

				AfxMessageBox(strMsg);
			}
			else
			{
				if ((dwD2100 & CMD_PC_RUNNING) == 0)
				{
					// ����5sû�б仯˵��PLC����
					m_nTimeOutTimes++;

					if (m_nTimeOutTimes > TIME_OUT_S)
					{
						m_LightControl.CloseLight();

						m_LightControl.Close();

						PostMessage(WM_COMMAND,IDC_BTN_STOP,0);

						m_pStatusBar->SetPaneText(0,_T("PLCʧ����������ϣ�����"));
						AfxMessageBox(_T("PLCʧ����������ϣ�����"));
						break;
					}
				}
				else
				{
					m_nTimeOutTimes = 0;

					dwD2100 &= ~CMD_PC_RUNNING;

					if (!m_PLCommand.WritePLCD(D2100,1,&dwD2100))
					{
						CString strMsg;
						if (m_PLCommand.m_ErrorType == Error_System)
						{
							strMsg.Format(_T("дPLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
						}
						else
						{
							strMsg.Format(_T("дPLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
						}

						m_pStatusBar->SetPaneText(0,strMsg);

						AfxMessageBox(strMsg);
					}
				}
			}

			SetTimer(TIMER_PLC_LISTEN,500,NULL);
		}
		break;
	}

	CScrollView::OnTimer(nIDEvent);
}

BOOL CCOXRayView::IsLisence()
{
	CString strListenFile = m_strAppPath + _T("\\lisence.dat");

	HANDLE hFile = CreateFile(strListenFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	BYTE byFileKey[KEY_LEN] = {NULL};

	DWORD dwReadSize = 0;


	if (!ReadFile(hFile,byFileKey,KEY_LEN,&dwReadSize,NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	if (dwReadSize != KEY_LEN)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CLisence lisence;

	lisence.GetLock();
	BYTE *key = lisence.GetKey();

	// �Ƚ�
	for (int i = 0; i < KEY_LEN - 8; i++)
	{
		if (key[i] != byFileKey[i])
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}
	CloseHandle(hFile);
	return TRUE;
}

void CCOXRayView::OnDeviceConnect()
{
	// TODO: �ڴ���������������
	if (m_bAcqConnected)
	{
		m_XISLFunction.Close();

		m_bAcqConnected = FALSE;
	}
	else
	{
		BeginWaitCursor();

		InitAcq();	

		EndWaitCursor();
	}
}

void CCOXRayView::OnUpdateDeviceConnect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bIsLisence && !m_bContinueCapture && m_bStopSnap);

	if (m_bAcqConnected)
	{
		pCmdUI->SetText(_T("�Ͽ��豸"));
	}
	else
	{
		pCmdUI->SetText(_T("�����豸"));
	}
}


void CCOXRayView::OnSettingPane()
{
	// TODO: �ڴ���������������
	double dbIntTimes[8];
	int nIntTimes = 8;

	BeginWaitCursor();
	if (m_XISLFunction.GetFrameTimes(dbIntTimes,&nIntTimes) != HIS_ALL_OK)
	{
		AfxMessageBox(_T("��ȡƽ��ģʽʧ��!"));

		EndWaitCursor();
		return;
	}

	CPaneSettingDlg dlg;
	dlg.SetConfig(&m_Ini);
	dlg.SetPaneMode(dbIntTimes,nIntTimes);
	if (dlg.DoModal() == IDOK)
	{	
		InitAcq();	
	}
	EndWaitCursor();
}


void CCOXRayView::OnUpdateSettingPane(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bIsLisence && m_bAcqConnected);
}


void CCOXRayView::OnBtnStaticCap()
{
	// TODO: �ڴ���������������
	if (!m_bAcqConnected)
	{
		AfxMessageBox(_T("û�м�⵽���ʵ�ƽ��!"));
		return;
	}

	if (!m_bStopSnap)
	{
		return;
	}

	m_pRightDialogBar->m_PageImgCapture.m_BtnStaticCap.EnableWindow(FALSE);

	CCOXRayDoc *pDoc = GetDocument();

	pDoc->SetTitle(_T("ͼ��ɼ�"));

	m_pHWindow->ClearWindow();

	if (!m_bPLCStarted)
	{
		m_dwCurrentLocation = m_pRightDialogBar->m_PageImgCapture.m_ComboBoxPosNum.GetCurSel() + 1;
	}

	CString strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentLocation);

	if (m_dwCurrentLocation == 1)
	{
		m_Pos1Time = CTime::GetCurrentTime();
	}

	double dbVolKV = 0.0,dbCurmA = 0.0;

	if (m_ProjectXml.IsWellFormed())
	{
		m_ProjectXml.ResetPos();
		if (m_ProjectXml.FindChildElem(_T("Project")))
		{
			m_ProjectXml.IntoElem();

			if (m_ProjectXml.FindChildElem(strPos))
			{
				dbVolKV = _ttof(m_ProjectXml.GetChildAttrib(_T("Voltage")));
				dbCurmA = _ttof(m_ProjectXml.GetChildAttrib(_T("Current")));
			}

			m_ProjectXml.OutOfElem();
		}
	}
	else
	{
		dbVolKV = m_Ini.GetDouble(_T("LightSetting"),_T("Voltage"),0.0);
		dbCurmA = m_Ini.GetDouble(_T("LightSetting"),_T("Current"),0.0);
	}

	if (m_LightControl.IsOpen())
	{
		m_pRightDialogBar->m_meterCurrent.UpdateNeedle(dbCurmA);
		m_pRightDialogBar->m_meterVol.UpdateNeedle(dbVolKV);

		m_LightControl.SetVoltage(dbVolKV);
		m_LightControl.SetCurrent(dbCurmA);
		m_LightControl.OpenLight();

		Sleep(100);
	}

	// ��ȡ�ɼ�֡��
	CString strFrame;
	m_pRightDialogBar->m_PageImgCapture.m_ComboBoxFrames.GetWindowText(strFrame);

	m_nCaptureFrames = _ttol(strFrame);

	m_Ini.WriteUInt(_T("PaneSetting"),_T("CaptureFrames"),m_nCaptureFrames);

	m_Stopwatch.Start();

	UINT nRet = m_XISLFunction.InitalAcquisition(m_nCaptureFrames,ACQ_SNAP,HIS_SEQ_AVERAGE);

	if (nRet != HIS_ALL_OK)
	{
		AfxMessageBox(_T("��ʼ��ƽ��ɼ�ʧ��"));

		return;
	}

	m_bStopSnap = FALSE;
	m_pStatusBar->SetPaneText(0,_T("���ھ�̬�ɼ�......"));

	Invalidate(TRUE);
}


void CCOXRayView::OnBtnContCap()
{
	// TODO: �ڴ���������������
	if (!m_bAcqConnected)
	{
		AfxMessageBox(_T("û�м�⵽���ʵ�ƽ��!"));
		return;
	}

	if (m_bContinueCapture)
	{
		m_XISLFunction.StopContinueCapture();

		m_bContinueCapture = FALSE;
		m_bStopSnap = TRUE;

		m_pRightDialogBar->m_PageImgCapture.m_BtnContinueCap.SetWindowText(_T("��ʼ�����ɼ�"));
		m_pRightDialogBar->m_PageImgCapture.m_BtnStaticCap.EnableWindow(TRUE);
	}
	else
	{
		CCOXRayDoc *pDoc = GetDocument();

		pDoc->SetTitle(_T("ͼ��ɼ�"));

		m_pHWindow->ClearWindow();

		m_dwCurrentLocation = m_pRightDialogBar->m_PageImgCapture.m_ComboBoxPosNum.GetCurSel() + 1;

		CString strPos;
		strPos.Format(_T("POS%d"),m_dwCurrentLocation);

		double dbVolKV = 0.0,dbCurmA = 0.0;

		if (m_ProjectXml.IsWellFormed())
		{
			m_ProjectXml.ResetPos();
			if (m_ProjectXml.FindChildElem(_T("Project")))
			{
				m_ProjectXml.IntoElem();

				if (m_ProjectXml.FindChildElem(strPos))
				{
					dbVolKV = _ttof(m_ProjectXml.GetChildAttrib(_T("Voltage")));
					dbCurmA = _ttof(m_ProjectXml.GetChildAttrib(_T("Current")));
				}

				m_ProjectXml.OutOfElem();
			}
		} 
		else
		{
			dbVolKV = m_Ini.GetDouble(_T("LightSetting"),_T("Voltage"),0.0);
			dbCurmA = m_Ini.GetDouble(_T("LightSetting"),_T("Current"),0.0);
		}

		if (m_LightControl.IsOpen())
		{
			m_pRightDialogBar->m_meterVol.UpdateNeedle(dbVolKV);
			m_pRightDialogBar->m_meterCurrent.UpdateNeedle(dbCurmA);
			m_LightControl.SetVoltage(dbVolKV);
			m_LightControl.SetCurrent(dbCurmA);
			m_LightControl.OpenLight();

			Sleep(100);
		}

		m_Stopwatch.Start();

		m_XISLFunction.StartContinueCapture();

		m_bContinueCapture = TRUE;
		m_bStopSnap = FALSE;

		m_pRightDialogBar->m_PageImgCapture.m_BtnContinueCap.SetWindowText(_T("ֹͣ�����ɼ�"));
		m_pRightDialogBar->m_PageImgCapture.m_BtnStaticCap.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageFilterParm.GetDlgItem(IDC_BTN_SNAP)->EnableWindow(FALSE);
	}
}


afx_msg LRESULT CCOXRayView::OnEndAcqMessage(WPARAM wParam, LPARAM lParam)
{
	PACQ_DATA pAcqData = (PACQ_DATA)wParam;

	switch (pAcqData->dwDataType)
	{
	case ACQ_SNAP:
		{
			CString strText;
			strText.Format(_T("������̬�ɼ� - %d/%d"),pAcqData->dwActSecBufferFrame,m_nCaptureFrames);

			m_pStatusBar->SetPaneText(0,strText);

			if (m_LightControl.IsOpen())
			{
				m_LightControl.SetVoltage(0.0);
				m_LightControl.SetCurrent(0.0);
				m_LightControl.CloseLight();
			}
		}
		break;

	case ACQ_CONT:
		{
			CString strText;
			strText.Format(_T("ֹͣ�����ɼ� - %d"),pAcqData->dwActSecBufferFrame);

			m_pStatusBar->SetPaneText(0,strText);

			if (m_LightControl.IsOpen())
			{
				m_LightControl.SetVoltage(0.0);
				m_LightControl.SetCurrent(0.0);
				m_LightControl.CloseLight();
			}

			if (m_VideoWriter.isOpened())
			{
				m_VideoWriter.release();
			}
		}
		break;

	case ACQ_OFFSET:	
		{
			CString strText;
			strText.Format(_T("��ɻ�׼У�� - %d"),pAcqData->dwActSecBufferFrame);

			m_pStatusBar->SetPaneText(0,strText);

			CString strOffsetPath = m_Ini.GetString(_T("PaneSetting"),_T("OffsetPath"));

			HANDLE hFile = CreateFile(strOffsetPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				0,
				NULL
				);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				WinHeaderType FileHeader = {0};
				FileHeader.FileType=0x7000;
				FileHeader.HeaderSize = sizeof(WinHeaderType);
				FileHeader.ImageHeaderSize = 32;
				FileHeader.FileSize = FileHeader.HeaderSize+FileHeader.ImageHeaderSize+pAcqData->nRows*pAcqData->nColumns*sizeof(WORD);
				FileHeader.ULX = FileHeader.ULY = 1;
				FileHeader.BRX = (WORD)pAcqData->nColumns;
				FileHeader.BRY = (WORD)pAcqData->nRows;
				FileHeader.NrOfFrames = 1;
				FileHeader.TypeOfNumbers = 4;//DATASHORT;
				FileHeader.IntegrationTime = 0;

				DWORD dwSize;
				WriteFile(hFile,&FileHeader,sizeof(WinHeaderType),&dwSize,NULL);

				DWORD dwOffset = FileHeader.ImageHeaderSize + FileHeader.HeaderSize;

				SetFilePointer(hFile,dwOffset,NULL,FILE_BEGIN);

				DWORD dwLen = FileHeader.FileSize - dwOffset;

				WriteFile(hFile,pAcqData->pData,dwLen,&dwSize,NULL);

				CloseHandle(hFile);
			}

		}
		break;

	case ACQ_GAIN:	
		{
			CString strText;
			strText.Format(_T("�������У�� - %d"),pAcqData->dwActSecBufferFrame);

			m_pStatusBar->SetPaneText(0,strText);

			CString strGainPath = m_Ini.GetString(_T("PaneSetting"),_T("GainPath"));

			HANDLE hFile = CreateFile(strGainPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				0,
				NULL
				);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				WinHeaderType FileHeader = {0};
				FileHeader.FileType=0x7000;
				FileHeader.HeaderSize = sizeof(WinHeaderType);
				FileHeader.ImageHeaderSize = 32;
				FileHeader.FileSize = FileHeader.HeaderSize+FileHeader.ImageHeaderSize+pAcqData->nRows*pAcqData->nColumns*sizeof(DWORD);
				FileHeader.ULX = FileHeader.ULY = 1;
				FileHeader.BRX = (WORD)pAcqData->nColumns;
				FileHeader.BRY = (WORD)pAcqData->nRows;
				FileHeader.NrOfFrames = 1;
				FileHeader.TypeOfNumbers = 32;//DATASHORT;
				FileHeader.IntegrationTime = 0;

				DWORD dwSize;
				WriteFile(hFile,&FileHeader,sizeof(WinHeaderType),&dwSize,NULL);

				DWORD dwOffset = FileHeader.ImageHeaderSize + FileHeader.HeaderSize;

				SetFilePointer(hFile,dwOffset,NULL,FILE_BEGIN);

				DWORD dwLen = FileHeader.FileSize - dwOffset;

				WriteFile(hFile,pAcqData->pData,dwLen,&dwSize,NULL);

				CloseHandle(hFile);

				if (m_LightControl.IsOpen())
				{
					m_LightControl.SetVoltage(0.0);
					m_LightControl.SetCurrent(0.0);
					m_LightControl.CloseLight();
				}
			}
		}
		break;
	}

	CCOXRayDoc *pDoc = GetDocument();

	HImage Image = HImage(pAcqData->pData,pAcqData->nColumns,pAcqData->nRows,"uint2");

	double dbDegree = m_Ini.GetDouble(_T("ImageProcess"),_T("Rotate"),0.0);

	if (dbDegree != 0.0)
	{
		Image = RotateImage(Image,dbDegree);
	}

	// ת��Ϊ8λ3ͨ��
//	Image = ConvertImage(Image,IPL_DEPTH_8U,3);

	// ��ת90��
//	Image = Image.RotateImage(270,"constant");

	BOOL bAutoProcess = m_pRightDialogBar->m_PageImgCapture.m_CheckAutoProcess.GetCheck();

	m_Ini.WriteBool(_T("ImageProcess"),_T("En_Auto"),bAutoProcess);

	if (bAutoProcess)
	{
		pDoc->SetImage(Image,FALSE);

		// ��ȡ�Զ��������
		double dbGamma = m_Ini.GetDouble(_T("ImageProcess"),_T("Gamma"),1.0);
		long maskWidth = m_Ini.GetInt(_T("Enhance"),_T("MaskWidth"),7);
		long maskHeight = m_Ini.GetInt(_T("Enhance"),_T("MaskWidth"),7);
		double dbFactor = m_Ini.GetDouble(_T("Enhance"),_T("Factor"),1.0);
		int times = m_Ini.GetInt(_T("Enhance"),_T("Times"),0);

		if (m_ProjectXml.IsWellFormed())
		{
			CString strPos;
			strPos.Format(_T("POS%d"),m_dwCurrentLocation);

			m_ProjectXml.ResetPos();
			if (m_ProjectXml.FindChildElem(_T("Project")))
			{
				m_ProjectXml.IntoElem();
				if (m_ProjectXml.FindChildElem(strPos))
				{
					// �Ƿ��Ѿ�������ƫ������
					BOOL bRecord = _ttoi(m_ProjectXml.GetChildAttrib(_T("Record")));
					if (bRecord)
					{
						m_ProjectXml.IntoElem();
						if (m_ProjectXml.FindChildElem(_T("ImageProcess")))
						{
							// ����ͼ����ڵ�
							m_ProjectXml.IntoElem();
							if (m_ProjectXml.FindChildElem(_T("Gamma")))
							{
								dbGamma = _ttof(m_ProjectXml.GetChildAttrib(_T("Factor")));
							}

							m_ProjectXml.ResetChildPos();
							if (m_ProjectXml.FindChildElem(_T("Enhance")))
							{
								maskWidth = _ttoi(m_ProjectXml.GetChildAttrib(_T("MaskWidth")));
								maskHeight = _ttoi(m_ProjectXml.GetChildAttrib(_T("MaskHeight")));
								dbFactor = _ttof(m_ProjectXml.GetChildAttrib(_T("Factor")));
								times = _ttoi(m_ProjectXml.GetChildAttrib(_T("Times")));
							}

							m_ProjectXml.OutOfElem();
						}

						m_ProjectXml.OutOfElem();
					}				
				}
				m_ProjectXml.OutOfElem();
			}
	
		}

		// �Զ�����
		if (dbGamma != 1.0)
		{
			Image = GammaImage(Image,dbGamma);
		}

		for (int i = 0; i < times; i++)
		{
			Image = EmphasizeImage(Image,maskWidth,maskHeight,dbFactor);
		}

		pDoc->SetImage(Image,TRUE,FALSE);

	}
	else
	{
		pDoc->SetImage(Image);
	}


	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());

	int nIndex = m_pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	m_pStatusBar->SetPaneText(nIndex,strTime);

	if (pAcqData->dwDataType == ACQ_SNAP && m_Ini.GetBool(_T("SaveSetting"),_T("En_AutoSave"),TRUE) && !m_bFilterParmSnap)
	{
		// ����ͼ��
		CString strPos,strPorductSpec;
		strPos.Format(_T("POS%d"),m_dwCurrentLocation);

		IMG_INFO imgInfo;
		imgInfo.strPos = strPos;

		CTime time = CTime::GetCurrentTime();

		CString strPN;
		m_pRightDialogBar->m_PageImgCapture.m_EditPN.GetWindowText(strPN);

		imgInfo.strProductName = strPN;
		imgInfo.time = time;
		imgInfo.strDepartment = m_Ini.GetString(_T("SaveSetting"),_T("Department"));

		imgInfo.strWorkerName = m_Ini.GetString(_T("SaveSetting"),_T("WorkerNumber"));
		imgInfo.dbVolKV = m_Ini.GetDouble(_T("LightSetting"),_T("Voltage"),0.0);
		imgInfo.dbCurrentMA = m_Ini.GetDouble(_T("LightSetting"),_T("Current"),0.0);
		
		if (m_ProjectXml.IsWellFormed())
		{
			m_ProjectXml.ResetPos();

			if (m_ProjectXml.FindChildElem(_T("Project")))
			{
				m_ProjectXml.IntoElem();

				if (m_ProjectXml.FindChildElem(_T("ProjectName")))
				{
					m_ProjectXml.IntoElem();

					imgInfo.strProjectName = m_ProjectXml.GetData();

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("CustomerName")))
				{
					m_ProjectXml.IntoElem();

					imgInfo.strCustomer = m_ProjectXml.GetData();

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("ProductName")))
				{
					m_ProjectXml.IntoElem();

					imgInfo.strProductName = m_ProjectXml.GetData();

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("ProductLengh")))
				{
					m_ProjectXml.IntoElem();

					strPorductSpec += m_ProjectXml.GetData() + _T("x");

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("ProductWidth")))
				{
					m_ProjectXml.IntoElem();

					strPorductSpec += m_ProjectXml.GetData() + _T("x");

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("ProductHeight")))
				{
					m_ProjectXml.IntoElem();

					strPorductSpec += m_ProjectXml.GetData();

					m_ProjectXml.OutOfElem();
				}

				imgInfo.strProductSpec = strPorductSpec;

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("InspectLevel")))
				{
					m_ProjectXml.IntoElem();

					imgInfo.level = _ttoi(m_ProjectXml.GetData());

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("InspectMode")))
				{
					m_ProjectXml.IntoElem();

					imgInfo.mode = _ttoi(m_ProjectXml.GetData());

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(_T("WorkerName")))
				{
					m_ProjectXml.IntoElem();

					imgInfo.strWorkerName = m_ProjectXml.GetData();

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.ResetChildPos();
				if (m_ProjectXml.FindChildElem(strPos))
				{
					imgInfo.dbVolKV = _ttof(m_ProjectXml.GetChildAttrib(_T("Voltage")));
					imgInfo.dbCurrentMA = _ttof(m_ProjectXml.GetChildAttrib(_T("Current")));
				}

				m_ProjectXml.OutOfElem();
			}
		}

		if (!m_Ini.GetBool(_T("SaveSetting"),_T("En_NoRemind"),FALSE))
		{
			// ��������ͼƬ���ô���
			CSaveSettingDlg dlg;
			dlg.SetConfig(&m_Ini);

			dlg.SetImgInfo(imgInfo);

			dlg.DoModal();
		}

		CString strSavePath,strFileName;

		if (imgInfo.strProjectName.IsEmpty())
		{
			strSavePath.Format(_T("%s\\data\\%s")
				,m_strAppPath,m_Pos1Time.Format(_T("%Y%m%d%H%M%S")));
		}
		else
		{
			strSavePath.Format(_T("%s\\data\\%s\\%s")
				,m_strAppPath,imgInfo.strProjectName,m_Pos1Time.Format(_T("%Y%m%d%H%M%S")));
		}
		
		if (!PathFileExists(strSavePath))
		{
			SHCreateDirectory(NULL,strSavePath);
		}


		//PRODUCT_CUSTOMER_POS_KV_MA_DATE_TIME
		strFileName.Format(_T("%s\\"),strSavePath);

		if (m_Ini.GetBool(_T("SaveSetting"),_T("En_Product"),FALSE))
		{
			strFileName += imgInfo.strProductName + _T("_");
		}

		if (m_Ini.GetBool(_T("SaveSetting"),_T("En_Customer"),FALSE))
		{
			strFileName += imgInfo.strCustomer + _T("_");
		}

		if (m_Ini.GetBool(_T("SaveSetting"),_T("En_Pos"),TRUE))
		{
			strFileName += strPos + _T("_");
		}

		if (m_Ini.GetBool(_T("SaveSetting"),_T("En_Voltage"),TRUE))
		{
			CString strVol;
			strVol.Format(_T("%.1fKV_"),imgInfo.dbVolKV);

			strFileName += strVol;
		}

		if (m_Ini.GetBool(_T("SaveSetting"),_T("En_Current"),TRUE))
		{
			CString strCurrent;
			strCurrent.Format(_T("%.2fmA_"),imgInfo.dbCurrentMA);

			strFileName += strCurrent;
		}

		strFileName += time.Format(_T("%Y%m%d_%H%M%S"));	
		strFileName += _T(".tif");

		imgInfo.strOrignalPath = strFileName;

		pDoc->DoSave(strFileName);

		m_pBottomDialogBar->DrawThumbnail(strSavePath,_T("*.tif"));

		if (m_nInpectMode != MANUL_MODE)
		{
			OnBtnCheck();
		}

		if (m_bDatabaseConnected)
		{
			// �������ݿ�
			m_nCurrentId = m_MyDatabase.InsertData(&imgInfo);
		}
		
	}

	if (m_bPLCStarted && m_bPLCConnected && m_nInpectMode != MANUL_MODE)
	{
		//����PLC�ɼ����
		DWORD dwD2100 = CMD_PC_RUNNING | CMD_SCAN_END;
		if (!m_PLCommand.WritePLCD(D2100,1,&dwD2100))
		{
			CString strMsg;
			if (m_PLCommand.m_ErrorType == Error_System)
			{
				strMsg.Format(_T("дPLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
			}
			else
			{
				strMsg.Format(_T("дPLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
			}

			AfxMessageBox(strMsg);
		}
	}

	m_bStopSnap = TRUE;
	m_bFilterParmSnap = FALSE;

	m_pRightDialogBar->m_PageImgCapture.m_BtnStaticCap.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageFilterParm.GetDlgItem(IDC_BTN_SNAP)->EnableWindow(TRUE);
	
	return 0;
}


afx_msg LRESULT CCOXRayView::OnEndFrameMessage(WPARAM wParam, LPARAM lParam)
{
	PACQ_DATA pAcqData = (PACQ_DATA)wParam;

	switch (pAcqData->dwDataType)
	{
	case ACQ_SNAP:
		{
			CString strText;
			strText.Format(_T("���ھ�̬�ɼ� - %d/%d"),pAcqData->dwActSecBufferFrame,m_nCaptureFrames);

			m_pStatusBar->SetPaneText(0,strText);
		}
		break;

	case ACQ_CONT:
		{
			CString strText;
			strText.Format(_T("���������ɼ� - %d"),pAcqData->dwActSecBufferFrame);

			m_pStatusBar->SetPaneText(0,strText);
			
		}
		break;

	case ACQ_OFFSET:	
		{
			CString strText;
			strText.Format(_T("���ڻ�׼У�� - %d"),pAcqData->dwActSecBufferFrame);

			m_pStatusBar->SetPaneText(0,strText);
		}
		break;

	case ACQ_GAIN:	
		{
			CString strText;
			strText.Format(_T("��������У�� - %d"),pAcqData->dwActSecBufferFrame);

			m_pStatusBar->SetPaneText(0,strText);
		}
		break;
	}

	CCOXRayDoc *pDoc = GetDocument();

	HImage Image = HImage(pAcqData->pData,pAcqData->nColumns,pAcqData->nRows,"uint2");

	double dbDegree = m_Ini.GetDouble(_T("ImageProcess"),_T("Rotate"),0.0);

	if (dbDegree != 0.0)
	{
		Image = RotateImage(Image,dbDegree);
	}

	// ת��Ϊ8λ3ͨ��
//	Image = ConvertImage(Image,IPL_DEPTH_8U,3);

	// ��ת90��
//	Image = Image.RotateImage(270,"constant");

	BOOL bAutoProcess = m_pRightDialogBar->m_PageImgCapture.m_CheckAutoProcess.GetCheck();

	m_Ini.WriteBool(_T("ImageProcess"),_T("En_Auto"),bAutoProcess);

	if (bAutoProcess)
	{
		pDoc->SetImage(Image,FALSE);

		// ��ȡ�Զ��������
		double dbGamma = m_Ini.GetDouble(_T("ImageProcess"),_T("Gamma"),1.0);
		long maskWidth = m_Ini.GetInt(_T("Enhance"),_T("MaskWidth"),7);
		long maskHeight = m_Ini.GetInt(_T("Enhance"),_T("MaskWidth"),7);
		double dbFactor = m_Ini.GetDouble(_T("Enhance"),_T("Factor"),1.0);
		int times = m_Ini.GetInt(_T("Enhance"),_T("Times"),0);

		if (m_ProjectXml.IsWellFormed())
		{
			CString strPos;
			strPos.Format(_T("POS%d"),m_dwCurrentLocation);

			m_ProjectXml.ResetPos();
			if (m_ProjectXml.FindChildElem(_T("Project")))
			{
				m_ProjectXml.IntoElem();
				if (m_ProjectXml.FindChildElem(strPos))
				{
					// �Ƿ��Ѿ�������ƫ������
					BOOL bRecord = _ttoi(m_ProjectXml.GetChildAttrib(_T("Record")));
					if (bRecord)
					{
						m_ProjectXml.IntoElem();
						if (m_ProjectXml.FindChildElem(_T("ImageProcess")))
						{
							// ����ͼ����ڵ�
							m_ProjectXml.IntoElem();
							if (m_ProjectXml.FindChildElem(_T("Gamma")))
							{
								dbGamma = _ttof(m_ProjectXml.GetChildAttrib(_T("Factor")));
							}

							m_ProjectXml.ResetChildPos();
							if (m_ProjectXml.FindChildElem(_T("Enhance")))
							{
								maskWidth = _ttoi(m_ProjectXml.GetChildAttrib(_T("MaskWidth")));
								maskHeight = _ttoi(m_ProjectXml.GetChildAttrib(_T("MaskHeight")));
								dbFactor = _ttof(m_ProjectXml.GetChildAttrib(_T("Factor")));
								times = _ttoi(m_ProjectXml.GetChildAttrib(_T("Times")));
							}

							m_ProjectXml.OutOfElem();
						}

						m_ProjectXml.OutOfElem();
					}				
				}
				m_ProjectXml.OutOfElem();
			}

		}

		// �Զ�����
		if (dbGamma != 1.0)
		{
			Image = GammaImage(Image,dbGamma);
		}

		for (int i = 0; i < times; i++)
		{
			Image = EmphasizeImage(Image,maskWidth,maskHeight,dbFactor);
		}

		pDoc->SetImage(Image,TRUE,FALSE);

	}
	else
	{
		pDoc->SetImage(Image);
	}

	return 0;
}


afx_msg LRESULT CCOXRayView::OnGammaChange(WPARAM wParam, LPARAM lParam)
{
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return 1;
	}

	m_bGammaPreview = TRUE;

	double dbGamma = m_pRightDialogBar->m_PageImgProcess.m_dbEditGamma;
	m_Ini.WriteDouble(_T("ImageProcess"),_T("Gamma"),dbGamma);

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	return 0;
}


afx_msg LRESULT CCOXRayView::OnGammaChangeEnsure(WPARAM wParam, LPARAM lParam)
{
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return 1;
	}

	HImage *pImage = pDoc->GetImage();

	if (pImage == NULL)
	{
		return 1;
	}

	m_bGammaPreview = FALSE;

	double dbGamma = m_pRightDialogBar->m_PageImgProcess.m_dbEditGamma;

	m_Ini.WriteDouble(_T("ImageProcess"),_T("Gamma"),dbGamma);

	if (dbGamma != 1.0)
	{
		HImage hGammaImage = pImage->CopyImage();

		hGammaImage = GammaImage(hGammaImage,dbGamma);

		pDoc->SetImage(hGammaImage,FALSE,FALSE);
	}

	return 0;
}

void CCOXRayView::SetOptionType( int nType )
{

	m_pRightDialogBar->m_PageImgProcess.m_BtnMove.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnLine.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnRect.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnDist.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnText.SetCheck(FALSE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.SetCheck(FALSE);

	m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(TRUE);

	m_nOptionType = nType;

	switch (nType)
	{
	case OP_MOVE:
		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.SetCheck(TRUE);
		break;

	case OP_SELECT:
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.SetCheck(TRUE);
		break;

	case OP_ZOOM:
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.SetCheck(TRUE);
		break;

	case OP_LINE:
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.SetCheck(TRUE);

		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(TRUE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(FALSE);
		break;

	case OP_RECT:
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.SetCheck(TRUE);

		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(TRUE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(FALSE);
		break;

	case OP_ELLIPSE:
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.SetCheck(TRUE);

		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(TRUE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(FALSE);
		break;

	case OP_DIST:
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.SetCheck(TRUE);

		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(TRUE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(FALSE);
		break;

	case OP_DEGREE:
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.SetCheck(TRUE);

		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(TRUE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(FALSE);
		break;

	case OP_TEXT:
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.SetCheck(TRUE);

		m_pRightDialogBar->m_PageImgProcess.m_BtnMove.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnSelect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnZoom.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnLine.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnRect.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnEllipse.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDist.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnDegree.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgProcess.m_BtnText.EnableWindow(TRUE);
		break;

	}
}


void CCOXRayView::OnBtnMove()
{
	// TODO: �ڴ���������������

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
	case OP_MOVE:
		return;
	}

	SetOptionType(OP_MOVE);
}


void CCOXRayView::OnBtnSelect()
{
	// TODO: �ڴ���������������
	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
	case OP_SELECT:
		return;
	}
	SetOptionType(OP_SELECT);
}


void CCOXRayView::OnBtnZoom()
{
	// TODO: �ڴ���������������
	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
	case OP_ZOOM:
		return;
	}
	SetOptionType(OP_ZOOM);
}


void CCOXRayView::OnBtnLine()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	double dbZoomScale = pDoc->GetZoomFactor();

	int nType = m_nOptionType;
	SetOptionType(OP_LINE);

	BOOL bNoRemind = m_Ini.GetBool(_T("PenSetting"),_T("En_NoRemind"),FALSE);

	if (!bNoRemind)
	{
		CPenSettingDlg dlg;
		dlg.SetConfig(&m_Ini);
		dlg.DoModal();
	}

	int nLineWidth = m_Ini.GetUInt(_T("PenSetting"),_T("LineWidth"),1);
	COLORREF color = m_Ini.GetUInt(_T("PenSetting"),_T("Color"),RGB(255,0,0));

	CString strText = _T("��ס��������ʼ��������Ҽ���ɡ�");

	m_pStatusBar->SetPaneText(0,strText);

	m_pHWindow->SetLineWidth(nLineWidth);
	m_pHWindow->SetRgb(GetRValue(color),GetGValue(color),GetBValue(color));
	HTuple htRow1 = 100,htColumn1 = 100,htRow2,htColumn2;

	try
	{
		htRow1 = m_pHWindow->DrawLine(&htColumn1,&htRow2,&htColumn2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("���ȵ���������ʼ��������Ҽ���ɡ�"));

		AfxMessageBox(strErrorMsg);

		SetOptionType(nType);
		return;
	}

	m_pHWindow->DispLine(htRow1,htColumn1,htRow2,htColumn2);

	PDrawInfo pDraw = new DrawInfo;
	memset(pDraw,0,sizeof(DrawInfo));
	pDraw->nDrawType = OP_LINE;
	pDraw->htRow1 = htRow1 / dbZoomScale;
	pDraw->htColumn1 = htColumn1 / dbZoomScale;
	pDraw->htRow2 = htRow2 / dbZoomScale;
	pDraw->htColumn2 = htColumn2 / dbZoomScale;
	pDraw->color = color;
	pDraw->nLineWidth = nLineWidth;

	pDoc->SubmitUndoDraw(pDraw);

	SetOptionType(nType);
}


void CCOXRayView::OnBtnRect()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	double dbZoomScale = pDoc->GetZoomFactor();

	int nType = m_nOptionType;
	SetOptionType(OP_RECT);

	BOOL bNoRemind = m_Ini.GetBool(_T("PenSetting"),_T("En_NoRemind"),FALSE);

	if (!bNoRemind)
	{
		CPenSettingDlg dlg;
		dlg.SetConfig(&m_Ini);
		dlg.DoModal();
	}

	int nLineWidth = m_Ini.GetUInt(_T("PenSetting"),_T("LineWidth"),1);
	COLORREF color = m_Ini.GetUInt(_T("PenSetting"),_T("Color"),RGB(255,0,0));

	CString strText = _T("��ס��������ʼ��������Ҽ���ɡ�");

	m_pStatusBar->SetPaneText(0,strText);

	m_pHWindow->SetLineWidth(nLineWidth);
	m_pHWindow->SetRgb(GetRValue(color),GetGValue(color),GetBValue(color));
	m_pHWindow->SetDraw("margin");

	HTuple htRow1,htColumn1,htRow2,htColumn2;
	
	try
	{
		htRow1 = m_pHWindow->DrawRectangle1(&htColumn1,&htRow2,&htColumn2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("���ȵ���������ʼ��������Ҽ���ɡ�"));

		AfxMessageBox(strErrorMsg);

		SetOptionType(nType);
		return;
	}

	m_pHWindow->DispRectangle1(htRow1,htColumn1,htRow2,htColumn2);

	PDrawInfo pDraw = new DrawInfo;
	memset(pDraw,0,sizeof(DrawInfo));
	pDraw->nDrawType = OP_RECT;
	pDraw->htRow1 = htRow1 / dbZoomScale;
	pDraw->htColumn1 = htColumn1 / dbZoomScale;
	pDraw->htRow2 = htRow2 / dbZoomScale;
	pDraw->htColumn2 = htColumn2 / dbZoomScale;
	pDraw->color = color;
	pDraw->nLineWidth = nLineWidth;

	pDoc->SubmitUndoDraw(pDraw);

	SetOptionType(nType);
}


void CCOXRayView::OnBtnEllipse()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	double dbZoomScale = pDoc->GetZoomFactor();

	int nType = m_nOptionType;
	SetOptionType(OP_ELLIPSE);

	BOOL bNoRemind = m_Ini.GetBool(_T("PenSetting"),_T("En_NoRemind"),FALSE);

	if (!bNoRemind)
	{
		CPenSettingDlg dlg;
		dlg.SetConfig(&m_Ini);
		dlg.DoModal();
	}
	int nLineWidth = m_Ini.GetUInt(_T("PenSetting"),_T("LineWidth"),1);
	COLORREF color = m_Ini.GetUInt(_T("PenSetting"),_T("Color"),RGB(255,0,0));

	CString strText = _T("��ס��������ʼ��������Ҽ���ɡ�");

	m_pStatusBar->SetPaneText(0,strText);

	m_pHWindow->SetLineWidth(nLineWidth);
	m_pHWindow->SetRgb(GetRValue(color),GetGValue(color),GetBValue(color));
	m_pHWindow->SetDraw("margin");

	HTuple htRow,htColumn,htPhi,htRadius1,htRadius2;
	
	try
	{
		htRow = m_pHWindow->DrawEllipse(&htColumn,&htPhi,&htRadius1,&htRadius2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("���ȵ���������ʼ��������Ҽ���ɡ�"));

		AfxMessageBox(strErrorMsg);

		SetOptionType(nType);
		return;
	}

	m_pHWindow->DispEllipse(htRow,htColumn,htPhi,htRadius1,htRadius2);

	PDrawInfo pDraw = new DrawInfo;
	memset(pDraw,0,sizeof(DrawInfo));
	pDraw->nDrawType = OP_ELLIPSE;
	pDraw->htRow1 = htRow / dbZoomScale;
	pDraw->htColumn1 = htColumn / dbZoomScale;
	pDraw->htRow2 = htRadius1 / dbZoomScale;
	pDraw->htColumn2 = htRadius2 / dbZoomScale;
	pDraw->htPhi = htPhi;
	pDraw->color = color;
	pDraw->nLineWidth = nLineWidth;

	pDoc->SubmitUndoDraw(pDraw);

	SetOptionType(nType);
}


void CCOXRayView::OnBtnDist()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	double dbZoomScale = pDoc->GetZoomFactor();

	int nType = m_nOptionType;
	SetOptionType(OP_DIST);

	BOOL bNoRemind = m_Ini.GetBool(_T("PenSetting"),_T("En_NoRemind"),FALSE);

	if (!bNoRemind)
	{
		CPenSettingDlg dlg;
		dlg.SetConfig(&m_Ini);
		dlg.DoModal();
	}

	UINT nLineWidth = 1;//m_Ini.GetUInt(_T("PenSetting"),_T("LineWidth"),1);
	COLORREF color = m_Ini.GetUInt(_T("PenSetting"),_T("Color"),RGB(255,0,0));
	UINT nFontSize = m_Ini.GetUInt(_T("PenSetting"),_T("FontSize"),20);

	CString strText = _T("��ס��������ʼ��������Ҽ���ɡ�");

	m_pStatusBar->SetPaneText(0,strText);

	m_pHWindow->SetLineWidth(nLineWidth);
	m_pHWindow->SetRgb(GetRValue(color),GetGValue(color),GetBValue(color));

	HTuple htRow1,htColumn1,htRow2,htColumn2,htDistance;
	HTuple htRow11,htColumn11,htRow12,htColumn12,htRow21,htColumn21,htRow22,htColumn22;
	double dbDistance,dbOffsetColumn,dbOffsetRow;

	try
	{
		htRow1 = m_pHWindow->DrawLine(&htColumn1,&htRow2,&htColumn2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("���ȵ���������ʼ��������Ҽ���ɡ�"));

		AfxMessageBox(strErrorMsg);

		SetOptionType(nType);
		return;
	}
	

	distance_pp(htRow1,htColumn1,htRow2,htColumn2,&dbDistance);

	dbOffsetColumn = (htRow2[0].D() - htRow1[0].D()) * 10 / dbDistance;
	dbOffsetRow = (htColumn2[0].D() - htColumn1[0].D()) * 10 / dbDistance;

	htRow11 = htRow1 + dbOffsetRow;
	htColumn11 = htColumn1 - dbOffsetColumn;

	htRow12 = htRow1 - dbOffsetRow;
	htColumn12 = htColumn1 + dbOffsetColumn;

	htRow21 = htRow2 + dbOffsetRow;
	htColumn21 = htColumn2 - dbOffsetColumn;

	htRow22 = htRow2 - dbOffsetRow;
	htColumn22 = htColumn2 + dbOffsetColumn;

	m_pHWindow->DispLine(htRow1,htColumn1,htRow2,htColumn2);
	m_pHWindow->DispLine(htRow11,htColumn11,htRow12,htColumn12);
	m_pHWindow->DispLine(htRow21,htColumn21,htRow22,htColumn22);
	
	dbDistance /= dbZoomScale;

	//
	double dbPerPixel = m_Ini.GetDouble(_T("CalibrationSetting"),_T("PerPixel"),0.0);
	UINT nUnitIndex = m_Ini.GetUInt(_T("CalibrationSetting"),_T("UnitIndex"),0);
	CString strUnit = Units[nUnitIndex];

	if (!m_bPLCStarted)
	{
		m_dwCurrentLocation = m_pRightDialogBar->m_PageImgCapture.m_ComboBoxPosNum.GetCurSel() + 1;
	}

	CString strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentLocation);
	if (m_ProjectXml.IsWellFormed())
	{
		m_ProjectXml.ResetPos();
		if (m_ProjectXml.FindChildElem(_T("Project")))
		{
			m_ProjectXml.IntoElem();
			if (m_ProjectXml.FindChildElem(strPos))
			{
				dbPerPixel = _ttof(m_ProjectXml.GetChildAttrib(_T("PerPixel")));
				strUnit = m_ProjectXml.GetChildAttrib(_T("Unit"));
			}
			m_ProjectXml.OutOfElem();
		}
	}

	CString strDistance;
	if (dbPerPixel > 0.0)
	{
		strDistance.Format(_T(" %.2f %s"),dbDistance * dbPerPixel,strUnit);	
	}
	else
	{
		strDistance.Format(_T(" %.2f pixel"),dbDistance);
	}
	CString strFont;
	
	strFont.Format(_T("-Arial-%d-"),nFontSize);

	USES_CONVERSION;
	char *text = W2A(strDistance);
	char *font = W2A(strFont);

	m_pHWindow->SetTposition(htRow2,htColumn2);
	m_pHWindow->SetFont(font);
	m_pHWindow->WriteString(text);

	PDrawInfo pDraw = new DrawInfo;
	memset(pDraw,0,sizeof(DrawInfo));
	pDraw->nDrawType = OP_DIST;
	pDraw->htRow1 = htRow1 / dbZoomScale;
	pDraw->htColumn1 = htColumn1 / dbZoomScale;
	pDraw->htRow2 = htRow2 / dbZoomScale;
	pDraw->htColumn2 = htColumn2 / dbZoomScale;
	pDraw->color = color;
	pDraw->nLineWidth = nLineWidth;
	pDraw->logFont.lfHeight = nFontSize;
	pDraw->dbPerPixel = dbPerPixel;
	pDraw->nUnitIndex = nUnitIndex;

	pDoc->SubmitUndoDraw(pDraw);

	SetOptionType(nType);
}

void CCOXRayView::OnBtnDegree()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	int nType = m_nOptionType;

	SetOptionType(OP_DEGREE);

	SetOptionType(nType);
}


void CCOXRayView::OnBtnText()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	switch (m_nOptionType)
	{
	case OP_LINE:
	case OP_RECT:
	case OP_ELLIPSE:
	case OP_DIST:
	case OP_DEGREE:
	case OP_TEXT:
	case OP_CALIB:
		return;
	}

	int nType = m_nOptionType;
	SetOptionType(OP_TEXT);

	double dbZoomScale = pDoc->GetZoomFactor();

	CPlaceTextDlg dlg;
	dlg.SetConfig(&m_Ini);
	if (dlg.DoModal() == IDOK)
	{
		LOGFONT logFont;
		dlg.m_Font.GetLogFont(&logFont);

		COLORREF color = dlg.m_colorText;

		CString strText = _T("��������ѡ�����ַ���λ�ã�������Ҽ�ȷ����");
		m_pStatusBar->SetPaneText(0,strText);

		m_pHWindow->SetRgb(GetRValue(color),GetGValue(color),GetBValue(color));

		HTuple htRow,htColumn;

		try
		{
			htRow = m_pHWindow->DrawPoint(&htColumn);
		}
		catch (HException &except)
		{
			CString strErrorMsg;
			strErrorMsg.Format(_T("���ȵ���������ʼ��������Ҽ���ɡ�"));

			AfxMessageBox(strErrorMsg);

			SetOptionType(nType);
			return;
		}
		

		//-FontName-Height-Width-Italic-Underlined-Strikeout-Bold-CharSet- 
		CString strFont;
		strFont.Format(_T("-Arial-%d-%d-%d-%d-%d-%d-DEFAULT_CHARSET-")
			,logFont.lfHeight
			,logFont.lfWidth
			,logFont.lfItalic
			,logFont.lfUnderline
			,logFont.lfStrikeOut
			,logFont.lfWeight == FW_BOLD
			);

		strText = dlg.m_strEditText;

		USES_CONVERSION;
		char *font = W2A(strFont);
		char *text = W2A(strText);

		m_pHWindow->SetFont(font);
		m_pHWindow->SetTposition(htRow,htColumn);
		m_pHWindow->WriteString(text);

		PDrawInfo pDraw = new DrawInfo;
		memset(pDraw,0,sizeof(DrawInfo));
		pDraw->nDrawType = OP_TEXT;
		pDraw->htRow1 = htRow / dbZoomScale;
		pDraw->htColumn1 = htColumn / dbZoomScale;
		pDraw->color = color;
		pDraw->nLineWidth = 1;
		memcpy(&pDraw->logFont,&logFont,sizeof(LOGFONT));
		pDraw->htText = text;

		pDoc->SubmitUndoDraw(pDraw);
	}
	
	SetOptionType(nType);
}

void CCOXRayView::OnUpdateSettingPen(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CCOXRayView::OnSettingPen()
{
	// TODO: �ڴ���������������
	CPenSettingDlg dlg;
	dlg.SetConfig(&m_Ini);

	dlg.DoModal();
}

void CCOXRayView::DrawHistory()
{
	CCOXRayDoc *pDoc = GetDocument();

	if (pDoc == NULL)
	{
		return;
	}

	CListDrawInfo *pListDraw = pDoc->GetDrawList();

	if (pListDraw == NULL)
	{
		return;
	}

	double dbZoomScale = pDoc->GetZoomFactor();

	POSITION pos = pListDraw->GetHeadPosition();
	while (pos)
	{
		PDrawInfo pDraw = pListDraw->GetNext(pos);

		if (pDraw)
		{
			DrawGraphics(pDraw,dbZoomScale);
		}
	}
}

void CCOXRayView::DrawGraphics( PDrawInfo pDraw,double dbZoomScale )
{
	HTuple htRow1,htColumn1,htRow2,htColumn2;
	htRow1 = pDraw->htRow1 * dbZoomScale;
	htColumn1 = pDraw->htColumn1 * dbZoomScale;
	htRow2 = pDraw->htRow2 * dbZoomScale;
	htColumn2 = pDraw->htColumn2 * dbZoomScale;

	m_pHWindow->SetLineWidth(pDraw->nLineWidth);
	m_pHWindow->SetRgb(GetRValue(pDraw->color),GetGValue(pDraw->color),GetBValue(pDraw->color));
	m_pHWindow->SetDraw("margin");
	switch (pDraw->nDrawType)
	{
	case OP_LINE:
		{
			m_pHWindow->DispLine(htRow1,htColumn1,htRow2,htColumn2);
		}
		break;

	case OP_RECT:
		{
			m_pHWindow->DispRectangle1(htRow1,htColumn1,htRow2,htColumn2);
		}
		break;

	case OP_ELLIPSE:
		{
			m_pHWindow->DispEllipse(htRow1,htColumn1,0,htRow2,htColumn2);
		}
		break;

	case OP_DIST:
		{
			// �����ֱ��ֱ�ߵ�2���̵�ֱ��
			double dbDistance,dbOffsetColumn,dbOffsetRow;
			HTuple htRow11,htColumn11,htRow12,htColumn12,htRow21,htColumn21,htRow22,htColumn22;

			distance_pp(htRow1,htColumn1,htRow2,htColumn2,&dbDistance);

			dbDistance /= dbZoomScale;

			dbOffsetColumn = (htRow2[0].D() - htRow1[0].D()) * 10 / dbDistance;
			dbOffsetRow = (htColumn2[0].D() - htColumn1[0].D()) * 10 / dbDistance;

			htRow11 = htRow1 + dbOffsetRow;
			htColumn11 = htColumn1 - dbOffsetColumn;

			htRow12 = htRow1 - dbOffsetRow;
			htColumn12 = htColumn1 + dbOffsetColumn;

			htRow21 = htRow2 + dbOffsetRow;
			htColumn21 = htColumn2 - dbOffsetColumn;

			htRow22 = htRow2 - dbOffsetRow;
			htColumn22 = htColumn2 + dbOffsetColumn;

			m_pHWindow->DispLine(htRow1,htColumn1,htRow2,htColumn2);
			m_pHWindow->DispLine(htRow11,htColumn11,htRow12,htColumn12);
			m_pHWindow->DispLine(htRow21,htColumn21,htRow22,htColumn22);

			CString strDistance,strFont;

			if (pDraw->dbPerPixel > 0)
			{
				strDistance.Format(_T(" %.2f %s"),dbDistance * pDraw->dbPerPixel,Units[pDraw->nUnitIndex]);
			}
			else
			{
				strDistance.Format(_T(" %.2f pixel"),dbDistance);
			}
			
			strFont.Format(_T("-Arial-%d-"),int(pDraw->logFont.lfHeight));

			USES_CONVERSION;
			char *text = W2A(strDistance);
			char *font = W2A(strFont);

			m_pHWindow->SetTposition(htRow2,htColumn2);
			m_pHWindow->SetFont(font);
			m_pHWindow->WriteString(text);

		}
		break;

	case OP_DEGREE:
		{

		}
		break;

	case OP_TEXT:
		{
			//-FontName-Height-Width-Italic-Underlined-Strikeout-Bold-CharSet- 
			CString strFont;
			strFont.Format(_T("-Arial-%d-%d-%d-%d-%d-%d-DEFAULT_CHARSET-")
				,pDraw->logFont.lfHeight
				,pDraw->logFont.lfWidth
				,pDraw->logFont.lfItalic
				,pDraw->logFont.lfUnderline
				,pDraw->logFont.lfStrikeOut
				,pDraw->logFont.lfWeight == FW_BOLD
				);

			USES_CONVERSION;
			char *font = W2A(strFont);

			m_pHWindow->SetFont(font);
			m_pHWindow->SetTposition(htRow1,htColumn1);
			m_pHWindow->WriteString(pDraw->htText);
		}
		break;
	}
}

void CCOXRayView::OnUpdateSettingCalibration(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCOXRayDoc *pDoc = GetDocument();

	if (pDoc)
	{
		HImage *pImage = pDoc->GetImage();

		pCmdUI->Enable(pImage != NULL);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CCOXRayView::OnSettingCalibration()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	int nType = m_nOptionType;
	SetOptionType(OP_CALIB);

	UINT nLineWidth = 1;//m_Ini.GetUInt(_T("PenSetting"),_T("LineWidth"),1);
	COLORREF color = m_Ini.GetUInt(_T("PenSetting"),_T("Color"),RGB(255,0,0));
	UINT nFontSize = m_Ini.GetUInt(_T("PenSetting"),_T("FontSize"),20);

	CString strText = _T("��ס��������ʼ��������Ҽ���ɡ�");

	m_pStatusBar->SetPaneText(0,strText);

	m_pHWindow->SetLineWidth(nLineWidth);
	m_pHWindow->SetRgb(GetRValue(color),GetGValue(color),GetBValue(color));


	HTuple htRow1,htColumn1,htRow2,htColumn2,htDistance;
	HTuple htRow11,htColumn11,htRow12,htColumn12,htRow21,htColumn21,htRow22,htColumn22;
	double dbDistance,dbOffsetColumn,dbOffsetRow;
	
	try
	{
		htRow1 = m_pHWindow->DrawLine(&htColumn1,&htRow2,&htColumn2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("���ȵ���������ʼ��������Ҽ���ɡ�"));

		AfxMessageBox(strErrorMsg);

		SetOptionType(nType);
		return;
	}

	distance_pp(htRow1,htColumn1,htRow2,htColumn2,&dbDistance);

	dbOffsetColumn = (htRow2[0].D() - htRow1[0].D()) * 10 / dbDistance;
	dbOffsetRow = (htColumn2[0].D() - htColumn1[0].D()) * 10 / dbDistance;

	htRow11 = htRow1 + dbOffsetRow;
	htColumn11 = htColumn1 - dbOffsetColumn;

	htRow12 = htRow1 - dbOffsetRow;
	htColumn12 = htColumn1 + dbOffsetColumn;

	htRow21 = htRow2 + dbOffsetRow;
	htColumn21 = htColumn2 - dbOffsetColumn;

	htRow22 = htRow2 - dbOffsetRow;
	htColumn22 = htColumn2 + dbOffsetColumn;

	m_pHWindow->DispLine(htRow1,htColumn1,htRow2,htColumn2);
	m_pHWindow->DispLine(htRow11,htColumn11,htRow12,htColumn12);
	m_pHWindow->DispLine(htRow21,htColumn21,htRow22,htColumn22);

	dbDistance /= pDoc->GetZoomFactor();

	CString strDistance,strFont;
	strDistance.Format(_T(" %.2fpixel"),dbDistance);
	strFont.Format(_T("-Arial-%d-"),nFontSize);

	USES_CONVERSION;
	char *text = W2A(strDistance);
	char *font = W2A(strFont);

	m_pHWindow->SetTposition(htRow2,htColumn2);
	m_pHWindow->SetFont(font);
	m_pHWindow->WriteString(text);

	CCalibrationSettingDlg dlg;
	dlg.SetPixel(dbDistance);
	if (dlg.DoModal() == IDOK)
	{
		double dbPerPixel = dlg.GetPerPixel();
		UINT nUnitIndex = dlg.GetSelectUnit();

		m_Ini.WriteDouble(_T("CalibrationSetting"),_T("PerPixel"),dbPerPixel);
		m_Ini.WriteUInt(_T("CalibrationSetting"),_T("UnitIndex"),nUnitIndex);
	}

	SetOptionType(nType);

	OnUpdate(NULL,WM_USER_NEWIMAGE,NULL);
	
}


void CCOXRayView::OnBtnSave()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	CListDrawInfo *pListDraw = pDoc->GetDrawList();

	// ����ͼ��
	CTime time = CTime::GetCurrentTime();

	CString strPN,strFileName,strSavePath;
	m_pRightDialogBar->m_PageImgCapture.m_EditPN.GetWindowText(strPN);
	m_pRightDialogBar->m_PageImgCapture.m_EditSavePath.GetWindowText(strSavePath);

	m_Ini.WriteString(_T("SaveSetting"),_T("SavePath"),strSavePath);

	if (!PathFileExists(strSavePath))
	{
		SHCreateDirectory(NULL,strSavePath);
	}

	strSavePath.TrimRight(_T("\\"));

	strFileName.Format(_T("%s\\%s_POS%d_%s"),strSavePath,strPN,m_dwCurrentLocation,time.Format(_T("%Y%m%d_%H%M%S")));

	strFileName += _T(".png");

	USES_CONVERSION;
	char *file = W2A(strFileName);

	if (pListDraw != NULL)
	{
		if (pListDraw->GetCount() > 0)
		{
			m_pHWindow->DumpWindow("png",file);
		}
		else
		{
			pDoc->OnSaveDocument(strFileName);
		}
	}
	else
	{
		pDoc->OnSaveDocument(strFileName);
	}

	AfxMessageBox(_T("����ͼƬ�ɹ�!"));

	if (m_bPLCStarted && m_bPLCConnected && m_nInpectMode == MANUL_MODE)
	{
		//����PLC�ɼ����
		DWORD dwD2100 = CMD_PC_RUNNING | CMD_SCAN_END;
		if (!m_PLCommand.WritePLCD(D2100,1,&dwD2100))
		{
			CString strMsg;
			if (m_PLCommand.m_ErrorType == Error_System)
			{
				strMsg.Format(_T("дPLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
			}
			else
			{
				strMsg.Format(_T("дPLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
			}

			AfxMessageBox(strMsg);
		}
	}
}


void CCOXRayView::OnUpdateDeviceOffset(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bIsLisence && m_bAcqConnected);
}


void CCOXRayView::OnDeviceOffset()
{
	// TODO: �ڴ���������������
	CPropertySheet sheet;

	CStepSetPath step1(TRUE);
	step1.SetConfig(&m_Ini);
	step1.m_psp.dwFlags |= PSP_USETITLE;
	step1.m_psp.pszTitle = _T("��׼У�� - ��һ���������ļ�·��");

	CStepTips step2(TRUE,TRUE);
	step2.SetXISLFucntion(&m_XISLFunction);
	step2.SetLightControl(&m_LightControl);
	step2.SetConfig(&m_Ini);
	step2.m_psp.dwFlags |= PSP_USETITLE;
	step2.m_psp.pszTitle = _T("��׼У�� - �ڶ�����ȷ�����ߴ��ڹر�״̬");
	step2.SetMessage(_T("����ȷ�����ߴ��ڹر�״̬��Ȼ������!"));

	sheet.AddPage(&step1);
	sheet.AddPage(&step2);

	sheet.SetWizardMode();
	sheet.SetActivePage(&step1);

	sheet.DoModal();
}


void CCOXRayView::OnUpdateDeviceGainOffset(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bIsLisence && m_bAcqConnected);
}


void CCOXRayView::OnDeviceGainOffset()
{
	// TODO: �ڴ���������������
	CPropertySheet sheet;

	CStepSetPath step1(FALSE);
	step1.SetConfig(&m_Ini);
	step1.m_psp.dwFlags |= PSP_USETITLE;
	step1.m_psp.pszTitle = _T("����У�� - ��һ���������ļ�·��");

	CStepTips step2(FALSE,FALSE);
	step2.SetXISLFucntion(&m_XISLFunction);
	step2.SetLightControl(&m_LightControl);
	step2.SetConfig(&m_Ini);
	step2.m_psp.dwFlags |= PSP_USETITLE;
	step2.m_psp.pszTitle = _T("����У�� - �ڶ�����ȷ�����ߴ��ڹر�״̬,����ƽ�������ǿյ�");
	step2.SetMessage(_T("����ȷ�����ߴ��ڹر�״̬, ����ƽ�������ǿյ�, Ȼ�����һ��!"));

	CStepTips step3(FALSE,TRUE);
	step3.SetXISLFucntion(&m_XISLFunction);
	step3.SetLightControl(&m_LightControl);
	step3.SetConfig(&m_Ini);
	step3.m_psp.dwFlags |= PSP_USETITLE;
	step3.m_psp.pszTitle = _T("����У�� - ��������ȷ�����ߴ��ڿ���״̬");
	step3.SetMessage(_T("����ȷ�����ߴ��ڿ���״̬��Ȼ������!"));

	sheet.AddPage(&step1);
	sheet.AddPage(&step2);
	sheet.AddPage(&step3);

	sheet.SetWizardMode();
	sheet.SetActivePage(&step1);

	sheet.DoModal();
}


void CCOXRayView::OnBtnCheck()
{
	// TODO: �ڴ���������������

	if (!m_ProjectXml.IsWellFormed())
	{
		AfxMessageBox(_T("û�м�����Ŀ�ļ���"));
		return;
	}

	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	if (!m_ProjectXml.IsWellFormed())
	{
		return;
	}

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	if (!m_bPLCStarted)
	{
		m_dwCurrentLocation = m_pRightDialogBar->m_PageImgCapture.m_ComboBoxPosNum.GetCurSel() + 1;
	}


	BOOL bFind = FALSE;
	m_pHWindow->SetDraw("margin");
	m_pHWindow->SetLineWidth(1);

	double dbZoomScale = pDoc->GetZoomFactor();

	HImage hImageDst = pImage->CopyImage();
	//16 λת��Ϊ8λ
	if (GetImageBits(hImageDst) == 16)
	{
		hImageDst = ConvertImage(hImageDst,IPL_DEPTH_8U,3);
	}

	double dbRow = 0.0,dbColumn = 0.0,dbPhi = 0.0,dbLen1 = 0.0,dbLen2 = 0.0;
	int nMinGray = 0,nMaxGray = 0,nOffsetGray = 0;
	HRegionArray hLightRegions,hDarkRegions;

	m_ProjectXml.ResetPos();

	CString strData,strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentLocation);
	if (m_ProjectXml.FindChildElem(_T("Project")))
	{
		m_ProjectXml.IntoElem();

		m_ProjectXml.ResetChildPos();

		if (m_ProjectXml.FindChildElem(strPos))
		{
			strData= m_ProjectXml.GetChildAttrib(_T("Areas"));
			int nAreaCount = _ttoi(strData);

			// ÿ�����ص�ʵ�ʾ���
			strData = m_ProjectXml.GetChildAttrib(_T("PerPixel"));
			double dbPerPixel = _ttof(strData);

			CString strUnit = m_ProjectXml.GetChildAttrib(_T("Unit"));

			if (dbPerPixel > 0.0 - EPSINON && dbPerPixel < 0.0 + EPSINON)
			{
				dbPerPixel = 1.0;
				strUnit = _T("pixel");
			}

			m_ProjectXml.IntoElem();

			m_ProjectXml.ResetChildPos();

			// ��Model �ļ�
			HShapeModel hShapModel;
			HImage hImageModel;
			if (m_ProjectXml.FindChildElem(_T("Shm")))
			{
				m_ProjectXml.IntoElem();

				CString strShmFile = m_ProjectXml.GetData();

				USES_CONVERSION;
				char *file = W2A(strShmFile);

				hShapModel.ReadShapeModel(file);

				m_ProjectXml.OutOfElem();
			}

			if (!hShapModel.IsHandleValid())
			{
				AfxMessageBox(_T("��ģ���ļ�����"));

				return;
			}

			m_ProjectXml.ResetChildPos();

			if (m_ProjectXml.FindChildElem(_T("Model")))
			{
				m_ProjectXml.IntoElem();

				CString strModelFile = m_ProjectXml.GetData();

				USES_CONVERSION;
				char *file = W2A(strModelFile);

				hImageModel = HImage::ReadImage(file);

				if (GetImageBits(hImageModel) == 16)
				{
					hImageModel = ConvertImage(hImageModel,IPL_DEPTH_8U,3);
				}

				m_ProjectXml.OutOfElem();
			}

			// �Ҷ�λ��
			double dbRowRef = 0.0,dbColumnRef = 0.0,dbAngle;

			m_ProjectXml.ResetChildPos();
			if (m_ProjectXml.FindChildElem(_T("RowRef")))
			{
				m_ProjectXml.IntoElem();

				dbRowRef = _ttof(m_ProjectXml.GetData());

				m_ProjectXml.OutOfElem();
			}

			m_ProjectXml.ResetChildPos();
			if (m_ProjectXml.FindChildElem(_T("ColumnRef")))
			{
				m_ProjectXml.IntoElem();

				dbColumnRef = _ttof(m_ProjectXml.GetData());

				m_ProjectXml.OutOfElem();
			}

			m_ProjectXml.ResetChildPos();
			if (m_ProjectXml.FindChildElem(_T("Angle")))
			{
				m_ProjectXml.IntoElem();

				dbAngle = _ttof(m_ProjectXml.GetData());

				m_ProjectXml.OutOfElem();
			}

			// �ж�ģ����ȶ�ͼ���ʽ�Ƿ�ƥ��
			if (GetImageBits(hImageModel) != GetImageBits(hImageDst))
			{
				AfxMessageBox(_T("ģ��ͼ�������ͼ���ʽ��ƥ�䣡"));
				return;
			}

			if (hImageModel.Width() != hImageDst.Width() || hImageModel.Height() != hImageDst.Height())
			{
				AfxMessageBox(_T("ģ��ͼ�������ͼ���С��һ����"));
				return;
			}

			HTuple htNumLevels,htAngleStart,htAngleExtent,htAngleStep,htScaleMin,htScaleMax,htScaleStep,htMetric,htMinContrast;
			htNumLevels = hShapModel.GetShapeModelParams(&htAngleStart,&htAngleExtent,&htAngleStep
				,&htScaleMin,&htScaleMax,&htScaleStep,&htMetric,&htMinContrast);

			HTuple htRow,htColumn,htAngle,htScore,htScale;
			htRow = hShapModel.FindShapeModel(*pImage,htAngleStart,htAngleExtent,0.8,1,0,"least_squares",htNumLevels,0.5,&htColumn,&htAngle,&htScore);

			if (htScore.Num() == 1)
			{
				m_pHWindow->SetColor("green");
				m_pHWindow->DispCross(htRow[0].D() * dbZoomScale,htColumn[0].D() * dbZoomScale,6.0,htAngle);

				HTuple htHomMat2D;
				vector_angle_to_rigid(dbRowRef,dbColumnRef,dbAngle,htRow,htColumn,htAngle,&htHomMat2D);

				hImageModel = hImageModel.AffineTransImage(htHomMat2D,"constant","false");

				for (int i = 0; i < nAreaCount;i++)
				{
					strData.Format(_T("Area%d"),i+1);

					m_ProjectXml.ResetChildPos();
					if (m_ProjectXml.FindChildElem(strData))
					{	
						nMinGray = _ttoi(m_ProjectXml.GetChildAttrib(_T("MinGray")));
						nMaxGray = _ttoi(m_ProjectXml.GetChildAttrib(_T("MaxGray")));
						dbRow = _ttof(m_ProjectXml.GetChildAttrib(_T("Row")));
						dbColumn = _ttof(m_ProjectXml.GetChildAttrib(_T("Column")));
						dbPhi = _ttof(m_ProjectXml.GetChildAttrib(_T("Phi")));
						dbLen1 = _ttof(m_ProjectXml.GetChildAttrib(_T("Length1")));
						dbLen2 = _ttof(m_ProjectXml.GetChildAttrib(_T("Length2")));

						HRegion hRegionRect = HRegion::GenRectangle2(dbRow,dbColumn,dbPhi,dbLen1,dbLen2);

						hRegionRect = hRegionRect.AffineTransRegion(htHomMat2D,"false");

						HImage hReduceImage = hImageDst.ReduceDomain(hRegionRect);
						HImage hReduceModel = hImageModel.ReduceDomain(hRegionRect);

						HRegion light,dark;
						//HRegion hRegionDifferent = hReduceImage.CheckDifference(hReduceModel,"diff_outside",nMinGray,nMaxGray,nOffsetGray,0,0);
						light = CheckDifference(hReduceModel,hReduceImage,nMinGray,nMaxGray,&dark);

						if (light.Area() > 0)
						{
							HRegionArray hConnects = light.Connection();

							hConnects = hConnects.ZoomRegion(dbZoomScale,dbZoomScale);

							hLightRegions.Append(hConnects);
						}

						if (dark.Area() > 0)
						{
							HRegionArray hConnects = dark.Connection();

							hConnects = hConnects.ZoomRegion(dbZoomScale,dbZoomScale);

							hDarkRegions.Append(hConnects);
						}
						
					}
				}

				hDarkRegions = hDarkRegions.Connection();
				hLightRegions = hLightRegions.Connection();

				HTuple htRadius,htRow,htColumn;
				HRegionArray hRegions = hDarkRegions.Append(hLightRegions);

				//htRadius = hRegions.AreaCenter(&htRow,&htColumn);
				htRow = hRegions.SmallestCircle(&htColumn,&htRadius);

				if (htRadius.Num() == 0)
				{
					DisplayMessage(m_pHWindow,"PASS",20,12,12,"green",TRUE);
				}
				else
				{
					// �ж�ȱ�ݵȼ�
					HRegion hUnionRegion = hRegions.Union1();

					int nInspectLevel = 1,nActualLevel = 1;

					nActualLevel = AdjustDefectLevel(&m_ProjectXml,htRadius,hUnionRegion,dbZoomScale,dbPerPixel,&nInspectLevel);

					CString strText;
					BOOL bPass = FALSE;
					if (nActualLevel < nInspectLevel)
					{
						// PASS
						strText.Format(_T("PASS - Level%d"),nActualLevel);
						bPass = TRUE;
					}
					else if (nActualLevel > MAX_LEVEL)
					{
						strText.Format(_T("FAIL - Out of Level%d"),nActualLevel);	
					}
					else
					{
						strText.Format(_T("FAIL - Level%d"),nActualLevel);
					}

					USES_CONVERSION;
					char *text = W2A(strText);

					m_pHWindow->SetColor("blue");
					m_pHWindow->Display(hDarkRegions);

					m_pHWindow->SetColor("yellow");
					m_pHWindow->Display(hLightRegions);

					if (bPass)
					{
						DisplayMessage(m_pHWindow,text,20,12,12,"green",TRUE);
					}
					else
					{
						DisplayMessage(m_pHWindow,text,20,12,12,"red",TRUE);
					}
					
				}
			}
			else
			{
				DisplayMessage(m_pHWindow,"No find model!",20,12,12,"red",TRUE);
			}

			m_ProjectXml.OutOfElem();
		}

		m_ProjectXml.OutOfElem();
	}
}


void CCOXRayView::OnBtnView()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayView::OnUpdateSettingComm(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bIsLisence && !m_bPLCStarted);
}


void CCOXRayView::OnSettingComm()
{
	// TODO: �ڴ���������������
	CCommSettingDlg dlg;
	dlg.SetConfig(&m_Ini);

	if (dlg.DoModal() == IDOK)
	{
		BOOL bAutoConnect = m_Ini.GetBool(_T("CommSetting"),_T("En_AutoConnect"),TRUE);
		CString strIPAddr = m_Ini.GetString(_T("CommSetting"),_T("IPAddress"));
		UINT nPort = m_Ini.GetUInt(_T("CommSetting"),_T("PortNum"),8000);

		BeginWaitCursor();
		if (bAutoConnect)
		{
			ConnectPLC();
		}
		else
		{
			m_PLCommand.Disconnect();
		}
		EndWaitCursor();
	}
}

DWORD WINAPI CCOXRayView::ConnectPLCThreadProc( LPVOID lpParm )
{
	CCOXRayView *pView = (CCOXRayView *)lpParm;

	pView->ConnectPLC();

	return 1;
}

void CCOXRayView::ConnectPLC()
{
	// ����PLC
	CString strIPAddr = m_Ini.GetString(_T("CommSetting"),_T("IPAddress"));
	UINT nPort = m_Ini.GetUInt(_T("CommSetting"),_T("PortNum"),8000);
	BOOL bAutoConnect = m_Ini.GetBool(_T("CommSetting"),_T("En_AutoConnect"),TRUE);

	if (bAutoConnect)
	{
		USES_CONVERSION;
		char *ip = W2A(strIPAddr);

		if (!m_PLCommand.Connect(ip,nPort))
		{
			CString strMsg;
			strMsg.Format(_T("����PLC�����(%s:%d)ʧ��,%s"),strIPAddr,nPort
				,CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));

			m_pStatusBar->SetPaneText(0,strMsg);

			AfxMessageBox(strMsg);
		}
	}
}


afx_msg LRESULT CCOXRayView::OnSocketMsg(WPARAM wParam, LPARAM lParam)
{
	WORD event = WSAGETSELECTEVENT(lParam);
	WORD err = WSAGETSELECTERROR(lParam);
	CString strIpAddr = m_Ini.GetString(_T("CommSetting"),_T("IPAddress"));
	UINT nPort = m_Ini.GetUInt(_T("CommSetting"),_T("PortNum"),8000);

	int nIndex = m_pStatusBar->CommandToIndex(ID_INDICATOR_PLC_CONNECT);

	switch (event)
	{
	case FD_CLOSE:
		{

			if (m_bPLCStarted)
			{
				PostMessage(WM_COMMAND,IDC_BTN_STOP,0);
			}

			m_bPLCConnected = FALSE;
			m_PLCommand.Disconnect();

			m_pStatusBar->SetPaneText(nIndex,_T("PLC : NO "));

			CString strMsg;
			strMsg.Format(_T("PLC����(%s:%d)���ӶϿ�,%s"),strIpAddr,nPort
				,CUtils::GetErrorMsg(err));

			m_pStatusBar->SetPaneText(0,strMsg);

			AfxMessageBox(strMsg);
		}
		break;

	case FD_CONNECT:
		{
			if (err == 0)
			{
				m_bPLCConnected = TRUE;

				m_pStatusBar->SetPaneText(nIndex,_T("PLC : YES "));
			}
			else
			{
				m_bPLCConnected = FALSE;
				m_PLCommand.Disconnect();

				m_pStatusBar->SetPaneText(nIndex,_T("PLC : NO "));

				CString strMsg;
				strMsg.Format(_T("����PLC�����(%s:%d)ʧ��,%s"),strIpAddr,nPort
					,CUtils::GetErrorMsg(err));

				m_pStatusBar->SetPaneText(0,strMsg);

				AfxMessageBox(strMsg);
			}
		}		
		break;
	}
	return 0;
}


void CCOXRayView::OnBtnStart()
{
	// TODO: �ڴ���������������
	if (!m_bPLCConnected)
	{
		AfxMessageBox(_T("PLC�����δ����!"));
		return;
	}

	if (!m_bAcqConnected)
	{
		AfxMessageBox(_T("û�м�⵽���ʵ�ƽ��!"));
		return;
	}

	// PLC�ص���ʼλ��
	BeginWaitCursor();

	DWORD dwD2100 = CMD_PC_RUNNING | CMD_PLC_RESET;

	if (!m_PLCommand.WritePLCD(D2100,1,&dwD2100))
	{
		CString strMsg;
		if (m_PLCommand.m_ErrorType == Error_System)
		{
			strMsg.Format(_T("дPLCʧ�ܣ�%s"),CUtils::GetErrorMsg(m_PLCommand.m_dwLastError));
		}
		else
		{
			strMsg.Format(_T("дPLC���ش��󣬴������0x%04X"),m_PLCommand.m_dwLastError);
		}

		AfxMessageBox(strMsg);
	}
	else
	{
		m_pRightDialogBar->m_PageImgCapture.m_BtnPlcStart.EnableWindow(FALSE);
		m_pRightDialogBar->m_PageImgCapture.m_BtnPlcStop.EnableWindow(TRUE);
		m_pRightDialogBar->m_PageImgCapture.m_ComboBoxCtlMode.EnableWindow(FALSE);

		m_bPLCStarted = TRUE;

		SetTimer(TIMER_PLC,500,NULL);
		SetTimer(TIMER_PLC_LISTEN,500,NULL);

		m_nTimeOutTimes = 0;
	}

	EndWaitCursor();
}


void CCOXRayView::OnBtnStop()
{
	// TODO: �ڴ���������������

	KillTimer(TIMER_PLC);
	KillTimer(TIMER_PLC_LISTEN);

	m_nTimeOutTimes = 0;

	m_pRightDialogBar->m_PageImgCapture.m_BtnPlcStart.EnableWindow(TRUE);
	m_pRightDialogBar->m_PageImgCapture.m_BtnPlcStop.EnableWindow(FALSE);
	m_pRightDialogBar->m_PageImgCapture.m_ComboBoxCtlMode.EnableWindow(TRUE);

	m_bPLCStarted = FALSE;
}


void CCOXRayView::OnCheckOnOff()
{
	// TODO: �ڴ���������������
}


void CCOXRayView::OnCheckStat()
{
	// TODO: �ڴ���������������
}


void CCOXRayView::OnUpdateDeviceLight(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bIsLisence && !m_bContinueCapture && m_bStopSnap);

	if (m_LightControl.IsOpen())
	{
		pCmdUI->SetText(_T("�Ͽ���Դ"));
	}
	else
	{
		pCmdUI->SetText(_T("���ӹ�Դ"));
	}
}


void CCOXRayView::OnDeviceLight()
{
	// TODO: �ڴ���������������
	BeginWaitCursor();
	if (m_LightControl.IsOpen())
	{
		m_LightControl.Close();
		m_pRightDialogBar->m_LedStat.SetLedState(0);
	}
	else
	{
		if (!m_LightControl.EnumLight())
		{
			AfxMessageBox(_T("û���ҵ����ʵĹ�Դ��"));
		}
		else
		{
			m_pRightDialogBar->m_LedStat.SetLedState(1);
			m_pWatchDogThread = AfxBeginThread((AFX_THREADPROC)LightWatchDogThreadProc,this,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED);

			if (m_pWatchDogThread == NULL)
			{
				AfxMessageBox(_T("������Դ�����߳�ʧ�ܣ�"));
			}
			else
			{
				m_pWatchDogThread->m_bAutoDelete = FALSE;
				m_pWatchDogThread->ResumeThread();
			}
		}
	}
}


void CCOXRayView::OnUpdateSettingLight(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//pCmdUI->Enable(m_LightControl.IsOpen());
}


void CCOXRayView::OnSettingLight()
{
	// TODO: �ڴ���������������
	CLightSettingDlg dlg;
	dlg.SetConfig(&m_Ini);

	dlg.DoModal();
}

DWORD WINAPI CCOXRayView::LightWatchDogThreadProc( LPVOID lpParm )
{
	CCOXRayView *pView = (CCOXRayView *)lpParm;
	pView->LightWatchDog();

	return 1;
}

void CCOXRayView::LightWatchDog()
{

	while (m_LightControl.IsOpen())
	{
		Sleep(10);

		m_LightControl.IsWatchDogOK();
	
		if (!m_LightControl.IsWatchDogOK())
		{
// 			if (m_LightControl.IsLightOn())
// 			{
// 				m_LightControl.CloseLight();
// 			}
// 
// 			m_LightControl.Close();

			if (m_pRightDialogBar->m_LedStat.m_hWnd)
			{
				m_pRightDialogBar->m_LedStat.SetLedState(0);
			}

			if (m_pRightDialogBar->m_LedOnOff.m_hWnd)
			{
				m_pRightDialogBar->m_LedOnOff.SetLedState(0);
			}

// 			if (m_hWnd)
// 			{
// 				AfxMessageBox(_T("��Դ�Ͽ����ӣ����鲢�������ӣ�"));
// 			}
			
//			break;
		}
		else
		{
			m_pRightDialogBar->m_LedStat.SetLedState(1);
		}
		

		if (m_LightControl.IsLightOn())
		{
			if (m_pRightDialogBar->m_LedOnOff.m_hWnd)
			{
				m_pRightDialogBar->m_LedOnOff.SetLedState(1);
			}
			
		}
		else
		{
			if (m_pRightDialogBar->m_LedOnOff.m_hWnd)
			{
				m_pRightDialogBar->m_LedOnOff.SetLedState(0);
			}
		}

		m_LightControl.SelCommand();

		WORD wFaltBits = 0;
		if (m_LightControl.ReportFault(&wFaltBits))
		{
			if (wFaltBits != 0)
			{
				CString strError;

				if ((wFaltBits >> 0) & 0x01)
				{
					strError = _T("Regulation");
				}

				if ((wFaltBits >> 1) & 0x01)
				{
					strError = _T("Interlock Open");
				}

				if ((wFaltBits >> 2) & 0x01)
				{
					strError = _T("Cathode Over KV Fault");
				}

				if ((wFaltBits >> 3) & 0x01)
				{
					strError = _T("Anode Over KV Fault");
				}

				if ((wFaltBits >> 4) & 0x01)
				{
					strError = _T("Over Temperature Fault");
				}

				if ((wFaltBits >> 5) & 0x01)
				{
					strError = _T("Arc Fault");
				}

				if ((wFaltBits >> 6) & 0x01)
				{
					strError = _T("Over Current Fault");
				}

				if ((wFaltBits >> 7) & 0x01)
				{
					strError = _T("Power Limit Fault");
				}

				if ((wFaltBits >> 8) & 0x01)
				{
					strError = _T("Over Voltage Fault");
				}

				m_pRightDialogBar->SetLightError(strError);

				m_LightControl.FaultClear();
			}
			else
			{
				m_pRightDialogBar->SetLightError(_T(""));
			}
		}

		double dbVolKV = 0.0,dbCurmA = 0.0,dbTemperature = 0.0;
		int nFilament = 0;
		if (m_LightControl.Monitor(&dbVolKV,&dbCurmA,&dbTemperature,&nFilament))
		{
			if (m_pRightDialogBar->m_meterVol.m_hWnd)
			{
				m_pRightDialogBar->m_meterVol.UpdateNeedle(dbVolKV);
			}
			
			if (m_pRightDialogBar->m_meterCurrent.m_hWnd)
			{
				m_pRightDialogBar->m_meterCurrent.UpdateNeedle(dbCurmA);
			}
			
		}
		
	}

	m_pRightDialogBar->SetLightError(_T(""));
	
	if (m_pRightDialogBar->m_LedStat.m_hWnd)
	{
		m_pRightDialogBar->m_LedStat.SetLedState(0);
	}

	if (m_pRightDialogBar->m_LedOnOff.m_hWnd)
	{
		m_pRightDialogBar->m_LedOnOff.SetLedState(0);
	}
	
	if (m_pRightDialogBar->m_meterVol.m_hWnd)
	{
		m_pRightDialogBar->m_meterVol.UpdateNeedle(0.0);
	}

	if (m_pRightDialogBar->m_meterCurrent.m_hWnd)
	{
		m_pRightDialogBar->m_meterCurrent.UpdateNeedle(0.0);
	}
}


void CCOXRayView::OnSettingSave()
{
	// TODO: �ڴ���������������
	CSaveSettingDlg dlg;
	dlg.SetConfig(&m_Ini);

	dlg.DoModal();
}

void CCOXRayView::OnUpdateProjectNew(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CCOXRayView::OnProjectNew()
{
	// TODO: �ڴ���������������
	CProjectNewDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		m_bLoadProject = m_ProjectXml.Load(dlg.GetProjectFile());

		if (m_bLoadProject)
		{
			m_ProjectXml.ResetPos();
			if (m_ProjectXml.FindChildElem(_T("Project")))
			{
				m_ProjectXml.IntoElem();

				if (m_ProjectXml.FindChildElem(_T("InspectMode")))
				{
					m_ProjectXml.IntoElem();

					m_nInpectMode = _ttoi(m_ProjectXml.GetData());

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.OutOfElem();
			}
		}

		if (m_pRightDialogBar->m_PageFilterParm.IsWindowVisible())
		{
			m_pRightDialogBar->m_PageFilterParm.ShowWindow(SW_HIDE);
			m_pRightDialogBar->m_PageFilterParm.ShowWindow(SW_SHOW);
		}
	}
}





void CCOXRayView::OnUpdateProjectLoad(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CCOXRayView::OnProjectLoad()
{
	// TODO: �ڴ���������������
	CString strInitialDir = m_strAppPath + _T("\\project");
	CFileDialog dlg(TRUE,_T("xml"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Project Files(*.xml)|*.xml||"));
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if (dlg.DoModal() == IDOK)
	{
		m_bLoadProject = m_ProjectXml.Load(dlg.GetPathName());

		if (m_bLoadProject)
		{
			m_ProjectXml.ResetPos();
			if (m_ProjectXml.FindChildElem(_T("Project")))
			{
				m_ProjectXml.IntoElem();

				if (m_ProjectXml.FindChildElem(_T("InspectMode")))
				{
					m_ProjectXml.IntoElem();

					m_nInpectMode = _ttoi(m_ProjectXml.GetData());

					m_ProjectXml.OutOfElem();
				}

				m_ProjectXml.OutOfElem();
			}
		}

		if (m_pRightDialogBar->m_PageFilterParm.IsWindowVisible())
		{
			m_pRightDialogBar->m_PageFilterParm.ShowWindow(SW_HIDE);
			m_pRightDialogBar->m_PageFilterParm.ShowWindow(SW_SHOW);
		}
	}
}


void CCOXRayView::OnUpdateProjectEdit(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bLoadProject);
}


void CCOXRayView::OnProjectEdit()
{
	// TODO: �ڴ���������������
	CProjectEditDlg dlg;
	dlg.SetConfig(&m_ProjectXml);

	if (dlg.DoModal() == IDOK)
	{
		m_ProjectXml.ResetPos();
		if (m_ProjectXml.FindChildElem(_T("Project")))
		{
			m_ProjectXml.IntoElem();

			if (m_ProjectXml.FindChildElem(_T("InspectMode")))
			{
				m_ProjectXml.IntoElem();

				m_nInpectMode = _ttoi(m_ProjectXml.GetData());

				m_ProjectXml.OutOfElem();
			}

			m_ProjectXml.OutOfElem();
		}

		if (m_pRightDialogBar->m_PageFilterParm.IsWindowVisible())
		{
			m_pRightDialogBar->m_PageFilterParm.ShowWindow(SW_HIDE);
			m_pRightDialogBar->m_PageFilterParm.ShowWindow(SW_SHOW);
		}
	}

}


void CCOXRayView::OnBtnSnap()
{
	// TODO: �ڴ���������������
	if (!m_bAcqConnected)
	{
		AfxMessageBox(_T("û�м�⵽���ʵ�ƽ��!"));
		return;
	}

	if (!m_bStopSnap)
	{
		return;
	}

	m_pRightDialogBar->m_PageImgCapture.m_BtnStaticCap.EnableWindow(FALSE);
	m_pRightDialogBar->m_PageFilterParm.GetDlgItem(IDC_BTN_SNAP)->EnableWindow(FALSE);

	CCOXRayDoc *pDoc = GetDocument();

	pDoc->SetTitle(_T("ͼ��ɼ�"));

	m_pHWindow->ClearWindow();

	double dbVolKV = 0.0,dbCurmA = 0.0;
	dbVolKV = m_pRightDialogBar->m_PageFilterParm.m_dbEditVolKV;
	dbCurmA = m_pRightDialogBar->m_PageFilterParm.m_dbEditCurrentmA;

	if (m_LightControl.IsOpen())
	{
		m_pRightDialogBar->m_meterCurrent.UpdateNeedle(dbCurmA);
		m_pRightDialogBar->m_meterVol.UpdateNeedle(dbVolKV);

		m_LightControl.SetVoltage(dbVolKV);
		m_LightControl.SetCurrent(dbCurmA);
		m_LightControl.OpenLight();

		Sleep(100);
	}

	// ��ȡ�ɼ�֡��
	CString strFrame;
	m_pRightDialogBar->m_PageImgCapture.m_ComboBoxFrames.GetWindowText(strFrame);

	m_nCaptureFrames = _ttol(strFrame);

	m_Stopwatch.Start();

	UINT nRet = m_XISLFunction.InitalAcquisition(m_nCaptureFrames,ACQ_SNAP,HIS_SEQ_AVERAGE);

	if (nRet != HIS_ALL_OK)
	{
		AfxMessageBox(_T("��ʼ��ƽ��ɼ�ʧ��"));

		return;
	}

	m_bStopSnap = FALSE;
	m_bFilterParmSnap = TRUE;
	m_pStatusBar->SetPaneText(0,_T("���ھ�̬�ɼ�......"));

	Invalidate(TRUE);
}


void CCOXRayView::OnBtnThreshold()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();

	if (pImage == NULL)
	{
		return;
	}

	double dbZoomScale = pDoc->GetZoomFactor();

	HRegion hRegion = pImage->GetDomain();

	hRegion = hRegion.ZoomRegion(dbZoomScale,dbZoomScale);

	if (m_pHistoDlg == NULL)
	{
		m_pHistoDlg = new CHistogramDlg(this);
		m_pHistoDlg->Create(IDD_DIALOG_GRAY_HISTO,this);
	}

	m_pHistoDlg->SetRegion(hRegion);
	m_pHistoDlg->UpdateWindow();
	m_pHistoDlg->ShowWindow(SW_SHOW);
	
}


void CCOXRayView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		pCmdUI->Enable(FALSE);

		return;
	}

	HImage *pImage = pDoc->GetImage();

	pCmdUI->Enable(pImage != NULL);
	
}


void CCOXRayView::OnFileSave()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}
	CString strFileName = pDoc->GetPathName();
	pDoc->DoSave(strFileName);
}

void CCOXRayView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		pCmdUI->Enable(FALSE);

		return;
	}

	HImage *pImage = pDoc->GetImage();

	pCmdUI->Enable(pImage != NULL);
}

void CCOXRayView::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	CCOXRayDoc *pDoc = GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	pDoc->DoSave(NULL);
}


void CCOXRayView::OnUpdateSettingInspectLevel(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bLoadProject);
}


void CCOXRayView::OnSettingInspectLevel()
{
	// TODO: �ڴ���������������
	CInspectLevelDlg dlg;
	dlg.SetConfig(&m_ProjectXml);

	dlg.DoModal();
}


void CCOXRayView::OnUpdateFileDatabase(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bDatabaseConnected);
}


void CCOXRayView::OnFileDatabase()
{
	// TODO: �ڴ���������������
	CDatabaseDlg dlg;
	dlg.SetDatabase(&m_MyDatabase);

	dlg.DoModal();
}

int CCOXRayView::AdjustDefectLevel( CMarkup *pXml,HTuple htRadius,HRegion hRegion,double dbScale,double dbPerPixel,int *plevel)
{
	int level = 1;

	// ��ȡ�����Ӿ���
	HRectangle1 rect1 = hRegion.SmallestRectangle1();

	if (pXml->IsWellFormed())
	{
		pXml->ResetPos();
		if (pXml->FindChildElem(_T("Project")))
		{
			pXml->IntoElem();
			pXml->ResetChildPos();
			if (pXml->FindChildElem(_T("InspectLevel")))
			{
				// ʵ������
				int width = _ttoi(pXml->GetChildAttrib(_T("Width"))) * dbScale / dbPerPixel;
				int height = _ttoi(pXml->GetChildAttrib(_T("Height"))) * dbScale / dbPerPixel;
				*plevel = _ttoi(pXml->GetChildAttrib(_T("Level")));

				pXml->IntoElem();

				CString strLevel;
				double dbSingle[MAX_LEVEL] = {0.0};
				int nPercent[MAX_LEVEL] = {0};
				for (int i = 0; i < MAX_LEVEL;i++)
				{
					strLevel.Format(_T("Level%d"),i+1);

					pXml->ResetChildPos();
					if (pXml->FindChildElem(strLevel))
					{
						// ʵ������
						dbSingle[i] = _ttof(pXml->GetChildAttrib(_T("Single"))) * dbScale * dbScale / (dbPerPixel * dbPerPixel);
						nPercent[i] = _ttoi(pXml->GetChildAttrib(_T("Percent")));
					}
				}

				// ���жϵ���ȱ��
				
				double diameter = htRadius.Max()[0].D();

				for (int i = level; i < MAX_LEVEL;i++)
				{
					if (diameter > dbSingle[i])
					{
						level = i + 1;
					}
					else
					{
						break;
					}
				}
				

				if (level < MAX_LEVEL)
				{
					// ������ȱ����ռǧ�ֱ�
					// �ȷֳ����ɸ�С����
					int cols = (rect1.Width() + width - 1) / width;
					int rows = (rect1.Height() + height - 1) / height;

					for (int col = 0; col < cols;col++)
					{
						double dbRow1,dbRow2,dbCol1,dbCol2;
						dbCol1 = rect1.UpperLeft().X() + width * col;
						dbCol2 = dbCol1 + width;
						for (int row = 0;row < rows;row++)
						{
							dbRow1 = rect1.UpperLeft().Y() + height * row;
							dbRow2 = dbRow1 + height;
							HRegion region = HRegion::GenRectangle1(dbRow1,dbCol1,dbRow2,dbCol2);

							// ȡ����
							HRegion intersection = hRegion.Intersection(region);

							// ���㽻�������
							HTuple htArea = intersection.Area();

							// ������ռǧ�ֱ�
							int percent = htArea[0].D() * 1000 / (width * height);

							// �����ĸ��ȼ�
							for (int i = level; i < MAX_LEVEL;i++)
							{
								if (percent > nPercent[i])
								{
									level = i + 1;
								}
							}
						}
					}
				}

				pXml->OutOfElem();
			}
			pXml->OutOfElem();
		}
	}

	return level;
}


void CCOXRayView::OnUpdateThreshold(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CCOXRayDoc *pDoc = GetDocument();

	pCmdUI->Enable(pDoc != NULL || pDoc->GetImage() != NULL);
}


void CCOXRayView::OnThreshold()
{
	// TODO: �ڴ���������������
	OnBtnThreshold();
}


void CCOXRayView::OnUpdateProjectRecord(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_ProjectXml.IsWellFormed());
}


void CCOXRayView::OnProjectRecord()
{
	// TODO: �ڴ���������������
	CString strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentLocation);

	m_ProjectXml.ResetPos();
	if (m_ProjectXml.FindChildElem(_T("Project")))
	{
		m_ProjectXml.IntoElem();
		if (m_ProjectXml.FindChildElem(strPos))
		{
			m_ProjectXml.SetChildAttrib(_T("Record"),1);

			m_ProjectXml.IntoElem();
			if (!m_ProjectXml.FindChildElem(_T("ImageProcess")))
			{
				m_ProjectXml.AddChildElem(_T("ImageProcess"));
			}

			// ����ͼ����ڵ�
			m_ProjectXml.IntoElem();
			if (!m_ProjectXml.FindChildElem(_T("Gamma")))
			{
				m_ProjectXml.AddChildElem(_T("Gamma"));
			}

			double dbGamma = m_Ini.GetDouble(_T("ImageProcess"),_T("Gamma"),1.0);
			m_ProjectXml.SetChildAttrib(_T("Factor"),dbGamma);

			m_ProjectXml.ResetChildPos();
			if (!m_ProjectXml.FindChildElem(_T("Enhance")))
			{
				m_ProjectXml.AddChildElem(_T("Enhance"));
			}
			
			int maskWidth = m_Ini.GetInt(_T("Enhance"),_T("MaskWidth"),7);
			int maskHeight = m_Ini.GetInt(_T("Enhance"),_T("MaskHeight"),7);
			double dbFactor = m_Ini.GetDouble(_T("Enhance"),_T("Factor"),1.0);
			int times = m_Ini.GetInt(_T("Enhance"),_T("Times"),0);

			m_ProjectXml.SetChildAttrib(_T("MaskWidth"),maskWidth);
			m_ProjectXml.SetChildAttrib(_T("MaskHeight"),maskHeight);
			m_ProjectXml.SetChildAttrib(_T("Factor"),dbFactor);
			m_ProjectXml.SetChildAttrib(_T("Times"),times);

			m_ProjectXml.Save();

			m_ProjectXml.OutOfElem();

			m_ProjectXml.OutOfElem();
		}
		m_ProjectXml.OutOfElem();
	}
}


BOOL CCOXRayView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
// 	if (pMsg->message == WM_KEYDOWN)
// 	{
// 		if (pMsg->wParam == VK_RETURN)
// 		{
// 			if (m_bAcqConnected && m_bStopSnap)
// 			{
// 				PostMessage(WM_COMMAND,IDC_BTN_STATIC_CAP);
// 				return TRUE;
// 			}
// 		}
// 	}

	return CScrollView::PreTranslateMessage(pMsg);
}
