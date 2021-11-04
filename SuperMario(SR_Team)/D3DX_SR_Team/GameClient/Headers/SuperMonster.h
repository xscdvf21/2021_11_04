#pragma once
#ifndef __SUPERMONSTER_H__

#include "GameObject.h"

USING(Engine)

class CSuperMonster :public CGameObject
{
protected:
	explicit CSuperMonster(LPDIRECT3DDEVICE9 pDevice);
	explicit CSuperMonster(const CSuperMonster& other);
	virtual ~CSuperMonster() = default;

public :
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() = 0;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr)  = 0;
	virtual _uint Update_GameObject(_float fDeltaTime) = 0;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) = 0;
	virtual HRESULT Render_GameObject() = 0;


	
public : //Set_�Լ�	
	void Set_Position(_float3 _vPos);

public:	//���°� ����
	_uint Hurt_HP(_uint _iDamage);						//������ ����
	_uint Attack_Damage();								//������ ����
	_uint Give_Money();									//������ Ȯ��
	CHealthCondition::HEALTH_DESC Get_Health_Status();	//���� ����


	


public:
	virtual CGameObject * Clone(void * pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	class CTexture* m_pTextureCom[STATE::STATE_END] = {};
	class CVIBuffer*	m_pVIBufferCom = nullptr;
	class CHealthCondition* m_pHealthConditionCom = nullptr;

	STATE m_eState = STATE::STATE_END;
	STATE m_ePreState = STATE::STATE_END;

	MONSTERINFO			m_tMonsterInfo;
};

#define __SUPERMONSTER_H__
#endif // !__SUPERMONSTER_H__


