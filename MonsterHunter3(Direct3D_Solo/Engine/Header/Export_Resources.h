#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

// ResourcesMgr
// Get
// Set
// General
inline HRESULT			Reserve_ContainerSize(const _ushort& wSize);

inline HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx, const _tchar* pBufferTag,
									BUFFERID eID,
									const _ulong& dwCntX = 129,
									const _ulong& dwCntZ = 129,
									const _ulong& dwVtxItv = 1);

inline void				Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);

inline HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pDevice,
									 const _ushort& wContainerIdx,
									 const _tchar* pBufferTag,
									 const _float& Radius,
									 const _uint& Slices,
									 const _uint& Stack,
									 const LPD3DXMESH ppMesh = NULL,
									 const LPD3DXBUFFER ppADjacency = NULL);


inline HRESULT			Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
										const _ushort& wContainerIdx,
										const _tchar* pTextureTag,
										TEXTURETYPE eType,
										const _tchar* pPath,
										const _uint& iCnt = 1);

inline HRESULT			Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
										const _ushort& wContainerIdx,
										const _tchar* pMeshTag,
										MESHTYPE eType,
										const _tchar* pFilePath,
										const _tchar* pFileName);

inline HRESULT			Ready_MeshesBoss(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pMeshTag,
	MESHTYPE eType,
	const _tchar* pFilePath,
	const _tchar* pFileName);

inline CComponent*			Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag);

inline void				Release_Resources(void);

#include "Export_Resources.inl"

END
#endif // Export_Resources_h__
