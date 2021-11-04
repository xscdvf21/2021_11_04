#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

// ResourcesMgr
inline HRESULT			Reserve_ContainerSize(const _ushort& wSize);

inline HRESULT			Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ushort& wContainerIdx, const _tchar* pBufferTag,
									BUFFERID eID,
									const _ulong& dwCntX = 129,
									const _ulong& dwCntZ = 129,
									const _ulong& dwVtxItv = 1);

inline void				Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);

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
									const _tchar* pFileName,
									const _tchar* pInstanceFile =nullptr,
									const _bool& bPhys = false);

inline CComponent*		Clone_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg = nullptr);
inline CComponent*		Get_Resource(const _ushort& wContainerIdx, const _tchar* pResourceTag, void* pArg = nullptr);


inline void				Delete_Resource(const _ushort& wContainerIdx, const _wstr& pResourceTag);
inline void				Delete_AllResource(const _ushort& wContainerIdx);


inline void				Release_Resources(void);

#include "Export_Resources.inl"

END
#endif // Export_Resources_h__
