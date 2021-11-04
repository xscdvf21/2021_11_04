#include "HealthCondition.h"


USING(Engine)

CHealthCondition::CHealthCondition(_Device pDevice)
	: CComponent(pDevice)
{
	ZeroMemory(&m_HealthDesc, sizeof(HEALTH_DESC));
	m_HealthDesc.iHP = 10;
}

CHealthCondition::CHealthCondition(const CHealthCondition& other)
	: CComponent(other)
	, m_HealthDesc(other.m_HealthDesc)
{
}

HRESULT CHealthCondition::Ready_Component_Prototype()
{
	if (FAILED(CComponent::Ready_Component_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CHealthCondition::Ready_Component(void* pArg)
{
	//여기 Arg는 Transform_desc임.
	if (FAILED(CComponent::Ready_Component(pArg)))
	{
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		memcpy(&m_HealthDesc, pArg, sizeof(HEALTH_DESC));
	}

	return S_OK;
}

CHealthCondition* CHealthCondition::Create(_Device pDevice)
{
	CHealthCondition* pInstance = new CHealthCondition(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CHealthCondition");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CHealthCondition::Clone(void* pArg)
{
	CHealthCondition* pClone = new CHealthCondition(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTransform");
		Safe_Release(pClone);
	}

	return pClone;
}

void CHealthCondition::Free()
{
	CComponent::Free();
}

_uint CHealthCondition::Hit_HP(_uint Damage)
{
	if (Damage >= m_HealthDesc.iHP)
	{
		m_HealthDesc.iHP = 0;		
	}
	else
	{
		m_HealthDesc.iHP -= Damage;
	}
	return m_HealthDesc.iHP;
}

CHealthCondition::HEALTH_DESC CHealthCondition::Get_Status()
{
	return m_HealthDesc;
}
