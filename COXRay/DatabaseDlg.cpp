// DatabaseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "DatabaseDlg.h"
#include "afxdialogex.h"
#include "FullScreenViewDlg.h"
#include "../common/excel/MyExcel.h"

// CDatabaseDlg 对话框

static int _List_Type(int col)
{
	if (col == 0)
	{
		return CEditList::eLast;
	}

	return CEditList::eEdit;
}

IMPLEMENT_DYNAMIC(CDatabaseDlg, CDialogEx)

CDatabaseDlg::CDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseDlg::IDD, pParent)
{
	m_pMyDatabase = NULL;
	m_nCurSelectItem = -1;
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
	DDX_Control(pDX,IDC_COMBO_DATABASE,m_ComboBoxDataBase);
}


BEGIN_MESSAGE_MAP(CDatabaseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDatabaseDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDatabaseDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CDatabaseDlg::OnBnClickedBtnQuery)
	//ON_NOTIFY(NM_CLICK, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnNMClickListQueryResult)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnNMDblclkListQueryResult)
	ON_BN_CLICKED(IDC_BTN_VIEW, &CDatabaseDlg::OnBnClickedBtnView)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnLvnKeydownListQueryResult)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_QUERY_RESULT, &CDatabaseDlg::OnNMRClickListQueryResult)
	ON_COMMAND(ID_DATABASE_DEL, &CDatabaseDlg::OnDatabaseDel)
	ON_COMMAND(ID_DATABASE_CLEAR, &CDatabaseDlg::OnDatabaseClear)
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, &CDatabaseDlg::OnCbnSelchangeComboDatabase)
	ON_COMMAND(ID_DATABASE_DROP, &CDatabaseDlg::OnDatabaseDrop)
	ON_COMMAND(ID_DATABASE_CLEAR_ALL, &CDatabaseDlg::OnDatabaseClearAll)
	ON_COMMAND(ID_DATABASE_EXPORT, &CDatabaseDlg::OnDatabaseExport)
	ON_COMMAND(ID_DATABASE_OPEN_PATH, &CDatabaseDlg::OnDatabaseOpenPath)
	ON_COMMAND(ID_DATABASE_MODIFY, &CDatabaseDlg::OnDatabaseModify)
	ON_MESSAGE(WM_USER_CLICK, &CDatabaseDlg::OnUserClick)
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

	m_ComboBoxDataBase.AddString(CString(main_table_name));
	m_ComboBoxDataBase.AddString(CString(sub_table_name));

	m_ComboBoxDataBase.SetCurSel(-1);

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

	CString strTable;
	m_ComboBoxDataBase.GetWindowText(strTable);

	USES_CONVERSION;
	char *table = W2A(strTable);

	if (!m_pMyDatabase->IsTableExist(table))
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
		,strTable,startTime.Format(_T("%Y-%m-%d %H:%M:%S")),endTime.Format(_T("%Y-%m-%d %H:%M:%S")));

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
	char *sql = W2A(strQuery);

	nCount = m_pMyDatabase->ExecuteQuery(sql);

	int nCols = m_ListCtrlQueryResult.GetHeaderCtrl()->GetItemCount();

	m_ListCtrlQueryResult.SetRedraw(FALSE);
	// 清空ListCtrl
	m_ListCtrlQueryResult.DeleteAllItems();

	for (int row = 0; row < nCount;row++)
	{
		unsigned len = 255;
		CString strData = CString(m_pMyDatabase->GetStringValue(row,0,&len));

		m_ListCtrlQueryResult.InsertItem(row,strData);

		for (int col = 1; col < nCols;col++)
		{
			strData = CString(m_pMyDatabase->GetStringValue(row,col,&len));
			m_ListCtrlQueryResult.SetItemText(row,col,strData);
		}
	}
	m_ListCtrlQueryResult.SetRedraw(TRUE);

	CString strText;
	strText.Format(_T("查询到 %d 条数据"),nCount);
	SetDlgItemText(IDC_TEXT_TOTAL,strText);

	EndWaitCursor();
}


void CDatabaseDlg::OnNMClickListQueryResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nItem = pNMItemActivate->iItem;

	int nTableIndex = m_ComboBoxDataBase.GetCurSel();

	if (nItem != -1)
	{
		CRect rect;
		m_ThumbnailPic.GetClientRect(&rect);

		CString strPath;
		if (nTableIndex == 0)
		{
			strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);
		}
		else
		{
			strPath = m_ListCtrlQueryResult.GetItemText(nItem,SUB_PATH);
		}
		

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

	int nTableIndex = m_ComboBoxDataBase.GetCurSel();

	if (nItem != -1)
	{
		CString strPath;
		if (nTableIndex == 0)
		{
			strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);
		}
		else
		{
			strPath = m_ListCtrlQueryResult.GetItemText(nItem,SUB_PATH);
		}
		

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

	int nTableIndex = m_ComboBoxDataBase.GetCurSel();

	CString strPath;
	if (nTableIndex == 0)
	{
		strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);
	}
	else
	{
		strPath = m_ListCtrlQueryResult.GetItemText(nItem,SUB_PATH);
	}

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

		int nTableIndex = m_ComboBoxDataBase.GetCurSel();

		CString strPath;
		if (nTableIndex == 0)
		{
			strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);
		}
		else
		{
			strPath = m_ListCtrlQueryResult.GetItemText(nItem,SUB_PATH);
		}

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

	int nTableIndex = m_ComboBoxDataBase.GetCurSel();

	if (nTableIndex == -1)
	{
		*pResult = 0;
		return;
	}

	int nCount = m_ListCtrlQueryResult.GetItemCount();

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos),HIWORD(dwPos));

	CMenu menu;

	menu.LoadMenu(IDR_MENU_DATABASE);

	CMenu *popue = menu.GetSubMenu(0);

	if (nCount == 0)
	{
		popue->EnableMenuItem(ID_DATABASE_MODIFY,MF_BYCOMMAND | MF_GRAYED);
		popue->EnableMenuItem(ID_DATABASE_DEL,MF_BYCOMMAND | MF_GRAYED);
		popue->EnableMenuItem(ID_DATABASE_CLEAR,MF_BYCOMMAND | MF_GRAYED);
		popue->EnableMenuItem(ID_DATABASE_EXPORT,MF_BYCOMMAND | MF_GRAYED);
	}

	if (pNMItemActivate->iItem == -1)
	{
		popue->EnableMenuItem(ID_DATABASE_MODIFY,MF_BYCOMMAND | MF_GRAYED);
		popue->EnableMenuItem(ID_DATABASE_OPEN_PATH,MF_BYCOMMAND | MF_GRAYED);
		popue->EnableMenuItem(ID_DATABASE_DEL,MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		if (m_ListCtrlQueryResult.IsEditable() && m_nCurSelectItem == pNMItemActivate->iItem)
		{
			popue->ModifyMenu(ID_DATABASE_MODIFY,MF_BYCOMMAND,ID_DATABASE_MODIFY,_T("确认修改"));
		}
		else
		{
			m_ListCtrlQueryResult.SetEditable(FALSE);
			popue->ModifyMenu(ID_DATABASE_MODIFY,MF_BYCOMMAND,ID_DATABASE_MODIFY,_T("修改记录"));
		}
	}

	popue->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);

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

	CString strTable;
	m_ComboBoxDataBase.GetWindowText(strTable);

	int nItem = m_ListCtrlQueryResult.GetNextSelectedItem(pos);

	CString strID = m_ListCtrlQueryResult.GetItemText(nItem,0);

	if (IDYES == AfxMessageBox(_T("即将从数据库中删除此行记录，是否继续？"),MB_YESNO | MB_DEFBUTTON2))
	{
		int id = _ttoi(strID);

		if (m_pMyDatabase->DeleteRecord(id,strTable))
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

	CString strTable;
	m_ComboBoxDataBase.GetWindowText(strTable);

	if (IDYES == AfxMessageBox(_T("即将从数据库中删除查询到的所有记录，是否继续？"),MB_YESNO | MB_DEFBUTTON2))
	{
		for (int nItem = 0; nItem < nCount;nItem++)
		{
			CString strID = m_ListCtrlQueryResult.GetItemText(nItem,0);
			int id = _ttoi(strID);

			if (m_pMyDatabase->DeleteRecord(id,strTable))
			{
				m_ListCtrlQueryResult.DeleteItem(nItem);
				nItem--;
				nCount--;
			}
		}
	}
}


void CDatabaseDlg::OnCbnSelchangeComboDatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTable;
	m_ComboBoxDataBase.GetWindowText(strTable);

	m_ComboBoxFields.ResetContent();
	m_ListCtrlQueryCondition.DeleteAllItems();
	m_ListCtrlQueryResult.DeleteAllItems();
	while (m_ListCtrlQueryResult.DeleteColumn(0));

	if (strTable.CompareNoCase(CString(main_table_name)) == 0)
	{
		int nFields = m_pMyDatabase->QueryTableFields(main_table_name);
		for (int nItem = 0; nItem < nFields;nItem++)
		{
			unsigned int len = 255;
			CString strField = CString(m_pMyDatabase->GetStringValue(nItem,0,&len));
			m_ComboBoxFields.AddString(strField);

			m_ListCtrlQueryResult.InsertColumn(nItem,strField,LVCFMT_LEFT,main_fileds_width[nItem]);
		}
		
	}
	else
	{
		int nFields = m_pMyDatabase->QueryTableFields(sub_table_name);
		for (int nItem = 0; nItem < nFields;nItem++)
		{
			unsigned int len = 255;
			CString strField = CString(m_pMyDatabase->GetStringValue(nItem,0,&len));
			m_ComboBoxFields.AddString(strField);

			m_ListCtrlQueryResult.InsertColumn(nItem,strField,LVCFMT_LEFT,sub_fileds_width[nItem]);
		}
	}

	m_ListCtrlQueryResult.SetColumnType((fGetType)_List_Type);

	m_ListCtrlQueryResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
}


void CDatabaseDlg::OnDatabaseDrop()
{
	// TODO: 在此添加命令处理程序代码
	int nIndex = m_ComboBoxDataBase.GetCurSel();
	if (nIndex == -1)
	{
		return;
	}

	CString strTable;
	m_ComboBoxDataBase.GetLBText(nIndex,strTable);

	if (IDYES == AfxMessageBox(_T("即将删除表，是否继续？"),MB_YESNO | MB_DEFBUTTON2))
	{
		if (m_pMyDatabase->DeleteTable(strTable))
		{
			m_ListCtrlQueryResult.DeleteAllItems();
		}
		else
		{
			AfxMessageBox(_T("删除表失败！"));
		}
		
	}
}


void CDatabaseDlg::OnDatabaseClearAll()
{
	// TODO: 在此添加命令处理程序代码
	int nIndex = m_ComboBoxDataBase.GetCurSel();
	if (nIndex == -1)
	{
		return;
	}

	CString strTable;
	m_ComboBoxDataBase.GetLBText(nIndex,strTable);

	if (IDYES == AfxMessageBox(_T("即将清空表中所有记录，是否继续？"),MB_YESNO | MB_DEFBUTTON2))
	{
		if (m_pMyDatabase->CleanupTable(strTable))
		{
			m_ListCtrlQueryResult.DeleteAllItems();
		}
		else
		{
			AfxMessageBox(_T("删除表失败！"));
		}

	}
}


void CDatabaseDlg::OnDatabaseExport()
{
	// TODO: 在此添加命令处理程序代码
	int nCount = m_ListCtrlQueryResult.GetItemCount();
	int nTableIndex = m_ComboBoxDataBase.GetCurSel();

	if (nCount == 0)
	{
		return;
	}

	CString strFilter = _T("Excel Worksheet(*.xlsx)|*.xlsx|Excel 97-2003 Worksheet(*.xls)|*.xls||");

	CFileDialog dlg(FALSE,_T("xlsx"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter,this);

	if(dlg.DoModal() != IDOK) 
	{
		return; 
	}

	CString strStrFile = dlg.GetPathName();  //选择保存路径名称

	if(::PathFileExists(strStrFile)) 
	{
		DeleteFile(strStrFile); 
	}

	CMyExcel myExcel;

	if (!myExcel.Open())
	{
		AfxMessageBox(_T("打开Excel失败，请确认是否安装Excel！"));
		return;
	}

	BeginWaitCursor();

	MyAlignment Xalign;
	MyNumberFormat numberFormat;

	//对齐方式
	Xalign.HorizontalAlignment = xlLeft;
	Xalign.VerticalAlignment = xlTop;

	CTime time = CTime::GetCurrentTime();
	myExcel.AddSheet(time.Format(_T("%Y-%m-%d")));

	myExcel.SetAlignment(Xalign);

	// 获取表头
	LVCOLUMN lvcol = {0};
	TCHAR szBuffer[256] = {NULL};
	lvcol.mask = LVCF_TEXT;
	lvcol.pszText = szBuffer;
	lvcol.cchTextMax = 256;
	int nCol = 0; 

	while (m_ListCtrlQueryResult.GetColumn(nCol,&lvcol))
	{
		myExcel.SetItemText(1,nCol+1,lvcol.pszText);

		nCol++;
	}

	int nRows = m_ListCtrlQueryResult.GetItemCount();
	int nCols = m_ListCtrlQueryResult.GetHeaderCtrl()->GetItemCount();
	for (int row = 0; row < nRows;row++)
	{
		for (int col = 0;col < nCols;col++)
		{

			if (nTableIndex == 0)
			{
				switch (col)
				{
				case COL_ID:
				case COL_LEVEL:
				case COL_MODE:
				case COL_TIME:
				case COL_VOLTAGE:
				case COL_CURRENT:
					break;

				default:
					//文本格式
					{
						CString strBegin;
						strBegin.Format(_T("%c%d"),_T('A') + col,row+2);

						myExcel.GetRange(strBegin,strBegin);

						numberFormat.GetText();

						myExcel.SetNumberFormat(numberFormat);

						myExcel.AutoRange();
					}
				}
			}
			else
			{
				switch (col)
				{
				case SUB_MODULE_NO:
				case SUB_PRODUCT_TIME:
					{
						CString strBegin;
						strBegin.Format(_T("%c%d"),_T('A') + col,row+2);

						myExcel.GetRange(strBegin,strBegin);

						numberFormat.GetText();

						myExcel.SetNumberFormat(numberFormat);

						myExcel.AutoRange();
					}
					break;
				}
			}
			myExcel.SetItemText(row+2,col+1,m_ListCtrlQueryResult.GetItemText(row,col));
		}
	}

	myExcel.AutoRange();
	myExcel.AutoColFit();

	myExcel.SaveAs(strStrFile);
	myExcel.Exit();

	CString strParameters;
	strParameters.Format(_T(" /select,%s"),strStrFile);

	ShellExecute(NULL,_T("open"),_T("Explorer.exe"),strParameters,NULL,SW_SHOW);

	EndWaitCursor();
}


void CDatabaseDlg::OnDatabaseOpenPath()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_ListCtrlQueryResult.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		return;
	}

	int nItem = m_ListCtrlQueryResult.GetNextSelectedItem(pos);

	int nTableIndex = m_ComboBoxDataBase.GetCurSel();

	CString strPath;
	if (nTableIndex == 0)
	{
		strPath = m_ListCtrlQueryResult.GetItemText(nItem,COL_ORIGIN);
	}
	else
	{
		strPath = m_ListCtrlQueryResult.GetItemText(nItem,SUB_PATH);
	}

	strPath.Replace(_T('/'),_T('\\'));

	CString strParameters;
	strParameters.Format(_T(" /select,%s"),strPath);

	ShellExecute(NULL,_T("open"),_T("Explorer.exe"),strParameters,NULL,SW_SHOW);
}


void CDatabaseDlg::OnDatabaseModify()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_ListCtrlQueryResult.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	
	if (m_ListCtrlQueryResult.IsEditable())
	{
		//确认修改
		m_ListCtrlQueryResult.SetEditable(FALSE);
		int nTableIndex = m_ComboBoxDataBase.GetCurSel();

		if (nTableIndex == 0)
		{
			IMG_INFO imgInfo;
			imgInfo.id = _ttoi(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_ID));
			imgInfo.strProjectName = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_PROJECT);
			imgInfo.strProductName = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_PRODUCT);
			imgInfo.strSerialNumber = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_SERIAL);
			imgInfo.strProductSpec = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_SPEC);
			imgInfo.strCustomer = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_CUSTOMER);
			imgInfo.strDepartment = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_DEPT);
			imgInfo.strWorkerName = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_WORKER);
			imgInfo.level = _ttoi(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_LEVEL));
			imgInfo.mode = _ttoi(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_MODE));
			imgInfo.strPos = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_POS);

			CString strTime = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_TIME);

			int Y,m,d,H,M,S;
			_stscanf(strTime,_T("%d-%d-%d %d:%d:%d"),&Y,&m,&d,&H,&M,&S);
			imgInfo.time = CTime(Y,m,d,H,M,S);
			imgInfo.dbVolKV = _ttof(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_VOLTAGE));
			imgInfo.dbCurrentMA = _ttof(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_CURRENT));
			imgInfo.strOrignalPath = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_ORIGIN);
			imgInfo.strProcessPath = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_PROCESS);
			imgInfo.bResult = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_RESULT) == _T("OK");
			imgInfo.strErrorMsg = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,COL_MSG);

			m_pMyDatabase->UpdateData(&imgInfo);
		}
		else
		{
			SUB_TABLE sub;
			sub.id = _ttoi(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_ID));

			CString strTime = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_TIME);

			int Y,m,d,H,M,S;
			_stscanf(strTime,_T("%d-%d-%d %d:%d:%d"),&Y,&m,&d,&H,&M,&S);
			sub.time = CTime(Y,m,d,H,M,S);
			sub.strProductDate = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_PRODUCT_TIME);
			sub.strModuleNo = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_MODULE_NO);
			sub.strDefectLen = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_DEFECT_LEN);
			sub.level = _ttoi(m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_LEVEL));
			sub.strUser = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_USER);
			sub.strPath = m_ListCtrlQueryResult.GetItemText(m_nCurSelectItem,SUB_PATH);

			m_pMyDatabase->UpdateSubTable(sub);
		}

		m_nCurSelectItem = -1;
	}
	else
	{
		m_ListCtrlQueryResult.SetEditable(TRUE);
		m_nCurSelectItem = m_ListCtrlQueryResult.GetNextSelectedItem(pos);
	}
}


afx_msg LRESULT CDatabaseDlg::OnUserClick(WPARAM wParam, LPARAM lParam)
{
	OnNMClickListQueryResult((NMHDR *)wParam,(LRESULT *)lParam);
	return 0;
}
