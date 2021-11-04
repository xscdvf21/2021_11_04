#pragma once
#ifndef __CSTAGE2_H__
#define __CSTAGE2_H__


#include "Scene.h"
class CStage2 : public CScene
{
public:
	CStage2();
	virtual ~CStage2();

public :
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;


	int&	Get_life() { return m_life; }
private :
	int		m_life;
	bool	m_bOne; //한번만 쓰게하기위해

	DWORD	m_dwtime;
};

#endif // !__CSTAGE2_H__



