#pragma once
#ifndef HangLever_h__
#define HangLever_h__

class CHangLever
{
public:
	explicit CHangLever();
	virtual ~CHangLever();

public:
	void		Set_Cody(_bool bCody)	{ m_bCodyOn = bCody; }
	void		Set_May(_bool bMay)		{ m_bMayOn = bMay; }
	void		Set_LeverStart(_bool bStart) { m_bLeverStart = bStart; }

protected:
	_bool			m_bCodyOn	= false;
	_bool			m_bMayOn	= false;
	_bool			m_bLeverStart = false;
};

#endif