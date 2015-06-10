#include "StdAfx.h"
#include "LightControl.h"

CLightControl::CLightControl(void) : CSerialPort(false,0)
{
}


CLightControl::~CLightControl(void)
{
	CloseLight();
}

bool CLightControl::EnumLight()
{
	// 从注册表中获取系统串口
	CRegKey RegKey;   
	int nCount = 0;
	CString strPort;
	DWORD dwPortNum = -1;

	if(RegKey.Open(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm")) == ERROR_SUCCESS)   
	{   
		while(true)   
		{   
			TCHAR szValueName[MAX_PATH] = {NULL};   
			BYTE  byValueData[MAX_PATH] = {NULL};   
			DWORD dwValueSize = MAX_PATH;   
			DWORD dwDataSize = MAX_PATH;   
			DWORD nType;   

			if(::RegEnumValue(HKEY(RegKey), nCount, szValueName, &dwValueSize, NULL, &nType, byValueData, &dwDataSize) == ERROR_NO_MORE_ITEMS)   
			{   
				break;
			}   

			nCount++;

			strPort = (LPCTSTR)byValueData;

			dwPortNum = _ttoi(strPort.Mid(3));

			if (Open(dwPortNum))
			{
				if (QueryDevice())
				{
					return true;
				}
				else
				{
					Close();
				}
			}
			
		}   
	}   

	return false;   
}

bool CLightControl::OpenLight()
{
	// <STX>ENBLX<CR>
	char cmd[] = "ENBL1";

	int len = strlen(cmd) + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD(cmd,receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(cmd,receive) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::CloseLight()
{
	// <STX>ENBLX<CR>
	char cmd[] = "ENBL0";

	int len = strlen(cmd) + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD(cmd,receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(cmd,receive) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::SendCMD( const char *cmd,char *receive,int &len )
{
	char sendCmd[20] = {NULL};
	sprintf_s(sendCmd,"%c%s%c",0x02,cmd,0x0d);
	int nCmdLen = strlen(sendCmd);

	WaitForSingleObject(m_hEvent,INFINITE);
	DWORD dwWrite = WriteSync(sendCmd,nCmdLen);
// 	if (dwWrite == 0)
// 	{
// 		SetEvent(m_hEvent);
// 		return false;
// 	}

	Sleep(10);
	
	char *pRsp = new char[len + 1];
	memset(pRsp,0,len+1);

	DWORD dwRead = ReadSync(pRsp,len,1000);

	Sleep(10);

	SetEvent(m_hEvent);

	if (dwRead == 0 || (pRsp[dwRead - 1] != 0x0d && pRsp[0] != 0x02))
	{
		delete []pRsp;
		return false;
	}

	len = dwRead - 2;
	memcpy(receive,pRsp+1,len);

	receive[len] = '\0';

	delete []pRsp;

	return true;
}

bool CLightControl::QueryDevice()
{
	// <STX>XNNN<CR>
	int len = 6;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD("FREV",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp("2000",receive) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::SetVoltage( double dbVolKV )
{
	// <STX>VPXXXX<CR>
	char cmd[10] = {0};
	sprintf_s(cmd,"VP%04d",(int)(dbVolKV * 10));

	int len = strlen(cmd) + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD(cmd,receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(cmd,receive) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::SetCurrent( double dbCurmA )
{
	// <STX>CPXXXX<CR>
	char cmd[10] = {0};
	sprintf_s(cmd,"CP%04d",(int)(dbCurmA * 1000));

	int len = strlen(cmd) + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD(cmd,receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(cmd,receive) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::Monitor( double *dbVolKV,double *dbCurmA,double *dbTemperature,int *nFilement )
{
	// <STX>VVVV<SP>CCCC<SP>TTTT<SP>FFFF<CR>
	int len = 21;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD("MON",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	// 以空格分隔
	int nValues[4] = {0};
	int nCount = 0;
	char *token = strtok(receive," ");
	while (token != NULL && nCount < 4)
	{
		nValues[nCount] = atoi(token);
		token = strtok(NULL," ");
		nCount++;
	}
	delete []receive;

	*dbVolKV = nValues[0] / 10.0;
	*dbCurmA = nValues[1] / 1000.0;
	*dbTemperature = nValues[2] / 10.0;
	*nFilement = nValues[3];

	return true;
}

bool CLightControl::IsLightOn()
{
	// <STX>X<CR>
	int len = 3;
	char *receive = new char[len + 1];
	memset(receive,0,len + 1);

	bool bRet = SendCMD("STAT",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(receive,"1") != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::IsWatchDogOK()
{
	// <STX>OK<CR>
	int len = 4;
	char *receive = new char[len + 1];
	memset(receive,0,len + 1);

	bool bRet = SendCMD("WDTE",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(receive,"OK") != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::EnableWatchDog( int enable )
{
	// <STX>WDOGX<CR>
	char cmd[10] = {0};
	sprintf_s(cmd,"WDOG%d",enable);

	int len = strlen(cmd) + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len + 1);

	bool bRet = SendCMD(cmd,receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(receive,cmd) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::IsWatchDogEnable()
{
	// <STX>CLR<CR>
	char cmd[] = "CLR";
	int len = strlen(cmd) + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len + 1);

	bool bRet = SendCMD(cmd,receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(receive,cmd) != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::FaultClear()
{
	// <STX>CLR<CR>
	int len = 5;
	char *receive = new char[len + 1];
	memset(receive,0,len + 1);

	bool bRet = SendCMD("CLR",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(receive,"CLR") != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}

bool CLightControl::ReportFault( WORD *wFaultBits )
{
	// <STX>X<SP>X<SP>X<SP>X<SP>X<SP>X<SP>X<SP>X<SP>X<CR>
	int len = 9 + 8 + 2;
	char *receive = new char[len + 1];
	memset(receive,0,len+1);

	bool bRet = SendCMD("FLT",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	// 以空格分隔
	int nCount = 8;
	char *token = strtok(receive," ");
	WORD wValue = 0;
	while (token != NULL && nCount >= 0)
	{
		wValue |= (atoi(token) << nCount);
		token = strtok(NULL," ");
		nCount--;
	}
	delete []receive;

	*wFaultBits = wValue;

	return true;
}

DWORD CLightControl::ReadSync( char *szBuffer,DWORD dwBufferLength,DWORD dwTimeOut /*= 100*/ )
{
	if(!IsOpen())
	{
		return 0;
	}
	szBuffer[0] = '\0';

	COMSTAT  Stat;
	DWORD dwError;

	DWORD dwCount = dwTimeOut / 10;
	do 
	{
		if(::ClearCommError(m_hCommHandle, &dwError, &Stat) && dwError > 0) //清除错误
		{
			::PurgeComm(m_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR); /*清除输入缓冲区*/
			return 0;
		}

		if (Stat.cbInQue < dwBufferLength)
		{
			Sleep(10);
			dwCount--;
		}

	} while (Stat.cbInQue < dwBufferLength && dwCount > 0);
	
	unsigned long uReadLength = 0;

	if(!::ReadFile(m_hCommHandle, szBuffer, dwBufferLength, &uReadLength, NULL))
	{
		if(::GetLastError() != ERROR_IO_PENDING)
		{
			uReadLength = 0;
		}
	}

	return uReadLength;
}

bool CLightControl::SelCommand()
{
	// <STX>1<CR>
	int len = 5;
	char *receive = new char[len + 1];
	memset(receive,0,len + 1);

	bool bRet = SendCMD("SEL",receive,len);

	if (!bRet)
	{
		delete []receive;
		return false;
	}

	if (strcmp(receive,"1") != 0)
	{
		delete []receive;
		return false;
	}

	delete []receive;
	return true;
}
