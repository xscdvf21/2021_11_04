#ifndef HierarchyLoader_h__
#define HierarchyLoader_h__

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ushort& wSceneIndex ,const _bool& bPhys =false);
	virtual ~CHierarchyLoader(void);

public:
	
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,LPD3DXFRAME *ppNewFrame);
		
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
									CONST D3DXMESHDATA *pMeshData,
									CONST D3DXMATERIAL *pMaterials,
									CONST D3DXEFFECTINSTANCE *pEffectInstances,
									DWORD NumMaterials,
									CONST DWORD *pAdjacency,
									LPD3DXSKININFO pSkinInfo,
									LPD3DXMESHCONTAINER *ppNewMeshContainer);

	
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	void	Allocate_Name(char** ppName, const char* pFrameName);
	_bool	Find_Normal(char * pFileName);
	_bool	Find_Material(char * pFileName);
	_bool	Find_Emissive(char * pFileName);
	_bool	Find_Specular(char * pFileName);

public:
	vector<char*>					Get_MeshInfo() { return m_vecFilPath; }

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	const _tchar*					m_pPath;
	vector<char*>					m_vecFilPath;
	_ushort							m_iSceneIndex = 0;

public:
	static CHierarchyLoader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ushort& wSceneIndex,const _bool& bPhys);
	static CHierarchyLoader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ushort& wSceneIndex);
	_ulong		Release(void);
private:
	void Get_Texture_Clone(LPDIRECT3DTEXTURE9& ppTexture, _tchar* szFileName, _tchar* szFullPath);
	_bool m_bPhys;
};

END
#endif // HierarchyLoader_h__
