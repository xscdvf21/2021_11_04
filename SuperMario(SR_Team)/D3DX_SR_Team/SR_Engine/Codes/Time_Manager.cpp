#include "Time_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTime_Manager)

CTime_Manager::CTime_Manager()
{
	ZeroMemory(&m_CPUCount, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_StartTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_GoalTime, sizeof(LARGE_INTEGER));

}

HRESULT CTime_Manager::Ready_Time_Manager()
{
	if (!QueryPerformanceCounter(&m_StartTime))
		return E_FAIL;

	if (!QueryPerformanceCounter(&m_GoalTime))
		return E_FAIL;

	if (!QueryPerformanceFrequency(&m_CPUCount))
		return E_FAIL;

	return S_OK;
}

_float CTime_Manager::Update_Time_Manager()
{
	if (!QueryPerformanceFrequency(&m_CPUCount))
		return 0.f;

	if (!QueryPerformanceCounter(&m_GoalTime))
		return 0.f;

	_float fDeltaTime = _float(m_GoalTime.QuadPart - m_StartTime.QuadPart) / m_CPUCount.QuadPart;
	m_StartTime.QuadPart = m_GoalTime.QuadPart;

	return fDeltaTime;
}

void CTime_Manager::Free()
{
}
