#pragma once
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PANE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PANE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PANE_EXPORTS
#define PANE_API __declspec(dllexport)
#else
#define PANE_API __declspec(dllimport)
#endif

#include "Acq.h"

#define ALLOC(dwBytes)			GlobalAlloc(GPTR,(dwBytes))

#define REALLOC(hMem, dwBytes)	GlobalReAlloc((hMem), (dwBytes), (GMEM_MOVEABLE|GMEM_ZEROINIT))

#define FREE(hMem)				GlobalFree((hMem))

// �Զ�����Ϣ
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
