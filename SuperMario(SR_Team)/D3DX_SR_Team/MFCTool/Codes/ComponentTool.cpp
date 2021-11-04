#include "stdafx.h"
#include "ComponentTool.h"

CComponentTool::CComponentTool(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddReference(m_pDevice);
}

CComponentTool::CComponentTool(const CComponentTool & other)
	: m_pDevice(other.m_pDevice)
{
	Safe_AddReference(m_pDevice);
}

HRESULT CComponentTool::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CComponentTool::Ready_Component(void * pArg)
{
	return S_OK;
}

void CComponentTool::Free()
{
	Safe_Release(m_pDevice);
}
