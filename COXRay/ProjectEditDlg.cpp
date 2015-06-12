// ProjectEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "ProjectEditDlg.h"
#include "afxdialogex.h"
#include "InspectLevelDlg.h"

// CProjectEditDlg 对话框

IMPLEMENT_DYNAMIC(CProjectEditDlg, CDialogEx)

CProjectEditDlg::CProjectEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectEditDlg::IDD, pParent)
	, m_strEditProjectNumber(_T(""))
	, m_strEditProductName(_T(""))
	, m_nEditLength(0)
	, m_nEditWidth(0)
	, m_nEditHeight(0)
{
	m_pXml = NULL;
	m_nTotalPos = 0;
}

CProjectEditDlg::~CProjectEditDlg()
{
}

void CProjectEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROJECT_NUMBER, m_strEditProjectNumber);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_strEditProductName);
	DDX_Text(pDX, IDC_EDIT_LENGHT, m_nEditLength);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nEditWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nEditHeight);
	DDX_Control(pDX, IDC_COMBO_CUSTOMER, m_ComboBoxCustomer);
	DDX_Control(pDX, IDC_COMBO_INSPECT_MODE, m_ComboBoxInspectMode);
	DDX_Control(pDX, IDC_LIST_POS, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_INSPECT_POS,m_ComboBoxPos);
	DDX_Control(pDX,IDC_COMBO_WORKER,m_ComboBoxWorker);
}


BEGIN_MESSAGE_MAP(CProjectEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProjectEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_INSPECT_LEVEL, &CProjectEditDlg::OnBnClickedBtnInspectLevel)
END_MESSAGE_MAP()


// CProjectEditDlg 消息处理程序


BOOL CProjectEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pXml);

	SetDlgItemText(IDC_BTN_INSPECT_LEVEL,_T("未设置"));

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

	for (int i = 1; i <= MAX_POS; i++)
	{
		CString strValue;
		strValue.Format(_T("%d"),i);

		m_ComboBoxPos.AddString(strValue);
	}

	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			// ProjectName
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ProjectName")))
			{
				m_pXml->IntoElem();

				m_strEditProjectNumber = m_pXml->GetData();

				m_pXml->OutOfElem();
			}

			// CustomerName
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("CustomerName")))
			{
				m_pXml->IntoElem();

				m_ComboBoxCustomer.SetWindowText(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			// ProductName
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ProductName")))
			{
				m_pXml->IntoElem();

				m_strEditProductName = m_pXml->GetData();

				m_pXml->OutOfElem();
			}

			// ProductLengh
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ProductLengh")))
			{
				m_pXml->IntoElem();

				m_nEditLength = _ttoi(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			// ProductWidth
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ProductWidth")))
			{
				m_pXml->IntoElem();

				m_nEditWidth = _ttoi(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			// ProductHeight
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("ProductHeight")))
			{
				m_pXml->IntoElem();

				m_nEditHeight = _ttoi(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			// InspectLevel
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("InspectLevel")))
			{
				if (_ttoi(m_pXml->GetChildAttrib(_T("Level"))) != 0)
				{
					SetDlgItemText(IDC_BTN_INSPECT_LEVEL,_T("已设置"));
				}
			}

			// InspectMode
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("InspectMode")))
			{
				m_pXml->IntoElem();

				m_ComboBoxInspectMode.SetCurSel(_ttoi(m_pXml->GetData()));

				m_pXml->OutOfElem();
			}

			// InspectPos
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("InspectPos")))
			{
				m_pXml->IntoElem();

				m_nTotalPos = _ttoi(m_pXml->GetData());

				m_ComboBoxPos.SetWindowText(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			// WorkerName
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("WorkerName")))
			{
				m_pXml->IntoElem();

				m_ComboBoxWorker.SetWindowText(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			m_pXml->OutOfElem();
		}
	}

	IntialListCtrl();

	UpdateListCtrl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CProjectEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CTime time = CTime::GetCurrentTime();

	CString strCustomerName,strWorkerName;
	m_ComboBoxCustomer.GetWindowText(strCustomerName);
	m_ComboBoxWorker.GetWindowText(strWorkerName);

	CString strPosCount;
	m_ComboBoxPos.GetWindowText(strPosCount);
	int nPosCount = _ttoi(strPosCount);

	m_IniCustomer.WriteString(strCustomerName,strCustomerName,NULL);
	m_IniCustomer.DeleteKey(strCustomerName,strCustomerName);

	m_pXml->ResetPos();
	if (!m_pXml->FindElem(_T("COXRay")))
	{
		m_pXml->AddElem(_T("COXRay"));
	}

	if (!m_pXml->FindChildElem(_T("Project")))
	{
		m_pXml->AddElem(_T("Project"));
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

	// 创建InspectMode节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("InspectMode")))
	{
		m_pXml->AddChildElem(_T("InspectMode"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(m_ComboBoxInspectMode.GetCurSel());
		m_pXml->OutOfElem();
	}

	// 创建InspectPos节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("InspectPos")))
	{
		m_pXml->AddChildElem(_T("InspectPos"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(strPosCount);
		m_pXml->OutOfElem();
	}

	// 创建WorkerName节点
	m_pXml->ResetChildPos();
	if (!m_pXml->FindChildElem(_T("WorkerName")))
	{
		m_pXml->AddChildElem(_T("WorkerName"));
	}

	{
		m_pXml->IntoElem();
		m_pXml->SetData(strWorkerName);
		m_pXml->OutOfElem();
	}

	if (nPosCount > m_nTotalPos)
	{
		// 新增
		strPosCount.Format(_T("POS%d"),m_nTotalPos);
		m_pXml->FindChildElem(strPosCount);

		for (int pos = m_nTotalPos; pos < nPosCount; pos++)
		{
			strPosCount.Format(_T("POS%d"),pos+1);

			m_pXml->AddChildElem(strPosCount,_T(""));
		}
	}
	else
	{
		// 删除
		for (int pos = nPosCount; pos < m_nTotalPos; pos++)
		{
			strPosCount.Format(_T("POS%d"),pos+1);

			if (m_pXml->FindChildElem(strPosCount))
			{
				m_pXml->RemoveElem();
			}
		}
	}


	m_pXml->Save();

	CDialogEx::OnOK();
}

void CProjectEditDlg::UpdateListCtrl()
{
	CString strPos,strData;
	int nTotalPos = 0;
	if (m_pXml->IsWellFormed())
	{
		m_pXml->ResetPos();

		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			// InspectPos
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("InspectPos")))
			{
				m_pXml->IntoElem();

				nTotalPos = _ttoi(m_pXml->GetData());

				m_pXml->OutOfElem();
			}

			for (int pos = 0; pos < nTotalPos; pos++)
			{
				strPos.Format(_T("POS%d"),pos+1);

				m_pXml->ResetChildPos();
				if (m_pXml->FindChildElem(strPos))
				{
					m_ListCtrl.InsertItem(pos,strPos);

					m_ListCtrl.SetItemText(pos,1,m_pXml->GetChildAttrib(_T("Voltage")));
					m_ListCtrl.SetItemText(pos,2,m_pXml->GetChildAttrib(_T("Current")));
					m_ListCtrl.SetItemText(pos,3,m_pXml->GetChildAttrib(_T("Areas")));

					strData = m_pXml->GetChildAttrib(_T("PerPixel"));
					if (strData.IsEmpty())
					{
						m_ListCtrl.SetItemText(pos,4,_T("NO"));
					}
					else
					{
						m_ListCtrl.SetItemText(pos,4,_T("YES"));
					}

					m_pXml->IntoElem();

					m_ListCtrl.SetItemText(pos,5,_T("NO"));
					if (m_pXml->FindChildElem(_T("Model")))
					{
						m_pXml->IntoElem();
						strData = m_pXml->GetData();

						if (PathFileExists(strData))
						{
							m_ListCtrl.SetItemText(pos,5,_T("YES"));
						}

						m_pXml->OutOfElem();
					}
					m_pXml->OutOfElem();
					
				}
			}

			m_pXml->OutOfElem();
		}
	}
}

void CProjectEditDlg::IntialListCtrl()
{
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	int nItem = 0;
	m_ListCtrl.InsertColumn(nItem++,_T("POS"),LVCFMT_LEFT,80);
	m_ListCtrl.InsertColumn(nItem++,_T("Voltage"),LVCFMT_LEFT,80);
	m_ListCtrl.InsertColumn(nItem++,_T("Current"),LVCFMT_LEFT,80);
	m_ListCtrl.InsertColumn(nItem++,_T("Areas"),LVCFMT_LEFT,100);
	m_ListCtrl.InsertColumn(nItem++,_T("Calib"),LVCFMT_LEFT,100);
	m_ListCtrl.InsertColumn(nItem++,_T("Model"),LVCFMT_LEFT,60);
	
}


void CProjectEditDlg::OnBnClickedBtnInspectLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	CInspectLevelDlg dlg;
	dlg.SetConfig(m_pXml);

	if (dlg.DoModal() == IDOK)
	{
		m_pXml->ResetPos();
		if (m_pXml->FindChildElem(_T("Project")))
		{
			m_pXml->IntoElem();

			// InspectLevel
			m_pXml->ResetChildPos();
			if (m_pXml->FindChildElem(_T("InspectLevel")))
			{
				if (_ttoi(m_pXml->GetChildAttrib(_T("Level"))) != 0)
				{
					SetDlgItemText(IDC_BTN_INSPECT_LEVEL,_T("已设置"));
				}
				else
				{
					SetDlgItemText(IDC_BTN_INSPECT_LEVEL,_T("未设置"));
				}
			}

			m_pXml->OutOfElem();
		}
		
	}
}
