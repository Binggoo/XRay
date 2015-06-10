// PageImgCapture.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageImgCapture.h"
#include "afxdialogex.h"


// CPageImgCapture 对话框

IMPLEMENT_DYNAMIC(CPageImgCapture, CDialogEx)

CPageImgCapture::CPageImgCapture(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageImgCapture::IDD, pParent)
{

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
}


BEGIN_MESSAGE_MAP(CPageImgCapture, CDialogEx)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_CTL_MODE, &CPageImgCapture::OnCbnSelchangeComboCtlMode)
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

	m_BtnOpen.SubclassDlgItem(ID_FILE_OPEN,this);
	m_BtnOpen.SetTooltipText(_T("打开图像"));

	m_BtnSave.SubclassDlgItem(IDC_BTN_SAVE,this);
	m_BtnSave.SetTooltipText(_T("保存图像"));

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

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
		if (LOWORD(wParam) != IDC_COMBO_CTL_MODE)
		{
			return ::SendMessage(GetParent()->GetParent()->GetSafeHwnd(),message,wParam,lParam);
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
