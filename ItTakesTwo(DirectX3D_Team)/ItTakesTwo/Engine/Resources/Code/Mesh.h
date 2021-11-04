#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"
#include "Texture.h"

BEGIN(Engine)
class ENGINE_DLL CMesh : public CResources
{
	struct  TexInfo
	{
		std::string				  ConstantTag;
		 CTexture*			  pTexture;
		_uint					  iPass = 0;
	};


protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);
public:
	virtual CComponent*		Clone(void* pArg = nullptr) PURE;
	virtual void			Free(void);
	PxTriangleMesh*			Get_PxMesh() { return m_pPxMesh; }
	void Add_Texture(const _int& iIndex, const _int& iSubsetIndex, const std::string& tagConstanteTable, CTexture* pTexture, const _uint& iPass);
protected:
	PxTriangleMesh*			m_pPxMesh = nullptr;
	map<_int, map<_int, vector<TexInfo>> >m_mapTexture;
	map<_int, map<_int, _int> >m_mapPass;
	map<_int, map<_int, _vec4> >m_mapGlow;
	map<_int, vector<_int> >m_vecCustomDarwCount;
};

END
#endif // Mesh_h__
