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
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() = 0;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr)  = 0;
	virtual _uint Update_GameObject(_float fDeltaTime) = 0;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) = 0;
	virtual HRESULT Render_GameObject() = 0;


	
public : //Set_함수	
	void Set_Position(_float3 _vPos);

public:	//상태값 관련
	_uint Hurt_HP(_uint _iDamage);						//공격을 맞음
	_uint Attack_Damage();								//공격을 가함
	_uint Give_Money();									//소지금 확인
	CHealthCondition::HEALTH_DESC Get_Health_Status();	//상태 리턴


	


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


