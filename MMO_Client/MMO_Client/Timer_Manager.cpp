#include "pch.h"
#include "Timer_Manager.h"

CTimer_Manager* CTimer_Manager::m_pInstance = nullptr;

CTimer_Manager::CTimer_Manager()
	: m_fDeltaTime(0.f)
{
}

CTimer_Manager::~CTimer_Manager()
{
}


void CTimer_Manager::Initialize()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PrevTime);
}


void CTimer_Manager::Update()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	m_fDeltaTime =
		float(currentTime.QuadPart - m_PrevTime.QuadPart) /
		float(m_Frequency.QuadPart);

	m_PrevTime = currentTime;

	if (m_fDeltaTime > 0.1f)
		m_fDeltaTime = 0.1f;
}