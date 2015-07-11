// SaveSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "SaveSettingDlg.h"
#include "afxdialogex.h"


// CSaveSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSaveSettingDlg, CDialogEx)

CSaveSettingDlg::CSaveSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSaveSettingDlg::IDD, pParent)
	, m_bCheckDate(TRUE)
	, m_bCheckVoltage(FALSE)
	, m_bCheckCurrent(FALSE)
	, m_bCheckCustomer(FALSE)
	, m_bCheckNoRemind(FALSE)
	, m_bCheckAutoSave(TRUE)
	, m_dbEditVoltage(0)
	, m_dbEditCurrent(0)
	, m_bCheckProductName(FALSE)
	, m_bCheckPos(FALSE)
	, m_bCheckSaveImgEdit(FALSE)
{
}

CSaveSettingDlg::~CSaveSettingDlg()
{
}

void CSaveSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_DATE, m_bCheckDate);
	DDX_Check(pDX, IDC_CHECK_VOLTAGE, m_bCheckVoltage);
	DDX_Check(pDX, IDC_CHECK_CURRENT, m_bCheckCurrent);
	DDX_Check(pDX, IDC_CHECK_CUSTOMER, m_bCheckCustomer);
	DDX_Check(pDX, IDC_CHECK_PRODUCT_NAME, m_bCheckProductName);
	DDX_Check(pDX, IDC_CHECK_POS, m_bCheckPos);
	DDX_Check(pDX,IDC_CHECK_NO_TIPS,m_bCheckNoRemind);
	DDX_Check(pDX,IDC_CHECK_AUTO_SAVE,m_bCheckAutoSave);
	DDX_Check(pDX,IDC_CHECK_SAVE_IMG_EDIT,m_bCheckSaveImgEdit);

	DDX_Text(pDX,IDC_EDIT_DATE,m_strEditDate);
	DDX_Text(pDX,IDC_EDIT_VOLTAGE,m_dbEditVoltage);
	DDX_Text(pDX,IDC_EDIT_CURRENT1,m_dbEditCurrent);
	DDX_Text(pDX,IDC_EDIT_CUSTOMER,m_strEditCustomer);
	DDX_Text(pDX,IDC_EDIT_PRODUCT_NAME,m_strEditProduct);
	DDX_Text(pDX,IDC_EDIT_POS,m_strEditPos);
}


BEGIN_MESSAGE_MAP(CSaveSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSaveSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSaveSettingDlg 消息处理程序


BOOL CSaveSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);

	m_bCheckDate = m_pIni->GetBool(_T("SaveSetting"),_T("En_Date"),TRUE);
	m_bCheckProductName = m_pIni->GetBool(_T("SaveSetting"),_T("En_Product"),FALSE);
	m_bCheckVoltage = m_pIni->GetBool(_T("SaveSetting"),_T("En_Voltage"),FALSE);
	m_bCheckCurrent = m_pIni->GetBool(_T("SaveSetting"),_T("En_Current"),FALSE);
	m_bCheckCustomer = m_pIni->GetBool(_T("SaveSetting"),_T("En_Customer"),FALSE);
	m_bCheckPos = m_pIni->GetBool(_T("SaveSetting"),_T("En_Pos"),FALSE);

	m_bCheckNoRemind = m_pIni->GetBool(_T("SaveSetting"),_T("En_NoRemind"),FALSE);
	m_bCheckAutoSave = m_pIni->GetBool(_T("SaveSetting"),_T("En_AutoSave"),TRUE);
	m_bCheckSaveImgEdit = m_pIni->GetBool(_T("SaveSetting"),_T("En_SaveImgEdit"),FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSaveSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_Product"),m_bCheckProductName);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_Customer"),m_bCheckCustomer);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_Pos"),m_bCheckPos);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_Voltage"),m_bCheckVoltage);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_Current"),m_bCheckCurrent);
	
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_NoRemind"),m_bCheckNoRemind);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_AutoSave"),m_bCheckAutoSave);
	m_pIni->WriteBool(_T("SaveSetting"),_T("En_SaveImgEdit"),m_bCheckSaveImgEdit);

	CDialogEx::OnOK();
}

void CSaveSettingDlg::SetImgInfo( IMG_INFO imgInfo )
{
	m_strEditProduct = imgInfo.strProductName;
	m_strEditCustomer = imgInfo.strCustomer;
	m_strEditPos = imgInfo.strPos;
	m_strEditDate = imgInfo.time.Format(_T("%Y%m%d_%H%M%S"));
	m_dbEditVoltage = imgInfo.dbVolKV;
	m_dbEditCurrent = imgInfo.dbCurrentMA;
}
