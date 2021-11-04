#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

Engine::CProtoMgr::CProtoMgr(void)
{

}

Engine::CProtoMgr::~CProtoMgr(void)
{
	Free();
}

HRESULT Engine::CProtoMgr::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent*	pComponent = Find_Prototype(pProtoTag);

	if (NULL != pComponent)
		return E_FAIL;

	m_mapProto.emplace(pProtoTag, pInstance);

	return S_OK;
}

CComponent* Engine::CProtoMgr::Clone_Prototype(const _tchar* pProtoTag)
{
	CComponent*	pComponent = Find_Prototype(pProtoTag);

	if (NULL == pComponent)
		return nullptr;

	return pComponent->Clone();
}

CComponent* Engine::CProtoMgr::Find_Prototype(const _tchar* pProtoTag)
{
	auto		iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;

	return iter->second;
}

void Engine::CProtoMgr::Free(void)
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}

