#pragma once
#ifndef __MONSTERC_H__


#include "SuperMonster.h"
#include "MonsterCBullet.h"
USING(Engine)
class CMonsterC final : public CSuperMonster
{
private:
	explicit CMonsterC(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterC(const CMonsterC& other);
	virtual ~CMonsterC() = default;

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
	static CMonsterC* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;


private :
	_float fAlphaTime = 0.f;

	_float m_fAttackTime = 0.f;

};
#define __MONSTERC_H__
#endif

