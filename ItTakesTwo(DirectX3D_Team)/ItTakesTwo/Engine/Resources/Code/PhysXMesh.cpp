#include "PhysXMesh.h"
#include "Phys_Manager.h"
USING(Engine)

Engine::CPhysXMesh::CPhysXMesh(LPDIRECT3DDEVICE9 pGraphicDev)
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

Engine::CPhysXMesh::CPhysXMesh(const CPhysXMesh& rhs)
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
	, m_pIndex32(rhs.m_pIndex32)
	, m_pIndex16(rhs.m_pIndex16)
{
	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();

}

Engine::CPhysXMesh::~CPhysXMesh(void)
{

}

HRESULT Engine::CPhysXMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
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


	// 메쉬 사이즈에 맞는 바운딩박스 생성을 위한 작업
	void*		pVertex = NULL;
	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
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
			byOffSet = (_ubyte)Decl[i].Offset;
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

	LPDIRECT3DINDEXBUFFER9 pbuffer;
	m_pMesh->GetIndexBuffer(&pbuffer);
	D3DINDEXBUFFER_DESC tDesc;
	pbuffer->GetDesc(&tDesc);
	PxTriangleMeshDesc tMesh;

	if (tDesc.Format == D3DFORMAT::D3DFMT_INDEX16)
	{

		m_pIndex16 = new INDEX16[m_pMesh->GetNumFaces()];
		void*		pIndex = NULL;

		pbuffer->Lock(0, 0, &pIndex, 0);
		for (DWORD i = 0; i < m_pMesh->GetNumFaces(); ++i)
		{
			m_pIndex16[i] = *((INDEX16*)(((_ubyte*)pIndex) + (i * sizeof(INDEX16))));
		}
		tMesh.triangles.stride = sizeof(INDEX16);
		tMesh.triangles.data = m_pIndex16;
		tMesh.flags = PxMeshFlag::e16_BIT_INDICES;

	}
	else if (tDesc.Format == D3DFORMAT::D3DFMT_INDEX32)
	{

		m_pIndex32 = new INDEX32[m_pMesh->GetNumFaces()];
		void*		pIndex = NULL;

		pbuffer->Lock(0, 0, &pIndex, 0);
		for (DWORD i = 0; i < m_pMesh->GetNumFaces(); ++i)
		{
			m_pIndex32[i] = *((INDEX32*)(((_ubyte*)pIndex) + (i * sizeof(INDEX32))));
		}
		tMesh.triangles.data = m_pIndex32;
		tMesh.triangles.stride = sizeof(INDEX32);
	}
	pbuffer->Unlock();
	pbuffer->Release();

	tMesh.points.count = m_dwVtxCnt;
	tMesh.points.stride = sizeof(_vec3);
	tMesh.points.data = m_pVtxPos;

	tMesh.triangles.count = m_pMesh->GetNumFaces();


	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = CPhys_Manager::GetInstance()->Get_Cook()->cookTriangleMesh(tMesh, writeBuffer, &result);
	if (!status)
		return S_OK;

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	m_pPxMesh = CPhys_Manager::GetInstance()->Get_Physics()->createTriangleMesh(readBuffer);

	return S_OK;
}

void Engine::CPhysXMesh::Render_Meshes(void)
{

}

void CPhysXMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{

}

_bool CPhysXMesh::Find_Alpha(const char * pFileName)
{
	return false;
}

void CPhysXMesh::Set_Buffer(vector<_vec3> vecStart, vector<_vec3> vecEnd)
{
	m_pMesh->GetVertexBuffer(&m_pVB);

	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		pVertex[i].vPosition = vecStart[i];
		pVertex[i].vTexUV = _vec2(0.f, 0.f);
	}

	m_pVB->Unlock();

}

CPhysXMesh* Engine::CPhysXMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CPhysXMesh* pInstance = new CPhysXMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CPhysXMesh::Clone(void* pArg)
{
	return new CPhysXMesh(*this);
}

void Engine::CPhysXMesh::Free(void)
{
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pOriMesh);

	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pVtxPos);
		Safe_Delete_Array(m_pIndex16);
		Safe_Delete_Array(m_pIndex32);
	}

	CMesh::Free();
}

