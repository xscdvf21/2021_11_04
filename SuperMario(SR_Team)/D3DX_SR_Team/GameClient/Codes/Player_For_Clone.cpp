#include "framework.h"
#include "Player.h"


HRESULT CPlayer::Add_Component_Lower()
{
	/*For.Com_VIBuffer*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

#pragma region 플레이어 텍스처

	//Idle
	if (FAILED(CGameObject::Add_Component(//+z
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Idle_Front",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_FRONT])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(// +x, -x, -z
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Idle_Up",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_BACK])))
		return E_FAIL;


	//Move	
	if (FAILED(CGameObject::Add_Component(// +x, -x, -z
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Move_Front",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_FRONT])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(//
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Move_Up",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_BACK])))
		return E_FAIL;


	
	//Jump // 백을 준비해야 함.
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Jump_Front",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_JUMP_FORNT])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component( // 임시
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Jump_Front",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_JUMP_BACK])))
		return E_FAIL;



	//Attack
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_HammerAttack_Front",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_ATTACK_SIDE])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_HammerAttack_Up",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_ATTACK_BACK])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_HammerAttack_Down",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_ATTACK_FRONT])))
		return E_FAIL;



	//Paper
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Paper_Front",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_PAPER_SIDE])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Paper_Up",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_PAPER_BACK])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Paper_Down",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_PAPER_FRONT])))
		return E_FAIL;
#pragma endregion

#pragma region 주석 
	//if (FAILED(CGameObject::Add_Component(////
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Player_Idle_Back",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_BACK])))
	//	return E_FAIL;

	//if (FAILED(CGameObject::Add_Component(////
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Player_Idle_Down",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_DOWN])))
	//	return E_FAIL;

	/*For.Com_Texture*/

	/*For.Com_Texture*/
	//if (FAILED(CGameObject::Add_Component(////
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Player_Move_Back",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_BACK])))
	//	return E_FAIL;

	/*For.Com_Texture*/
	//if (FAILED(CGameObject::Add_Component(////
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Player_Move_Down",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_DOWN])))
	//	return E_FAIL;

	/*if (FAILED(CGameObject::Add_Component(////
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Jump_Back",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_JUMP_BACK])))
		return E_FAIL;*/

	//if (FAILED(CGameObject::Add_Component(////
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Player_HammerAttack_Back",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_ATTACK_BACK])))
	//	return E_FAIL;


	//if (FAILED(CGameObject::Add_Component(
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Player_Paper_Back",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_PAPER_BACK])))
	//	return E_FAIL;

#pragma endregion
	return S_OK;
}

HRESULT CPlayer::Add_Component_Lower_For_2D()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Idle",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_SPECIAL])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Move",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_SPECIAL])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Turn",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_TURN_SPECIAL])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Sit",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_SIT_SPECIAL])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Jump",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_JUMP_SPECIAL])))
		return E_FAIL;

	
	return S_OK;
}

bool CPlayer::Update_Sprite()
{
	if (m_ePreState != m_eState)
	{
		switch (m_eState)
		{
		case STATE::STATE_IDLE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0; // 지금 스프라이트 넘버.

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_IDLE_BACK:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_IDLE_UP:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_IDLE_DOWN:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_MOVE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_MOVE_BACK:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_MOVE_UP:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_MOVE_DOWN:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_JUMP_FORNT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 3;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_JUMP_BACK:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 3;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_ATTACK_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_ATTACK_BACK:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_ATTACK_UP:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_ATTACK_DOWN:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_PAPER_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_PAPER_BACK:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_PAPER_UP:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
		case STATE::STATE_PAPER_DOWN:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 5.f;
			break;
			//==================================== 한승현 추가.
		case STATE::STATE_ATTACK_SIDE:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 1.f;
			break;
		case STATE::STATE_PAPER_SIDE:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 1.f;
			break;
			//==================================== 한승현 추가.
		case STATE::STATE_IDLE_SPECIAL:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 0;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 1.f;
			break;

		case STATE::STATE_MOVE_SPECIAL:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 2;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 10.f;
			break;

		case STATE::STATE_TURN_SPECIAL:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 0;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 1.f;
			break;

		case STATE::STATE_SIT_SPECIAL:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 0;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 1.f;
			break;

		case STATE::STATE_JUMP_SPECIAL:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 0;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 1.f;
			m_tFrame.fFrameSpeed = 1.f;
			break;

		case STATE::STATE_SEARCH_BACK:
			break;
		case STATE::STATE_END:
			break;
		}
		m_ePreState = m_eState;
		return true;
	}
	return false;
}

HRESULT CPlayer::KeyCheck()
{
	return S_OK;
}
