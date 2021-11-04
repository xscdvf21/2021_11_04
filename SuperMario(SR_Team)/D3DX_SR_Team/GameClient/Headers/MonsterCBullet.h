#pragma once
#ifndef __MONSTERCBULLET_H__

#include "GameObject.h"


USING(Engine)

class CMonsterCBullet final : public CGameObject
{
private:
	explicit CMonsterCBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterCBullet(const CMonsterCBullet& other);
	virtual ~CMonsterCBullet() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);

public:
	static CMonsterCBullet*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public: //Set
	void Set_Pos(const _float3 _vPos) { m_pTransformCom->Set_Position(_vPos); }

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom;

	_float					m_pLifeTime = 0.f;
	_float3  vDir;

	_float					fJumpPower = 3.f;
	_float					fStartPosY = 0;
	_float					m_fJumpTime = 0.f;
};




#define __MONSTERCBULLET_H__
#endif // !__MONSTERCBULLET_H__
