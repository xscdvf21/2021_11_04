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
											const _ulong& dwCntX , 
											const _ulong& dwCntZ , 
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

	case BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pDevice,
	const _ushort & wContainerIdx,
	const _tchar * pBufferTag, 
	const _float & Radius, 
	const _uint & Slices, 
	const _uint & Stack, 
	const LPD3DXMESH ppMesh, 
	const LPD3DXBUFFER ppADjacency)
{
	if (nullptr == m_pmapResources)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CSphere::Create(pDevice, Radius, Slices, Stack, ppMesh, ppADjacency);


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

CComponent* Engine::CResourcesMgr::Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	if (nullptr == m_pmapResources)
		return nullptr;

	CResources*		pPrototype = Find_Resources(wContainerIdx, pResourceTag);

	return pPrototype->Clone();
}


HRESULT Engine::CResourcesMgr::Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	NULL_CHECK_RETURN(m_pmapResources, E_FAIL);

	CResources*	pResource = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResource)
		return E_FAIL;

	switch (eType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	//case MESH_NAVI:
	//	pResource = CNaviMesh::Create(pGraphicDev);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);
	m_pmapResources[wContainerIdx].emplace(pMeshTag, pResource);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_MeshesBoss(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName)
{
	NULL_CHECK_RETURN(m_pmapResources, E_FAIL);

	CResources*	pResource = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResource)
		return E_FAIL;

	pResource = CDynamicMesh::CreateBoss(pGraphicDev, pFilePath, pFileName);

	NULL_CHECK_RETURN(pResource, E_FAIL);
	m_pmapResources[wContainerIdx].emplace(pMeshTag, pResource);


	return S_OK;

}

