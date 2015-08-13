#pragma once

#include <windows.h>
#include <tchar.h>
#include <afxtempl.h>
#include <afx.h>

// #ifndef _DEBUG
// #define DbgPrintA(x)
// #define DbgPrintW(x)
// #else
#define DbgPrintA(x) _DbgPrintA x
#define DbgPrintW(x) _DbgPrintW x
//#endif

#ifndef _UNICODE
#define DbgPrint DbgPrintA
#else
#define DbgPrint DbgPrintW
#endif

void _DbgPrintA(char *szFormat,...);
void _DbgPrintW(wchar_t *szFormat,...);

class CUtils
{
public:
	CUtils(void);
	~CUtils(void);

	//////////////////////////////////////////////////////////////////////////
	// �ַ�ת��
	//////////////////////////////////////////////////////////////////////////
	/* UTF8 ת ASCII */
	static char *UTF8_TO_ANSI(const char *szUTF8);
	/* ANSI ת UTF8*/
	static char *ANSI_TO_UTF8(const char *szAnsi);
	static void ANSI_TO_UTF8(const char *szSrc,char *szDest,int dlen);

	static char *ANSI_TO_UTF8(const void *pAnsi,unsigned int *len);
	static int utf8_to_utf16(const BYTE *s,const USHORT ns,WCHAR *d,const USHORT nd);
	static int utf16_to_utf8(const WCHAR *s,const USHORT ns,BYTE *d,const USHORT nd);


	// ����·��
	static CString GetAppPath();
	static CString GetAppVersion(const CString strPath);
	static CString GetFilePathWithoutName(const CString strFilePath);
	static CString GetFileName(const CString strFilePath);
	static CString GetFileTitle(const CString strFilePath);
	static CString GetFileExt(const CString strFilePath);
	static CString GetErrorMsg(DWORD dwErrorCode);

	// WriteLog
	static void WriteLogFile(HANDLE hFile,BOOL bTimestamp,LPCTSTR lpszFormat,...);

	// ������С
	static CString GetDiskFreeAndTotalSize( CString strPath,PULARGE_INTEGER puiTotalSize,PULARGE_INTEGER puiUseSize);
	static CString AdjustFileSize( ULONGLONG ullFileSize );
	static CString AdjustSpeed( ULONGLONG ullSpeed );
	static ULONGLONG GetFolderSize(CString strPath);

	// �ļ���
	static BOOL CreateAllDirectories(CString strDir);
	static void DeleteDirectory( LPCTSTR lpszPath );

	static void TokenString(CString str,CString token,CStringArray *pstrArray);

};

