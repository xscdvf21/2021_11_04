#pragma once
#include "../../Reference/Headers/Base.h"
USING(Engine)
class CTimeMgr:public CBase
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	virtual ~CTimeMgr() = default;

public:
	HRESULT Ready_Time_Manager();
	_float Update_Time_Manager();

public:
	virtual void Free() override;

private:
	LARGE_INTEGER m_CPUCount;
	LARGE_INTEGER m_StartTime;
	LARGE_INTEGER m_GoalTime;

};

