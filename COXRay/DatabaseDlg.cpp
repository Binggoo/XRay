// DatabaseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "DatabaseDlg.h"
#include "afxdialogex.h"
#include "FullScreenViewDlg.h"

// CDatabaseDlg 对话框

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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnNMDblclkListQueryResult)
	ON_BN_CLICKED(IDC_BTN_VIEW, &CDatabaseDlg::OnBnClickedBtnView)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnLvnKeydownListQueryResult)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnNMRClickListQueryResult)
	ON_COMMAND(ID_DATABASE_DEL, &CDatabaseDlg::OnDatabaseDel)
	ON_COMMAND(ID_DATABASE_CLEAR, &CDatabaseDlg::OnDatabaseClear)
END_MESSAGE_MAP()


// CDatabaseDlg 消息处理程序


BOOL CDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pMyDatabase);

	m_DataTimeCtrlStart.SetFormat(_T("yyyy-MM-dd HH:mm"));
	m_DataTimeCtrlEnd.SetFormat(_T("yyyy-MM-dd HH:mm"));

	CTime time(0);
	m_DataTimeCtrlStart.SetTime(&time);

	m_ComboBoxUnion.AddString(_T("AND"));
	m_ComboBoxUnion.AddString(_T("OR"));
	m_ComboBoxUnion.SetCurSel(0);

	for (int i = 0; i < QUERY_FIELDS_COUNT;i++)
	{
		m_ComboBoxFields.AddString(CString(query_fields[i]));
	}

	InitialListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDatabaseDlg::InitialListCtrl()
{
	CRect rect;
	int nWidth = 0;
	// 查询条件
	m_ListCtrlQueryCondition.GetClientRect(&rect);
	nWidth = rect.Width() - 60;

	m_ListCtrlQueryCondition.InsertColumn(0,_T("操作符"),LVCFMT_LEFT,60);
	m_ListCtrlQueryCondition.InsertColumn(1,_T("字段"),LVCFMT_LEFT,nWidth / 2);
	m_ListCtrlQueryCondition.InsertColumn(2,_T("内容"),LVCFMT_LEFT,nWidth / 2);

	m_ListCtrlQueryCondition.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 查询结
	for (int nItem = 0;nItem < TABLE_FIELDS_COUNT;nItem++)
	{
		m_ListCtrlQueryResult.InsertColumn(nItem,CString(table_fields[nItem]),LVCFMT_LEFT,fileds_width[nItem]);
	}

	m_ListCtrlQueryResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
}


void CDatabaseDlg::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ComboBoxFields.GetCurSel() == -1)
	{
		AfxMessageBox(_T("请选择一个字段！"));
		return;
	}

	CString strOperator,strField,strContent;
	m_ComboBoxUnion.GetWindowText(strOperator);
	m_ComboBoxFields.GetWindowText(strField);

	GetDlgItemText(IDC_EDIT_CONTENT,strContent);

	int nCount = m_ListCtrlQueryCondition.GetItemCount();

	m_ListCtrlQueryCondition.InsertItem(nCount,strOperator);
	m_ListCtrlQueryCondition.SetItemText(nCount,1,strField);
	m_ListCtrlQueryCondition.SetItemText(nCount,2,strContent);
}


void CDatabaseDlg::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	// 判断表是否存在
	if (!m_pMyDatabase->IsTableExist(table_name))
	{
		AfxMessageBox(_T("数据表不存在！"));
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

	// 查询
	USES_CONVERSION;
	char *sql = W2A(strQuery);

	nCount = m_pMyDatabase->ExecuteQuery(sql);

	m_ListCtrlQueryResult.SetRedraw(FALSE);
	// 清空ListCtrl
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
	// TODO: 在此添加控件通知处理程序代码
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


BOOL CDatabaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDatabaseDlg::OnNMDblclkListQueryResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nItem = pNMItemActivate->iItem;

	if (nItem != -1)
	{
		CString strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);

		CFullScreenViewDlg dlg;
		dlg.SetFilePath(strPath);

		dlg.DoModal();
	}

	*pResult = 0;
}


void CDatabaseDlg::OnBnClickedBtnView()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListCtrlQueryResult.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		return;
	}

	int nItem = m_ListCtrlQueryResult.GetNextSelectedItem(pos);

	CString strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);

	AfxGetApp()->OpenDocumentFile(strPath);
}


void CDatabaseDlg::OnLvnKeydownListQueryResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListCtrlQueryResult.GetFirstSelectedItemPosition();
	
	int nItem = m_ListCtrlQueryResult.GetNextSelectedItem(pos);

	if (pLVKeyDow->wVKey == VK_UP || pLVKeyDow->wVKey == VK_DOWN )
	{
		if (pLVKeyDow->wVKey == VK_UP)
		{
			if (nItem > 0)
			{
				nItem--;
			}
		}
		else
		{
			
			if (nItem < m_ListCtrlQueryResult.GetItemCount() - 1)
			{
				nItem++;
			}
		}

		if (nItem < 0 || nItem >= m_ListCtrlQueryResult.GetItemCount())
		{
			*pResult = 0;
			return;
		}

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


void CDatabaseDlg::OnNMRClickListQueryResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos),HIWORD(dwPos));

		CMenu menu;

		menu.LoadMenu(IDR_MENU_DATABASE);

		CMenu *popue = menu.GetSubMenu(0);

		popue->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
	}
	*pResult = 0;
}


void CDatabaseDlg::OnDatabaseDel()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_ListCtrlQueryResult.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		return;
	}

	int nItem = m_ListCtrlQueryResult.GetNextSelectedItem(pos);

	CString strID = m_ListCtrlQueryResult.GetItemText(nItem,0);

	if (IDYES == AfxMessageBox(_T("即将从数据库中删除此行记录，是否继续？"),MB_YESNO | MB_DEFBUTTON2))
	{
		int id = _ttoi(strID);

		if (m_pMyDatabase->DeleteRecord(id))
		{
			m_ListCtrlQueryResult.DeleteItem(nItem);
		}
	}
}


void CDatabaseDlg::OnDatabaseClear()
{
	// TODO: 在此添加命令处理程序代码
	int nCount = m_ListCtrlQueryResult.GetItemCount();

	if (nCount == 0)
	{
		return;
	}

	if (IDYES == AfxMessageBox(_T("即将从数据库中删除查询到的所有记录，是否继续？"),MB_YESNO | MB_DEFBUTTON2))
	{
		for (int nItem = 0; nItem < nCount;nItem++)
		{
			CString strID = m_ListCtrlQueryResult.GetItemText(nItem,0);
			int id = _ttoi(strID);

			if (m_pMyDatabase->DeleteRecord(id))
			{
				m_ListCtrlQueryResult.DeleteItem(nItem);
				nItem--;
				nCount--;
			}
		}
	}
}
