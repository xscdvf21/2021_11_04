#pragma once
#ifndef __MONSTERD_H__

#include "SuperMonster.h"


USING(Engine)
class CMonsterD final : public CSuperMonster
{
private:
	explicit CMonsterD(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterD(const CMonsterD& other);
	virtual ~CMonsterD() = default ;

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
	HRESULT IsOnTerrain();
	HRESULT Set_Transform();

public:
	static CMonsterD* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

};


#define __MONSTERD_H__
#endif // !__MONSTERD_H__



