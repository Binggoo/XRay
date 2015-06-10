
// COXRay.h : COXRay 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define FILE_FILTER _T("Support Files(*.jpg;*.jpeg;*.bmp;*.png;*.tif;*.tiff)|*.jpg;*.jpeg;*.bmp;*.png;*.tif;*.tiff|\
JPEG Files(*.jpg;*.jpeg)|*.jpg|\
BMP Files(*.bmp)|*.bmp|\
PNG Files(*.png)|*.png|\
TIFF Files(*.tif;*.tiff)|*.tif;*.tiff||")

#define CONFIG_NAME _T("\\COXRay.ini")
#define DEFAULT_SAVE_PATH _T("\\image")

#define	WM_USER_NEWIMAGE        (WM_USER + 1)
#define WM_GAMMA_CHANGE_PREVIEW (WM_USER + 2)
#define WM_GAMMA_CHANGE_ENSURE  (WM_USER + 3)
#define WM_ADD_GRAY_RANGE       (WM_USER + 4)

// CCOXRayApp:
// 有关此类的实现，请参阅 COXRay.cpp
//

class CCOXRayApp : public CWinApp
{
public:
	CCOXRayApp();

private:
	ULONG_PTR	m_gdiplusToken;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
};

extern CCOXRayApp theApp;
