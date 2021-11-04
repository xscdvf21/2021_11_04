#pragma once
#ifndef __PLAYERSTATE_H__

#include "Base.h"
#include "Client_Define.h"
#include "Engine_Define.h"

#define MOVE_UP		0x01
#define MOVE_DOWN	0x02
#define MOVE_LEFT	0x04
#define MOVE_RIGHT	0x08

class CPlayerState : public CBase
{
protected:
	explicit CPlayerState();
	virtual ~CPlayerState() = default;

protected:
	virtual void SetState(class CSpongeBob* pPlayer) PURE;

protected:
	virtual void KeyInput() PURE;
	virtual void Transition(class CSpongeBob* pPlayer) PURE;

protected:
	_uint m_iKeyState = 0;
};


#define __PLAYERSTATE_H__
#endif