#pragma once
#ifndef __PLAYERIDLE_H__

#include "PlayerState.h"
#include "Client_Define.h"
#include "Engine_Define.h"

class CPlayerIdle : public CPlayerState
{
	DECLARE_SINGLETON(CPlayerIdle)

public:
	explicit CPlayerIdle();
	virtual ~CPlayerIdle() = default;

public:
	virtual void SetState(class CSpongeBob* pPlayer) override;

private:
	virtual void Transition(class CSpongeBob* pPlayer) override;
	virtual void KeyInput() override;

public:
	virtual void Free() override;


};


#define __PLAYERIDLE_H__
#endif