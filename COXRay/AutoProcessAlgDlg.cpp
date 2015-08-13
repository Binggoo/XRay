// AutoProcessAlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COXRay.h"
#include "AutoProcessAlgDlg.h"
#include "afxdialogex.h"

// CAutoProcessAlgDlg 对话框

IMPLEMENT_DYNAMIC(CAutoProcessAlgDlg, CDialogEx)

CAutoProcessAlgDlg::CAutoProcessAlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoProcessAlgDlg::IDD, pParent)
{
	m_pvImgProcessAlgs = NULL;
}

CAutoProcessAlgDlg::~CAutoProcessAlgDlg()
{
}

void CAutoProcessAlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxAlg);
	DDX_Control(pDX, IDC_LIST2, m_ListBoxApply);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_PGCArgs);
}


BEGIN_MESSAGE_MAP(CAutoProcessAlgDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CAutoProcessAlgDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CAutoProcessAlgDlg::OnBnClickedBtnRemove)
	ON_BN_CLICKED(IDC_BTN_UP, &CAutoProcessAlgDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CAutoProcessAlgDlg::OnBnClickedBtnDown)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAutoProcessAlgDlg::OnLbnSelchangeList2)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


// CAutoProcessAlgDlg 消息处理程序


BOOL CAutoProcessAlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT(m_pvImgProcessAlgs != NULL);

	// TODO:  在此添加额外的初始化
	int nItem = m_ListBoxAlg.AddString(_T("旋转"));
	m_ListBoxAlg.SetItemData(nItem,IMG_ROTATE);

	nItem = m_ListBoxAlg.AddString(_T("增强"));
	m_ListBoxAlg.SetItemData(nItem,IMG_ENHANCE);

	nItem = m_ListBoxAlg.AddString(_T("GAMMA校正"));
	m_ListBoxAlg.SetItemData(nItem,IMG_GAMMA);

	nItem = m_ListBoxAlg.AddString(_T("负片"));
	m_ListBoxAlg.SetItemData(nItem,IMG_INVERT);

	nItem = m_ListBoxAlg.AddString(_T("高斯"));
	m_ListBoxAlg.SetItemData(nItem,IMG_GAUSS);

	nItem = m_ListBoxAlg.AddString(_T("均值"));
	m_ListBoxAlg.SetItemData(nItem,IMG_MEAN);

	nItem = m_ListBoxAlg.AddString(_T("中值"));
	m_ListBoxAlg.SetItemData(nItem,IMG_MEDIAN);


	VImgProcessAlgs::iterator it;
	BOOST_FOREACH (ShareImgProcessAlgs alg,*m_pvImgProcessAlgs)
	{
		int nAlg = alg->nAlg;
		CString strItem;
		switch (nAlg)
		{
		case IMG_ROTATE:
			strItem = _T("旋转");
			break;

		case IMG_ENHANCE:
			strItem = _T("增强");
			break;

		case IMG_GAMMA:
			strItem = _T("GAMMA校正");
			break;

		case IMG_INVERT:
			strItem = _T("负片");
			break;

		case IMG_GAUSS:
			strItem = _T("高斯");
			break;

		case IMG_MEAN:
			strItem = _T("均值");
			break;

		case IMG_MEDIAN:
			strItem = _T("中值");
			break;
		}

		nItem = m_ListBoxApply.AddString(strItem);
		m_ListBoxApply.SetItemData(nItem,nAlg);
	}

	m_PGCArgs.EnableHeaderCtrl(TRUE,_T("参数"),_T("值"));
	m_PGCArgs.EnableDescriptionArea(FALSE);
	
	CRect rect;
	m_PGCArgs.GetClientRect(&rect);
	HDITEM item;
	item.cxy = rect.Width() / 2;
	item.mask = HDI_WIDTH;
	m_PGCArgs.GetHeaderCtrl().SetItem(0,&item);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAutoProcessAlgDlg::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_ListBoxAlg.GetCurSel();

	if (nItem == -1)
	{
		return;
	}

	int nIPIndex = m_ListBoxAlg.GetItemData(nItem);

	CString strText;
	m_ListBoxAlg.GetText(nItem,strText);

	nItem = m_ListBoxApply.AddString(strText);
	m_ListBoxApply.SetItemData(nItem,nIPIndex);

	m_ListBoxApply.SetCurSel(nItem);

	AddImgProcessAlg(nIPIndex);

	OnLbnSelchangeList2();
}


void CAutoProcessAlgDlg::OnBnClickedBtnRemove()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_ListBoxApply.GetCurSel();

	if (nItem == -1)
	{
		return;
	}

	m_ListBoxApply.DeleteString(nItem);

	RemoveImgProcessAlg(nItem);

	if (m_PGCArgs.GetPropertyCount() > 0)
	{
		m_PGCArgs.RemoveAll();
		m_PGCArgs.Invalidate();
	}

	m_nLastSel = -1;
}


void CAutoProcessAlgDlg::OnBnClickedBtnUp()
{
	// TODO: 在此添加控件通知处理程序代码

	int nItem = m_ListBoxApply.GetCurSel();

	if (nItem == -1 || nItem == 0)
	{
		return;
	}

	MoveImgProcessAlg(nItem,nItem - 1);

	int nIPIndex = m_ListBoxApply.GetItemData(nItem);

	CString strText;
	m_ListBoxApply.GetText(nItem,strText);

	m_ListBoxApply.DeleteString(nItem);

	nItem = m_ListBoxApply.InsertString(nItem-1,strText);
	m_ListBoxApply.SetItemData(nItem,nIPIndex);
	m_ListBoxApply.SetCurSel(nItem);
}


void CAutoProcessAlgDlg::OnBnClickedBtnDown()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_ListBoxApply.GetCurSel();

	if (nItem == -1 || nItem == m_ListBoxApply.GetCount() - 1)
	{
		return;
	}

	MoveImgProcessAlg(nItem,nItem + 1);

	int nIPIndex = m_ListBoxApply.GetItemData(nItem);

	CString strText;
	m_ListBoxApply.GetText(nItem,strText);

	m_ListBoxApply.DeleteString(nItem);

	nItem = m_ListBoxApply.InsertString(nItem+1,strText);
	m_ListBoxApply.SetItemData(nItem,nIPIndex);

	m_ListBoxApply.SetCurSel(nItem);
}


void CAutoProcessAlgDlg::OnLbnSelchangeList2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCulSel = m_ListBoxApply.GetCurSel();

	if (nCulSel == -1 || m_nLastSel == nCulSel)
	{
		return;
	}

	if (m_PGCArgs.GetPropertyCount() > 0)
	{
		m_PGCArgs.RemoveAll();
		m_PGCArgs.Invalidate();
	}

	m_nLastSel = nCulSel;

	ShareImgProcessAlgs alg = m_pvImgProcessAlgs->at(nCulSel);

	int nIndex = m_ListBoxApply.GetItemData(nCulSel);

	switch (nIndex)
	{
	case IMG_ROTATE:
		{
			CString strKey,strValue;
			strKey = alg->vAlgs[0]->strKey;
			strValue = alg->vAlgs[0]->strValue;

			CMFCPropertyGridProperty *pProp1 = new CMFCPropertyGridProperty(strKey,strValue,NULL,0);

			pProp1->AddOption(_T("0"));
			pProp1->AddOption(_T("90"));
			pProp1->AddOption(_T("180"));
			pProp1->AddOption(_T("270"));

			pProp1->AllowEdit(FALSE);

			m_PGCArgs.AddProperty(pProp1);

		}
		break;

	case IMG_ENHANCE:
		{
			//Mask Width
			CString strKey,strValue;
			strKey = alg->vAlgs[0]->strKey;
			strValue = alg->vAlgs[0]->strValue;

			CMFCPropertyGridProperty *pProp1 = new CMFCPropertyGridProperty(strKey,strValue,NULL,0);

			pProp1->AddOption(_T("3"));
			pProp1->AddOption(_T("5"));
			pProp1->AddOption(_T("7"));
			pProp1->AddOption(_T("11"));

			pProp1->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp1);

			//Mask Height
			strKey = alg->vAlgs[1]->strKey;
			strValue = alg->vAlgs[1]->strValue;

			CMFCPropertyGridProperty *pProp2 = new CMFCPropertyGridProperty(strKey,strValue,NULL,1);

			pProp2->AddOption(_T("3"));
			pProp2->AddOption(_T("5"));
			pProp2->AddOption(_T("7"));
			pProp2->AddOption(_T("11"));

			pProp2->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp2);

			//Factor
			strKey = alg->vAlgs[2]->strKey;
			strValue = alg->vAlgs[2]->strValue;

			CMFCPropertyGridProperty *pProp3 = new CMFCPropertyGridProperty(strKey,strValue,NULL,2);

			pProp3->AddOption(_T("0.3"));
			pProp3->AddOption(_T("0.5"));
			pProp3->AddOption(_T("0.7"));
			pProp3->AddOption(_T("1.0"));
			pProp3->AddOption(_T("1.4"));
			pProp3->AddOption(_T("1.8"));
			pProp3->AddOption(_T("2.0"));

			pProp3->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp3);

			//Times
			strKey = alg->vAlgs[3]->strKey;
			strValue = alg->vAlgs[3]->strValue;

			CMFCPropertyGridProperty *pProp4 = new CMFCPropertyGridProperty(strKey,strValue,NULL,3);

			pProp4->AddOption(_T("0"));
			pProp4->AddOption(_T("1"));
			pProp4->AddOption(_T("2"));
			pProp4->AddOption(_T("3"));

			pProp4->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp4);

		}
		break;

	case IMG_GAMMA:
		{
			CString strKey,strValue;
			strKey = alg->vAlgs[0]->strKey;
			strValue = alg->vAlgs[0]->strValue;

			CMFCPropertyGridProperty *pProp1 = new CMFCPropertyGridProperty(strKey,strValue,NULL,0);

			m_PGCArgs.AddProperty(pProp1);

		}
		break;

	case IMG_INVERT:
		{
		}
		break;

	case IMG_GAUSS:
		{
			CString strKey,strValue;
			strKey = alg->vAlgs[0]->strKey;
			strValue = alg->vAlgs[0]->strValue;

			CMFCPropertyGridProperty *pProp1 = new CMFCPropertyGridProperty(strKey,strValue,NULL,0);

			pProp1->AddOption(_T("3"));
			pProp1->AddOption(_T("5"));
			pProp1->AddOption(_T("7"));
			pProp1->AddOption(_T("11"));

			pProp1->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp1);

		}
		break;

	case IMG_MEAN:
		{
			//Mask Width
			CString strKey,strValue;
			strKey = alg->vAlgs[0]->strKey;
			strValue = alg->vAlgs[0]->strValue;

			CMFCPropertyGridProperty *pProp1 = new CMFCPropertyGridProperty(strKey,strValue,NULL,0);

			pProp1->AddOption(_T("3"));
			pProp1->AddOption(_T("5"));
			pProp1->AddOption(_T("7"));
			pProp1->AddOption(_T("11"));

			pProp1->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp1);

			//Mask Height
			strKey = alg->vAlgs[1]->strKey;
			strValue = alg->vAlgs[1]->strValue;

			CMFCPropertyGridProperty *pProp2 = new CMFCPropertyGridProperty(strKey,strValue,NULL,1);

			pProp2->AddOption(_T("3"));
			pProp2->AddOption(_T("5"));
			pProp2->AddOption(_T("7"));
			pProp2->AddOption(_T("11"));

			pProp2->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp2);
		}
		break;

	case IMG_MEDIAN:
		{
			//Mask Type
			CString strKey,strValue;
			strKey = alg->vAlgs[0]->strKey;
			strValue = CString(Mask_Type[alg->vAlgs[0]->GetInt()]);

			CMFCPropertyGridProperty *pProp1 = new CMFCPropertyGridProperty(strKey,strValue,NULL,0);

			pProp1->AddOption(CString(Mask_Type[0]));
			pProp1->AddOption(CString(Mask_Type[1]));

			pProp1->AllowEdit(FALSE);

			m_PGCArgs.AddProperty(pProp1);

			//Mask Height
			strKey = alg->vAlgs[1]->strKey;
			strValue = alg->vAlgs[1]->strValue;

			CMFCPropertyGridProperty *pProp2 = new CMFCPropertyGridProperty(strKey,strValue,NULL,1);

			pProp2->AddOption(_T("1"));
			pProp2->AddOption(_T("2"));
			pProp2->AddOption(_T("3"));
			pProp2->AddOption(_T("4"));
			pProp2->AddOption(_T("5"));
			pProp2->AddOption(_T("6"));
			pProp2->AddOption(_T("7"));

			pProp2->AllowEdit(TRUE);

			m_PGCArgs.AddProperty(pProp2);
		}
		break;
	}
}

void CAutoProcessAlgDlg::SetImgProcessAlg( PVImgProcessAlgs pV )
{
	m_pvImgProcessAlgs = pV;
}

void CAutoProcessAlgDlg::AddImgProcessAlg( int nIndex )
{
	ShareImgProcessAlgs alg(new ImgProcessAlgs());

	alg->nAlg = nIndex;

	switch (nIndex)
	{
	case IMG_ROTATE:
		{
			ShareKeyValue keyValue(new KeyValue);
			keyValue->strKey = _T("旋转角度");
			keyValue->strValue = _T("0");

			alg->vAlgs.push_back(keyValue);
		}
		

		break;

	case IMG_ENHANCE:
		{
			ShareKeyValue keyValue1(new KeyValue);
			keyValue1->strKey = _T("Mask宽度");
			keyValue1->strValue = _T("3");
			alg->vAlgs.push_back(keyValue1);

			ShareKeyValue keyValue2(new KeyValue);
			keyValue2->strKey = _T("Mask高度");
			keyValue2->strValue = _T("3");
			alg->vAlgs.push_back(keyValue2);

			ShareKeyValue keyValue3(new KeyValue);
			keyValue3->strKey = _T("强度系数");
			keyValue3->strValue = _T("1.0");
			alg->vAlgs.push_back(keyValue3);

			ShareKeyValue keyValue4(new KeyValue);
			keyValue4->strKey = _T("增强次数");
			keyValue4->strValue = _T("1");
			alg->vAlgs.push_back(keyValue4);
		}
		

		break;

	case IMG_GAMMA:
		{
			ShareKeyValue keyValue(new KeyValue);
			keyValue->strKey = _T("GAMMA系数");
			keyValue->strValue = _T("1.0");
			alg->vAlgs.push_back(keyValue);
		}
		break;

	case IMG_INVERT:
		break;

	case IMG_GAUSS:
		{
			ShareKeyValue keyValue(new KeyValue);
			keyValue->strKey = _T("滤波器大小");
			keyValue->strValue = _T("3");
			alg->vAlgs.push_back(keyValue);
		}
		break;

	case IMG_MEAN:
		{
			ShareKeyValue keyValue1(new KeyValue);
			keyValue1->strKey = _T("Mask宽度");
			keyValue1->strValue = _T("3");
			alg->vAlgs.push_back(keyValue1);

			ShareKeyValue keyValue2(new KeyValue);
			keyValue2->strKey = _T("Mask高度");
			keyValue2->strValue = _T("3");
			alg->vAlgs.push_back(keyValue2);
		}
		break;

	case IMG_MEDIAN:
		{
			ShareKeyValue keyValue1(new KeyValue);
			keyValue1->strKey = _T("Mask类型");
			keyValue1->strValue = _T("1");
			alg->vAlgs.push_back(keyValue1);

			ShareKeyValue keyValue2(new KeyValue);
			keyValue2->strKey = _T("Mask半径");
			keyValue2->strValue = _T("3");
			alg->vAlgs.push_back(keyValue2);
		}
		break;
	}

	m_pvImgProcessAlgs->push_back(alg);
}

void CAutoProcessAlgDlg::RemoveImgProcessAlg( int nItem )
{
	ASSERT(nItem < m_pvImgProcessAlgs->size());

	m_pvImgProcessAlgs->erase(m_pvImgProcessAlgs->begin() + nItem);
}

void CAutoProcessAlgDlg::MoveImgProcessAlg( int from,int to )
{
	ShareImgProcessAlgs alg = m_pvImgProcessAlgs->at(from);

	m_pvImgProcessAlgs->erase(m_pvImgProcessAlgs->begin() + from);

	m_pvImgProcessAlgs->insert(m_pvImgProcessAlgs->begin() + to,alg);

}

LRESULT CAutoProcessAlgDlg::OnPropertyChanged( WPARAM wParam,LPARAM lParam )
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;

	int nCurItem = m_ListBoxApply.GetCurSel();

	DWORD id = pProp->GetData();

	COleVariant var = pProp->GetValue();

	ShareImgProcessAlgs alg = m_pvImgProcessAlgs->at(nCurItem);

	alg->vAlgs[id]->strValue = var.bstrVal;

	if (alg->nAlg == IMG_MEDIAN)
	{
		int index = 0;
		if (alg->vAlgs[id]->strValue == CString(Mask_Type[1]))
		{
			index = 1;
		}

		alg->vAlgs[id]->strValue.Format(_T("%d"),index);
	}

	return 1;
}
