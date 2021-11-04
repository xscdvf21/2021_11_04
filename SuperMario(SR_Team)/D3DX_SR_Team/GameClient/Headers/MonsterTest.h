#pragma once
#ifndef __MONSTER_TEST_H__

#include "SuperMonster.h"

USING(Engine)
class CMonsterTest final : public CSuperMonster
{
private:
	explicit CMonsterTest(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterTest(const CMonsterTest& other);
	virtual ~CMonsterTest() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Set_Transform();

public:
	static CMonsterTest* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	class CTexture* m_pTextureComp=nullptr;

};


#define __MONSTER_TEST_H__
#endif // !__MONSTERA_H__




