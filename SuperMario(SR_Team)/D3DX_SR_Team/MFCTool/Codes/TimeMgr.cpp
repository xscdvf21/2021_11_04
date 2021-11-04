#include "stdafx.h"
#include "..\Headers\TimeMgr.h"


IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
	ZeroMemory(&m_CPUCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_StartTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_GoalTime, sizeof(LARGE_INTEGER));
}

HRESULT CTimeMgr::Ready_Time_Manager()
{
	if (!QueryPerformanceCounter(&m_StartTime))
		return E_FAIL;

	if (!QueryPerformanceCounter(&m_GoalTime))
		return E_FAIL;

	if (!QueryPerformanceFrequency(&m_CPUCount))
		return E_FAIL;

	return S_OK;
}

_float CTimeMgr::Update_Time_Manager()
{
	if (!QueryPerformanceFrequency(&m_CPUCount))
		return 0.f;

	if (!QueryPerformanceCounter(&m_GoalTime))
		return 0.f;

	_float fDeltaTime = _float(m_GoalTime.QuadPart - m_StartTime.QuadPart) / m_CPUCount.QuadPart;
	m_StartTime.QuadPart = m_GoalTime.QuadPart;

	return fDeltaTime;
}

void CTimeMgr::Free()
{
}
