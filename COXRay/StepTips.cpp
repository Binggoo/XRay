// StepTips.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "StepTips.h"
#include "afxdialogex.h"


// CStepTips 对话框

IMPLEMENT_DYNAMIC(CStepTips, CPropertyPage)

CStepTips::CStepTips(BOOL bOffset,BOOL bFinished)
	: CPropertyPage(CStepTips::IDD)
{
	m_bOffset = bOffset;
	m_bFinish = bFinished;

	m_pXISLFunction = NULL;
	m_pLightControl = NULL;
	m_pIni = NULL;
}

CStepTips::~CStepTips()
{
}

void CStepTips::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStepTips, CPropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CStepTips 消息处理程序


BOOL CStepTips::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pXISLFunction && m_pLightControl && m_pIni);

	SetDlgItemText(IDC_TEXT_TIPS,m_strMsg);

	m_Font.CreateFont(20,
					  0,
					  0,
					  0,
					  FW_BOLD,
					  TRUE,
					  FALSE,
					  FALSE,
					  DEFAULT_CHARSET,
					  OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY,
					  DEFAULT_PITCH | FF_SWISS,
					  _T("Arial"));

	GetDlgItem(IDC_TEXT_TIPS)->SetFont(&m_Font);

	if (m_bOffset)
	{
		if (m_pLightControl->IsLightOn())
		{
			m_pLightControl->CloseLight();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CStepTips::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent=(CPropertySheet*)GetParent(); 


	if (m_bFinish)
	{
		pParent->SetWizardButtons(PSWIZB_FINISH|PSWIZB_BACK); 
	}
	else
	{
		pParent->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK); 
	}

	return CPropertyPage::OnSetActive();
}


BOOL CStepTips::OnWizardFinish()
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT nRet = 0;
	BeginWaitCursor();

	if (m_bOffset)
	{
		nRet = m_pXISLFunction->IntialOffset(100,HIS_SEQ_AVERAGE);
	}
	else
	{
		nRet = m_pXISLFunction->IntialGain(100,HIS_SEQ_AVERAGE);
	}

	if (nRet != HIS_ALL_OK)
	{
		AfxMessageBox(_T("平板采集图像失败!"));
		return FALSE;
	}

	EndWaitCursor();

	return CPropertyPage::OnWizardFinish();
}


LRESULT CStepTips::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	BeginWaitCursor();

	if (m_pLightControl->IsLightOn())
	{
		m_pLightControl->CloseLight();
	}

	UINT nRet = m_pXISLFunction->IntialOffset(100,HIS_SEQ_AVERAGE);

	if (nRet != HIS_ALL_OK)
	{
		AfxMessageBox(_T("平板采集图像失败!"));
		return FALSE;
	}

	double m_dbEditVol = m_pIni->GetDouble(_T("LightSetting"),_T("Voltage"),0.0);
	double m_dbEditCurrent = m_pIni->GetDouble(_T("LightSetting"),_T("Current"),0.0);

	m_pLightControl->SetVoltage(m_dbEditVol);
	m_pLightControl->SetCurrent(m_dbEditCurrent);

	m_pLightControl->OpenLight();

	EndWaitCursor();

	return CPropertyPage::OnWizardNext();
}


HBRUSH CStepTips::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_TEXT_TIPS)
	{
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
