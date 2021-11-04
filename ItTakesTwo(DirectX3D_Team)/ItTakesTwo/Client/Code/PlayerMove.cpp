#include "stdafx.h"
#include "PlayerMove.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CPlayerMove)

CPlayerMove::CPlayerMove()
{
}

void CPlayerMove::SetState(CSpongeBob * pPlayer)
{
	CPlayerState::KeyInput();
	Transition(pPlayer);
}

void CPlayerMove::Transition(CSpongeBob* pPlayer)
{
	if (!(m_iKeyState & (MOVE_LEFT | MOVE_RIGHT | MOVE_DOWN | MOVE_UP)));
}

void CPlayerMove::KeyInput()
{
	CPlayerState::KeyInput();
}

void CPlayerMove::Free()
{
}
