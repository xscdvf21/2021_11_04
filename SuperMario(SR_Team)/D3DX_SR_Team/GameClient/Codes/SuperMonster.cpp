#include "framework.h"
#include "..\Headers\SuperMonster.h"
#include "../../SR_Engine/Headers/GameObject.h"




CSuperMonster::CSuperMonster(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CSuperMonster::CSuperMonster(const CSuperMonster & other)
	:CGameObject (other)
{
}

HRESULT CSuperMonster::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSuperMonster::Ready_GameObject(void * pArg /*= nullptr*/)
{
	return S_OK;
}

_uint CSuperMonster::Update_GameObject(_float fDeltaTime)
{
	if (Get_Health_Status().iHP == 0)
	{
		Set_Dead();
	}


	return _uint();
}

_uint CSuperMonster::LateUpdate_GameObject(_float fDeltaTime)
{
	return _uint();
}

HRESULT CSuperMonster::Render_GameObject()
{
	return S_OK;
}

void CSuperMonster::Set_Position(_float3 _vPos)
{
	m_pTransformCom->Set_Position(_vPos);
	m_pTransformCom->Update_Transform();
}

//리턴값 = 맞은 대미지
_uint CSuperMonster::Hurt_HP(_uint _iDamage)
{
	_uint iDamage = _iDamage - (m_pHealthConditionCom->Get_Status().iBaseDefence);

	if (iDamage == 0)
	{
		iDamage = 1;
	}
	
	m_pHealthConditionCom->Hit_HP(iDamage);

	return iDamage;
}
//리턴값 = 공격력
_uint CSuperMonster::Attack_Damage()
{
	return m_pHealthConditionCom->Get_Status().iBaseAttack;
}

_uint CSuperMonster::Give_Money()
{
	return m_pHealthConditionCom->Get_Status().iMoney;
}

CHealthCondition::HEALTH_DESC CSuperMonster::Get_Health_Status()
{
	return m_pHealthConditionCom->Get_Status();
}


CGameObject * CSuperMonster::Clone(void * pArg /*= nullptr*/)
{
	return nullptr;
}

void CSuperMonster::Free()
{
	//for (auto& pObj : m_pTextureCom)
	//{
	//	if (pObj != nullptr)
	//	Safe_Release(pObj);
	//}


	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pHealthConditionCom);

	CGameObject::Free();

}
