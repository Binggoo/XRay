// PageImgCapture.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageImgCapture.h"
#include "afxdialogex.h"
#include "CodeRuleDlg.h"

// CPageImgCapture 对话框

IMPLEMENT_DYNAMIC(CPageImgCapture, CDialogEx)

CPageImgCapture::CPageImgCapture(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageImgCapture::IDD, pParent)
{
	m_pIni = NULL;
}

CPageImgCapture::~CPageImgCapture()
{
}

void CPageImgCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CTL_MODE, m_ComboBoxCtlMode);
	DDX_Control(pDX, IDC_COMBO_FRAMES, m_ComboBoxFrames);
	DDX_Control(pDX, IDC_EDIT_SAVE_PATH, m_EditSavePath);
	DDX_Control(pDX, IDC_EDIT_PN, m_EditPN);
	DDX_Control(pDX,IDC_COMBO_POS_NUMBER,m_ComboBoxPosNum);
	DDX_Control(pDX,IDC_CHECK_AUTO_PROCESS,m_CheckAutoProcess);
	DDX_Control(pDX,IDC_COMBO_LEVEL,m_ComboBoxLevel);
	DDX_Control(pDX,IDC_COMBO_RECORD_MODE,m_ComboBoxRecordMode);
	DDX_Control(pDX,IDC_EDIT_DATE,m_EditDate);
	DDX_Control(pDX,IDC_EDIT_MOLD_NO,m_EditModuleNo);
	DDX_Control(pDX,IDC_EDIT_DEFECT_LEN,m_EditDefectLen);
}


BEGIN_MESSAGE_MAP(CPageImgCapture, CDialogEx)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_CTL_MODE, &CPageImgCapture::OnCbnSelchangeComboCtlMode)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CPageImgCapture::OnBnClickedBtnClear)
	ON_CBN_SELCHANGE(IDC_COMBO_RECORD_MODE, &CPageImgCapture::OnCbnSelchangeComboRecordMode)
	ON_EN_CHANGE(IDC_EDIT_DEFECT_LEN, &CPageImgCapture::OnEnChangeEditDefectLen)
	ON_EN_CHANGE(IDC_EDIT_DATE, &CPageImgCapture::OnEnChangeEditDate)
END_MESSAGE_MAP()


// CPageImgCapture 消息处理程序


BOOL CPageImgCapture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetClientRect(&m_rect);

	CRect rectClient;
	this->GetOwner()->GetClientRect(&rectClient);
	CTabCtrl *pTab = (CTabCtrl *)this->GetOwner();

	CRect rcHead;
	pTab->GetItemRect(0,&rcHead);
	rectClient.top += rcHead.Height() + 2;
	rectClient.left += 2;
	rectClient.right -= 2;
	rectClient.bottom -= 2;
	MoveWindow(rectClient);

	m_ComboBoxCtlMode.AddString(_T("手动控制"));
	m_ComboBoxCtlMode.AddString(_T("PLC控制"));
	m_ComboBoxCtlMode.SetCurSel(0);

	for (int i = 1; i <= MAX_POS;i++)
	{
		CString strPos;
		strPos.Format(_T("%d"),i);

		m_ComboBoxPosNum.AddString(strPos);
	}

	m_ComboBoxPosNum.SetCurSel(0);

	for (int i = 0; i <= MAX_LEVEL;i++)
	{
		CString strLevel;
		strLevel.Format(_T("%d"),i);

		m_ComboBoxLevel.AddString(strLevel);
	}

	m_ComboBoxLevel.SetCurSel(0);

	m_ComboBoxRecordMode.AddString(_T("序列号"));
	m_ComboBoxRecordMode.AddString(_T("生产日期"));
	m_ComboBoxRecordMode.AddString(_T("序号递增"));

	m_ComboBoxRecordMode.SetCurSel(0);

	m_ComboBoxFrames.AddString(_T("8"));
	m_ComboBoxFrames.AddString(_T("16"));
	m_ComboBoxFrames.AddString(_T("32"));
	m_ComboBoxFrames.AddString(_T("64"));
	m_ComboBoxFrames.SetCurSel(1);

	m_BtnStaticCap.SubclassDlgItem(IDC_BTN_STATIC_CAP,this);
	m_BtnStaticCap.SetTooltipText(_T("静态采集图像"));

	m_BtnContinueCap.SubclassDlgItem(IDC_BTN_CONT_CAP,this);
	m_BtnContinueCap.SetTooltipText(_T("连续采集图像"));

	m_BtnPlcStart.SubclassDlgItem(IDC_BTN_START,this);
	m_BtnPlcStart.SetTooltipText(_T("PLC控制开始"));

	m_BtnPlcStop.SubclassDlgItem(IDC_BTN_STOP,this);
	m_BtnPlcStop.SetTooltipText(_T("PLC控制结束"));

	m_BtnView.SubclassDlgItem(IDC_BTN_VIEW,this);
	m_BtnView.SetTooltipText(_T("查看图像"));

	m_BtnCheck.SubclassDlgItem(IDC_BTN_CHECK,this);
	m_BtnCheck.SetTooltipText(_T("图像检测"));

	m_BtnBrowse.SubclassDlgItem(IDC_BTN_BROWSE,this);
	m_BtnBrowse.SetTooltipText(_T("图像保存路径"));

// 	m_BtnOpen.SubclassDlgItem(ID_FILE_OPEN,this);
// 	m_BtnOpen.SetTooltipText(_T("打开图像"));

	m_BtnSave.SubclassDlgItem(IDC_BTN_SAVE,this);
	m_BtnSave.SetTooltipText(_T("保存图像"));

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	m_EditPN.ShowWindow(SW_SHOW);

	m_EditDate.ShowWindow(SW_HIDE);
	m_EditModuleNo.ShowWindow(SW_HIDE);
	m_EditDefectLen.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageImgCapture::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_MINIMIZED)
	{
		return;
	}

	CWnd *pWnd = NULL;
	pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		DWORD dwFlag = SIZE_MOVE_X | SIZE_MOVE_Y;

		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_EDIT_SAVE_PATH:
		case IDC_EDIT_PN:
			dwFlag |= SIZE_ELASTIC_X;
			break;

		case IDC_STATIC:
			break;
		}
		
		ChangeSize(pWnd,cx,cy,dwFlag);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	GetClientRect(&m_rect);
}

void CPageImgCapture::ChangeSize( CWnd *pWnd,int cx,int cy,DWORD flag )
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
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// 改变Y坐标
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//改变宽度
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// 改变高度
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y)
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


void CPageImgCapture::OnCbnSelchangeComboCtlMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelIndex = m_ComboBoxCtlMode.GetCurSel();

	m_ComboBoxPosNum.EnableWindow(!nSelIndex);

	GetDlgItem(IDC_BTN_START)->EnableWindow(nSelIndex);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STATIC_CAP)->EnableWindow(!nSelIndex);
	GetDlgItem(IDC_BTN_CONT_CAP)->EnableWindow(!nSelIndex);
}


LRESULT CPageImgCapture::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_COMMAND:
		{
			switch LOWORD(wParam)
			{
			case IDC_COMBO_CTL_MODE:
			case IDC_BTN_CLEAR:
			case IDC_COMBO_RECORD_MODE:
			case IDC_EDIT_DEFECT_LEN:
			case IDC_EDIT_DATE:
				break;

			default:
				return ::SendMessage(GetParent()->GetParent()->GetSafeHwnd(),message,wParam,lParam);
			}
			
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


BOOL CPageImgCapture::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
				if (GetKeyState(VK_SHIFT) < 0)
				{
					//Shift + Enter
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_COMMAND,IDC_BTN_SAVE,0);
				}
				else if (GetFocus() == GetDlgItem(IDC_EDIT_PN))
				{
					CString strPN,strCodeRule;
					m_EditPN.GetWindowText(strPN);

					if (m_pIni)
					{
						strCodeRule = m_pIni->GetString(_T("SaveSetting"),_T("CodeRule"));
					}

					strPN = CCodeRuleDlg::CodeString(strPN,strCodeRule);

					m_EditPN.SetWindowText(strPN);
				}
			}
			break;

		case VK_F12:
			{
				::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_COMMAND,IDC_BTN_STATIC_CAP,0);
			}
			break;

		}
		
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPageImgCapture::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditPN.SetWindowText(_T(""));

	m_EditPN.SetFocus();
}


void CPageImgCapture::OnCbnSelchangeComboRecordMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelectIndex = m_ComboBoxRecordMode.GetCurSel();
	m_EditPN.SetWindowText(_T(""));
	m_EditDate.SetWindowText(_T(""));
	m_EditModuleNo.SetWindowText(_T(""));
	m_EditDefectLen.SetWindowText(_T(""));

	switch (nSelectIndex)
	{
	case Record_PN:
		m_EditPN.ShowWindow(SW_SHOW);

		m_EditDate.ShowWindow(SW_HIDE);
		m_EditModuleNo.ShowWindow(SW_HIDE);
		m_EditDefectLen.ShowWindow(SW_HIDE);
		break;

	case Record_Date:
		m_EditPN.ShowWindow(SW_HIDE);

		m_EditDate.ShowWindow(SW_SHOW);
		m_EditModuleNo.ShowWindow(SW_SHOW);
		m_EditDefectLen.ShowWindow(SW_SHOW);
		break;

	case Record_NO:
		m_EditPN.ShowWindow(SW_HIDE);

		m_EditDate.ShowWindow(SW_SHOW);
		m_EditModuleNo.ShowWindow(SW_HIDE);
		m_EditDefectLen.ShowWindow(SW_SHOW);
		break;
	}
}


void CPageImgCapture::OnEnChangeEditDefectLen()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strText;
	m_EditDefectLen.GetWindowText(strText);

	if (strText.CompareNoCase(_T("OK")) == 0)
	{
		m_ComboBoxLevel.SetCurSel(0);
	}
	else
	{
		double dbLen = _ttof(strText);

		int nIndex = 0;

		int nLevelCount = m_pIni->GetInt(_T("InspectLevel"),_T("Count"),1);

		CString strLevel;
		for (int i = 1; i < nLevelCount;i++)
		{
			strLevel.Format(_T("Level%d"),i);

			if (dbLen < m_pIni->GetDouble(_T("InspectLevel"),strLevel,0.0))
			{
				nIndex = i;
				break;
			}
		}

		strLevel.Format(_T("Level%d"),nLevelCount);
		if (dbLen >= m_pIni->GetDouble(_T("InspectLevel"),strLevel,0.0) + EPSINON 
			|| dbLen >= m_pIni->GetDouble(_T("InspectLevel"),strLevel,0.0) - EPSINON)
		{
			nIndex = nLevelCount;
		}

		if (dbLen == 0.0)
		{
			nIndex = 0;
		}

		m_ComboBoxLevel.SetCurSel(nIndex);
	}
}


void CPageImgCapture::OnEnChangeEditDate()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strText;
	m_EditDate.GetWindowText(strText);

	if (strText.GetLength() >= 4)
	{
		m_EditModuleNo.SetFocus();
	}
}
