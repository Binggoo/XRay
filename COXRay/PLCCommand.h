#pragma once
#define WM_SOCKET_MSG   (WM_USER + 1000)

typedef struct _STRUCT_PLC_HEAD
{
	BYTE SubHead[4]; //CMD - "5000" RSP - "D000"
	BYTE NetID[2];   //"00"
	BYTE PLCID[2];   //"FF"
	BYTE IOID[4];    //"03FF"
	BYTE TargetID[2];//"00"
	BYTE DataLen[4]; //从EndCode开始计算长度
	BYTE EndCode[4]; //CMD - CPU监视定时器“0010” RSP - 结束代码，“0000”正常，其他异常
}PLC_HEAD,*PPLC_HEAD;

class CPLCCommand
{
public:
	CPLCCommand(void);
	~CPLCCommand(void);

	void SetNotify(HWND hWnd) {m_hNotifyWnd = hWnd;}
	bool Connect(const char *ip,int port);
	bool Disconnect();
	bool ReadPLCD(int nPort,int &nLen,PDWORD pValues);
	bool WritePLCD(int nPort,int nLen,PDWORD pValues);

	BOOL IsConnected() {return m_ClientSocket != INVALID_SOCKET;}

	DWORD  m_dwLastError;
	ErrorType m_ErrorType;
private:
	SOCKET m_ClientSocket;
	HWND   m_hNotifyWnd;

	static BOOL Send( SOCKET socket,char *buf,DWORD &dwLen,LPWSAOVERLAPPED lpOverlapped,PDWORD pdwErrorCode,DWORD dwTimeout = 5000 );
	static BOOL Recv( SOCKET socket,char *buf,DWORD &dwLen,LPWSAOVERLAPPED lpOverlapped,PDWORD pdwErrorCode, DWORD dwTimeout = 5000);
};

