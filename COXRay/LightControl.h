#pragma once
#include "../common/SerialPort.h"

class CLightControl : public CSerialPort
{
public:
	CLightControl(void);
	~CLightControl(void);

	bool EnumLight();
	bool OpenLight();
	bool CloseLight();
	bool QueryDevice();
	bool SetVoltage(double dbVolKV);
	bool SetCurrent(double dbCurmA);
	bool Monitor(double *dbVolKV,double *dbCurmA,double *dbTemperature,int *nFilement);
	bool IsLightOn();
	bool IsWatchDogOK();
	bool EnableWatchDog(int enable);
	bool IsWatchDogEnable();
	bool FaultClear();
	bool ReportFault(WORD *wFaultBits);
	bool SelCommand();

protected:
	bool SendCMD(const char *cmd,char *receive,int &len);
	DWORD ReadSync(char *szBuffer,DWORD dwBufferLength,DWORD dwTimeOut = 100);
};

