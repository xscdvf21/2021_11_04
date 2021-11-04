#pragma once

#include"GameObject.h"
#include "MiddleBossABullet.h"
//Koopa

enum BOSSASTATE
{
	BOSSA_IDLE
	,BOSSA_MOVE
	,BOSSA_END
};


USING(Engine)
class CMiddleBossA :
	public CGameObject
{
private:
	explicit CMiddleBossA(LPDIRECT3DDEVICE9 pDevice);
	explicit CMiddleBossA(const CMiddleBossA& other);
	virtual ~CMiddleBossA() = default;


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CMiddleBossA* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	void Update_Sprite(_float fDeltaTime);
	void Player_Check(CManagement* _pManagement, _float fDeltaTime);
	bool Monster_Sprite_Dir(_float3 _vTargetPos, _float3 _vPos, _float fDeltaTime);
	bool Attack_TimingChecker(_float _fDeltaTime);
	bool ImAttacking(CManagement* _pManagement, _float fDeltaTime);

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom[STATE::STATE_END] = {};
	class CTransform*		m_pTargetTransform = nullptr;

	STATE m_eState = STATE::STATE_END;
	STATE m_ePreState = STATE::STATE_END;

	float m_fRotY = 0.f;
	float m_fAccel = 0.f;
	float m_fDeccel = 0.f;


	bool m_bCheck = false;

	float m_fBulletRot = 0.f;
	float m_fBulletTime = 0.f;
	float m_fThrowTime = 0.f;
	float m_fShotGunTime = 0.f;

//	float m_fBulletRot = 0.f;

};

