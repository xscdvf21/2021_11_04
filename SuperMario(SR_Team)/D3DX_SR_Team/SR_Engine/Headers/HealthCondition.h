#pragma once

#ifndef __HEALTH_CONDITION_H__
#define __HEALTH_CONDITION_H__

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CHealthCondition final : public CComponent
{
public:

	typedef struct tagHealthDescription
	{
		_uint iHP;
		_uint iBaseAttack;		//�⺻ ���ݷ�
		_uint iBaseDefence;		//�⺻ ����
		_uint iMoney;			//�⺻ ���� �ڱ� - ���
	
	}HEALTH_DESC;

private:
	explicit CHealthCondition(_Device pDevice);
	explicit CHealthCondition(const CHealthCondition& other);
	virtual ~CHealthCondition() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;

public:
	static CHealthCondition* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

public:
	_uint Hit_HP(_uint Damage);
	CHealthCondition::HEALTH_DESC Get_Status();

private:
	HEALTH_DESC m_HealthDesc;

};
END

#endif // !__HEALTH_CONDITION_H__