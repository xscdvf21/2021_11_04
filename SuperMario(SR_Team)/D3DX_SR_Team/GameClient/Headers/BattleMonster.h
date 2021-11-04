#pragma once
#ifndef __BATTLEMONSTERE_H__

#include "SuperMonster.h"

USING(Engine)

class CBattleMonster final : public CSuperMonster
{
private:
	explicit CBattleMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CBattleMonster(const CBattleMonster& other);
	virtual ~CBattleMonster() = default;

public:
	// CSuperMonster��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);


	void Update_Sprite(_float fDeltaTime); //��������Ʈ ��� ��������.
	void Update_Frame();//��������Ʈ ����ȯ �ӵ�.
public:
	static CBattleMonster*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual void			Free()	override;

};

#define __BATTLEMONSTERE_H__
#endif // !__BATTLEMONSTERE_H__