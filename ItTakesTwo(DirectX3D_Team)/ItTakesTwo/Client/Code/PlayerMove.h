#pragma once
#ifndef __PLAYERMOVE_H__

#include "PlayerState.h"
#include "Client_Define.h"
#include "Engine_Define.h"

class CPlayerMove : public CPlayerState
{
	DECLARE_SINGLETON(CPlayerMove)

public:
	explicit CPlayerMove();
	virtual ~CPlayerMove() = default;

public:
	virtual void SetState(class CSpongeBob* pPlayer) override;

private:
	virtual void Transition(class CSpongeBob* pPlayer) override;
	virtual void KeyInput() override;

public:
	virtual void Free() override;


};


#define __PLAYERMOVE_H__
#endif