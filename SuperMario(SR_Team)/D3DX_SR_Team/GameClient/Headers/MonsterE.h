#pragma once
#ifndef __MONSTERE_H__

#include "SuperMonster.h"

USING(Engine)
class CMonsterE final : public CSuperMonster
{
private:
	explicit CMonsterE(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonsterE(const CMonsterE& other);
	virtual ~CMonsterE() = default;

public :
	// CSuperMonster��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;


private :
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);

	void Update_Sprite(_float fDeltaTime); //��������Ʈ ��� ��������.
	void Update_Frame();//��������Ʈ ����ȯ �ӵ�.

public :
	static CMonsterE*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual void			Free()	override;

private :
	_float		fMoveTime = 0.f; //������ �ð�
	_float		fMoveSpeed = 1.f; //������ �ӵ�.
};



#define __MONSTERE_H__
#endif // !__MONSTERE_H__


