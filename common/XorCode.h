#pragma once
class CXorCode
{
public:
	CXorCode(void);
	~CXorCode(void);

	static CString Encode(CString strSource);
	static int Encode(CString strSource,PBYTE pByte);
	static CString Decode(CString strSource);
	static CString Decode(PBYTE pByte,int len);

protected:
	static int GetRand(double min,double max);
};

