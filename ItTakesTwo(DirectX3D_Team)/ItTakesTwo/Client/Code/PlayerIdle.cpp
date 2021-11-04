#include "stdafx.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "SpongeBob.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CPlayerIdle)

CPlayerIdle::CPlayerIdle()
{
}

void CPlayerIdle::SetState(CSpongeBob * pPlayer)
{
	CPlayerState::KeyInput();
	Transition(pPlayer);
}

void CPlayerIdle::Transition(CSpongeBob* pPlayer)
{
	if (m_iKeyState & (MOVE_LEFT | MOVE_RIGHT | MOVE_DOWN | MOVE_UP))
		pPlayer->SetState(CPlayerMove::GetInstance());
}

void CPlayerIdle::KeyInput()
{
	CPlayerState::KeyInput();
}

void CPlayerIdle::Free()
{
}
