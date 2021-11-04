#pragma once
#ifndef __CBOSSSTAGE_H__
#define __CBOSSSTAGE_H__

#include "Scene.h"
class CBossStage : public CScene
{
public:
	CBossStage();
	virtual ~CBossStage();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__CBOSSSTAGE_H__





