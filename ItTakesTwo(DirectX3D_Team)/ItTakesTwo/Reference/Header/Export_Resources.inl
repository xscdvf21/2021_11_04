// ResourcesMgr
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


// Texture
HRESULT			Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pTextureTag,
	TEXTURETYPE eType,
	const _tchar* pPath,
	const _uint& iCnt)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}


// Mesh
HRESULT			Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pMeshTag,
	MESHTYPE eType,
	const _tchar* pFilePath,
	const _tchar* pFileName,
	const _tchar* pInstanceFile,
	const _bool& bPhys)
{
	return CResourcesMgr::GetInstance()->Ready_Meshes(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName, pInstanceFile, bPhys);
}

// Clone
CComponent*			Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg)
{
	return CResourcesMgr::GetInstance()->Clone_Resource(wContainerIdx, pResourceTag, pArg);
}

Engine::CComponent* Engine::Get_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg /*= nullptr*/)
{
	return CResourcesMgr::GetInstance()->Get_Resource(wContainerIdx, pResourceTag, pArg);
}

void Engine::Delete_Resource(const _ushort& wContainerIdx, const _wstr& pResourceTag)
{
	CResourcesMgr::GetInstance()->Delete_Resource(wContainerIdx, pResourceTag);
}

void				Release_Resources(void)
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}
void Engine::Delete_AllResource(const _ushort& wContainerIdx)
{
	CResourcesMgr::GetInstance()->Deleta_AllResource(wContainerIdx);
}
