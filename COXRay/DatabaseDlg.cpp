// DatabaseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COXRay.h"
#include "DatabaseDlg.h"
#include "afxdialogex.h"


// CDatabaseDlg �Ի���

IMPLEMENT_DYNAMIC(CDatabaseDlg, CDialogEx)

CDatabaseDlg::CDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseDlg::IDD, pParent)
{
	m_pMyDatabase = NULL;
}

CDatabaseDlg::~CDatabaseDlg()
{
}

void CDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DataTimeCtrlStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DataTimeCtrlEnd);
	DDX_Control(pDX, IDC_COMBO_UNION, m_ComboBoxUnion);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_ComboBoxFields);
	DDX_Control(pDX, IDC_LIST_QUERT_CONDITION, m_ListCtrlQueryCondition);
	DDX_Control(pDX, IDC_LIST_QUERY_RESULT, m_ListCtrlQueryResult);
	DDX_Control(pDX, IDC_PIC_IMG, m_ThumbnailPic);
}


BEGIN_MESSAGE_MAP(CDatabaseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDatabaseDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDatabaseDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CDatabaseDlg::OnBnClickedBtnQuery)
	ON_NOTIFY(NM_CLICK, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnNMClickListQueryResult)
END_MESSAGE_MAP()


// CDatabaseDlg ��Ϣ�������


BOOL CDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ASSERT(m_pMyDatabase);

	m_ThumbnailPic.ModifyStyle(0,SS_BITMAP);

	m_DataTimeCtrlStart.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_DataTimeCtrlEnd.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	m_ComboBoxUnion.AddString(_T("AND"));
	m_ComboBoxUnion.AddString(_T("OR"));
	m_ComboBoxUnion.SetCurSel(0);

	for (int i = 0; i < QUERY_FIELDS_COUNT;i++)
	{
		m_ComboBoxFields.AddString(CString(query_fields[i]));
	}

	InitialListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDatabaseDlg::InitialListCtrl()
{
	CRect rect;
	int nWidth = 0;
	// ��ѯ����
	m_ListCtrlQueryCondition.GetClientRect(&rect);
	nWidth = rect.Width() - 60;

	m_ListCtrlQueryCondition.InsertColumn(0,_T("������"),LVCFMT_LEFT,60);
	m_ListCtrlQueryCondition.InsertColumn(1,_T("�ֶ�"),LVCFMT_LEFT,nWidth / 2);
	m_ListCtrlQueryCondition.InsertColumn(2,_T("����"),LVCFMT_LEFT,nWidth / 2);

	m_ListCtrlQueryCondition.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// ��ѯ��
	int nItem = 0;
	m_ListCtrlQueryResult.InsertColumn(nItem,CString(table_fields[nItem]),LVCFMT_LEFT,40);
	nItem++;

	for (;nItem < TABLE_FIELDS_COUNT - 2;nItem++)
	{
		m_ListCtrlQueryResult.InsertColumn(nItem,CString(table_fields[nItem]),LVCFMT_LEFT,100);
	}

	m_ListCtrlQueryResult.InsertColumn(nItem,CString(table_fields[nItem]),LVCFMT_LEFT,60);
	nItem++;

	m_ListCtrlQueryResult.InsertColumn(nItem,CString(table_fields[nItem]),LVCFMT_LEFT,150);
	nItem++;

	m_ListCtrlQueryResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}


void CDatabaseDlg::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_ComboBoxFields.GetCurSel() == -1)
	{
		AfxMessageBox(_T("��ѡ��һ���ֶΣ�"));
		return;
	}

	CString strOperator,strField,strContent;
	m_ComboBoxUnion.GetWindowText(strOperator);
	m_ComboBoxFields.GetWindowText(strField);

	GetDlgItemText(IDC_EDIT_CONTENT,strContent);

	int nCount = m_ListCtrlQueryResult.GetItemCount();

	m_ListCtrlQueryCondition.InsertItem(nCount,strOperator);
	m_ListCtrlQueryCondition.SetItemText(nCount,1,strField);
	m_ListCtrlQueryCondition.SetItemText(nCount,2,strContent);
}


void CDatabaseDlg::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_ListCtrlQueryCondition.GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = m_ListCtrlQueryCondition.GetNextSelectedItem(pos);
		m_ListCtrlQueryCondition.DeleteItem(nItem);

		pos = m_ListCtrlQueryCondition.GetFirstSelectedItemPosition();
	}
}


void CDatabaseDlg::OnBnClickedBtnQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �жϱ��Ƿ����
	if (!m_pMyDatabase->IsTableExist(table_name))
	{
		AfxMessageBox(_T("���ݱ����ڣ�"));
		return ;
	}

	BeginWaitCursor();

	m_ThumbnailPic.SetBitmap(NULL);

	CString strQuery,strTmp;
	CTime startTime,endTime;
	m_DataTimeCtrlStart.GetTime(startTime);
	m_DataTimeCtrlEnd.GetTime(endTime);

	strQuery.Format(_T("SELECT * FROM %s WHERE (time BETWEEN '%s' AND '%s') ")
		,CString(table_name),startTime.Format(_T("%Y-%m-%d %H:%M:%S")),endTime.Format(_T("%Y-%m-%d %H:%M:%S")));

	int nCount = m_ListCtrlQueryCondition.GetItemCount();

	for (int i = 0; i < nCount;i++)
	{
		CString strOperator,strFiled,strContent;

		strOperator = m_ListCtrlQueryCondition.GetItemText(i,0);
		strFiled = m_ListCtrlQueryCondition.GetItemText(i,1);
		strContent = m_ListCtrlQueryCondition.GetItemText(i,2);

		if (strContent.Left(1) == _T("!"))
		{
			strContent.TrimLeft(_T("!"));

			if (strContent.Left(1) == _T("%") || strContent.Right(1) == _T("%"))
			{
				strTmp.Format(_T("%s %s NOT LIKE '%s' "),strOperator,strFiled,strContent);
			}
			else
			{
				// strTmp.Format(_T("%s %s <> '%s'"),strOperator,strFiled,strContent);
				strTmp.Format(_T("%s %s != '%s' "),strOperator,strFiled,strContent);
			}
		}
		else if (strContent.Left(1) == _T("%") || strContent.Right(1) == _T("%"))
		{
			strTmp.Format(_T("%s %s LIKE '%s' "),strOperator,strFiled,strContent);
		}
		else
		{
			strTmp.Format(_T("%s %s = '%s' "),strOperator,strFiled,strContent);
		}

		strQuery += strTmp;
	}

	strQuery += _T(";");

	// ��ѯ
	USES_CONVERSION;
	char *sql = W2A(strQuery);

	nCount = m_pMyDatabase->ExecuteQuery(sql);

	m_ListCtrlQueryResult.SetRedraw(FALSE);
	// ���ListCtrl
	m_ListCtrlQueryResult.DeleteAllItems();

	for (int row = 0; row < nCount;row++)
	{
		unsigned len = 255;
		CString strData = CString(m_pMyDatabase->GetStringValue(row,0,&len));

		m_ListCtrlQueryResult.InsertItem(row,strData);

		for (int col = 1; col < TABLE_FIELDS_COUNT;col++)
		{
			strData = CString(m_pMyDatabase->GetStringValue(row,col,&len));
			m_ListCtrlQueryResult.SetItemText(row,col,strData);
		}
	}
	m_ListCtrlQueryResult.SetRedraw(TRUE);

	EndWaitCursor();
}


void CDatabaseDlg::OnNMClickListQueryResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem = pNMItemActivate->iItem;

	if (nItem != -1)
	{
		CRect rect;
		m_ThumbnailPic.GetClientRect(&rect);

		CString strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);

		Bitmap img(strPath);

		Bitmap *pThumbNail = (Bitmap *)img.GetThumbnailImage(rect.Width(),rect.Height());

		HBITMAP		hBmp = NULL;
		pThumbNail->GetHBITMAP(NULL,&hBmp);

		m_ThumbnailPic.SetBitmap(hBmp);

		delete pThumbNail;
	}

	*pResult = 0;
}
