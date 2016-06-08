
#include "Time.h"
#include "../../Game/Core/stdafx.h"

CTime* CTime::m_pcInstance = nullptr;

CTime::CTime()
{
	m_dDeltaTime = 0.0;
	m_dFrequency = 0.0;
	m_nCurrentTime = 0;
	m_nPreviousTime = 0;
	m_nStartTime = 0;
}

CTime* CTime::GetInstance()
{
	if (m_pcInstance == nullptr)
		m_pcInstance = new CTime();

	return m_pcInstance;
}

void CTime::DeleteInstance()
{
	if (m_pcInstance != nullptr)
		SAFE_DELETE(m_pcInstance);
}

bool CTime::Initialize()
{
	//set frequency
	__int64 nFrequency;
	BOOL error = QueryPerformanceFrequency((LARGE_INTEGER*)&nFrequency);
	if (error == false)
	{
		DEBUG_OUTPUT("Time Class Failed to get Frequency!\n");
		return false;
	}

	m_dFrequency = MILISECOND_DIVISOR / (double)nFrequency;


	//set start time
	error = QueryPerformanceCounter((LARGE_INTEGER*)&m_nStartTime);
	if (error == false)
	{
		DEBUG_OUTPUT("Time Class Failed to get Start Time!\n");
		return false;
	}

	//start game time
	error = Update();
	if (error == false)
	{
		DEBUG_OUTPUT("Time Class Failed Initial Update!\n");
		return false;
	}

	return true;
}

bool CTime::Update()
{
	//set current time
	BOOL error = QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
	if (error == false)
	{
		DEBUG_OUTPUT("Time Class Failed to get Current Time!\n");
		return false;
	}

	//set delta time
	m_dDeltaTime = ((m_nCurrentTime - m_nPreviousTime) *m_dFrequency) / MILISECOND_DIVISOR;
	if (m_dDeltaTime > 0.1)
		m_dDeltaTime = 0.1;

	//set previous time
	m_nPreviousTime = m_nCurrentTime;

	return true;
}

float CTime::CurrentTime()
{
	//set current time
	BOOL error = QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
	if (error == false)
	{
		DEBUG_OUTPUT("Time Class Failed to get Current Time!\n");
		return 0.0f;
	}

	//return current time
	return (float)(((m_nCurrentTime - m_nStartTime) * m_dFrequency) / MILISECOND_DIVISOR);
}
