#include "StdAfx.h"
#include "PLCCommand.h"
#include <mstcpip.h>

#define PLC_TITLE  _T("500000FF03FF00")
#define READ_PLCD  _T("500000FF03FF000018000A04010000D*%06d%04X")
#define WRITE_PLCD _T("500000FF03FF00%04X000A14010000D*%06d%04X")

CPLCCommand::CPLCCommand(void)
{
	m_ClientSocket = INVALID_SOCKET;
	m_dwLastError = 0;
	m_ErrorType = Error_System;
}

CPLCCommand::~CPLCCommand(void)
{
	Disconnect();
}

BOOL CPLCCommand::Send( SOCKET socket,char *buf,DWORD &dwLen,LPWSAOVERLAPPED lpOverlapped,PDWORD pdwErrorCode,DWORD dwTimeout /*= 5000 */ )
{
	WSAOVERLAPPED ol = {0};

	if (lpOverlapped)
	{
		ol = *lpOverlapped;
	}
	else
	{
		ol.hEvent = WSACreateEvent();
	}

	WSABUF wsaBuf;
	wsaBuf.buf = buf;
	wsaBuf.len = dwLen;

	DWORD dwRet = 0,dwFlags = 0;
	BOOL bResult = TRUE;

	dwRet = WSASend(socket,&wsaBuf,1,&dwLen,dwFlags,&ol,NULL);
	if (dwRet == SOCKET_ERROR)
	{
		*pdwErrorCode = WSAGetLastError();

		if (*pdwErrorCode != ERROR_IO_PENDING)
		{
			bResult = FALSE;
			goto END;
		}
	}

	dwRet = WSAWaitForMultipleEvents(1,&ol.hEvent,FALSE,dwTimeout,FALSE);

	if (dwRet == WSA_WAIT_FAILED || dwRet == WSA_WAIT_TIMEOUT)
	{
		*pdwErrorCode = WSAGetLastError();
		bResult = FALSE;
		goto END;
	}

	WSAResetEvent(ol.hEvent);

	if (!WSAGetOverlappedResult(socket,&ol,&dwLen,FALSE,&dwFlags))
	{
		*pdwErrorCode = WSAGetLastError();
		bResult = FALSE;
		goto END;
	}

	*pdwErrorCode = 0;


END:
	if (lpOverlapped == NULL)
	{
		WSACloseEvent(ol.hEvent);
	}

	return bResult;
}

BOOL CPLCCommand::Recv( SOCKET socket,char *buf,DWORD &dwLen,LPWSAOVERLAPPED lpOverlapped,PDWORD pdwErrorCode, DWORD dwTimeout /*= 5000*/ )
{
	WSAOVERLAPPED ol = {0};

	if (lpOverlapped)
	{
		ol = *lpOverlapped;
	}
	else
	{
		ol.hEvent = WSACreateEvent();
	}

	WSABUF wsaBuf;
	wsaBuf.buf = buf;
	wsaBuf.len = dwLen;

	DWORD dwRet = 0,dwFlags = 0;
	BOOL bResult = TRUE;

	dwRet = WSARecv(socket,&wsaBuf,1,&dwLen,&dwFlags,&ol,NULL);
	if (dwRet == SOCKET_ERROR)
	{
		*pdwErrorCode = WSAGetLastError();

		if (*pdwErrorCode != ERROR_IO_PENDING)
		{
			bResult = FALSE;
			goto END;
		}
	}

	dwRet = WSAWaitForMultipleEvents(1,&ol.hEvent,FALSE,dwTimeout,FALSE);

	if (dwRet == WSA_WAIT_FAILED || dwRet == WSA_WAIT_TIMEOUT)
	{
		*pdwErrorCode = WSAGetLastError();
		bResult = FALSE;
		goto END;
	}

	WSAResetEvent(ol.hEvent);

	if (!WSAGetOverlappedResult(socket,&ol,&dwLen,FALSE,&dwFlags))
	{
		*pdwErrorCode = WSAGetLastError();

		bResult = FALSE;
		goto END;
	}

	*pdwErrorCode = 0;

END:
	if (lpOverlapped == NULL)
	{
		WSACloseEvent(ol.hEvent);
	}

	return bResult;
}


bool CPLCCommand::Connect( const char *ip,int port )
{
	m_dwLastError = 0;
	m_ErrorType = Error_System;

	Disconnect();

	m_ClientSocket = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);

	if (m_ClientSocket == INVALID_SOCKET)
	{
		m_dwLastError = WSAGetLastError();
		return false;
	}

	SOCKADDR_IN ServerAddr = {0};
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(ip);
	ServerAddr.sin_port = htons(port);

	// 设置检测网络是否断开
	BOOL bKeepAlive = TRUE;
	if (setsockopt(m_ClientSocket,SOL_SOCKET,SO_KEEPALIVE,(char *)&bKeepAlive,sizeof(bKeepAlive)) == SOCKET_ERROR)
	{
		m_dwLastError = WSAGetLastError();
		
		closesocket(m_ClientSocket);

		m_ClientSocket = INVALID_SOCKET;

		return false;
	}

	// set KeepAlive parameter
	tcp_keepalive alive_in;
	tcp_keepalive alive_out;
	alive_in.keepalivetime    = 500;  // 0.5s
	alive_in.keepaliveinterval  = 1000; //1s
	alive_in.onoff                       = TRUE;
	unsigned long ulBytesReturn = 0;
	int nRet = WSAIoctl(m_ClientSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
	if (nRet == SOCKET_ERROR)
	{
		m_dwLastError = WSAGetLastError();
		
		closesocket(m_ClientSocket);

		m_ClientSocket = INVALID_SOCKET;

		return false;
	}

	WSAAsyncSelect(m_ClientSocket,m_hNotifyWnd,WM_SOCKET_MSG,FD_CLOSE | FD_CONNECT);

	//先判读host是否存在
// 	HOSTENT *host = gethostbyaddr((char *)&ServerAddr.sin_addr.s_addr,4,PF_INET);
// 
// 	if (host == NULL)
// 	{
// 		m_dwLastError = WSAGetLastError();
// 
// 		closesocket(m_ClientSocket);
// 
// 		m_ClientSocket = INVALID_SOCKET;
// 
// 		return false;
// 	}

	if (connect(m_ClientSocket,(PSOCKADDR)&ServerAddr,sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		DWORD dwErrorCode = WSAGetLastError();

		if (dwErrorCode != WSAEWOULDBLOCK)
		{
			m_dwLastError = dwErrorCode;
			closesocket(m_ClientSocket);

			m_ClientSocket = INVALID_SOCKET;

			return false;
		}

	}

	return true;
}

bool CPLCCommand::Disconnect()
{
	if (m_ClientSocket != INVALID_SOCKET)
	{
		closesocket(m_ClientSocket);
		m_ClientSocket = INVALID_SOCKET;
	}

	return true;
}

bool CPLCCommand::ReadPLCD( int nPort,int &nLen,PDWORD pValue )
{
	m_dwLastError = 0;
	m_ErrorType = Error_System;

	DWORD dwLen = 0,dwErrorCode;

	// 发送读Dx命令
	CString strRead;
	strRead.Format(READ_PLCD,nPort,nLen);

	USES_CONVERSION;
	char *pBufRead = W2A(strRead);

	dwLen = strlen(pBufRead);

	if (!Send(m_ClientSocket,pBufRead,dwLen,NULL,&dwErrorCode))
	{
		m_dwLastError = dwErrorCode;

		return false;
	}

	// 接收返回
	// 先接受返回头，查看是否有误
	PLC_HEAD plcHead = {0};
	dwLen = sizeof(PLC_HEAD);
	if (!Recv(m_ClientSocket,(char *)&plcHead,dwLen,NULL,&dwErrorCode))
	{
		m_dwLastError = dwErrorCode;

		return false;
	}

	// 获取数据包长度，16进制
	char szDataLen[5] = {0};
	memcpy(szDataLen,plcHead.DataLen,4);
	DWORD dwDataLen = strtoul(szDataLen,NULL,16);

	dwLen = dwDataLen - 4;
	char *pBufRev = new char[dwLen + 1];
	memset(pBufRev,0,dwLen + 1);

	if (dwLen > 0)
	{
		if (!Recv(m_ClientSocket,pBufRev,dwLen,NULL,&dwErrorCode))
		{
			m_dwLastError = dwErrorCode;

			delete []pBufRev;

			return false;
		}
	}

	// 返回代码
	char szEndCode[5] = {0};
	memcpy(szEndCode,plcHead.EndCode,4);
	DWORD dwEndCode = strtoul(szEndCode,NULL,16);
	if (dwEndCode != 0)
	{
		// 返回失败，读取剩余的数据
		m_ErrorType = Error_PLC;
		m_dwLastError = dwEndCode;

		delete []pBufRev;

		return false;
	}
	
	// 成功返回，获取数据
	nLen = (dwLen + 3) / 4; 
	for (int i = 0; i < nLen;i++)
	{
		char szData[5] = {0};
		memcpy(szData,pBufRev + i * 4,4);

		pValue[i] = strtoul(szData,NULL,16);
	}
	
	delete []pBufRev;

	return true;
}

bool CPLCCommand::WritePLCD( int nPort,int nLen,PDWORD pValues )
{
	m_dwLastError = 0;
	m_ErrorType = Error_System;

	DWORD dwLen = 0,dwErrorCode;

	// 发送读Dx命令
	dwLen = 0x18 + nLen * 4;
	CString strWrite;
	strWrite.Format(WRITE_PLCD,dwLen,nPort,nLen);

	for (int i = 0; i < nLen;i++)
	{
		CString strValue;
		strValue.Format(_T("%04X"),pValues[i]);
		strWrite += strValue;
	}

	USES_CONVERSION;
	char *pBufWrite = W2A(strWrite);

	dwLen = strlen(pBufWrite);

	if (!Send(m_ClientSocket,pBufWrite,dwLen,NULL,&dwErrorCode))
	{
		m_dwLastError = dwErrorCode;

		return false;
	}

	// 接收返回
	// 先接受返回头，查看是否有误
	PLC_HEAD plcHead = {0};
	dwLen = sizeof(PLC_HEAD);
	if (!Recv(m_ClientSocket,(char *)&plcHead,dwLen,NULL,&dwErrorCode))
	{
		m_dwLastError = dwErrorCode;

		return false;
	}

	// 获取数据包长度，16进制
	char szDataLen[5] = {0};
	memcpy(szDataLen,plcHead.DataLen,4);
	DWORD dwDataLen = strtoul(szDataLen,NULL,16);

	dwLen = dwDataLen - 4;
	char *pBufRev = new char[dwLen + 1];
	memset(pBufRev,0,dwLen + 1);

	if (dwLen > 0)
	{
		if (!Recv(m_ClientSocket,pBufRev,dwLen,NULL,&dwErrorCode))
		{
			m_dwLastError = dwErrorCode;

			delete []pBufRev;

			return false;
		}
	}

	// 返回代码
	char szEndCode[5] = {0};
	memcpy(szEndCode,plcHead.EndCode,4);
	DWORD dwEndCode = strtoul(szEndCode,NULL,16);
	if (dwEndCode != 0)
	{
		// 返回失败，读取剩余的数据
		m_ErrorType = Error_PLC;
		m_dwLastError = dwEndCode;

		delete []pBufRev;

		return false;
	}


	delete []pBufRev;

	return true;
}

