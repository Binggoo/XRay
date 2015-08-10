// PageImgCapture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageImgCapture.h"
#include "afxdialogex.h"
#include "CodeRuleDlg.h"

// CPageImgCapture �Ի���

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


// CPageImgCapture ��Ϣ�������


BOOL CPageImgCapture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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

	m_ComboBoxCtlMode.AddString(_T("�ֶ�����"));
	m_ComboBoxCtlMode.AddString(_T("PLC����"));
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

	m_ComboBoxRecordMode.AddString(_T("���к�"));
	m_ComboBoxRecordMode.AddString(_T("��������"));
	m_ComboBoxRecordMode.AddString(_T("��ŵ���"));

	m_ComboBoxRecordMode.SetCurSel(0);

	m_ComboBoxFrames.AddString(_T("8"));
	m_ComboBoxFrames.AddString(_T("16"));
	m_ComboBoxFrames.AddString(_T("32"));
	m_ComboBoxFrames.AddString(_T("64"));
	m_ComboBoxFrames.SetCurSel(1);

	m_BtnStaticCap.SubclassDlgItem(IDC_BTN_STATIC_CAP,this);
	m_BtnStaticCap.SetTooltipText(_T("��̬�ɼ�ͼ��"));

	m_BtnContinueCap.SubclassDlgItem(IDC_BTN_CONT_CAP,this);
	m_BtnContinueCap.SetTooltipText(_T("�����ɼ�ͼ��"));

	m_BtnPlcStart.SubclassDlgItem(IDC_BTN_START,this);
	m_BtnPlcStart.SetTooltipText(_T("PLC���ƿ�ʼ"));

	m_BtnPlcStop.SubclassDlgItem(IDC_BTN_STOP,this);
	m_BtnPlcStop.SetTooltipText(_T("PLC���ƽ���"));

	m_BtnView.SubclassDlgItem(IDC_BTN_VIEW,this);
	m_BtnView.SetTooltipText(_T("�鿴ͼ��"));

	m_BtnCheck.SubclassDlgItem(IDC_BTN_CHECK,this);
	m_BtnCheck.SetTooltipText(_T("ͼ����"));

	m_BtnBrowse.SubclassDlgItem(IDC_BTN_BROWSE,this);
	m_BtnBrowse.SetTooltipText(_T("ͼ�񱣴�·��"));

// 	m_BtnOpen.SubclassDlgItem(ID_FILE_OPEN,this);
// 	m_BtnOpen.SetTooltipText(_T("��ͼ��"));

	m_BtnSave.SubclassDlgItem(IDC_BTN_SAVE,this);
	m_BtnSave.SetTooltipText(_T("����ͼ��"));

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	m_EditPN.ShowWindow(SW_SHOW);

	m_EditDate.ShowWindow(SW_HIDE);
	m_EditModuleNo.ShowWindow(SW_HIDE);
	m_EditDefectLen.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPageImgCapture::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	if(pWnd)  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����   
	{  
		CRect rectCtrl;   //��ȡ�ؼ��仯ǰ�Ĵ�С    
		pWnd->GetWindowRect(&rectCtrl);  
		ScreenToClient(&rectCtrl);//���ؼ���Сת��Ϊ�ڶԻ����е��������� 

		int iLeft = rectCtrl.left;
		int iTop = rectCtrl.top;
		int iWidth = rectCtrl.Width();
		int iHeight = rectCtrl.Height();

		// �ı�X����
		if ((flag & SIZE_MOVE_X) == SIZE_MOVE_X)
		{
			iLeft = iLeft * cx / m_rect.Width();
		}

		// �ı�Y����
		if ((flag & SIZE_MOVE_Y) == SIZE_MOVE_Y)
		{
			iTop = iTop * cy / m_rect.Height();
		}

		//�ı���
		if ((flag & SIZE_ELASTIC_X) == SIZE_ELASTIC_X)
		{
			iWidth = iWidth * cx / m_rect.Width();
		}

		// �ı�߶�
		if ((flag & SIZE_ELASTIC_Y) == SIZE_ELASTIC_Y)
		{
			iHeight = iHeight * cy / m_rect.Height();
		}

		//�ı���
		if ((flag & SIZE_ELASTIC_X_EX) == SIZE_ELASTIC_X_EX)
		{
			iWidth = cx - iLeft - 10;
		}

		// �ı�߶�
		if ((flag & SIZE_ELASTIC_Y_EX) == SIZE_ELASTIC_Y_EX)
		{
			iHeight = cy - iTop - 10;
		}

		pWnd->MoveWindow(iLeft,iTop,iWidth,iHeight);

	}  
}


void CPageImgCapture::OnCbnSelchangeComboCtlMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSelIndex = m_ComboBoxCtlMode.GetCurSel();

	m_ComboBoxPosNum.EnableWindow(!nSelIndex);

	GetDlgItem(IDC_BTN_START)->EnableWindow(nSelIndex);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STATIC_CAP)->EnableWindow(!nSelIndex);
	GetDlgItem(IDC_BTN_CONT_CAP)->EnableWindow(!nSelIndex);
}


LRESULT CPageImgCapture::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_EditPN.SetWindowText(_T(""));

	m_EditPN.SetFocus();
}


void CPageImgCapture::OnCbnSelchangeComboRecordMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strText;
	m_EditDate.GetWindowText(strText);

	if (strText.GetLength() >= 4)
	{
		m_EditModuleNo.SetFocus();
	}
}
