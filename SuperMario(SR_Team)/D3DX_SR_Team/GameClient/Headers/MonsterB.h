#pragma once
#ifndef __MONSTERB_H__

#include "SuperMonster.h"

USING(Engine)
class CMonsterB final : public CSuperMonster
{
private:
	explicit CMonsterB(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterB(const CMonsterB& other);
	virtual ~CMonsterB() = default;

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
	HRESULT Set_Transform();

	void Update_Sprite(_float fDeltaTime); //��������Ʈ ��� ��������.
	void Update_Frame();//��������Ʈ ����ȯ �ӵ�.
public:
	static CMonsterB* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

};

#define __MONSTERB_H__
#endif // !__MONSTERB_H__

