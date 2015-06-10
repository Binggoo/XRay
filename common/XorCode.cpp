#include "StdAfx.h"
#include "XorCode.h"


CXorCode::CXorCode(void)
{
}


CXorCode::~CXorCode(void)
{
}

CString CXorCode::Encode( CString strSource )
{
	CString strReturn;

	int nRand = GetRand(1,0x20);
	TCHAR cRand = (TCHAR)(nRand ^ 0x27);

	CString strAscii;
	strAscii.Format(_T("%02X"),cRand);
	strReturn += strAscii;

	int nLen = strSource.GetLength();
	TCHAR tcEncode;
	for (int i = 0;i < nLen;i++)
	{
		tcEncode = (TCHAR)(strSource.GetAt(i)^nRand);
		strAscii.Format(_T("%02X"),tcEncode);		
		strReturn += strAscii;
	}

	return strReturn;
}

int CXorCode::Encode( CString strSource,PBYTE pByte )
{
	int len = 0;

	int nRand = GetRand(1,0x20);
	TCHAR cRand = (TCHAR)(nRand ^ 0x27);

	memcpy(pByte,&cRand,2);
	len += 2;

	int nLen = strSource.GetLength();
	TCHAR tcEncode;
	for (int i = 0;i < nLen;i++)
	{
		tcEncode = (TCHAR)(strSource.GetAt(i)^nRand);

		memcpy(&pByte[len],&tcEncode,2);

		len += 2;
	}

	return len;
}

CString CXorCode::Decode( CString strSource )
{
	CString strReturn;

	TCHAR cRand = (TCHAR)_tcstoul(strSource.Left(2),NULL,16);
	int nRand = cRand ^ 0x27;

	int nLen = strSource.GetLength();
	CString strAscii;
	TCHAR tcEncode;
	for (int i = 2;i<nLen;i += 2)
	{
		strAscii = strSource.Mid(i,2);
		tcEncode = (TCHAR)_tcstoul(strAscii,NULL,16);
		strReturn += (TCHAR)((tcEncode ^ nRand));
	}

	return strReturn;
}

CString CXorCode::Decode( PBYTE pByte,int len )
{
	CString strReturn;

	TCHAR cRand = *(TCHAR *)pByte;
	int nRand = cRand ^ 0x27;

	TCHAR tcEncode;
	for (int i = 2;i < len;i += 2)
	{
		tcEncode = *(TCHAR *)(&pByte[i]);
		strReturn += (TCHAR)((tcEncode ^ nRand));
	}

	return strReturn;
}

int CXorCode::GetRand( double min,double max )
{
	srand(GetTickCount());
	int randnum = (int)(rand() * (max-min)/RAND_MAX + max);
	return randnum;
}
