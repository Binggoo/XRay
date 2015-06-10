// PlaceTextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "PlaceTextDlg.h"
#include "afxdialogex.h"


// CPlaceTextDlg 对话框

IMPLEMENT_DYNAMIC(CPlaceTextDlg, CDialogEx)

CPlaceTextDlg::CPlaceTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlaceTextDlg::IDD, pParent)
	, m_strEditText(_T("Sample Text"))
	, m_bCheckBoxBold(FALSE)
	, m_bCheckBoxItalic(FALSE)
	, m_bCheckBoxUnderLine(FALSE)
	, m_bCheckBoxStrikeOut(FALSE)
{
	m_colorText = RGB(255,0,0);
	m_pIni = NULL;
}

CPlaceTextDlg::~CPlaceTextDlg()
{
}

void CPlaceTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strEditText);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bCheckBoxBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bCheckBoxItalic);
	DDX_Check(pDX, IDC_CHECK_UNDERLINE, m_bCheckBoxUnderLine);
	DDX_Check(pDX, IDC_CHECK_STRIKEOUT, m_bCheckBoxStrikeOut);
	DDX_Control(pDX, IDC_COMBO_FONT_SIZE, m_ComboBoxFontSize);
}


BEGIN_MESSAGE_MAP(CPlaceTextDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_COLOR, &CPlaceTextDlg::OnStnClickedStaticColor)
	ON_BN_CLICKED(IDC_CHECK_BOLD, &CPlaceTextDlg::OnBnClickedCheckBold)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, &CPlaceTextDlg::OnBnClickedCheckItalic)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, &CPlaceTextDlg::OnBnClickedCheckUnderline)
	ON_BN_CLICKED(IDC_CHECK_STRIKEOUT, &CPlaceTextDlg::OnBnClickedCheckStrikeout)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDOK, &CPlaceTextDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_SIZE, &CPlaceTextDlg::OnCbnSelchangeComboFontSize)
	ON_CBN_EDITCHANGE(IDC_COMBO_FONT_SIZE, &CPlaceTextDlg::OnCbnEditchangeComboFontSize)
END_MESSAGE_MAP()


// CPlaceTextDlg 消息处理程序


BOOL CPlaceTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pIni);
	int nHeight = m_pIni->GetInt(_T("TextSetting"),_T("Height"),20);
	m_bCheckBoxBold = m_pIni->GetBool(_T("TextSetting"),_T("En_Bold"),FALSE);
	m_bCheckBoxItalic = m_pIni->GetBool(_T("TextSetting"),_T("En_Italic"),FALSE);
	m_bCheckBoxUnderLine = m_pIni->GetBool(_T("TextSetting"),_T("En_UnderLine"),FALSE);
	m_bCheckBoxStrikeOut = m_pIni->GetBool(_T("TextSetting"),_T("En_StrikeOut"),FALSE);
	m_colorText = m_pIni->GetUInt(_T("TextSetting"),_T("Color"),RGB(255,0,0));

	m_Font.CreateFont(nHeight,
		0,
		0,
		0,
		m_bCheckBoxBold ? FW_BOLD : FW_NORMAL,
		m_bCheckBoxItalic,
		m_bCheckBoxUnderLine,
		m_bCheckBoxStrikeOut,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial")
		);

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

	CString strFontSize;
	strFontSize.Format(_T("%d"),nHeight);
	m_ComboBoxFontSize.SetWindowText(strFontSize);

	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CPlaceTextDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT_TEXT:
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

void CPlaceTextDlg::OnStnClickedStaticColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg(m_colorText);

	if (dlg.DoModal() != IDCANCEL)
	{
		m_colorText = dlg.GetColor();
		Invalidate();
	}
}


void CPlaceTextDlg::OnBnClickedCheckBold()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);

	logFont.lfWeight = m_bCheckBoxBold ? FW_BOLD :FW_NORMAL;

	m_Font.DeleteObject();

	m_Font.CreateFontIndirect(&logFont);

	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);
}


void CPlaceTextDlg::OnBnClickedCheckItalic()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);

	logFont.lfItalic = m_bCheckBoxItalic;

	m_Font.DeleteObject();

	m_Font.CreateFontIndirect(&logFont);

	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);
}


void CPlaceTextDlg::OnBnClickedCheckUnderline()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);

	logFont.lfUnderline = m_bCheckBoxUnderLine;

	m_Font.DeleteObject();

	m_Font.CreateFontIndirect(&logFont);

	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);
}


void CPlaceTextDlg::OnBnClickedCheckStrikeout()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);

	logFont.lfStrikeOut = m_bCheckBoxStrikeOut;

	m_Font.DeleteObject();

	m_Font.CreateFontIndirect(&logFont);

	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);
}


BOOL CPlaceTextDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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


void CPlaceTextDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strSize;
	m_ComboBoxFontSize.GetWindowText(strSize);

	m_pIni->WriteString(_T("TextSetting"),_T("Height"),strSize);
	m_pIni->WriteBool(_T("TextSetting"),_T("En_Bold"),m_bCheckBoxBold);
	m_pIni->WriteBool(_T("TextSetting"),_T("En_Italic"),m_bCheckBoxItalic);
	m_pIni->WriteBool(_T("TextSetting"),_T("En_UnderLine"),m_bCheckBoxUnderLine);
	m_pIni->WriteBool(_T("TextSetting"),_T("En_StrikeOut"),m_bCheckBoxStrikeOut);
	m_pIni->WriteUInt(_T("TextSetting"),_T("Color"),m_colorText);
	CDialogEx::OnOK();
}


void CPlaceTextDlg::OnCbnSelchangeComboFontSize()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelectIndex = m_ComboBoxFontSize.GetCurSel();

	CString strSize;
	m_ComboBoxFontSize.GetLBText(nSelectIndex,strSize);

	int nHeight = _ttoi(strSize);

	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);
	logFont.lfHeight = nHeight;

	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&logFont);
		

	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);
	
}


void CPlaceTextDlg::OnCbnEditchangeComboFontSize()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSize;
	m_ComboBoxFontSize.GetWindowText(strSize);

	int nHeight = _ttoi(strSize);

	LOGFONT logFont;
	m_Font.GetLogFont(&logFont);
	logFont.lfHeight = nHeight;

	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&logFont);


	GetDlgItem(IDC_EDIT_TEXT)->SetFont(&m_Font);
}
