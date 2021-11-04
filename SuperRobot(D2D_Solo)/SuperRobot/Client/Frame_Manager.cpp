#include "stdafx.h"
#include "Frame_Manager.h"
#include "Time_Manager.h"


CFrame_Manager::CFrame_Manager()
	:m_fFpsTime(0.f)
{
}


CFrame_Manager::~CFrame_Manager()
{
}

bool CFrame_Manager::Frame_Lock()
{
	QueryPerformanceCounter(&m_GoalTime);
	QueryPerformanceFrequency(&m_CPUCount);
	m_fDeltaTime += float(m_GoalTime.QuadPart - m_StartTime.QuadPart) / m_CPUCount.QuadPart; 
	m_StartTime.QuadPart = m_GoalTime.QuadPart; 
	if (m_fSPF < m_fDeltaTime)
	{
		m_fDeltaTime = 0.f; 
		++m_iFPS; 
		return true; 
	}
	return false;
}

void CFrame_Manager::Ready_Frame_Manager(float fFPS)
{
	m_fSPF = 1.f / fFPS; // 0.016f
	m_iFPS = 0; 
	m_fDeltaTime = 0.f; 
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
	QueryPerformanceCounter(&m_StartTime);
	QueryPerformanceCounter(&m_GoalTime);
	QueryPerformanceFrequency(&m_CPUCount);
}

void CFrame_Manager::Render_Frame_Manager()
{
	//SetWindowTextW(g_hWND, m_szFPS);
	m_fFpsTime += CTime_Manager::Get_Instance()->Get_TimeDelta(); 
	if (1.f <= m_fFpsTime)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_iFPS); 
		m_iFPS = 0; 
		m_fFpsTime =0.f; 
	}
	_matrix matTrans; 
	D3DXMatrixTranslation(&matTrans, 50.f, 50.f, 0.f); 
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans); 
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szFPS, lstrlen(m_szFPS), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0,255));
}
