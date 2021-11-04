#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "TriCol.h"
#include "RcTex.h"
#include "RcScreen.h"
#include "TerrainTex.h"
#include "NormalTerrainTex.h"
#include "CubeTex.h"
#include "RoundTex.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"
#include "TrailTex.h"
#include "PhysXMesh.h"
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

	void			Render_Buffer(const _ushort& wContainerIdx, 
								const _tchar* pBufferTag);

	HRESULT			Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx,
									const _tchar* pTextureTag,
									TEXTURETYPE eType,
									const _tchar* pPath,
									const _uint& iCnt = 1);



	HRESULT			Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName, const _tchar* pInstancefile = nullptr, const _bool& bPhys= false);
	void			Deleta_AllResource(const _ushort& wContainerIdx);
	void			Delete_Resource(const _ushort& wContainerIdx, const _wstr& tagResource);
	CComponent*		Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg = nullptr);
	CComponent*		Get_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg = nullptr);

private:
	CResources*		Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag);

private:
	map<_wstr, CResources*>*			m_pmapResources;
	typedef map<_wstr, CResources*>		MAPRESOURCES;

	_ulong										m_dwContainerIdx;
	
public:
	virtual void		Free(void);

};

END
#endif // ResourcesMgr_h__
