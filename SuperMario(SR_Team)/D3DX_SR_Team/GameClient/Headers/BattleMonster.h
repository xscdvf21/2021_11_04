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
	// CSuperMonster을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);


	void Update_Sprite(_float fDeltaTime); //스프라이트 어떤걸 가져올지.
	void Update_Frame();//스프라이트 씬전환 속도.
public:
	static CBattleMonster*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual void			Free()	override;

};

#define __BATTLEMONSTERE_H__
#endif // !__BATTLEMONSTERE_H__