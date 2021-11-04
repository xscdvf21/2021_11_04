#pragma once
#ifndef __MONSTERA_H__

#include "SuperMonster.h"

USING(Engine)
class CMonsterA final : public CSuperMonster
{
private:
	explicit CMonsterA(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterA(const CMonsterA& other);
	virtual ~CMonsterA() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);
	HRESULT IsOnTerrain();

public:
	HRESULT Set_Transform();

private:
	void Update_Sprite(_float fDeltaTime); //��������Ʈ ��� ��������.
	void Update_Frame();//��������Ʈ ����ȯ �ӵ�.

public:
	static CMonsterA* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;


};


#define __MONSTERA_H__
#endif // !__MONSTERA_H__




