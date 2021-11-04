#include "..\Headers\Component.h"


USING(Engine)

CComponent::CComponent(_Device pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddReference(m_pDevice);			//<- ³ÀµÖµµ µÊ?
}

CComponent::CComponent(const CComponent& other)
	: m_pDevice(other.m_pDevice)
{
	Safe_AddReference(m_pDevice);
}

HRESULT CComponent::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Ready_Component(void* pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
