
// COXRayDoc.cpp : CCOXRayDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "COXRay.h"
#endif

#include "MainFrm.h"
#include "COXRayDoc.h"
#include "COXRayView.h"

#include <propkey.h>

#include "RotateDlg.h"
#include "ThresholdDlg.h"
#include "FilterMeanDlg.h"
#include "FilterMedianDlg.h"
#include "FilterGaussDlg.h"
#include "FilterEnhanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCOXRayDoc

IMPLEMENT_DYNCREATE(CCOXRayDoc, CDocument)

BEGIN_MESSAGE_MAP(CCOXRayDoc, CDocument)
	ON_COMMAND(IDC_BTN_ZOOM_IN, &CCOXRayDoc::OnBtnZoomIn)
	ON_COMMAND(IDC_BTN_FIT_WINDOW, &CCOXRayDoc::OnBtnFitWindow)
	ON_COMMAND(IDC_BTN_ZOOM_OUT, &CCOXRayDoc::OnBtnZoomOut)
	ON_COMMAND(IDC_BTN_ZOOM_11, &CCOXRayDoc::OnBtnZoom11)
	ON_COMMAND(IDC_BTN_ROTATE_LEFT, &CCOXRayDoc::OnBtnRotateLeft)
	ON_COMMAND(IDC_BTN_ROTATE_RIGHT, &CCOXRayDoc::OnBtnRotateRight)
	ON_COMMAND(IDC_BTN_ROTATE, &CCOXRayDoc::OnBtnRotate)
	ON_COMMAND(IDC_BTN_MIRROR, &CCOXRayDoc::OnBtnMirror)
	ON_COMMAND(IDC_BTN_NEGATIVE, &CCOXRayDoc::OnBtnNegative)
	ON_COMMAND(IDC_BTN_FLIP, &CCOXRayDoc::OnBtnFlip)
	ON_COMMAND(IDC_BTN_MEAN, &CCOXRayDoc::OnBtnMean)
	ON_COMMAND(IDC_BTN_MEDIAN, &CCOXRayDoc::OnBtnMedian)
	ON_COMMAND(IDC_BTN_GAUSS, &CCOXRayDoc::OnBtnGauss)
	ON_COMMAND(ID_EDIT_UNDO, &CCOXRayDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CCOXRayDoc::OnUpdateEditUndo)
	ON_COMMAND(IDC_BTN_ENHANCE, &CCOXRayDoc::OnBtnEnhance)
END_MESSAGE_MAP()


// CCOXRayDoc 构造/析构

CCOXRayDoc::CCOXRayDoc()
{
	// TODO: 在此添加一次性构造代码
	m_pHWorkImage = NULL;
	m_pOriginImage = NULL;
	m_dbZoomFactor = 1;
	m_nChanels = 1;
}

CCOXRayDoc::~CCOXRayDoc()
{
	if (m_pHWorkImage != NULL)
	{
		delete m_pHWorkImage;
		m_pHWorkImage = NULL;
	}

	if (m_pOriginImage != NULL)
	{
		delete m_pOriginImage;
		m_pOriginImage = NULL;
	}

	ClearUndoDraw();
	ClearUndoImage();
	m_UndoType.RemoveAll();
}

BOOL CCOXRayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	SetTitle(_T("图像采集"));
	return TRUE;
}




// CCOXRayDoc 序列化

void CCOXRayDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CCOXRayDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CCOXRayDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CCOXRayDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCOXRayDoc 诊断

#ifdef _DEBUG
void CCOXRayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCOXRayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCOXRayDoc 命令


BOOL CCOXRayDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	USES_CONVERSION;
	char *filename = W2A(lpszPathName);

	if (m_pHWorkImage == NULL)
	{
		m_pHWorkImage = new HImage();
	}

	if (m_pOriginImage == NULL)
	{
		m_pOriginImage = new HImage();
	}

	// 清空Undo
	ClearUndoDraw();
	ClearUndoImage();
	m_UndoType.RemoveAll();

	m_Stopwatch.Start();
	try
	{
		*m_pHWorkImage = HImage::ReadImage(filename);
	}
	catch (HException &except)
	{
		CString strErrorMsg;

		strErrorMsg.Format(_T("读图像失败，请确认是否为图像格式！"));

		AfxMessageBox(strErrorMsg);
		return FALSE;
	}
	
	m_Stopwatch.Stop();

	// 备份原始图像
	*m_pOriginImage = *m_pHWorkImage;

	m_dbZoomFactor = 1;
	m_nChanels = m_pHWorkImage->CountChannels();

	// 转换为灰度图像
	if (m_nChanels > 1)
	{
		*m_pHWorkImage = m_pHWorkImage->Rgb1ToGray();
		m_nChanels = 1;
	}

	CString strTime,strInfo,strExt,strZoom;
	strExt = CUtils::GetFileExt(lpszPathName);
	strExt.MakeUpper();

	int bits = GetImageBits(m_pHWorkImage->GetImageType()[0].S());
	bits *= m_nChanels;

	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	strInfo.Format(_T("%s %dx%dx%d"),strExt,m_pHWorkImage->Width(),m_pHWorkImage->Height(),bits);
	strZoom.Format(_T("%.0f%%"),m_dbZoomFactor * 100);

	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_IMG_INFO);
	pStatusBar->SetPaneText(nIndex,strInfo);

	nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_ZOOM);
	pStatusBar->SetPaneText(nIndex,strZoom);

	//UpdateAllViews(NULL,WM_USER_NEWIMAGE);
	OnBtnFitWindow();

	return TRUE;
}

BOOL CCOXRayDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strExt = CUtils::GetFileExt(lpszPathName);
	CString strType = GetImageType(strExt);

	USES_CONVERSION;
	char *filename = W2A(lpszPathName);
	char *ext = W2A(strType);

	if (m_pHWorkImage)
	{
		HImage hImageTemp = m_pHWorkImage->CopyImage();

		int bits = GetImageBits(hImageTemp.GetImageType()[0].S());

		if (bits == 16 && strcmp(ext,"jpeg") == 0)
		{
			HTuple htPtr,htWidth,htHeight,htType;
			htPtr = hImageTemp.GetImagePointer1(&htType,&htWidth,&htHeight);

			int width = htWidth[0].L();
			int height = htHeight[0].L();

			IplImage *src = cvCreateImageHeader(cvSize(width,height),IPL_DEPTH_16U,1);
			cvSetData(src,(void *)htPtr[0].L(),width * 2);

			IplImage *dst = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
			
			cvConvertImage(src,dst);

			cvSaveImage(filename,dst);

			cvReleaseImageHeader(&src);
			cvReleaseImage(&dst);
		}
		else
		{
			hImageTemp.WriteImage(ext,0,filename);
		}
		
	}
	//return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

BOOL CCOXRayDoc::DoSave( LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/ )
{
	if (m_pHWorkImage == NULL)
	{
		return FALSE;
	}

	CString newName = pszPathName;
	BOOL bModified = IsModified();

	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty())
	{
		bSaveAs = TRUE;
	}

	if (bSaveAs)
	{
		CFileDialog dlg(FALSE,_T("jpg"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,FILE_FILTER);

		if (dlg.DoModal() == IDCANCEL)
		{
			return FALSE;
		}

		newName = dlg.GetPathName();
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			TRY 
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}
			END_CATCH_ALL
		}
		EndWaitCursor();
		return FALSE;
	}

	EndWaitCursor();
	if (bReplace) 
	{
		// Reset the title and change the document name
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName); // must be set
	}
	else // SaveCopyAs
	{
		SetModifiedFlag(bModified);
	}
	return TRUE;        // success
}

void CCOXRayDoc::OnBtnZoomIn()
{
	if (m_dbZoomFactor >= 10 || m_pHWorkImage == NULL)
	{
		return;
	}

	if (m_dbZoomFactor < 0.1)
	{
		m_dbZoomFactor += 0.01;
	}
	else
	{
		m_dbZoomFactor += 0.1;
	}

	CString strZoom;
	strZoom.Format(_T("%.0f%%"),m_dbZoomFactor * 100);
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_ZOOM);
	pStatusBar->SetPaneText(nIndex,strZoom);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnFitWindow()
{
	// TODO: 在此添加命令处理程序代码
	// 获取VIEW窗口大小
	FitWindow(TRUE);
	
}


void CCOXRayDoc::OnBtnZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	if (m_dbZoomFactor <= 0.01 + EPSINON || m_dbZoomFactor <= 0.01 - EPSINON|| m_pHWorkImage == NULL)
	{
		return;
	}

	if (m_dbZoomFactor <= 0.1 + EPSINON || m_dbZoomFactor <= 0.1 - EPSINON )
	{
		m_dbZoomFactor -= 0.01;
	}
	else
	{
		m_dbZoomFactor -= 0.1;
	}

	CString strZoom;
	strZoom.Format(_T("%.0f%%"),m_dbZoomFactor * 100);
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_ZOOM);
	pStatusBar->SetPaneText(nIndex,strZoom);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnZoom11()
{
	// TODO: 在此添加命令处理程序代码
	if (m_dbZoomFactor == 1 || m_pHWorkImage == NULL)
	{
		return;
	}

	m_dbZoomFactor = 1;

	CString strZoom;
	strZoom.Format(_T("%.0f%%"),m_dbZoomFactor * 100);
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_ZOOM);
	pStatusBar->SetPaneText(nIndex,strZoom);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnRotateLeft()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	SubmitUndoImage();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = RotateImage(Image,90);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);

}


void CCOXRayDoc::OnBtnRotateRight()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	SubmitUndoImage();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = RotateImage(Image,270);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnRotate()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	CRotateDlg dlg;

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	SubmitUndoImage();

	double dbRotateDegree = dlg.GetRotateDegree();
	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = RotateImage(Image,dbRotateDegree);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnMirror()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	SubmitUndoImage();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = FlipImage(Image,1);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnNegative()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	SubmitUndoImage();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = InvertImage(Image);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnFlip()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	SubmitUndoImage();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = FlipImage(Image,0);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

void CCOXRayDoc::OnBtnMean()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	long maskWidth,maskHeight;

	CFilterMeanDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	SubmitUndoImage();

	maskHeight = dlg.GetMaskHeight();
	maskWidth = dlg.GetMaskWidth();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = MeanImage(Image,maskWidth,maskHeight);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnMedian()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	CFilterMedianDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	SubmitUndoImage();

	int iMaskType = dlg.GetMaskType();
	long lMaskRadius = dlg.GetMaskRadius();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = MedianImage(Image,iMaskType,lMaskRadius);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnBtnGauss()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	CFilterGaussDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	SubmitUndoImage();

	long size = dlg.GetFilterSize();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = GaussImage(Image,size);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

void CCOXRayDoc::OnBtnEnhance()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pHWorkImage == NULL)
	{
		return;
	}

	CFilterEnhanceDlg dlg;
	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	SubmitUndoImage();

	long maskWidth = dlg.GetMaskWith();
	long maskHeight = dlg.GetMaskHeight();
	double dbFactor = dlg.GetFactor();

	m_Stopwatch.Start();
	HImage Image = m_pHWorkImage->CopyImage();
	*m_pHWorkImage = EmphasizeImage(Image,maskWidth,maskHeight,dbFactor);
//	HImage hImageHighPass = m_pHWorkImage->HighpassImage(maskWidth,maskHeight);
//	*m_pHWorkImage = m_pHWorkImage->AddImage(hImageHighPass,dbFactor,0);
	m_Stopwatch.Stop();

	CString strTime;
	strTime.Format(_T("Time: %.2f s"),m_Stopwatch.GetSecond());
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_TIME);
	pStatusBar->SetPaneText(nIndex,strTime);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

void CCOXRayDoc::SubmitUndoImage()
{
	if (m_UndoImage.GetCount() >= MAX_UNDO_LEVELS)
	{
		HImage *pImage = m_UndoImage.GetTail();
		if (pImage)
		{
			delete pImage;
		}

		m_UndoImage.RemoveTail();

		// 移除Image之后的所有Draw
		while (m_UndoType.GetCount() > 0)
		{
			int undo = m_UndoType.GetTail();
			m_UndoType.RemoveTail();	
			if (undo == UNDO_DRAW)
			{
				PDrawInfo pDraw = m_UndoDraw.GetTail();
				if (pDraw)
				{
					delete pDraw;
				}
				m_UndoDraw.RemoveTail();
			}
			else
			{	
				break;
			}
		} 
	}

	HImage *newImage = new HImage();
	*newImage = *m_pHWorkImage;

	m_UndoType.AddHead(UNDO_IMAGE);
	m_UndoImage.AddHead(newImage);
}


void CCOXRayDoc::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	if (m_UndoType.GetCount() == 0)
	{
		return;
	}

	int undo = m_UndoType.GetHead();

	if (undo == UNDO_DRAW)
	{
		PDrawInfo pDraw = m_UndoDraw.GetHead();
		if (pDraw)
		{
			delete pDraw;
		}
		m_UndoDraw.RemoveHead();
	}
	else
	{
		HImage *pImage = m_UndoImage.GetHead();

		if (pImage)
		{
			delete m_pHWorkImage;
			m_pHWorkImage = pImage;
		}

		m_UndoImage.RemoveHead();
	}

	m_UndoType.RemoveHead();

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}


void CCOXRayDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_UndoImage.GetCount() > 0);
}

void CCOXRayDoc::SetImage( HImage Image,BOOL bUpdateWindow/* = TRUE*/,BOOL bNew /*= TRUE*/ )
{
	if (!m_pHWorkImage)
	{
		m_pHWorkImage = new HImage();
	}

	if (!m_pOriginImage)
	{
		m_pOriginImage = new HImage();
	}

	if (bNew)
	{
		ClearUndoDraw();
		ClearUndoImage();
		m_UndoType.RemoveAll();

		*m_pOriginImage = Image.CopyImage();
	}
	else
	{
		SubmitUndoImage();
	}
	
	*m_pHWorkImage = Image.CopyImage();

	//UpdateAllViews(NULL,WM_USER_NEWIMAGE);
	FitWindow(bUpdateWindow);
}

void CCOXRayDoc::ClearUndoDraw()
{
	POSITION pos = m_UndoDraw.GetHeadPosition();
	while (pos)
	{
		PDrawInfo pDrawInfo = m_UndoDraw.GetNext(pos);

		if (pDrawInfo)
		{
			delete pDrawInfo;
		}
	}
	m_UndoDraw.RemoveAll();
}

void CCOXRayDoc::ClearUndoImage()
{
	POSITION pos = m_UndoImage.GetHeadPosition();
	while (pos)
	{
		HImage *pImage = m_UndoImage.GetNext(pos);

		if (pImage)
		{
			delete pImage;
		}
	}
	m_UndoImage.RemoveAll();
}

void CCOXRayDoc::SubmitUndoDraw( PDrawInfo pDrawInfo )
{
	m_UndoType.AddHead(UNDO_DRAW);
	m_UndoDraw.AddHead(pDrawInfo);
}

CString CCOXRayDoc::GetImageType( CString strExt )
{
	if (strExt.CompareNoCase(_T("JPG")) == 0 || strExt.CompareNoCase(_T("JEPG")) == 0)
	{
		return _T("jpeg");
	}
	else if (strExt.CompareNoCase(_T("TIF")) == 0 || strExt.CompareNoCase(_T("TIFF")) == 0)
	{
		return _T("tiff");
	}
	else
	{
		return strExt.MakeLower();
	}
}

void CCOXRayDoc::FitWindow( BOOL bUpdateWindow /*= TRUE*/ )
{
	CView *pView = ((CMainFrame *)AfxGetMainWnd())->GetActiveView();

	if (pView == NULL || m_pHWorkImage == NULL)
	{
		return;
	}

	CRect rect;
	pView->GetClientRect(&rect);

	double dbScaleWidth = 0.0,dbScaleHeight = 0.0;
	dbScaleWidth = (double)rect.Width() / m_pHWorkImage->Width();
	dbScaleHeight = (double)rect.Height() / m_pHWorkImage->Height();
	m_dbZoomFactor = min(dbScaleWidth,dbScaleHeight);

	CString strZoom;
	strZoom.Format(_T("%.0f%%"),m_dbZoomFactor * 100);
	CStatusBar *pStatusBar = ((CMainFrame *)AfxGetMainWnd())->GetStatusBar();

	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_ZOOM);
	pStatusBar->SetPaneText(nIndex,strZoom);

	if (bUpdateWindow)
	{
		UpdateAllViews(NULL,WM_USER_NEWIMAGE);
	}
}
