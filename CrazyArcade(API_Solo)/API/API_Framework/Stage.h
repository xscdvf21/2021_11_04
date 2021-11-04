#pragma once

#ifndef __STAGE_H__
#define __STAGE_H__

#include "Scene.h"
class CStage : public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private :
	DWORD		m_dwtime;
	DWORD		m_dwtime2;
	bool		m_bOne;
};


#endif // !__STAGE_H__
