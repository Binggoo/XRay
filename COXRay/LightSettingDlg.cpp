// LightSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "LightSettingDlg.h"
#include "afxdialogex.h"


// CLightSettingDlg 对话框

IMPLEMENT_DYNAMIC(CLightSettingDlg, CDialogEx)

CLightSettingDlg::CLightSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightSettingDlg::IDD, pParent)
	, m_dbEditVol(0)
	, m_dbEditCurrent(0.0)
{
	m_pIni = NULL;
}

CLightSettingDlg::~CLightSettingDlg()
{
}

void CLightSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_VOL, m_SliderCtrlVol);
	DDX_Control(pDX, IDC_SLIDER_CURRENT, m_SliderCtrlCur);
	DDX_Text(pDX, IDC_EDIT_VOL, m_dbEditVol);
	//DDV_MinMaxDouble(pDX, m_dbEditVol, 0, 160);
	DDX_Text(pDX, IDC_EDIT_CURRENT, m_dbEditCurrent);
	//DDV_MinMaxDouble(pDX, m_dbEditCurrent, 0.2, 3.12);
}


BEGIN_MESSAGE_MAP(CLightSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLightSettingDlg::OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_VOL, &CLightSettingDlg::OnEnChangeEditVol)
	ON_EN_CHANGE(IDC_EDIT_CURRENT, &CLightSettingDlg::OnEnChangeEditCurrent)
END_MESSAGE_MAP()


// CLightSettingDlg 消息处理程序


BOOL CLightSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni != NULL);
	m_dbEditVol = m_pIni->GetDouble(_T("LightSetting"),_T("Voltage"),0.0);
	m_dbEditCurrent = m_pIni->GetDouble(_T("LightSetting"),_T("Current"),0.0);

	m_SliderCtrlVol.SetRange(0,1600);
	m_SliderCtrlVol.SetTicFreq(200);
	m_SliderCtrlVol.SetPos((int)m_dbEditVol * 10);

	m_SliderCtrlCur.SetRange(20,312);
	m_SliderCtrlCur.SetTicFreq(50);
	m_SliderCtrlCur.SetPos((int)m_dbEditCurrent * 100);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLightSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pIni->WriteDouble(_T("LightSetting"),_T("Voltage"),m_dbEditVol);
	m_pIni->WriteDouble(_T("LightSetting"),_T("Current"),m_dbEditCurrent);

	CDialogEx::OnOK();
}


void CLightSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar; 
	CString strText; 
	int dValue[10] = {0}; 

	//多个 SLIDER 控件控制 
	switch( pSlider->GetDlgCtrlID() ) 
	{ 
	case IDC_SLIDER_VOL:
		m_dbEditVol = pSlider->GetPos() / 10.0;
		UpdateData(FALSE);
		break;

	case IDC_SLIDER_CURRENT:
		m_dbEditCurrent = pSlider->GetPos() / 100.0;
		UpdateData(FALSE);

		break;
	} 

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CLightSettingDlg::OnEnChangeEditVol()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_SliderCtrlVol.SetPos((int)(m_dbEditVol * 10));
}


void CLightSettingDlg::OnEnChangeEditCurrent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_SliderCtrlCur.SetPos((int)(m_dbEditCurrent * 100));
}
