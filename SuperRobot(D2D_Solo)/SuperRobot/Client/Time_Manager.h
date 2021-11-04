#pragma once
class CTime_Manager
{
	DECLEARE_SINGLETON(CTime_Manager)
private:
	CTime_Manager();
	~CTime_Manager();
public:
	float Get_TimeDelta() { return m_fDeltaTime; }
public:
	void Ready_Time_Manager(); 
	void Update_Time_Manager(); 

private:
	float m_fDeltaTime ; 
	LARGE_INTEGER m_CPUCount;
	LARGE_INTEGER m_StartTime; 
	LARGE_INTEGER m_GoalTime; 

};

