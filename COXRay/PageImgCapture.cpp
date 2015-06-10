// PageImgCapture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "PageImgCapture.h"
#include "afxdialogex.h"


// CPageImgCapture �Ի���

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

	m_BtnOpen.SubclassDlgItem(ID_FILE_OPEN,this);
	m_BtnOpen.SetTooltipText(_T("��ͼ��"));

	m_BtnSave.SubclassDlgItem(IDC_BTN_SAVE,this);
	m_BtnSave.SetTooltipText(_T("����ͼ��"));

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

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
		if (LOWORD(wParam) != IDC_COMBO_CTL_MODE)
		{
			return ::SendMessage(GetParent()->GetParent()->GetSafeHwnd(),message,wParam,lParam);
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
