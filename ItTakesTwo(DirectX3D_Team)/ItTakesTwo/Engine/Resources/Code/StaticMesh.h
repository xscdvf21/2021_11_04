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
	LPD3DXMESH				m_pMesh;		// �븻 ������ ���������� ���Ե� �޽� �� ��ü

	LPD3DXBUFFER			m_pAdjacency;		// �����ϴ� ������ ������ �����ϱ� ���� �ּ�(ù ��° �ּ� ����)
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;		// ������� ����


	D3DXMATERIAL*			m_pMtrl;			// ���������� �ؽ����� �̸��� �����ϱ� ���� ����ü ������
												// ������� ���� == �ؽ�ó�� ���� == ������ ����
	LPDIRECT3DTEXTURE9*		m_ppTextures;		
	LPDIRECT3DTEXTURE9*		m_ppNmTextures;		
	LPDIRECT3DTEXTURE9*		m_ppMmTextures;		
	LPDIRECT3DTEXTURE9*		m_ppEmTextures;		
	LPDIRECT3DTEXTURE9*		m_ppSmTextures;		


	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// ������ ũ��(������) ����
	_vec3*					m_pVtxPos;			// ���� ���� �� ������ ���� ������ ���� ������
	INDEX32*				m_pIndex32 = nullptr;
	INDEX16*				m_pIndex16 = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	TREENode*				m_pRootNode =nullptr;
	// �ٿ�� �ڽ��� ���� ���� �۾�, �븻 ������ ���� �޽��� ������ �븻 ���� �����ϱ� ����
	LPD3DXMESH				m_pOriMesh;	// ���� �ε� �ÿ� ���� �޽� �İ�ü


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
