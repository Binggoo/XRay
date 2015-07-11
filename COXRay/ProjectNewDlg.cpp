// ProjectNewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "ProjectNewDlg.h"
#include "afxdialogex.h"
#include "InspectLevelDlg.h"

// CProjectNewDlg 对话框

IMPLEMENT_DYNAMIC(CProjectNewDlg, CDialogEx)

CProjectNewDlg::CProjectNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectNewDlg::IDD, pParent)
	, m_strEditProjectNumber(_T(""))
	, m_strEditProductName(_T(""))
	, m_nEditLength(0)
	, m_nEditWidth(0)
	, m_nEditHeight(0)
{
	m_pXml = NULL;
}

CProjectNewDlg::~CProjectNewDlg()
{
	if (m_pXml)
	{
		delete m_pXml;
	}
}

void CProjectNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROJECT_NUMBER, m_strEditProjectNumber);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_strEditProductName);
	DDX_Text(pDX, IDC_EDIT_LENGHT, m_nEditLength);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nEditWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nEditHeight);
	DDX_Control(pDX, IDC_COMBO_CUSTOMER, m_ComboBoxCustomer);
	DDX_Control(pDX, IDC_COMBO_INSPECT_MODE, m_ComboBoxInspectMode);
	DDX_Control(pDX,IDC_COMBO_INSPECT_POS,m_ComboBoxInspectPos);
}


BEGIN_MESSAGE_MAP(CProjectNewDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LENGHT, &CProjectNewDlg::OnDeltaposSpinLenght)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTH, &CProjectNewDlg::OnDeltaposSpinWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HEIGHT, &CProjectNewDlg::OnDeltaposSpinHeight)
	ON_BN_CLICKED(IDOK, &CProjectNewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_INSPECT_LEVEL, &CProjectNewDlg::OnBnClickedBtnInspectLevel)
END_MESSAGE_MAP()


// CProjectNewDlg 消息处理程序


BOOL CProjectNewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strCustomerFile;
	strCustomerFile.Format(_T("%s\\customers.txt"),CUtils::GetFilePathWithoutName(CUtils::GetAppPath()));

	m_IniCustomer.SetPathName(strCustomerFile);

	CStringArray strArray;
	m_IniCustomer.GetSectionNames(&strArray);

	int nCount = strArray.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		m_ComboBoxCustomer.AddString(strArray.GetAt(i));
	}

	m_ComboBoxInspectMode.AddString(_T("手动"));
	m_ComboBoxInspectMode.AddString(_T("半自动"));
	m_ComboBoxInspectMode.AddString(_T("全自动"));
	m_ComboBoxInspectMode.SetCurSel(1);

	for (int i = 1; i <= MAX_POS; i++)
	{
		CString strValue;
		strValue.Format(_T("%d"),i);

		m_ComboBoxInspectPos.AddString(strValue);
	}

	m_ComboBoxInspectPos.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CProjectNewDlg::OnDeltaposSpinLenght(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (pNMUpDown->iDelta == -1) //向上
	{
		m_nEditLength++;
	}
	else if (pNMUpDown->iDelta == 1) //向下
	{
		if (m_nEditLength != 0)
		{
			m_nEditLength--;
		}
	}
	UpdateData(FALSE);

	*pResult = 0;
}


void CProjectNewDlg::OnDeltaposSpinWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (pNMUpDown->iDelta == -1) //向上
	{
		m_nEditWidth++;
	}
	else if (pNMUpDown->iDelta == 1) //向下
	{
		if (m_nEditWidth != 0)
		{
			m_nEditWidth--;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CProjectNewDlg::OnDeltaposSpinHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (pNMUpDown->iDelta == -1) //向上
	{
		m_nEditHeight++;
	}
	else if (pNMUpDown->iDelta == 1) //向下
	{
		if (m_nEditHeight != 0)
		{
			m_nEditHeight--;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CProjectNewDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strProjectDir = CUtils::GetFilePathWithoutName(CUtils::GetAppPath()) + _T("\\project");

	if (!PathFileExists(strProjectDir))
	{
		SHCreateDirectory(NULL,strProjectDir);
	}

	m_strProjectFile.Format(_T("%s\\%s.xml"),strProjectDir,m_strEditProjectNumber);
	
	CTime time = CTime::GetCurrentTime();

	CString strCustomerName;
	m_ComboBoxCustomer.GetWindowText(strCustomerName);

	CString strPosCount;
	m_ComboBoxInspectPos.GetWindowText(strPosCount);
	int nPosCount = _ttoi(strPosCount);

	m_IniCustomer.WriteString(strCustomerName,strCustomerName,NULL);
	m_IniCustomer.DeleteKey(strCustomerName,strCustomerName);

	if (m_pXml == NULL)
	{
		m_pXml = new CMarkup;
	}

	m_pXml->ResetPos();
	if (!m_pXml->FindElem(_T("COXRay")))
	{
		m_pXml->AddElem(_T("COXRay"));
	}

	if (!m_pXml->FindChildElem(_T("Project")))
	{
		m_pXml->AddChildElem(_T("Project"));
	}

	m_pXml->IntoElem();

	// 创建ProjectName节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("ProjectName")))
	{
		m_pXml->AddChildElem(_T("ProjectName"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(m_strEditProjectNumber);
		m_pXml->OutOfElem();
	}

	// 创建Time节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("Time")))
	{
		m_pXml->AddChildElem(_T("Time"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(time.Format(_T("%Y-%m-%d %H:%M:%S")));
		m_pXml->OutOfElem();
	}

	// 创建CustomerName节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("CustomerName")))
	{
		m_pXml->AddChildElem(_T("CustomerName"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(strCustomerName);
		m_pXml->OutOfElem();
	}

	// 创建ProductName节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("ProductName")))
	{
		m_pXml->AddChildElem(_T("ProductName"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(m_strEditProductName);
		m_pXml->OutOfElem();
	}

	// 创建ProductLengh节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("ProductLengh")))
	{
		m_pXml->AddChildElem(_T("ProductLengh"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(m_nEditLength);
		m_pXml->OutOfElem();
	}

	// 创建ProductLengh节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("ProductWidth")))
	{
		m_pXml->AddChildElem(_T("ProductWidth"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(m_nEditWidth);
		m_pXml->OutOfElem();
	}

	// 创建ProductHeight节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("ProductHeight")))
	{
		m_pXml->AddChildElem(_T("ProductHeight"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(m_nEditHeight);
		m_pXml->OutOfElem();
	}

	m_pXml->FindChildElem(_T("InspectLevel"));
	
	m_pXml->AddChildElem(_T("InspectMode"),m_ComboBoxInspectMode.GetCurSel());
	m_pXml->AddChildElem(_T("InspectPos"),strPosCount);

	for (int i = 0; i < nPosCount; i++)
	{
		strPosCount.Format(_T("POS%d"),i+1);

		m_pXml->AddChildElem(strPosCount,_T(""));
	}

	m_pXml->Save(m_strProjectFile);

	CDialogEx::OnOK();
}


void CProjectNewDlg::OnBnClickedBtnInspectLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pXml != NULL)
	{
		delete m_pXml;
	}

	m_pXml = new CMarkup;


	m_pXml->AddElem(_T("COXRay"));
	m_pXml->AddChildElem(_T("Project"));

	m_pXml->IntoElem();

	m_pXml->AddChildElem(_T("ProjectName"),m_strEditProjectNumber);
	m_pXml->AddChildElem(_T("Time"));
	m_pXml->AddChildElem(_T("CustomerName"));
	m_pXml->AddChildElem(_T("ProductName"),m_strEditProductName);
	m_pXml->AddChildElem(_T("ProductLengh"),m_nEditLength);
	m_pXml->AddChildElem(_T("ProductWidth"),m_nEditWidth);
	m_pXml->AddChildElem(_T("ProductHeight"),m_nEditHeight);

	m_pXml->OutOfElem();

	CInspectLevelDlg dlg;
	dlg.SetConfig(m_pXml);

	dlg.DoModal();
}
