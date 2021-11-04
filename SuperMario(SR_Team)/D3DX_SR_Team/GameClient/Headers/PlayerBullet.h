#pragma once
#ifndef __PLAYERBULLET_H__

#include "GameObject.h"
#include "PlayerBullet.h"
USING(Engine)

class CPlayerBullet final : public CGameObject
{
private :
	explicit CPlayerBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayerBullet(const CPlayerBullet& other);
	virtual ~CPlayerBullet() = default;


public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private :
	HRESULT Add_Component();


public :
	static CPlayerBullet*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public : //Set
	void Set_Pos(const _float3 _vPos) { m_pTransformCom->Set_Position(_vPos); }



private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom;

	_float					m_pLifeTime = 0.f;

};





#define __PLAYERBULLET_H__
#endif // !__PLAYERBULLET_H__


