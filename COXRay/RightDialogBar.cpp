// RightDialogBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "RightDialogBar.h"

#include "MainFrm.h"
#include "COXRayDoc.h"
#include "COXRayView.h"

#define LOGO_NAME _T("\\logo.png")

// CRightDialogBar �Ի���

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


// CRightDialogBar ��Ϣ�������


BOOL CRightDialogBar::OnInitDialog()
{
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// DialogBar�б�����ִ�д˾䣬ʹ����������Ч
	UpdateData(FALSE);

	// ����LOG
	CString strPath = CUtils::GetAppPath();

	CString strLogoName = CUtils::GetFilePathWithoutName(strPath) + LOGO_NAME;

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PIC_LOGO); // �õ� Picture Control ���
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
	m_tabMain.InsertItem(nPage++,_T("ͼ��ɼ�"));
	m_tabMain.InsertItem(nPage++,_T("ͼ����"));
	m_tabMain.InsertItem(nPage++,_T("������"));

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
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CRightDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���
	BOOL bRet = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);

	OnInitDialog();

	return bRet;
}

void CRightDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	if(pWnd)  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����   
	{  
		CRect rectCtrl;   //��ȡ�ؼ��仯ǰ�Ĵ�С    
		pWnd->GetWindowRect(&rectCtrl);  
		ScreenToClient(&rectCtrl);//���ؼ���Сת��Ϊ�ڶԻ����е��������� 

		int iLeft = rectCtrl.left;
		int iTop = rectCtrl.top;
		int iWidth = rectCtrl.Width();
		int iHeight = rectCtrl.Height();

		// �ı�X����
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X && m_rect.Width() != 0)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// �ı�Y����
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y && m_rect.Height() != 0)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//�ı���
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X && m_rect.Width() != 0)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// �ı�߶�
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y && m_rect.Height() != 0)
		{
			iHeight = iHeight * cy / m_rect.Height();
		}

		//�ı���
		if ((flag & SIZE_ELASTIC_X_EX) == SIZE_ELASTIC_X_EX)
		{
			iWidth = cx - iLeft - 10;
		}

		// �ı�߶�
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

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_TEXT_LIGHT_ERROR)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,0,0));
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
