#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr(void)
	: m_pmapResources(nullptr)
{

}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pmapResources)
		return E_FAIL;

	m_pmapResources = new MAPRESOURCES[wSize];

	m_dwContainerIdx = wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pBufferTag,
	BUFFERID eID,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	if (nullptr == m_pmapResources)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;

	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;

	case BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_NORMALTERRAINTEX:
		pResources = CNormalTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;

	case BUFFER_ROUNDTEX:
		pResources = CRoundTex::Create(pGraphicDev);
		break;

	case BUFFER_TRAILTEX:
		pResources = CTrailTex::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort & wContainerIdx, const _tchar * pBufferTag)
{
	auto	iter = find_if(m_pmapResources[wContainerIdx].begin(), m_pmapResources[wContainerIdx].end(), CTag_Finder(pBufferTag));

	if (iter == m_pmapResources[wContainerIdx].end())
		return;

	dynamic_cast<CVIBuffer*>(iter->second)->Render_Buffer();
}

CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	auto	iter = find_if(m_pmapResources[wContainerIdx].begin(), m_pmapResources[wContainerIdx].end(), CTag_Finder(pResourceTag));

	if (iter == m_pmapResources[wContainerIdx].end())
		return nullptr;

	return iter->second;
}

void Engine::CResourcesMgr::Free(void)
{
	for (_ulong i = 0; i < m_dwContainerIdx; ++i)
	{
		for_each(m_pmapResources[i].begin(), m_pmapResources[i].end(), CDeleteMap());
		m_pmapResources[i].clear();
	}

	Safe_Delete_Array(m_pmapResources);
}

HRESULT Engine::CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt /*= 1*/)
{
	NULL_CHECK_RETURN(m_pmapResources, E_FAIL);

	CResources*	pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pPath, eType, iCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].emplace(pTextureTag, pResources);

	return S_OK;
}

CComponent* Engine::CResourcesMgr::Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg)
{
	if (nullptr == m_pmapResources)
		return nullptr;

	CResources*		pPrototype = Find_Resources(wContainerIdx, pResourceTag);
	if (!pPrototype)
		return nullptr;
	return pPrototype->Clone(pArg);
}

HRESULT Engine::CResourcesMgr::Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName,const _tchar* pInstancefile, const _bool& bPhys)
{
	NULL_CHECK_RETURN(m_pmapResources, E_FAIL);

	CResources*	pResource = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResource)
		return E_FAIL;

	switch (eType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName, wContainerIdx);
		break;

	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName, wContainerIdx,bPhys);
		break;

	case MESH_NAVI:
		pResource = CNaviMesh::Create(pGraphicDev);
		break;

	case MESH_PHYS:
		pResource = CPhysXMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);
	m_pmapResources[wContainerIdx].emplace(pMeshTag, pResource);

	return S_OK;
}

void Engine::CResourcesMgr::Delete_Resource(const _ushort& wContainerIdx, const _wstr& tagResource)
{
	auto&	iter = m_pmapResources[wContainerIdx].find(tagResource.c_str());

	if (iter == m_pmapResources[wContainerIdx].end())
		return;
	auto* pResource = iter->second;

	m_pmapResources[wContainerIdx].erase(iter);


	Safe_Release(pResource);
}

void Engine::CResourcesMgr::Deleta_AllResource(const _ushort& wContainerIdx)
{
	for (auto& iter : m_pmapResources[wContainerIdx])
	{
		Force_Release(iter.second);
	}
	m_pmapResources[wContainerIdx].clear();
}

Engine::CComponent* Engine::CResourcesMgr::Get_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg /*= nullptr*/)
{
	if (nullptr == m_pmapResources)
		return nullptr;

	CResources*		pPrototype = Find_Resources(wContainerIdx, pResourceTag);
	if (pPrototype)
		pPrototype->AddRef();
	return pPrototype;
}

