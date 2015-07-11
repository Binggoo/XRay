// RightDialogBar.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "RightDialogBar.h"

#include "MainFrm.h"
#include "COXRayDoc.h"
#include "COXRayView.h"

#define LOGO_NAME _T("\\logo.png")

// CRightDialogBar 对话框

IMPLEMENT_DYNAMIC(CRightDialogBar, CDialogBar)

CRightDialogBar::CRightDialogBar(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CRightDialogBar::IDD, pParent*/)
{
	m_pXml = NULL;
	m_pIni = NULL;
}

CRightDialogBar::~CRightDialogBar()
{
}

void CRightDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ON_OFF, m_LedOnOff);
	DDX_Control(pDX, IDC_CHECK_STAT, m_LedStat);
	DDX_Control(pDX,IDC_METER_VOL,m_meterVol);
	DDX_Control(pDX,IDC_METER_CURRENT,m_meterCurrent);
	DDX_Control(pDX,IDC_TAB,m_tabMain);
}


BEGIN_MESSAGE_MAP(CRightDialogBar, CDialogBar)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CRightDialogBar::OnTcnSelchangeTab)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRightDialogBar 消息处理程序


BOOL CRightDialogBar::OnInitDialog()
{
	// TODO:  在此添加额外的初始化

	// DialogBar中必须先执行此句，使变量关联有效
	UpdateData(FALSE);

	// 设置LOG
	CString strPath = CUtils::GetAppPath();

	CString strLogoName = CUtils::GetFilePathWithoutName(strPath) + LOGO_NAME;

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PIC_LOGO); // 得到 Picture Control 句柄
	CRect rect;
	pWnd->GetClientRect(&rect);

	CImage image;
	image.Load(strLogoName);

	if (!image.IsNull())
	{
		HBITMAP hBmp = image.Detach();
		pWnd->SetBitmap(hBmp);
	}


	m_LedOnOff.SetIcons(IDI_LED_NONE,IDI_LED_RED);
	m_LedStat.SetIcons(IDI_LED_NONE,IDI_LED_GREEN);

	m_meterVol.SetNeedleColor(RGB(255, 255, 235));
	m_meterVol.SetColorTick(TRUE);
	m_meterVol.SetTicks(4);
	m_meterVol.SetSubTicks(4);
	m_meterVol.SetAngleRange(180,360);
	m_meterVol.SetRange(0,160);
	m_meterVol.SetUnits(_T("kV"));
	m_meterVol.SetValueDecimals(1);
	m_meterVol.UpdateNeedle(0.0);

	m_meterCurrent.SetNeedleColor(RGB(255, 255, 235));
	m_meterCurrent.SetColorTick(TRUE);
	m_meterCurrent.SetTicks(4);
	m_meterCurrent.SetSubTicks(2);
	m_meterCurrent.SetAngleRange(180,360);
	m_meterCurrent.SetRange(0,4);
	m_meterCurrent.SetUnits(_T("mA"));
	m_meterCurrent.SetValueDecimals(2);
	m_meterCurrent.UpdateNeedle(0.0);

	// Tab
	int nPage = 0;
	m_tabMain.InsertItem(nPage++,_T("图像采集"));
	m_tabMain.InsertItem(nPage++,_T("图像处理"));
	m_tabMain.InsertItem(nPage++,_T("检测参数"));

	m_PageImgCapture.Create(IDD_PAGE_CAPTURE,&m_tabMain);
	m_PageImgProcess.Create(IDD_PAGE_IMG_PROCESS,&m_tabMain);
//	m_PageData.Create(IDD_PAGE_DATA,&m_tabMain);
	m_PageFilterParm.Create(IDD_PAGE_FILTER_PARM,&m_tabMain);

	m_PageImgCapture.ShowWindow(SW_SHOW);
	m_PageImgProcess.ShowWindow(SW_HIDE);
//	m_PageData.ShowWindow(SW_HIDE);
	m_PageFilterParm.ShowWindow(SW_HIDE);
	m_nCurSelPage = 0;

	m_tabMain.SetCurSel(m_nCurSelPage);

	m_pPageDlg[0] = &m_PageImgCapture;
	m_pPageDlg[1] = &m_PageImgProcess;
//	m_pPageDlg[2] = &m_PageData;
	m_pPageDlg[2] = &m_PageFilterParm;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CRightDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRet = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);

	OnInitDialog();

	return bRet;
}

void CRightDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_MINIMIZED || cx == 0 || cy == 0)
	{
		return;
	}

	CWnd *pWnd = NULL;
	pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		DWORD dwFlag = SIZE_MOVE_X /*| SIZE_MOVE_Y*/;
		ChangeSize(pWnd,cx,cy,dwFlag);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	GetClientRect(&m_rect);

}


void CRightDialogBar::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_tabMain.GetCurSel() == 2 && !m_pXml->IsWellFormed())
	{
		m_tabMain.SetCurSel(m_nCurSelPage);
		*pResult = 0;
		return;
	}

	m_pPageDlg[m_nCurSelPage]->ShowWindow(SW_HIDE);
	m_nCurSelPage = m_tabMain.GetCurSel();
	m_pPageDlg[m_nCurSelPage]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CRightDialogBar::ChangeSize( CWnd *pWnd, int cx,int cy,DWORD flag )
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
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X && m_rect.Width() != 0)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// 改变Y坐标
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y && m_rect.Height() != 0)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//改变宽度
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X && m_rect.Width() != 0)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// 改变高度
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y && m_rect.Height() != 0)
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

void CRightDialogBar::SetConfig( CMarkup *pXml,CIni *pIni )
{
	m_pXml = pXml;
	m_pIni = pIni;
	m_PageFilterParm.SetConfig(pXml);
	m_PageImgCapture.SetConfig(pIni);
}

void CRightDialogBar::SetLightError( CString strError )
{
	if (m_hWnd)
	{
		SetDlgItemText(IDC_TEXT_LIGHT_ERROR,strError);
	}
}


HBRUSH CRightDialogBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_TEXT_LIGHT_ERROR)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,0,0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
