#include "StdAfx.h"
#include "Stopwatch.h"


CStopwatch::CStopwatch(void)
{
	QueryPerformanceFrequency(&m_swFreq);
}


CStopwatch::~CStopwatch(void)
{
}

void CStopwatch::Start()
{
	QueryPerformanceCounter(&m_swStart);
}

void CStopwatch::Stop()
{
	QueryPerformanceCounter(&m_swStop);
}

double CStopwatch::GetMicrosecond()
{
	return double(m_swStop.QuadPart - m_swStart.QuadPart)/m_swFreq.QuadPart * 1000;
}

double CStopwatch::GetSecond()
{
	return double(m_swStop.QuadPart - m_swStart.QuadPart)/m_swFreq.QuadPart;
}
