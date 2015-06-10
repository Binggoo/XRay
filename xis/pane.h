#pragma once
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PANE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PANE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef PANE_EXPORTS
#define PANE_API __declspec(dllexport)
#else
#define PANE_API __declspec(dllimport)
#endif

#include "Acq.h"

#define ALLOC(dwBytes)			GlobalAlloc(GPTR,(dwBytes))

#define REALLOC(hMem, dwBytes)	GlobalReAlloc((hMem), (dwBytes), (GMEM_MOVEABLE|GMEM_ZEROINIT))

#define FREE(hMem)				GlobalFree((hMem))

// 自定义消息
#define WM_ACQ_SNAP_END_FRAME (WM_USER + 100)
#define WM_ACQ_CONT_END_FRAME (WM_USER + 101)
#define WM_ACQ_GAIN_END_FRAME (WM_USER + 102)
#define WM_ACQ_OFFSET_END_FRAME (WM_USER + 103)
#define WM_ACQ_Brightoc_END_FRAME (WM_USER + 104)



#define WM_ACQ_SNAP_END_ACQ (WM_USER + 110)
#define WM_ACQ_CONT_END_ACQ (WM_USER + 111)
#define WM_ACQ_GAIN_END_ACQ (WM_USER + 112)
#define WM_ACQ_OFFSET_END_ACQ (WM_USER + 113)
#define WM_ACQ_Brightoc_END_ACQ (WM_USER + 114)

#define ACQ_CONT			1
#define ACQ_OFFSET			2
#define ACQ_GAIN			4
#define ACQ_SNAP			8
#define	ACQ_Brightoc		16

typedef struct _STRUCT_ACQ_DATA
{
	PVOID pData;
	BYTE  byBytes;
	UINT  nRows;
	UINT  nColumns;
	DWORD dwDataType;
	DWORD dwActAcqFrame;
	DWORD dwActSecBufferFrame;
}ACQ_DATA,*PACQ_DATA;

class PANE_API CXISLFunction
{
public:
	CXISLFunction(void);
	~CXISLFunction(void);

	UINT InitDetector(int iSelectIndex);
	UINT GetSensorsCount();
	UINT GetFrameWidthHeight(UINT *pnWidth,UINT *pnHeight);
	void SetNotify(HWND hWnd);
	UINT InitalAcquisition(DWORD dwFrames,DWORD dwAcqType,UINT nOption);
	UINT GetFrameTimes(double *pdbIntTimes, int *nIntTimes);
	UINT SetCameraSeting(DWORD dwCameraMode,DWORD dwTimes);
	void SetOffsetBuffer(PVOID pBuffer,DWORD dwLen);
	void SetGainBuffer(PVOID pBuffer,DWORD dwLen);
	void SetPixelBuffer(PVOID pBuffer,DWORD dwLen);

	UINT StartContinueCapture();
	UINT StopContinueCapture();

	UINT IntialOffset(DWORD dwFrames,UINT nOption);

	UINT IntialGain(DWORD dwFrames,UINT nOption);

	void Close();

private:
	static void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc);
	static void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc);
};
