#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"
BEGIN(Engine)
class CFrustum;
class ENGINE_DLL CStaticMesh : public CMesh
{
	enum Edge
	{
		FLB, FRB, FLT , FRT,
		BLB, BRB, BLT , BRT, 
		EDGE_END
	};
	typedef struct tSubsetInfo
	{
		_int	iSubsetCount;
		_vec3	vEDGE[EDGE_END];

	}SUBSETINFO;
	typedef struct tSubPoint
	{
		_int	iSubsetCount;
		_vec3	vPos;

	}SUBPOINT;
	typedef struct tTreeNode
	{
		tTreeNode* pParent;
		tTreeNode* pLeft;
		tTreeNode* pRight;

		_int	iSubsetCount;
		_vec3	vPos;
	}TREENode;
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	const _vec3*			Get_VtxPos(void) const { return m_pVtxPos; }
	_ulong					Get_VtxCnt(void) const { return m_dwVtxCnt; }
	_ulong					Get_Stride(void) const { return m_dwStride; }

public:
	HRESULT					Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex);
	void					Render_Meshes(void);
	//void					Render_Meshes(LPD3DXEFFECT& pEffect);
	void					Render_Meshes(LPD3DXEFFECT & pEffect, _uint iPassIndex = 0);
	void					Culling(CFrustum* pFrustum, _float fScale =1);

private:
	_bool					Find_Normal(char* pFileName);
	_bool					Find_Material(char * pFileName);
	_bool					Find_Emissive(char* pFileName);
	_bool					Find_Specular(char* pFileName);
	_bool					Find_Alpha(char* pFileName);
	_bool					Find_Alpha2(char * pFileName);
	_bool					Find_Gravity(char* pFileName);
	_bool					Find_Panel(char* pFileName);
	void					Set_Buffer(vector<_vec3> vecStart, vector<_vec3> vecEnd);

private:
	void					Build_Info();
	void					Build_Tree();

	void					Sort_Point(vector<SUBPOINT>& pVecPoint,const _bool bFlag);
	void					Search_Tree(TREENode* pTree, const _vec3 vPos, const _bool bFlag /*=false*/);
	void					Create_SubTree(TREENode* pParent, TREENode* pSelf, vector<SUBPOINT> vecPoint, const _bool bFlag);
	void					Free_Tree(TREENode* pNode);
	void					Cull_Subset(CFrustum* pFrustum, TREENode* pNode, _float fScale);
	TREENode*				Get_RootNode() { return m_pRootNode; }

private:
	LPD3DXMESH				m_pMesh;		// 노말 정보가 최종적으로 삽입된 메쉬 컴 객체

	LPD3DXBUFFER			m_pAdjacency;		// 인접하는 폴리곤 정보를 보관하기 위한 주소(첫 번째 주소 보관)
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;		// 서브셋의 개수


	D3DXMATERIAL*			m_pMtrl;			// 재질정보와 텍스쳐의 이름을 보관하기 위한 구조체 포인터
												// 서브셋의 개수 == 텍스처의 개수 == 재질의 개수
	LPDIRECT3DTEXTURE9*		m_ppTextures;		
	LPDIRECT3DTEXTURE9*		m_ppNmTextures;		
	LPDIRECT3DTEXTURE9*		m_ppMmTextures;		
	LPDIRECT3DTEXTURE9*		m_ppEmTextures;		
	LPDIRECT3DTEXTURE9*		m_ppSmTextures;		


	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// 정점의 크기(사이즈) 보관
	_vec3*					m_pVtxPos;			// 정점 정보 중 포지션 값만 얻어오기 위한 포인터
	INDEX32*				m_pIndex32 = nullptr;
	INDEX16*				m_pIndex16 = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	TREENode*				m_pRootNode =nullptr;
	// 바운딩 박스와 관계 없는 작업, 노말 정보가 없는 메쉬의 정점에 노말 값을 삽입하기 위함
	LPD3DXMESH				m_pOriMesh;	// 최초 로딩 시에 얻어온 메쉬 컴객체


	vector<SUBSETINFO> m_vecSubSet;
	vector<SUBPOINT> m_vecSubPOINT;
	vector<_int>		m_vecDrawSubset;
	_bool				m_bIsMap = false;

public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName,const _ushort wSceneIndex);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free(void);
	bool Find_StringTag(const std::string& FileName, const std::string& tag);

private:
	const _bool Read_BakePx_From_File(const string& tagFile);
	void Get_Texture_Clone(LPDIRECT3DTEXTURE9& ppTexture, _tchar* szFileName, _tchar* szFullPath);
	_ushort m_iSceneIndex = 0;
};



END
#endif // StaticMesh_h__
