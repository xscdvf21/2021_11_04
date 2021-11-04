#pragma once
class CFrame_Manager
{
public:
	CFrame_Manager();
	~CFrame_Manager();
public:
	bool Frame_Lock();
	void Ready_Frame_Manager(float fFPS);
	void Render_Frame_Manager(); 

private:
	float			m_fSPF; // 1 / 60 = 0.016f
	
	float			m_fFpsTime; 
	float			m_fDeltaTime;
	LARGE_INTEGER	m_CPUCount;
	LARGE_INTEGER	m_StartTime;
	LARGE_INTEGER	m_GoalTime;

	int				m_iFPS;
	TCHAR			m_szFPS[64]; 
};

