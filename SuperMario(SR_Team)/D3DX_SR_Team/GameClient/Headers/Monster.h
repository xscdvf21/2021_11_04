#pragma once
#ifndef __MONSTER_H__

#include "SuperMonster.h"

USING(Engine)
class CMonster final : public CSuperMonster
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonster(const CMonster& other);
	virtual ~CMonster() = default;

public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private :
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Set_Transform();
	HRESULT	IsBillboarding();

public :
	//void Set_Position(_float3 _vPos); //SuperClass 슈퍼몬스터에 옮겼음.
	static CMonster* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

	LPD3DXFONT  m_pFont;
	int i = 0;
};


#define __MONSTER_H__
#endif // !__MONSTER_H__




