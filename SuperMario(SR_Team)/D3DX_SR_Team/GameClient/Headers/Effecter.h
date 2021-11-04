#pragma once

#ifndef __EFFECTER_H_
#define __EFFECTER_H_

#include "GameObject.h"



USING(Engine)
class CEffecter :
	public CGameObject
{
protected:
	explicit CEffecter(LPDIRECT3DDEVICE9 pDevice);
	explicit CEffecter(const CEffecter& other);
	virtual ~CEffecter() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT UpLoad_EffectFire(CManagement* pMan, _uint iEffectType);
	HRESULT UpLoad_EffectGreenBall(CManagement* pMan, _uint iEffectType);
	HRESULT UpLoad_EffectPaper(CManagement* pMan, _uint iEffectType);
	HRESULT UpLoad_EffectHammer(CManagement* pMan, _uint iEffectType);

protected:
	HRESULT Add_Component();

public:
	static CEffecter* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

protected:
	list<CGameObject*> m_listParticle[EFFECTTYPE::EFFECTTYPE_END] = {};
	list<CGameObject*> m_listParticleClone;

protected:
	_float3                m_vStartPoint;

};
//상속내릴 구조로 보였는데 왜 다 private였어요?

#endif // !__EFFECTER_H_