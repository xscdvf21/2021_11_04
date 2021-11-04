#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pAdjacency(nullptr)
	, m_pSubset(nullptr)
	, m_dwSubsetCnt(0)
	, m_pMtrl(nullptr)
	, m_ppTextures(nullptr)
	, m_pMesh(nullptr)
	, m_dwVtxCnt(0)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pMtrl(rhs.m_pMtrl)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_pVtxPos(rhs.m_pVtxPos)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}

	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();

}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

const D3DXFRAME_DERIVED * CStaticMesh::Get_FrameByName(const char * pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[256] = L"";
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath,
								D3DXMESH_MANAGED,
								m_pGraphicDev,
								&m_pAdjacency,
								&m_pSubset,
								NULL, 
								&m_dwSubsetCnt, 
								&m_pOriMesh)))
		return E_FAIL;

	// 메쉬가 지닌 정점의 속성 정보를 얻어오는 함수
	_ulong	dwFVF = m_pOriMesh->GetFVF();

	// 메쉬의 노말 정보가 없는 경우 코드로 삽입
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}
	
	// 메쉬 사이즈에 맞는 바운딩박스 생성을 위한 작업
	void*		pVertex = NULL;

	m_dwVtxCnt = m_pMesh->GetNumVertices();	// 메쉬가 지닌 정점의 개수를 반환

	m_pVtxPos = new _vec3[m_dwVtxCnt];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte		byOffSet = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = Decl[i].Offset;
			break;
		}
	}

	// 정점이 지닌 fvf정보를 기준으로 정점의 크기를 계산하자
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (i * m_dwStride + byOffSet)));
	}
	
	m_pMesh->UnlockVertexBuffer();
	
	// LPD3DXBUFFER가 담고 있는 버퍼 정보 중 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar		szFileName[256] = L"";

		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 
							0, 
							m_pMtrl[i].pTextureFilename,
							strlen(m_pMtrl[i].pTextureFilename),
							szFileName,
							256);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
			return E_FAIL;
	}
	
	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh* pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pOriMesh);

	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Delete_Array(m_ppTextures);

	if(false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);


	CMesh::Free();
}

