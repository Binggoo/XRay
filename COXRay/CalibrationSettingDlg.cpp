// CalibrationSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "CalibrationSettingDlg.h"
#include "afxdialogex.h"


// CCalibrationSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CCalibrationSettingDlg, CDialogEx)

CCalibrationSettingDlg::CCalibrationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibrationSettingDlg::IDD, pParent)
	, m_dbEditPixel(0)
	, m_dbEditPhysicalDistance(0)
{

}

CCalibrationSettingDlg::~CCalibrationSettingDlg()
{
}

void CCalibrationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PIXEL, m_dbEditPixel);
	DDX_Text(pDX, IDC_EDIT_PHYSICAL_DISTANCE, m_dbEditPhysicalDistance);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboBoxUnit);
}


BEGIN_MESSAGE_MAP(CCalibrationSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCalibrationSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCalibrationSettingDlg ��Ϣ�������


BOOL CCalibrationSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	for (int i = 0; i < UNIT_COUNT;i++)
	{
		m_ComboBoxUnit.AddString(Units[i]);
	}

	if (m_nSelectIndex < 0 || m_nSelectIndex > UNIT_COUNT - 1)
	{
		m_nSelectIndex = 0;
	}
	m_ComboBoxUnit.SetCurSel(m_nSelectIndex);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCalibrationSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_nSelectIndex = m_ComboBoxUnit.GetCurSel();

	CDialogEx::OnOK();
}

double CCalibrationSettingDlg::GetPerPixel()
{
	if (m_dbEditPixel > 0)
	{
		return m_dbEditPhysicalDistance / m_dbEditPixel;
	}
	else
	{
		return 0.0;
	}
}
