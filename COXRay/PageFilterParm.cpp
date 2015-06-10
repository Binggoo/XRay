// PageFilterParm.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageFilterParm.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "COXRayDoc.h"
#include "COXRayView.h"


// CPageFilterParm 对话框

IMPLEMENT_DYNAMIC(CPageFilterParm, CDialogEx)

CPageFilterParm::CPageFilterParm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageFilterParm::IDD, pParent)
	, m_dbEditVolKV(0.0)
	, m_dbEditCurrentmA(0.00)
{
	m_pXml = NULL;
	m_dwCurrentPos = 1;
	m_pHistoDlg = NULL;
	m_bLoacte = FALSE;
	m_nEditErrorValue = 0;
}

CPageFilterParm::~CPageFilterParm()
{
	if (m_pHistoDlg)
	{
		m_pHistoDlg->DestroyWindow();
		delete m_pHistoDlg;
	}
}

void CPageFilterParm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VOLTAGE, m_dbEditVolKV);
	DDX_Text(pDX, IDC_EDIT_CURRENT, m_dbEditCurrentmA);
	DDX_Control(pDX, IDC_LIST_AREAS, m_ListCtrlInpectAreas);
	DDX_Text(pDX,IDC_EDIT_ERROR,m_nEditErrorValue);
	DDV_MinMaxUInt(pDX, m_nEditErrorValue, 0, 255);
}


BEGIN_MESSAGE_MAP(CPageFilterParm, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE, &CPageFilterParm::OnDeltaposSpinVoltage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CURRENT, &CPageFilterParm::OnDeltaposSpinCurrent)
	ON_BN_CLICKED(IDC_BTN_LAST, &CPageFilterParm::OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CPageFilterParm::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_DELETE_AREA, &CPageFilterParm::OnBnClickedBtnDeleteArea)
	ON_BN_CLICKED(IDC_BTN_ADD_RANGE, &CPageFilterParm::OnBnClickedBtnAddRange)
	ON_BN_CLICKED(IDC_BTN_SET, &CPageFilterParm::OnBnClickedBtnSetLight)
	ON_BN_CLICKED(IDC_BTN_VIEW_AREA, &CPageFilterParm::OnBnClickedBtnViewArea)
	ON_BN_CLICKED(IDC_BTN_ADD_AREA, &CPageFilterParm::OnBnClickedBtnAddArea)
	ON_BN_CLICKED(IDC_BTN_INSPECT, &CPageFilterParm::OnBnClickedBtnInspect)
	ON_MESSAGE(WM_ADD_GRAY_RANGE, &CPageFilterParm::OnAddGrayRange)
	ON_BN_CLICKED(IDC_BTN_LOCATE, &CPageFilterParm::OnBnClickedBtnLocate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ERROR, &CPageFilterParm::OnDeltaposSpinError)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AREAS, &CPageFilterParm::OnNMClickListAreas)
	ON_BN_CLICKED(IDC_BTN_SNAP, &CPageFilterParm::OnBnClickedBtnSnap)
END_MESSAGE_MAP()


// CPageFilterParm 消息处理程序


BOOL CPageFilterParm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetClientRect(&m_rect);

	CRect rectClient;
	this->GetOwner()->GetClientRect(&rectClient);
	CTabCtrl *pTab = (CTabCtrl *)this->GetOwner();

	CRect rcHead;
	pTab->GetItemRect(0,&rcHead);
	rectClient.top += rcHead.Height() + 2;
	rectClient.left += 2;
	rectClient.right -= 2;
	rectClient.bottom -= 2;
	MoveWindow(rectClient);

	InitialListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageFilterParm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{
		UpdateListCtrl();
	}
}


BOOL CPageFilterParm::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CPageFilterParm::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_MINIMIZED)
	{
		return;
	}

	CWnd *pWnd = NULL;
	pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		DWORD dwFlag = SIZE_MOVE_X | SIZE_MOVE_Y;
		ChangeSize(pWnd,cx,cy,dwFlag);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	GetClientRect(&m_rect);
}

void CPageFilterParm::ChangeSize( CWnd *pWnd,int cx,int cy,DWORD flag )
{
	if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建   
	{  
		CRect rectCtrl;   //获取控件变化前的大小    
		pWnd->GetWindowRect(&rectCtrl);  
		ScreenToClient(&rectCtrl);//将控件大小转换为在对话框中的区域坐标 

		int iLeft = rectCtrl.left;
		int iTop = rectCtrl.top;
		int iWidth = rectCtrl.Width();
		int iHeight = rectCtrl.Height();

		// 改变X坐标
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// 改变Y坐标
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//改变宽度
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// 改变高度
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y)
		{
			iHeight = iHeight * cy / m_rect.Height();
		}

		//改变宽度
		if ((flag & SIZE_ELASTIC_X_EX) == SIZE_ELASTIC_X_EX)
		{
			iWidth = cx - iLeft - 10;
		}

		// 改变高度
		if ((flag & SIZE_ELASTIC_Y_EX) == SIZE_ELASTIC_Y_EX)
		{
			iHeight = cy - iTop - 10;
		}

		pWnd->MoveWindow(iLeft,iTop,iWidth,iHeight);

	}  
}

void CPageFilterParm::InitialListCtrl()
{
	m_ListCtrlInpectAreas.SetExtendedStyle(/*LVS_EX_CHECKBOXES | */LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	int nItem = 0;
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("Area"),LVCFMT_LEFT,80);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("MinGray"),LVCFMT_LEFT,80);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("MaxGray"),LVCFMT_LEFT,80);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("Row"),LVCFMT_LEFT,120);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("Column"),LVCFMT_LEFT,120);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("Phi"),LVCFMT_LEFT,120);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("Length1"),LVCFMT_LEFT,120);
	m_ListCtrlInpectAreas.InsertColumn(nItem++,_T("Length2"),LVCFMT_LEFT,120);
}

void CPageFilterParm::UpdateListCtrl()
{
	m_ListCtrlInpectAreas.DeleteAllItems();
	m_dbEditVolKV = m_dbEditCurrentmA = 0.0;

	CString strPos,strData;
	strPos.Format(_T("POS%d 检测区域滤波参数设置"),m_dwCurrentPos);
	SetDlgItemText(IDC_TEXT_POS,strPos);

	strPos.Format(_T("POS%d"),m_dwCurrentPos);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetMainPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			if (m_pXml->FindChildElem(_T("InspectPos")))
			{
				m_pXml->IntoElem();
				strData = m_pXml->GetData();

				int nPosCount = _ttoi(strData);

				GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_SHOW);
				if (nPosCount <= 1)
				{
					GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_HIDE);
				}
				else if (m_dwCurrentPos == 1)
				{
					GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_HIDE);
				}
				else if (m_dwCurrentPos == nPosCount)
				{
					GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_HIDE);
				}

				m_pXml->OutOfElem();
			}

			if (m_pXml->FindChildElem(strPos))
			{
				strData= m_pXml->GetChildAttrib(_T("Areas"));
				int nAreaCount = _ttoi(strData);

				strData = m_pXml->GetChildAttrib(_T("Voltage"));
				m_dbEditVolKV = _ttof(strData);

				strData = m_pXml->GetChildAttrib(_T("Current"));
				m_dbEditCurrentmA = _ttof(strData);

				m_pXml->IntoElem();

				for (int i = 0; i < nAreaCount;i++)
				{
					strData.Format(_T("Area%d"),i+1);

					if (m_pXml->FindChildElem(strData))
					{	
						m_ListCtrlInpectAreas.InsertItem(i,strData);

						m_ListCtrlInpectAreas.SetItemText(i,1,m_pXml->GetChildAttrib(_T("MinGray")));
						m_ListCtrlInpectAreas.SetItemText(i,2,m_pXml->GetChildAttrib(_T("MaxGray")));
						m_ListCtrlInpectAreas.SetItemText(i,3,m_pXml->GetChildAttrib(_T("Row")));
						m_ListCtrlInpectAreas.SetItemText(i,4,m_pXml->GetChildAttrib(_T("Column")));
						m_ListCtrlInpectAreas.SetItemText(i,5,m_pXml->GetChildAttrib(_T("Phi")));
						m_ListCtrlInpectAreas.SetItemText(i,6,m_pXml->GetChildAttrib(_T("Length1")));
						m_ListCtrlInpectAreas.SetItemText(i,7,m_pXml->GetChildAttrib(_T("Length2")));
					}
				}

				m_pXml->OutOfElem();
			}

			m_pXml->OutOfElem();
		}
		
	}

	UpdateData(FALSE);
}


void CPageFilterParm::OnDeltaposSpinVoltage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditVolKV += 0.1;

		if (m_dbEditVolKV > 160)
		{
			m_dbEditVolKV = 160;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditVolKV -= 0.1;
		
		if (m_dbEditVolKV < 0)
		{
			m_dbEditVolKV = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CPageFilterParm::OnDeltaposSpinCurrent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditCurrentmA += 0.01;

		if (m_dbEditCurrentmA > 3.12)
		{
			m_dbEditCurrentmA = 3.12;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditCurrentmA -= 0.01;

		if (m_dbEditCurrentmA < 0)
		{
			m_dbEditCurrentmA = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CPageFilterParm::OnBnClickedBtnLast()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dwCurrentPos--;

	UpdateListCtrl();

	Refresh();
}


void CPageFilterParm::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dwCurrentPos++;

	UpdateListCtrl();

	Refresh();
}


void CPageFilterParm::OnBnClickedBtnDeleteArea()
{
	// TODO: 在此添加控件通知处理程序代码
	// 从最后一个ARER删除

	int nCount = m_ListCtrlInpectAreas.GetItemCount();

	CString strPos,strArea;
	strPos.Format(_T("POS%d"),m_dwCurrentPos);
	strArea = m_ListCtrlInpectAreas.GetItemText(nCount-1,0);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetMainPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			if (m_pXml->FindChildElem(strPos))
			{
				m_pXml->IntoElem();

				if (m_pXml->FindChildElem(strArea))
				{
					m_pXml->RemoveChildElem();

					m_ListCtrlInpectAreas.DeleteItem(nCount-1);

					m_pXml->Save();
				}

				m_pXml->OutOfElem();
			}

			m_pXml->OutOfElem();
		}
	}

	Refresh();
}


void CPageFilterParm::OnBnClickedBtnAddRange()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	CCOXRayDoc *pDoc = pView->GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	HWindow *pHWindow = pView->GetHWindow();
	if (pHWindow == NULL)
	{
		return;
	}

	m_bLoacte = FALSE;

	POSITION pos = m_ListCtrlInpectAreas.GetFirstSelectedItemPosition();

	int nItem = m_ListCtrlInpectAreas.GetNextSelectedItem(pos);
	double dbRow = 0.0,dbColumn = 0.0,dbPhi = 0.0,dbLen1 = 0.0,dbLen2 = 0.0;

	dbRow = _ttof(m_ListCtrlInpectAreas.GetItemText(nItem,3)) * pDoc->GetZoomFactor();
	dbColumn = _ttof(m_ListCtrlInpectAreas.GetItemText(nItem,4)) * pDoc->GetZoomFactor();
	dbPhi = _ttof(m_ListCtrlInpectAreas.GetItemText(nItem,5));
	dbLen1 = _ttof(m_ListCtrlInpectAreas.GetItemText(nItem,6)) * pDoc->GetZoomFactor();
	dbLen2 = _ttof(m_ListCtrlInpectAreas.GetItemText(nItem,7)) * pDoc->GetZoomFactor();

	HRegion hRegion = HRegion::GenRectangle2(dbRow,dbColumn,dbPhi,dbLen1,dbLen2);

	if (m_pHistoDlg == NULL)
	{
		m_pHistoDlg = new CHistogramDlg(this);
		m_pHistoDlg->Create(IDD_DIALOG_GRAY_HISTO,this);
	}

	m_pHistoDlg->SetRegion(hRegion);
	m_pHistoDlg->UpdateWindow();
	m_pHistoDlg->ShowWindow(SW_SHOW);
	*/
	UpdateData(TRUE);

	POSITION pos = m_ListCtrlInpectAreas.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		return;
	}

	int nItem = m_ListCtrlInpectAreas.GetNextSelectedItem(pos);
	CString strMinGray,strMaxGray;
	strMinGray.Format(_T("-%d"),m_nEditErrorValue);
	strMaxGray.Format(_T("%d"),m_nEditErrorValue);

	m_ListCtrlInpectAreas.SetItemText(nItem,1,strMinGray);
	m_ListCtrlInpectAreas.SetItemText(nItem,2,strMaxGray);

	CString strPos,strArea;
	strPos.Format(_T("POS%d"),m_dwCurrentPos);
	strArea = m_ListCtrlInpectAreas.GetItemText(nItem,0);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetMainPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			if (!m_pXml->FindChildElem(strPos))
			{
				m_pXml->AddChildElem(strPos);
			}

			m_pXml->IntoElem();

			if (m_pXml->FindChildElem(strArea))
			{
				m_pXml->SetChildAttrib(_T("MinGray"),strMinGray);
				m_pXml->SetChildAttrib(_T("MaxGray"),strMaxGray);

				m_pXml->Save();
			}

			m_pXml->OutOfElem();

			m_pXml->OutOfElem();
		}
	}
}


void CPageFilterParm::OnBnClickedBtnSetLight()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentPos);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetMainPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();
			
			if (!m_pXml->FindChildElem(strPos))
			{
				m_pXml->AddChildElem(strPos);
			}

			CString strVolKV,strCurrentmA;
			strVolKV.Format(_T("%.1f"),m_dbEditVolKV);
			strCurrentmA.Format(_T("%.2f"),m_dbEditCurrentmA);

			m_pXml->SetChildAttrib(_T("Voltage"),strVolKV);
			m_pXml->SetChildAttrib(_T("Current"),strCurrentmA);

			m_pXml->Save();

			m_pXml->OutOfElem();
		}
	}
}


void CPageFilterParm::OnBnClickedBtnViewArea()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	CCOXRayDoc *pDoc = pView->GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	HWindow *pHWindow = pView->GetHWindow();
	if (pHWindow == NULL)
	{
		return;
	}

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	pHWindow->SetDraw("margin");
	pHWindow->SetLineWidth(1);
	pHWindow->SetColor("green");

	double dbZoomScale = pDoc->GetZoomFactor();

	double dbRow = 0.0,dbColumn = 0.0,dbPhi = 0.0,dbLen1 = 0.0,dbLen2 = 0.0,dbRowTrans = 0.0,dbColumnTrans = 0.0;

	m_pXml->ResetMainPos();

	CString strData,strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentPos);
	if (m_pXml->FindChildElem(_T("Project")))
	{
		m_pXml->IntoElem();

		m_pXml->ResetChildPos();

		if (m_pXml->FindChildElem(strPos))
		{
			strData= m_pXml->GetChildAttrib(_T("Areas"));
			int nAreaCount = _ttoi(strData);

			m_pXml->IntoElem();

			m_pXml->ResetChildPos();

			// 读Model 文件
			HShapeModel hShapModel;
			if (m_pXml->FindChildElem(_T("Shm")))
			{
				m_pXml->IntoElem();

				CString strShmFile = m_pXml->GetData();

				USES_CONVERSION;
				char *file = W2A(strShmFile);

				hShapModel.ReadShapeModel(file);

				m_pXml->OutOfElem();
			}

			if (!hShapModel.IsHandleValid())
			{
				AfxMessageBox(_T("读模板文件出错！"));

				return;
			}


			// 找定位点
			double dbRowRef = 0.0,dbColumnRef = 0.0,dbOrientation = 0.0;

			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("RowRef")))
			{
				m_pXml->IntoElem();

				dbRowRef = _ttof(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ColumnRef")))
			{
				m_pXml->IntoElem();

				dbColumnRef = _ttof(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("Orientation")))
			{
				m_pXml->IntoElem();

				dbOrientation = _ttof(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			HTuple htNumLevels,htAngleStart,htAngleExtent,htAngleStep,htScaleMin,htScaleMax,htScaleStep,htMetric,htMinContrast;
			htNumLevels = hShapModel.GetShapeModelParams(&htAngleStart,&htAngleExtent,&htAngleStep
				,&htScaleMin,&htScaleMax,&htScaleStep,&htMetric,&htMinContrast);

			HTuple htRow,htColumn,htAngle,htScore,htScale;
			htRow = hShapModel.FindShapeModel(*pImage,htAngleStart,htAngleExtent,0.8,1,0,"least_squares",htNumLevels,0.5,&htColumn,&htAngle,&htScore);

			DisplayShapMatchingResult(pHWindow,hShapModel,"green",htRow,htColumn,htAngle,1,1,dbZoomScale);

			if (htScore.Num() == 1)
			{
				HTuple HomMat2D;
				vector_angle_to_rigid(dbRowRef,dbColumnRef,0,htRow,htColumn,htAngle,&HomMat2D);

				for (int i = 0; i < nAreaCount;i++)
				{
					strData.Format(_T("Area%d"),i+1);

					m_pXml->ResetChildPos();
					if (m_pXml->FindChildElem(strData))
					{	
						dbRow = _ttof(m_pXml->GetChildAttrib(_T("Row")));
						dbColumn = _ttof(m_pXml->GetChildAttrib(_T("Column")));
						dbPhi = _ttof(m_pXml->GetChildAttrib(_T("Phi")));
						dbLen1 = _ttof(m_pXml->GetChildAttrib(_T("Length1")));
						dbLen2 = _ttof(m_pXml->GetChildAttrib(_T("Length2")));

						HRegion hRect = HRegion::GenRectangle2(dbRow,dbColumn,dbPhi,dbLen1,dbLen2);

						hRect = hRect.AffineTransRegion(HomMat2D,"false");

						hRect = hRect.ZoomRegion(dbZoomScale,dbZoomScale);

						pHWindow->Display(hRect);

					}
				}

			}
			else
			{
				DisplayMessage(pHWindow,"No find model!",20,12,12,"red",TRUE);
			}

			m_pXml->OutOfElem();
		}

		m_pXml->OutOfElem();
	}
}


void CPageFilterParm::OnBnClickedBtnAddArea()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	CCOXRayDoc *pDoc = pView->GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	HWindow *pHWindow = pView->GetHWindow();
	if (pHWindow == NULL)
	{
		return;
	}

	CStatusBar *pStatusBar = pFrame->GetStatusBar();

	CString strText = _T("按住鼠标左键开始，点鼠标右键完成。");

	pStatusBar->SetPaneText(0,strText);

	m_bLoacte = FALSE;

	pHWindow->SetDraw("margin");
	pHWindow->SetLineWidth(1);
	pHWindow->SetColor("green");

	int nType = pView->GetOptionType();
	pView->SetOptionType(OP_RECT);

	HTuple htRow,htColumn,htPhi,htLen1,htLen2;
	
	try
	{
		htRow = pHWindow->DrawRectangle2(&htColumn,&htPhi,&htLen1,&htLen2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("请先点鼠标左键开始，点鼠标右键完成。"));

		AfxMessageBox(strErrorMsg);

		pView->SetOptionType(nType);
		return;
	}

	pHWindow->DispRectangle2(htRow,htColumn,htPhi,htLen1,htLen2);

	HRegion hRegion = HRegion::GenRectangle2(htRow,htColumn,htPhi,htLen1,htLen2);

	if (m_pHistoDlg == NULL)
	{
		m_pHistoDlg = new CHistogramDlg(this);
		m_pHistoDlg->Create(IDD_DIALOG_GRAY_HISTO,this);
	}

	m_pHistoDlg->SetRegion(hRegion);
	m_pHistoDlg->UpdateWindow();
	m_pHistoDlg->ShowWindow(SW_SHOW);

	int nCount = m_ListCtrlInpectAreas.GetItemCount();

	CString strArea,strPos,strData;
	strArea.Format(_T("Area%d"),nCount+1);
	m_ListCtrlInpectAreas.InsertItem(nCount,strArea);

	strPos.Format(_T("POS%d"),m_dwCurrentPos);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetMainPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			if (!m_pXml->FindChildElem(strPos))
			{
				m_pXml->AddChildElem(strPos);
			}

			m_pXml->SetChildAttrib(_T("Areas"),nCount+1);

			// 进入POS_X
			m_pXml->IntoElem();

			m_pXml->AddChildElem(strArea);
			m_pXml->SetChildAttrib(_T("MinGray"),0);
			m_pXml->SetChildAttrib(_T("MaxGray"),0);

			strData.Format(_T("%.f"),htRow[0].D() / pDoc->GetZoomFactor());
			m_ListCtrlInpectAreas.SetItemText(nCount,3,strData);
			m_pXml->SetChildAttrib(_T("Row"),strData);

			strData.Format(_T("%f"),htColumn[0].D() / pDoc->GetZoomFactor());
			m_ListCtrlInpectAreas.SetItemText(nCount,4,strData);
			m_pXml->SetChildAttrib(_T("Column"),strData);

			strData.Format(_T("%f"),htPhi[0].D());
			m_ListCtrlInpectAreas.SetItemText(nCount,5,strData);
			m_pXml->SetChildAttrib(_T("Phi"),strData);

			strData.Format(_T("%f"),htLen1[0].D() / pDoc->GetZoomFactor());
			m_ListCtrlInpectAreas.SetItemText(nCount,6,strData);
			m_pXml->SetChildAttrib(_T("Length1"),strData);

			strData.Format(_T("%f"),htLen2[0].D() / pDoc->GetZoomFactor());
			strData.Format(_T("%f"),htLen2[0].D() / pDoc->GetZoomFactor());
			m_pXml->SetChildAttrib(_T("Length2"),strData);

			m_ListCtrlInpectAreas.SetItemState(nCount,  LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	

			m_pXml->Save();

			// 离开POS_X
			m_pXml->OutOfElem();

			m_pXml->OutOfElem();
		}
	}

	pView->SetOptionType(nType);
}

void CPageFilterParm::OnBnClickedBtnInspect()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	CCOXRayDoc *pDoc = pView->GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	HWindow *pHWindow = pView->GetHWindow();
	if (pHWindow == NULL)
	{
		return;
	}

	/*
	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	pHWindow->SetDraw("margin");
	pHWindow->SetLineWidth(1);
	pHWindow->SetColor("green");

	int nCount = m_ListCtrlInpectAreas.GetItemCount();
	double dbRow = 0.0,dbColumn = 0.0,dbPhi = 0.0,dbLen1 = 0.0,dbLen2 = 0.0,dbMinGray = 0.0,dbMaxGray = 0.0;
	HRegionArray hRegions;
	double dbZommFactor = pDoc->GetZoomFactor();
	HImage hScaleImage = pImage->ZoomImageFactor(dbZommFactor,dbZommFactor,"constant");
	for (int i = 0; i < nCount; i++)
	{
		dbMinGray = _ttof(m_ListCtrlInpectAreas.GetItemText(i,1));
		dbMaxGray = _ttof(m_ListCtrlInpectAreas.GetItemText(i,2));
		dbRow = _ttof(m_ListCtrlInpectAreas.GetItemText(i,3)) * pDoc->GetZoomFactor();
		dbColumn = _ttof(m_ListCtrlInpectAreas.GetItemText(i,4)) * pDoc->GetZoomFactor();
		dbPhi = _ttof(m_ListCtrlInpectAreas.GetItemText(i,5));
		dbLen1 = _ttof(m_ListCtrlInpectAreas.GetItemText(i,6)) * pDoc->GetZoomFactor();
		dbLen2 = _ttof(m_ListCtrlInpectAreas.GetItemText(i,7)) * pDoc->GetZoomFactor();

		HRegion hRegion = HRegion::GenRectangle2(dbRow,dbColumn,dbPhi,dbLen1,dbLen2);

		HImage hReduceImage = hScaleImage.ReduceDomain(hRegion);

		HRegion hRegionThreshold = hReduceImage.Threshold(dbMinGray,dbMaxGray);
		HRegionArray hConnects = hRegionThreshold.Connection();

		hRegions.Append(hConnects);
	}

	pHWindow->Display(hRegions);
	*/

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	pHWindow->SetDraw("margin");
	pHWindow->SetLineWidth(1);

	double dbZoomScale = pDoc->GetZoomFactor();

	//16 位转换为8位
	HImage hImageDst = pImage->CopyImage();

	if (GetImageBits(hImageDst) == 16)
	{
		hImageDst = ConvertImage(hImageDst,IPL_DEPTH_8U,3);
	}

	double dbRow = 0.0,dbColumn = 0.0,dbPhi = 0.0,dbLen1 = 0.0,dbLen2 = 0.0;
	int nMinGray = 0,nMaxGray = 0,nOffsetGray = 0;
	HRegionArray hRegions;

	m_pXml->ResetMainPos();

	CString strData,strPos;
	strPos.Format(_T("POS%d"),m_dwCurrentPos);
	if (m_pXml->FindChildElem(_T("Project")))
	{
		m_pXml->IntoElem();

		m_pXml->ResetChildPos();

		if (m_pXml->FindChildElem(strPos))
		{
			strData= m_pXml->GetChildAttrib(_T("Areas"));
			int nAreaCount = _ttoi(strData);

			m_pXml->IntoElem();

			m_pXml->ResetChildPos();

			// 读Model 文件
			HShapeModel hShapModel;
			HImage hImageModel;
			if (m_pXml->FindChildElem(_T("Shm")))
			{
				m_pXml->IntoElem();

				CString strShmFile = m_pXml->GetData();

				USES_CONVERSION;
				char *file = W2A(strShmFile);

				hShapModel.ReadShapeModel(file);

				m_pXml->OutOfElem();
			}

			if (!hShapModel.IsHandleValid())
			{
				AfxMessageBox(_T("读模板文件出错！"));

				return;
			}

			m_pXml->ResetChildPos();

			if (m_pXml->FindChildElem(_T("Model")))
			{
				m_pXml->IntoElem();

				CString strModelFile = m_pXml->GetData();

				USES_CONVERSION;
				char *file = W2A(strModelFile);

				hImageModel = HImage::ReadImage(file);

				if (GetImageBits(hImageModel) == 16)
				{
					hImageModel = ConvertImage(hImageModel,IPL_DEPTH_8U,3);
				}

				m_pXml->OutOfElem();
			}

			// 找定位点
			double dbRowRef = 0.0,dbColumnRef = 0.0;

			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("RowRef")))
			{
				m_pXml->IntoElem();

				dbRowRef = _ttof(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ColumnRef")))
			{
				m_pXml->IntoElem();

				dbColumnRef = _ttof(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			HTuple htNumLevels,htAngleStart,htAngleExtent,htAngleStep,htScaleMin,htScaleMax,htScaleStep,htMetric,htMinContrast;
			htNumLevels = hShapModel.GetShapeModelParams(&htAngleStart,&htAngleExtent,&htAngleStep
				,&htScaleMin,&htScaleMax,&htScaleStep,&htMetric,&htMinContrast);

			HTuple htRow,htColumn,htAngle,htScore,htScale;
			htRow = hShapModel.FindShapeModel(*pImage,htAngleStart,htAngleExtent,0.8,1,0,"least_squares",htNumLevels,0.5,&htColumn,&htAngle,&htScore);

			if (htScore.Num() == 1)
			{
				pHWindow->SetColor("green");
				pHWindow->DispCross(htRow[0].D() * dbZoomScale,htColumn[0].D() * dbZoomScale,10.0,htAngle);

				HTuple htHomMat2D;
				vector_angle_to_rigid(dbRowRef,dbColumnRef,0,htRow,htColumn,htAngle,&htHomMat2D);

				hImageModel = hImageModel.AffineTransImage(htHomMat2D,"constant","false");

				for (int i = 0; i < nAreaCount;i++)
				{
					strData.Format(_T("Area%d"),i+1);

					m_pXml->ResetChildPos();
					if (m_pXml->FindChildElem(strData))
					{	
						nMinGray = _ttoi(m_pXml->GetChildAttrib(_T("MinGray")));
						nMaxGray = _ttoi(m_pXml->GetChildAttrib(_T("MaxGray")));
						dbRow = _ttof(m_pXml->GetChildAttrib(_T("Row")));
						dbColumn = _ttof(m_pXml->GetChildAttrib(_T("Column")));
						dbPhi = _ttof(m_pXml->GetChildAttrib(_T("Phi")));
						dbLen1 = _ttof(m_pXml->GetChildAttrib(_T("Length1")));
						dbLen2 = _ttof(m_pXml->GetChildAttrib(_T("Length2")));

						HRegion hRegionRect = HRegion::GenRectangle2(dbRow,dbColumn,dbPhi,dbLen1,dbLen2);

						hRegionRect = hRegionRect.AffineTransRegion(htHomMat2D,"false");

						HImage hReduceImage = hImageDst.ReduceDomain(hRegionRect);
						HImage hReduceModel = hImageModel.ReduceDomain(hRegionRect);
						

						HRegion hRegionDifferent = hReduceImage.CheckDifference(hReduceModel,"diff_outside",nMinGray,nMaxGray,nOffsetGray,0,0);

						if (hRegionDifferent.Area() > 0)
						{
							HRegionArray hConnects = hRegionDifferent.Connection();

							hConnects = hConnects.ZoomRegion(dbZoomScale,dbZoomScale);

							hRegions.Append(hConnects);
						}

					}
				}

				hRegions = hRegions.Connection();
				HTuple htArea,htRow,htColumn;

				htArea = hRegions.AreaCenter(&htRow,&htColumn);

				if (htArea.Num() == 0)
				{
					DisplayMessage(pHWindow,"PASS",20,12,12,"green",TRUE);
				}
				else
				{
					pHWindow->SetColor("red");
					pHWindow->Display(hRegions);
				}

			}
			else
			{
				DisplayMessage(pHWindow,"No find model!",20,12,12,"red",TRUE);
			}

			m_pXml->OutOfElem();
		}

		m_pXml->OutOfElem();
	}
}

void CPageFilterParm::Refresh()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	CCOXRayDoc *pDoc = pView->GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

afx_msg LRESULT CPageFilterParm::OnAddGrayRange(WPARAM wParam, LPARAM lParam)
{
	if (m_bLoacte)
	{
		m_bLoacte = FALSE;

		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		if (pFrame == NULL)
		{
			return 1;
		}

		CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
		if (pView == NULL)
		{
			return 1;
		}
		CCOXRayDoc *pDoc = pView->GetDocument();
		if (pDoc == NULL)
		{
			return 1;
		}

		HImage *pImage = pDoc->GetImage();
		if (pImage == NULL)
		{
			return 1;
		}

		HWindow *pHWindow = pView->GetHWindow();
		if (pHWindow == NULL)
		{
			return 1;
		}

		//16 位转换为8位
		HImage hImageDst = pImage->CopyImage();

// 		if (GetImageBits(hImageDst) == 16)
// 		{
// 			hImageDst = ConvertImage(hImageDst,IPL_DEPTH_8U,3);
// 		}

		double dbZoomScale = 1 / pDoc->GetZoomFactor();

		HRegion htRegion = m_hRegionLoacte.ZoomRegion(dbZoomScale,dbZoomScale);
		// 定位
		HImage hReduceImage = hImageDst.ReduceDomain(htRegion);

		htRegion = hReduceImage.Threshold(wParam,lParam);

		htRegion = htRegion.Connection();

		double dbRowRef,dbColumnRef,dbOrientation;
		htRegion.AreaCenter(&dbRowRef,&dbColumnRef);
		dbOrientation = htRegion.OrientationRegion();

		hReduceImage = hReduceImage.ReduceDomain(htRegion);

		HShapeModel hShapModel = hReduceImage.CreateShapeModel("auto",-ANGEL,2*ANGEL,"auto","none","use_polarity","auto","auto");

		// 保存定位
		CString strFilePath = CUtils::GetFilePathWithoutName(CUtils::GetAppPath()) + _T("\\shm");

		if (!PathFileExists(strFilePath))
		{
			SHCreateDirectory(NULL,strFilePath);
		}

		CString strShmFile,strProjectName,strPos,strModelFile;

		strPos.Format(_T("POS%d"),m_dwCurrentPos);

		if (m_pXml->IsWellFormed())
		{
			m_pXml->ResetMainPos();

			if (m_pXml->FindChildElem(_T("Project")))
			{
				m_pXml->IntoElem();

				if (m_pXml->FindChildElem(_T("ProjectName")))
				{
					m_pXml->IntoElem();
					strProjectName = m_pXml->GetData();
					m_pXml->OutOfElem();

					strShmFile.Format(_T("%s\\%s_%s.shm"),strFilePath,strProjectName,strPos);
					strModelFile.Format(_T("%s\\%s_%s.png"),strFilePath,strProjectName,strPos);

					USES_CONVERSION;
					char *file = W2A(strShmFile);
					char *image = W2A(strModelFile);
					hShapModel.WriteShapeModel(file);

					file = W2A(strModelFile);
					hImageDst.WriteImage("png",0,image);

					if (m_pXml->FindChildElem(strPos))
					{
						m_pXml->IntoElem();

						m_pXml->ResetChildPos();
						if (m_pXml->FindChildElem(_T("Shm")))
						{
							m_pXml->IntoElem();
							m_pXml->SetData(strShmFile);
							m_pXml->OutOfElem();
						}
						else
						{
							m_pXml->AddChildElem(_T("Shm"),strShmFile);
						}
						
						m_pXml->ResetChildPos();
						if (m_pXml->FindChildElem(_T("Model")))
						{
							m_pXml->IntoElem();
							m_pXml->SetData(strModelFile);
							m_pXml->OutOfElem();
						}
						else
						{
							m_pXml->AddChildElem(_T("Model"),strModelFile);
						}

						CString strData;
						strData.Format(_T("%.f"),dbRowRef);

						m_pXml->ResetChildPos();
						if (m_pXml->FindChildElem(_T("RowRef")))
						{
							m_pXml->IntoElem();
							m_pXml->SetData(strData);
							m_pXml->OutOfElem();
						}
						else
						{
							m_pXml->AddChildElem(_T("RowRef"),strData);
						}

						strData.Format(_T("%.f"),dbColumnRef);
						if (m_pXml->FindChildElem(_T("ColumnRef")))
						{
							m_pXml->IntoElem();
							m_pXml->SetData(strData);
							m_pXml->OutOfElem();
						}
						else
						{
							m_pXml->AddChildElem(_T("ColumnRef"),strData);
						}

						strData.Format(_T("%.f"),dbOrientation);
						if (m_pXml->FindChildElem(_T("Orientation")))
						{
							m_pXml->IntoElem();
							m_pXml->SetData(strData);
							m_pXml->OutOfElem();
						}
						else
						{
							m_pXml->AddChildElem(_T("Orientation"),strData);
						}
						
						m_pXml->Save();

						m_pXml->OutOfElem();
					}
				}

				m_pXml->OutOfElem();
			}
		}
		
	}

	return 0;

	/*
	POSITION pos = m_ListCtrlInpectAreas.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		AfxMessageBox(_T("请选中列表中的某一行！"));
		return 1;
	}

	int nItem = m_ListCtrlInpectAreas.GetNextSelectedItem(pos);

	CString strMinGray,strMaxGray;
	strMinGray.Format(_T("%d"),wParam);
	strMaxGray.Format(_T("%d"),lParam);

	m_ListCtrlInpectAreas.SetItemText(nItem,1,strMinGray);
	m_ListCtrlInpectAreas.SetItemText(nItem,2,strMaxGray);

	CString strPos,strArea;
	strPos.Format(_T("POS%d"),m_dwCurrentPos);
	strArea = m_ListCtrlInpectAreas.GetItemText(nItem,0);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetMainPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			if (!m_pXml->FindChildElem(strPos))
			{
				m_pXml->AddChildElem(strPos);
			}

			m_pXml->IntoElem();

			if (m_pXml->FindChildElem(strArea))
			{
				m_pXml->SetChildAttrib(_T("MinGray"),strMinGray);
				m_pXml->SetChildAttrib(_T("MaxGray"),strMaxGray);

				m_pXml->Save();
			}

			m_pXml->OutOfElem();

			m_pXml->OutOfElem();
		}
	}
	return 0;
	*/
}


void CPageFilterParm::OnBnClickedBtnLocate()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	CCOXRayDoc *pDoc = pView->GetDocument();
	if (pDoc == NULL)
	{
		return;
	}

	HImage *pImage = pDoc->GetImage();
	if (pImage == NULL)
	{
		return;
	}

	HWindow *pHWindow = pView->GetHWindow();
	if (pHWindow == NULL)
	{
		return;
	}

	pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	CStatusBar *pStatusBar = pFrame->GetStatusBar();

	CString strText = _T("按住鼠标左键开始，点鼠标右键完成。");

	pStatusBar->SetPaneText(0,strText);

	m_bLoacte = TRUE;

	pHWindow->SetDraw("margin");
	pHWindow->SetLineWidth(1);
	pHWindow->SetColor("green");

	int nType = pView->GetOptionType();
	pView->SetOptionType(OP_RECT);

	HTuple htRow1,htColumn1,htRow2,htColumn2;
	
	try
	{
		htRow1 = pHWindow->DrawRectangle1(&htColumn1,&htRow2,&htColumn2);
	}
	catch (HException &except)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("请先点鼠标左键开始，点鼠标右键完成。"));

		AfxMessageBox(strErrorMsg);

		pView->SetOptionType(nType);
		return;
	}

	pHWindow->DispRectangle1(htRow1,htColumn1,htRow2,htColumn2);

	m_hRegionLoacte = HRegion::GenRectangle1(htRow1,htColumn1,htRow2,htColumn2);

	if (m_pHistoDlg == NULL)
	{
		m_pHistoDlg = new CHistogramDlg(this);
		m_pHistoDlg->Create(IDD_DIALOG_GRAY_HISTO,this);
	}

	m_pHistoDlg->SetRegion(m_hRegionLoacte);
	m_pHistoDlg->UpdateWindow();
	m_pHistoDlg->ShowWindow(SW_SHOW);

	pView->SetOptionType(nType);
}


void CPageFilterParm::OnDeltaposSpinError(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditErrorValue++;

		if (m_nEditErrorValue > 256)
		{
			m_nEditErrorValue = 255;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditErrorValue--;

		if (m_nEditErrorValue < 1)
		{
			m_nEditErrorValue = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CPageFilterParm::OnNMClickListAreas(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		if (pFrame == NULL)
		{
			*pResult = 0;
			return;
		}

		CCOXRayView *pView = (CCOXRayView *)pFrame->GetActiveView();
		if (pView == NULL)
		{
			*pResult = 0;
			return;
		}
		CCOXRayDoc *pDoc = pView->GetDocument();
		if (pDoc == NULL)
		{
			*pResult = 0;
			return;
		}

		HImage *pImage = pDoc->GetImage();
		if (pImage == NULL)
		{
			*pResult = 0;
			return;
		}

		HWindow *pHWindow = pView->GetHWindow();
		if (pHWindow == NULL)
		{
			*pResult = 0;
			return;
		}

		pDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);

		pHWindow->SetDraw("margin");
		pHWindow->SetLineWidth(1);
		pHWindow->SetColor("green");

		double dbZoomScale = pDoc->GetZoomFactor();

		double dbRow = 0.0,dbColumn = 0.0,dbPhi = 0.0,dbLen1 = 0.0,dbLen2 = 0.0,dbRowTrans = 0.0,dbColumnTrans = 0.0;

		m_pXml->ResetMainPos();

		CString strData,strPos;
		strPos.Format(_T("POS%d"),m_dwCurrentPos);
		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			m_pXml->ResetChildPos();

			if (m_pXml->FindChildElem(strPos))
			{
				strData= m_pXml->GetChildAttrib(_T("Areas"));
				int nAreaCount = _ttoi(strData);

				m_pXml->IntoElem();

				m_pXml->ResetChildPos();

				// 读Model 文件
				HShapeModel hShapModel;
				if (m_pXml->FindChildElem(_T("Shm")))
				{
					m_pXml->IntoElem();

					CString strShmFile = m_pXml->GetData();

					USES_CONVERSION;
					char *file = W2A(strShmFile);

					hShapModel.ReadShapeModel(file);

					m_pXml->OutOfElem();
				}

				if (!hShapModel.IsHandleValid())
				{
					AfxMessageBox(_T("读模板文件出错！"));

					return;
				}


				// 找定位点
				double dbRowRef = 0.0,dbColumnRef = 0.0,dbOrientation = 0.0;

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("RowRef")))
				{
					m_pXml->IntoElem();

					dbRowRef = _ttof(m_pXml->GetData());

					m_pXml->OutOfElem();
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("ColumnRef")))
				{
					m_pXml->IntoElem();

					dbColumnRef = _ttof(m_pXml->GetData());

					m_pXml->OutOfElem();
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Orientation")))
				{
					m_pXml->IntoElem();

					dbOrientation = _ttof(m_pXml->GetData());

					m_pXml->OutOfElem();
				}

				HTuple htNumLevels,htAngleStart,htAngleExtent,htAngleStep,htScaleMin,htScaleMax,htScaleStep,htMetric,htMinContrast;
				htNumLevels = hShapModel.GetShapeModelParams(&htAngleStart,&htAngleExtent,&htAngleStep
					,&htScaleMin,&htScaleMax,&htScaleStep,&htMetric,&htMinContrast);

				HTuple htRow,htColumn,htAngle,htScore,htScale;
				htRow = hShapModel.FindShapeModel(*pImage,htAngleStart,htAngleExtent,0.8,1,0,"least_squares",htNumLevels,0.5,&htColumn,&htAngle,&htScore);

				if (htScore.Num() == 1)
				{
					pHWindow->SetColor("green");
					pHWindow->DispCross(htRow[0].D() * dbZoomScale,htColumn[0].D() * dbZoomScale,10.0,htAngle);

					HTuple HomMat2D;
					vector_angle_to_rigid(dbRowRef,dbColumnRef,0,htRow,htColumn,htAngle,&HomMat2D);

					//for (int i = 0; i < nAreaCount;i++)
					{
						strData.Format(_T("Area%d"),nItem+1);

						m_pXml->ResetChildPos();
						if (m_pXml->FindChildElem(strData))
						{	
							dbRow = _ttof(m_pXml->GetChildAttrib(_T("Row")));
							dbColumn = _ttof(m_pXml->GetChildAttrib(_T("Column")));
							dbPhi = _ttof(m_pXml->GetChildAttrib(_T("Phi")));
							dbLen1 = _ttof(m_pXml->GetChildAttrib(_T("Length1")));
							dbLen2 = _ttof(m_pXml->GetChildAttrib(_T("Length2")));

							HRegion hRect = HRegion::GenRectangle2(dbRow,dbColumn,dbPhi,dbLen1,dbLen2);

							hRect = hRect.AffineTransRegion(HomMat2D,"false");

							hRect = hRect.ZoomRegion(dbZoomScale,dbZoomScale);

							pHWindow->Display(hRect);

						}
					}

				}
				else
				{
					DisplayMessage(pHWindow,"No find model!",20,12,12,"red",TRUE);
				}

				m_pXml->OutOfElem();
			}

			m_pXml->OutOfElem();
		}
	}

	*pResult = 0;
}


void CPageFilterParm::OnBnClickedBtnSnap()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_COMMAND,IDC_BTN_SNAP,0);
}
