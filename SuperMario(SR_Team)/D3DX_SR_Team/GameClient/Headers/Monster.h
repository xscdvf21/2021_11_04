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
	// CGameObject��(��) ���� ��ӵ�
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
	//void Set_Position(_float3 _vPos); //SuperClass ���۸��Ϳ� �Ű���.
	static CMonster* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

	LPD3DXFONT  m_pFont;
	int i = 0;
};


#define __MONSTER_H__
#endif // !__MONSTER_H__




