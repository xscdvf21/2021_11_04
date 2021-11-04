#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "TriCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "Sphere.h"
#include "Texture.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

BEGIN(Engine)

class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT			Reserve_ContainerSize(const _ushort& wSize);

	HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _ushort& wContainerIdx, const _tchar* pBufferTag,
								BUFFERID eID, 
								const _ulong& dwCntX = 129,
								const _ulong& dwCntZ = 129, 
								const _ulong& dwVtxItv = 1);

	HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pDevice,
								const _ushort& wContainerIdx,
								const _tchar* pBufferTag,
								const _float& Radius,
								const _uint& Slices,
								const _uint& Stack,
								const LPD3DXMESH ppMesh = NULL,
								const LPD3DXBUFFER ppADjacency = NULL);

	void			Render_Buffer(const _ushort& wContainerIdx, 
								const _tchar* pBufferTag);

	HRESULT			Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx,
									const _tchar* pTextureTag,
									TEXTURETYPE eType,
									const _tchar* pPath,
									const _uint& iCnt = 1);


	HRESULT			Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _ushort& wContainerIdx,
								const _tchar* pMeshTag,
								MESHTYPE eType, 
								const _tchar* pFilePath, 
								const _tchar* pFileName);


	HRESULT			Ready_MeshesBoss(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ushort& wContainerIdx,
		const _tchar* pMeshTag,
		MESHTYPE eType,
		const _tchar* pFilePath,
		const _tchar* pFileName);


	CComponent*			Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag);

private:
	CResources*		Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag);

private:
	map<const _tchar*, CResources*>*			m_pmapResources;
	typedef map<const _tchar*, CResources*>		MAPRESOURCES;

	_ulong										m_dwContainerIdx;
	
public:
	virtual void		Free(void);

};

END
#endif // ResourcesMgr_h__
