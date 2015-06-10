#pragma once
#include <Windows.h>

class CStopwatch
{
public:
	CStopwatch(void);
	~CStopwatch(void);

	void Start();
	void Stop();
	double GetMicrosecond();
	double GetSecond();

private:
	LARGE_INTEGER m_swFreq, m_swStart, m_swStop;
};

