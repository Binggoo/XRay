// PenSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PenSettingDlg.h"
#include "afxdialogex.h"


// CPenSettingDlg 对话框

IMPLEMENT_DYNAMIC(CPenSettingDlg, CDialogEx)

CPenSettingDlg::CPenSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPenSettingDlg::IDD, pParent)
	, m_bCheckNoTips(FALSE)
{
	m_pIni = NULL;
	m_nFontSize = 10;
	m_nLineWidth = 1;
	m_colorText = RGB(255,0,0);
}

CPenSettingDlg::~CPenSettingDlg()
{
}

void CPenSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FONT_SIZE, m_ComboBoxFontSize);
	DDX_Control(pDX, IDC_COMBO_LINE_WIDTH, m_ComboBoxLineWidth);
	DDX_Check(pDX, IDC_CHECK_TIPS, m_bCheckNoTips);
}


BEGIN_MESSAGE_MAP(CPenSettingDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_COLOR, &CPenSettingDlg::OnStnClickedStaticColor)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_SIZE, &CPenSettingDlg::OnCbnSelchangeComboFontSize)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_WIDTH, &CPenSettingDlg::OnCbnSelchangeComboLineWidth)
	ON_CBN_EDITCHANGE(IDC_COMBO_FONT_SIZE, &CPenSettingDlg::OnCbnEditchangeComboFontSize)
	ON_CBN_EDITCHANGE(IDC_COMBO_LINE_WIDTH, &CPenSettingDlg::OnCbnEditchangeComboLineWidth)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPenSettingDlg 消息处理程序


BOOL CPenSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);
	m_nFontSize = m_pIni->GetUInt(_T("PenSetting"),_T("FontSize"),10);
	m_nLineWidth = m_pIni->GetUInt(_T("PenSetting"),_T("LineWidth"),1);
	m_colorText = m_pIni->GetUInt(_T("PenSetting"),_T("Color"),RGB(255,0,0));
	m_bCheckNoTips = m_pIni->GetBool(_T("PenSetting"),_T("En_NoRemind"),FALSE);

	CString strFontSize,strLineWidth;
	strFontSize.Format(_T("%d"),m_nFontSize);
	strLineWidth.Format(_T("%d"),m_nLineWidth);

	m_ComboBoxFontSize.AddString(_T("8"));
	m_ComboBoxFontSize.AddString(_T("9"));
	m_ComboBoxFontSize.AddString(_T("10"));
	m_ComboBoxFontSize.AddString(_T("11"));
	m_ComboBoxFontSize.AddString(_T("12"));
	m_ComboBoxFontSize.AddString(_T("14"));
	m_ComboBoxFontSize.AddString(_T("16"));
	m_ComboBoxFontSize.AddString(_T("18"));
	m_ComboBoxFontSize.AddString(_T("20"));
	m_ComboBoxFontSize.AddString(_T("22"));
	m_ComboBoxFontSize.AddString(_T("24"));
	m_ComboBoxFontSize.AddString(_T("26"));
	m_ComboBoxFontSize.AddString(_T("28"));
	m_ComboBoxFontSize.AddString(_T("36"));
	m_ComboBoxFontSize.AddString(_T("48"));
	m_ComboBoxFontSize.AddString(_T("72"));

	m_ComboBoxFontSize.SetWindowText(strFontSize);

	m_ComboBoxLineWidth.AddString(_T("1"));
	m_ComboBoxLineWidth.AddString(_T("2"));
	m_ComboBoxLineWidth.AddString(_T("3"));
	m_ComboBoxLineWidth.AddString(_T("5"));
	m_ComboBoxLineWidth.AddString(_T("7"));

	m_ComboBoxLineWidth.SetWindowText(strLineWidth);
	
	m_Font.CreateFont(m_nFontSize,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial"));

	GetDlgItem(IDC_TEXT_HELLO)->SetFont(&m_Font);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPenSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	CString strText;
	m_ComboBoxFontSize.GetWindowText(strText);
	m_nFontSize = _ttoi(strText);

	m_ComboBoxLineWidth.GetWindowText(strText);
	m_nLineWidth = _ttoi(strText);

	m_pIni->WriteUInt(_T("PenSetting"),_T("FontSize"),m_nFontSize);
	m_pIni->WriteUInt(_T("PenSetting"),_T("LineWidth"),m_nLineWidth);
	m_pIni->WriteUInt(_T("PenSetting"),_T("Color"),m_colorText);
	m_pIni->WriteBool(_T("PenSetting"),_T("En_NoRemind"),m_bCheckNoTips);
	

	CDialogEx::OnOK();
}


void CPenSettingDlg::OnStnClickedStaticColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg(m_colorText);

	if (dlg.DoModal() != IDCANCEL)
	{
		m_colorText = dlg.GetColor();

		Invalidate();
	}
}

void CPenSettingDlg::SetConfig( CIni *pIni )
{
	m_pIni = pIni;
}


HBRUSH CPenSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_TEXT_HELLO:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_colorText);
		break;

	case IDC_STATIC_COLOR:
		hbr = CreateSolidBrush(m_colorText);
		break;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CPenSettingDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pWnd)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STATIC_COLOR:
			SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND)));
			return TRUE;
		}
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CPenSettingDlg::OnCbnSelchangeComboFontSize()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelectIndex = m_ComboBoxFontSize.GetCurSel();

	CString strSize;
	m_ComboBoxFontSize.GetLBText(nSelectIndex,strSize);

	m_nFontSize = _ttoi(strSize);

	if (m_nFontSize > 0)
	{
		m_Font.DeleteObject();

		m_Font.CreateFont(m_nFontSize,
			0,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			_T("Arial"));

		GetDlgItem(IDC_TEXT_HELLO)->SetFont(&m_Font);
	}
}


void CPenSettingDlg::OnCbnSelchangeComboLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelectIndex = m_ComboBoxLineWidth.GetCurSel();

	CString strSize;
	m_ComboBoxLineWidth.GetLBText(nSelectIndex,strSize);

	m_nLineWidth = _ttoi(strSize);

	if (m_nLineWidth > 0)
	{
		Invalidate();
	}
}


void CPenSettingDlg::OnCbnEditchangeComboFontSize()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSize;
	m_ComboBoxFontSize.GetWindowText(strSize);

	m_nFontSize = _ttoi(strSize);

	if (m_nFontSize > 0)
	{
		m_Font.DeleteObject();

		m_Font.CreateFont(m_nFontSize,
			0,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			_T("Arial"));

		GetDlgItem(IDC_TEXT_HELLO)->SetFont(&m_Font);
	}
	
}


void CPenSettingDlg::OnCbnEditchangeComboLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSize;
	m_ComboBoxLineWidth.GetWindowText(strSize);

	m_nLineWidth = _ttoi(strSize);

	if (m_nLineWidth > 0)
	{
		Invalidate();
	}
}


void CPenSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rectText;
	GetDlgItem(IDC_TEXT_HELLO)->GetWindowRect(&rectText);
	ScreenToClient(&rectText);

	CPen pen(PS_SOLID,m_nLineWidth,m_colorText);

	CPen *oldPen = dc.SelectObject(&pen);

	dc.MoveTo(rectText.left,rectText.bottom + 10);
	dc.LineTo(rectText.right,rectText.bottom + 10);

	dc.SelectObject(oldPen);
}
