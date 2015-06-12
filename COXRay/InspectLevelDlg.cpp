// InspectLevelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "InspectLevelDlg.h"
#include "afxdialogex.h"


// CInspectLevelDlg 对话框

IMPLEMENT_DYNAMIC(CInspectLevelDlg, CDialogEx)

CInspectLevelDlg::CInspectLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInspectLevelDlg::IDD, pParent)
	, m_bCheckLevel1(FALSE)
	, m_bCheckLevel2(FALSE)
	, m_bCheckLevel3(FALSE)
	, m_bCheckLevel4(FALSE)
	, m_bCheckLevel5(FALSE)
	, m_bCheckLevel6(FALSE)
	, m_bCheckLevel7(FALSE)
	, m_bCheckLevel8(FALSE)

	, m_dbEditSingleLevel1(1.0)
	, m_dbEditSingleLevel2(2.0)
	, m_dbEditSingleLevel3(3.0)
	, m_dbEditSingleLevel4(4.0)
	, m_dbEditSingleLevel5(5.0)
	, m_dbEditSingleLevel6(6.0)
	, m_dbEditSingleLevel7(7.0)
	, m_dbEditSingleLevel8(8.0)

	, m_nEditPercentLevel1(5)
	, m_nEditPercentLevel2(15)
	, m_nEditPercentLevel3(20)
	, m_nEditPercentLevel4(25)
	, m_nEditPercentLevel5(30)
	, m_nEditPercentLevel6(35)
	, m_nEditPercentLevel7(40)
	, m_nEditPercentLevel8(45)

	, m_nEditAreaWidth(20)
	, m_nEditAreaHeight(20)
{
	m_pXml = NULL;
}

CInspectLevelDlg::~CInspectLevelDlg()
{
}

void CInspectLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX,IDC_CHECK1,m_bCheckLevel1);
	DDX_Check(pDX,IDC_CHECK2,m_bCheckLevel2);
	DDX_Check(pDX,IDC_CHECK3,m_bCheckLevel3);
	DDX_Check(pDX,IDC_CHECK4,m_bCheckLevel4);
	DDX_Check(pDX,IDC_CHECK5,m_bCheckLevel5);
	DDX_Check(pDX,IDC_CHECK6,m_bCheckLevel6);
	DDX_Check(pDX,IDC_CHECK7,m_bCheckLevel7);
	DDX_Check(pDX,IDC_CHECK8,m_bCheckLevel8);

	DDX_Text(pDX, IDC_EDIT1, m_dbEditSingleLevel1);
	DDX_Text(pDX, IDC_EDIT2, m_dbEditSingleLevel2);
	DDX_Text(pDX, IDC_EDIT3, m_dbEditSingleLevel3);
	DDX_Text(pDX, IDC_EDIT4, m_dbEditSingleLevel4);
	DDX_Text(pDX, IDC_EDIT5, m_dbEditSingleLevel5);
	DDX_Text(pDX, IDC_EDIT6, m_dbEditSingleLevel6);
	DDX_Text(pDX, IDC_EDIT7, m_dbEditSingleLevel7);
	DDX_Text(pDX, IDC_EDIT8, m_dbEditSingleLevel8);

	DDX_Text(pDX, IDC_EDIT9, m_nEditPercentLevel1);
	DDX_Text(pDX, IDC_EDIT10, m_nEditPercentLevel2);
	DDX_Text(pDX, IDC_EDIT11, m_nEditPercentLevel3);
	DDX_Text(pDX, IDC_EDIT12, m_nEditPercentLevel4);
	DDX_Text(pDX, IDC_EDIT13, m_nEditPercentLevel5);
	DDX_Text(pDX, IDC_EDIT14, m_nEditPercentLevel6);
	DDX_Text(pDX, IDC_EDIT15, m_nEditPercentLevel7);
	DDX_Text(pDX, IDC_EDIT16, m_nEditPercentLevel8);

	DDX_Text(pDX, IDC_EDIT17, m_nEditAreaWidth);
	DDX_Text(pDX, IDC_EDIT18, m_nEditAreaHeight);

}


BEGIN_MESSAGE_MAP(CInspectLevelDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInspectLevelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CInspectLevelDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CInspectLevelDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CInspectLevelDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CInspectLevelDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CInspectLevelDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CInspectLevelDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CInspectLevelDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CInspectLevelDlg::OnBnClickedCheck8)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CInspectLevelDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CInspectLevelDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CInspectLevelDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CInspectLevelDlg::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CInspectLevelDlg::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CInspectLevelDlg::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CInspectLevelDlg::OnDeltaposSpin7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CInspectLevelDlg::OnDeltaposSpin8)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CInspectLevelDlg::OnDeltaposSpin9)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CInspectLevelDlg::OnDeltaposSpin10)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CInspectLevelDlg::OnDeltaposSpin11)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CInspectLevelDlg::OnDeltaposSpin12)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN13, &CInspectLevelDlg::OnDeltaposSpin13)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN14, &CInspectLevelDlg::OnDeltaposSpin14)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN15, &CInspectLevelDlg::OnDeltaposSpin15)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN16, &CInspectLevelDlg::OnDeltaposSpin16)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN17, &CInspectLevelDlg::OnDeltaposSpin17)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN18, &CInspectLevelDlg::OnDeltaposSpin18)
END_MESSAGE_MAP()


// CInspectLevelDlg 消息处理程序


BOOL CInspectLevelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pXml);

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetPos();
		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();
			if (m_pXml->FindChildElem(_T("InspectLevel")))
			{
				int level = _ttoi(m_pXml->GetChildAttrib(_T("Level")));
				m_nEditAreaWidth = _ttoi(m_pXml->GetChildAttrib(_T("Width")));
				m_nEditAreaHeight = _ttoi(m_pXml->GetChildAttrib(_T("Height")));

				m_pXml->IntoElem();

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level1")))
				{
					m_bCheckLevel1 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel1 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel1 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level2")))
				{
					m_bCheckLevel2 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel2 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel2 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level3")))
				{
					m_bCheckLevel3= _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel3 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel3 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level4")))
				{
					m_bCheckLevel4 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel4 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel4 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level5")))
				{
					m_bCheckLevel5 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel5 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel5 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level6")))
				{
					m_bCheckLevel6 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel6 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel6 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level7")))
				{
					m_bCheckLevel7 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel7 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel7 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(_T("Level8")))
				{
					m_bCheckLevel8 = _ttoi(m_pXml->GetChildAttrib(_T("Enable")));
					m_dbEditSingleLevel8 = _ttof(m_pXml->GetChildAttrib(_T("Single")));
					m_nEditPercentLevel8 = _ttoi(m_pXml->GetChildAttrib(_T("Percent")));
				}

				m_pXml->OutOfElem();
			}
			m_pXml->OutOfElem();
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInspectLevelDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int level = 0;
	if (m_bCheckLevel8)
	{
		level = 8;
	}
	else if (m_bCheckLevel7)
	{
		level = 7;
	}
	else if (m_bCheckLevel6)
	{
		level = 6;
	}
	else if (m_bCheckLevel5)
	{
		level = 5;
	}
	else if (m_bCheckLevel4)
	{
		level = 4;
	}
	else if (m_bCheckLevel3)
	{
		level = 3;
	}
	else if (m_bCheckLevel2)
	{
		level = 2;
	}
	else if (m_bCheckLevel1)
	{
		level = 1;
	}

	CString strData;

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetPos();
		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();
			if (!m_pXml->FindChildElem(_T("InspectLevel")))
			{
				m_pXml->AddChildElem(_T("InspectLevel"));
			}

			// 设置属性
			m_pXml->SetChildAttrib(_T("Level"),level);
			m_pXml->SetChildAttrib(_T("Width"),(int)m_nEditAreaWidth);
			m_pXml->SetChildAttrib(_T("Height"),(int)m_nEditAreaHeight);

			// 进入节点
			{
				m_pXml->IntoElem();

				// Level1
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level1")))
				{
					m_pXml->AddChildElem(_T("Level1"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel1);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel1);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel1);

				// Level2
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level2")))
				{
					m_pXml->AddChildElem(_T("Level2"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel2);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel2);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel2);

				// level3
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level3")))
				{
					m_pXml->AddChildElem(_T("Level3"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel3);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel3);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel3);

				// level4
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level4")))
				{
					m_pXml->AddChildElem(_T("Level4"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel4);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel4);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel4);

				// level5
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level5")))
				{
					m_pXml->AddChildElem(_T("Level5"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel5);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel5);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel5);

				// level6
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level6")))
				{
					m_pXml->AddChildElem(_T("Level6"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel6);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel6);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel6);

				// level7
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level7")))
				{
					m_pXml->AddChildElem(_T("Level7"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel7);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel7);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel7);

				// level8
				m_pXml->ResetChildPos();
				if (!m_pXml->FindChildElem(_T("Level8")))
				{
					m_pXml->AddChildElem(_T("Level8"));
				}

				m_pXml->SetChildAttrib(_T("Enable"),m_bCheckLevel8);
				m_pXml->SetChildAttrib(_T("Single"),m_dbEditSingleLevel8);
				m_pXml->SetChildAttrib(_T("Percent"),(int)m_nEditPercentLevel8);

				m_pXml->OutOfElem();
			}

			m_pXml->Save();
			m_pXml->OutOfElem();
		}
	}

	CDialogEx::OnOK();
}


void CInspectLevelDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (!m_bCheckLevel1)
	{
		m_bCheckLevel2 = m_bCheckLevel3 = m_bCheckLevel4 = m_bCheckLevel5 
			= m_bCheckLevel6 = m_bCheckLevel7 = m_bCheckLevel8 = FALSE;

		UpdateData(FALSE);
	}
}


void CInspectLevelDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel2)
	{
		m_bCheckLevel1 = TRUE;
	}
	else
	{
		m_bCheckLevel3 = m_bCheckLevel4 = m_bCheckLevel5 
			= m_bCheckLevel6 = m_bCheckLevel7 = m_bCheckLevel8 = FALSE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel3)
	{
		m_bCheckLevel1 = m_bCheckLevel2 = TRUE;
	}
	else
	{
		m_bCheckLevel4 = m_bCheckLevel5 
			= m_bCheckLevel6 = m_bCheckLevel7 = m_bCheckLevel8 = FALSE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel4)
	{
		m_bCheckLevel1 = m_bCheckLevel2 = m_bCheckLevel3 = TRUE;
	}
	else
	{
		m_bCheckLevel5 = m_bCheckLevel6 = m_bCheckLevel7 = m_bCheckLevel8 = FALSE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel5)
	{
		m_bCheckLevel1 = m_bCheckLevel2 = m_bCheckLevel3 = m_bCheckLevel4 = TRUE;
	}
	else
	{
		m_bCheckLevel6 = m_bCheckLevel7 = m_bCheckLevel8 = FALSE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel6)
	{
		m_bCheckLevel1 = m_bCheckLevel2 = m_bCheckLevel3 
			= m_bCheckLevel4 = m_bCheckLevel5 = TRUE;
	}
	else
	{
		m_bCheckLevel7 = m_bCheckLevel8 = FALSE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel7)
	{
		m_bCheckLevel1 = m_bCheckLevel2 = m_bCheckLevel3 
			= m_bCheckLevel4 = m_bCheckLevel5 = m_bCheckLevel6 = TRUE;
	}
	else
	{
		m_bCheckLevel8 = FALSE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckLevel8)
	{
		m_bCheckLevel1 = m_bCheckLevel2 = m_bCheckLevel3 
			= m_bCheckLevel4 = m_bCheckLevel5 = m_bCheckLevel6 = m_bCheckLevel7 = TRUE;
	}

	UpdateData(FALSE);
}


void CInspectLevelDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel1 += 0.1;

		if (m_dbEditSingleLevel1 > m_dbEditSingleLevel2)
		{
			m_dbEditSingleLevel1 = m_dbEditSingleLevel2;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel1 -= 0.1;

		if (m_dbEditSingleLevel1 < 0.0)
		{
			m_dbEditSingleLevel1 = 0.0;
		}
	}
	UpdateData(FALSE);

	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel2 += 0.1;

		if (m_dbEditSingleLevel2 > m_dbEditSingleLevel3)
		{
			m_dbEditSingleLevel2 = m_dbEditSingleLevel3;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel2 -= 0.1;

		if (m_dbEditSingleLevel2 < m_dbEditSingleLevel1)
		{
			m_dbEditSingleLevel2 = m_dbEditSingleLevel1;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel3 += 0.1;

		if (m_dbEditSingleLevel3 > m_dbEditSingleLevel4)
		{
			m_dbEditSingleLevel3 = m_dbEditSingleLevel4;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel3 -= 0.1;

		if (m_dbEditSingleLevel3 < m_dbEditSingleLevel2)
		{
			m_dbEditSingleLevel3 = m_dbEditSingleLevel2;
		}
	}
	UpdateData(FALSE);

	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel4 += 0.1;

		if (m_dbEditSingleLevel4 > m_dbEditSingleLevel5)
		{
			m_dbEditSingleLevel4 = m_dbEditSingleLevel5;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel4 -= 0.1;

		if (m_dbEditSingleLevel4 < m_dbEditSingleLevel3)
		{
			m_dbEditSingleLevel4 = m_dbEditSingleLevel3;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel5 += 0.1;

		if (m_dbEditSingleLevel5 > m_dbEditSingleLevel6)
		{
			m_dbEditSingleLevel5 = m_dbEditSingleLevel6;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel5 -= 0.1;

		if (m_dbEditSingleLevel5 < m_dbEditSingleLevel4)
		{
			m_dbEditSingleLevel5 = m_dbEditSingleLevel4;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel6 += 0.1;

		if (m_dbEditSingleLevel6 > m_dbEditSingleLevel7)
		{
			m_dbEditSingleLevel6 = m_dbEditSingleLevel7;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel6 -= 0.1;

		if (m_dbEditSingleLevel6 < m_dbEditSingleLevel5)
		{
			m_dbEditSingleLevel6 = m_dbEditSingleLevel5;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel7 += 0.1;

		if (m_dbEditSingleLevel7 > m_dbEditSingleLevel8)
		{
			m_dbEditSingleLevel7 = m_dbEditSingleLevel8;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel7 -= 0.1;

		if (m_dbEditSingleLevel7 < m_dbEditSingleLevel6)
		{
			m_dbEditSingleLevel7 = m_dbEditSingleLevel6;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dbEditSingleLevel8 += 0.1;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_dbEditSingleLevel8 -= 0.1;

		if (m_dbEditSingleLevel8 < m_dbEditSingleLevel7)
		{
			m_dbEditSingleLevel8 = m_dbEditSingleLevel7;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel1 += 1;

		if (m_nEditPercentLevel1 > m_nEditPercentLevel2)
		{
			m_nEditPercentLevel1 = m_nEditPercentLevel2;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel1 -= 1;

		if (m_nEditPercentLevel1 < 0)
		{
			m_nEditPercentLevel1 = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel2 += 1;

		if (m_nEditPercentLevel2 > m_nEditPercentLevel3)
		{
			m_nEditPercentLevel2 = m_nEditPercentLevel3;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel2 -= 1;

		if (m_nEditPercentLevel2 < m_nEditPercentLevel1)
		{
			m_nEditPercentLevel2 = m_nEditPercentLevel1;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin11(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel3 += 1;

		if (m_nEditPercentLevel3 > m_nEditPercentLevel4)
		{
			m_nEditPercentLevel3 = m_nEditPercentLevel4;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel3 -= 1;

		if (m_nEditPercentLevel3 < m_nEditPercentLevel2)
		{
			m_nEditPercentLevel3 = m_nEditPercentLevel2;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin12(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel4 += 1;

		if (m_nEditPercentLevel4 > m_nEditPercentLevel5)
		{
			m_nEditPercentLevel4 = m_nEditPercentLevel5;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel4 -= 1;

		if (m_nEditPercentLevel4 < m_nEditPercentLevel3)
		{
			m_nEditPercentLevel4 = m_nEditPercentLevel3;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin13(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel5 += 1;

		if (m_nEditPercentLevel5 > m_nEditPercentLevel6)
		{
			m_nEditPercentLevel5 = m_nEditPercentLevel6;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel5 -= 1;

		if (m_nEditPercentLevel5 < m_nEditPercentLevel4)
		{
			m_nEditPercentLevel5 = m_nEditPercentLevel4;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin14(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel6 += 1;

		if (m_nEditPercentLevel6 > m_nEditPercentLevel7)
		{
			m_nEditPercentLevel6 = m_nEditPercentLevel7;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel6 -= 1;

		if (m_nEditPercentLevel6 < m_nEditPercentLevel5)
		{
			m_nEditPercentLevel6 = m_nEditPercentLevel5;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin15(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel7 += 1;

		if (m_nEditPercentLevel7 > m_nEditPercentLevel8)
		{
			m_nEditPercentLevel7 = m_nEditPercentLevel8;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel7 -= 1;

		if (m_nEditPercentLevel7 < m_nEditPercentLevel6)
		{
			m_nEditPercentLevel7 = m_nEditPercentLevel6;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin16(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditPercentLevel8 += 1;

		if (m_nEditPercentLevel8 > 1000)
		{
			m_nEditPercentLevel8 = 1000;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditPercentLevel8 -= 1;

		if (m_nEditPercentLevel8 < m_nEditPercentLevel7)
		{
			m_nEditPercentLevel8 = m_nEditPercentLevel7;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin17(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditAreaWidth += 1;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditAreaWidth -= 1;

		if (m_nEditAreaWidth < 0)
		{
			m_nEditAreaWidth = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CInspectLevelDlg::OnDeltaposSpin18(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_nEditAreaHeight += 1;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_nEditAreaHeight -= 1;

		if (m_nEditAreaHeight < 0)
		{
			m_nEditAreaHeight = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}
