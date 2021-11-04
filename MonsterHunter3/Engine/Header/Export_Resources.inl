#include "Export_Resources.h"
// ResourcesMgr
// Get
// Set
// General
HRESULT			Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}

HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx, const _tchar* pBufferTag,
	BUFFERID eID,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwVtxItv);
}

void				Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
}

inline HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 pDevice,
	const _ushort & wContainerIdx, 
	const _tchar * pBufferTag, 
	const _float & Radius, 
	const _uint & Slices, 
	const _uint & Stack, 
	const LPD3DXMESH ppMesh, 
	const LPD3DXBUFFER ppADjacency)
{
	CResourcesMgr::GetInstance()->Ready_Buffer(pDevice, wContainerIdx, pBufferTag, Radius, Slices, Stack, ppMesh, ppADjacency);


	return S_OK;
}

HRESULT			Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pTextureTag,
	TEXTURETYPE eType,
	const _tchar* pPath,
	const _uint& iCnt)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}

HRESULT			Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pMeshTag,
	MESHTYPE eType,
	const _tchar* pFilePath,
	const _tchar* pFileName)
{
	return CResourcesMgr::GetInstance()->Ready_Meshes(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}

inline HRESULT Ready_MeshesBoss(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName)
{
	return CResourcesMgr::GetInstance()->Ready_MeshesBoss(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}


CComponent*			Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone_Resource(wContainerIdx, pResourceTag);
}

void				Release_Resources(void)
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}