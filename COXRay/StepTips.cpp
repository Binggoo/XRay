// StepTips.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "StepTips.h"
#include "afxdialogex.h"


// CStepTips �Ի���

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


// CStepTips ��Ϣ�������


BOOL CStepTips::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CStepTips::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���
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
		AfxMessageBox(_T("ƽ��ɼ�ͼ��ʧ��!"));
		return FALSE;
	}

	EndWaitCursor();

	return CPropertyPage::OnWizardFinish();
}


LRESULT CStepTips::OnWizardNext()
{
	// TODO: �ڴ����ר�ô����/����û���
	BeginWaitCursor();

	if (m_pLightControl->IsLightOn())
	{
		m_pLightControl->CloseLight();
	}

	UINT nRet = m_pXISLFunction->IntialOffset(100,HIS_SEQ_AVERAGE);

	if (nRet != HIS_ALL_OK)
	{
		AfxMessageBox(_T("ƽ��ɼ�ͼ��ʧ��!"));
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

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_TEXT_TIPS)
	{
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
