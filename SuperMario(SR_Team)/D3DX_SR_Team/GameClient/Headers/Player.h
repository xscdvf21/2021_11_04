#pragma once  
#ifndef __PLAYER_H__

//enum PlayerAni
//{
//	ANI_IDLE_FRONT,
//	ANI_IDLE_BACK,
//	ANI_MOVE_FRONT, 
//	ANI_MOVE_BACK,
//	ANI_JUMP_FRONT,
//	ANI_JUMP_BACK,
//	ANI_END
//
//};

#include "GameObject.h"
#include "PlayerBullet.h"
#include "TestCamera.h"
#include "PlayerBullet.h"

USING(Engine)
class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer(const CPlayer& other);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public://테스트
	_uint Get_CoinState() { return iCoinState; }
	void Set_CoinState() { if (iCoinState != 0) { iCoinState--; } }

	PLAYERINFO Get_Info() { return m_tPlayerInfo; }
	
public:
	void Set_Info(PLAYERINFO _INFO) { m_tPlayerInfo = _INFO; }
	_uint iCoinState = 100;

private:
	HRESULT Add_Component();

private://Player_For_Clone
	HRESULT Add_Component_Lower();
	HRESULT Add_Component_Lower_For_2D();
	bool Update_Sprite();
	HRESULT KeyCheck();


private:
	//Player_For_Logic
	HRESULT KeyCheck(_float fDeltaTime);
	HRESULT MoveKey_Manager(_float fDeltaTime);
	HRESULT JumpKey_Manager(_float fDeltaTime);
	HRESULT AttackKey_Manager(_float fDeltaTime);
	HRESULT PaperKey_Manager(_float fDeltaTime);
	HRESULT State_VariableCorrection();

	HRESULT Movement(_float fDeltaTime);
	HRESULT Jump(_float fDeltaTime);
	void Update_Frame(_float fDeltaTime);
	void bCheck_Collection();
	void Bullet_Product();
	void TestCode(_float fDeltaTime);

	HRESULT Jump_Collection(int _AniState);

private://2D


public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom[STATE::STATE_END];

	STATE					m_eState = STATE::STATE_END;
	STATE					m_ePreState = STATE::STATE_END;
	STATE					m_eBufferState = STATE::STATE_END;

	int						iTextureIndex = 0;

	float					ani = 0.f;

	float					fStartPosBuffer = 0.f;

	_bool					bMoveCheck = false;
	_bool					bJump = false;
	_bool					bJumpStart = true;

	_bool					bAttackCheck = false;	//플레이어 공격중인지 체크
	_bool					bPaperCheck = false;	//플레이어 페이퍼 체크.
	

	_float					fJumpPower = 10.f;
	//_float					fStartPosY = 0;
	_float					m_fJumpTime = 0.f;

	_float					m_fBulletTime = 0.f;

	int						iKeyCheck = 0;

	int						m_iBulletCounter = 0;

	float					m_fAngel;

	bool					m_bSwitch = false;

	_float					m_fSpeed = 10.f;
	_float					m_fSpeedDamage = 0.5f;

	_bool					m_bAttackCheck = false;
	_bool					m_bPaperCheck = false;
	_bool					m_pDownKeyCheck = false;
	_bool					m_pUPKeyCheck = false;

	_float					m_f2dAccelLimit = 5.f;
	_float					m_f2dAccelSlide = 5.f;
	_float					m_f2dAccel = 0.f;
	_float					m_f2dDeccel = 0.f;


#pragma region 한승현 테스트
	float m_fAccel = 0.f;
	float m_fDeccl = 1.f;
	float m_fDDeccel = 0.f;
#pragma endregion

	HRESULT KeyCheck_2D(_float fDeltaTime);

	HRESULT MoveKey_Manager_2D(_float fDeltaTime);

	HRESULT JumpKey_Manager_2D(_float fDeltaTime);

};

#define __PLAYER_H__
#endif