#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMesh(pGraphicDev)
	, m_pLoader(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;
	
	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
											D3DXMESH_MANAGED, 
											m_pGraphicDev,
											m_pLoader,	// hierarchyLoder
											NULL,
											&m_pRootFrame,
											&pAniCtrl)))	// AniCtrl
	return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);


	_matrix		matTemp, matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(-90));

	matTemp = matRotX * matRotY;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

HRESULT CDynamicMesh::Ready_MeshesBoss(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader,	// hierarchyLoder
		NULL,
		&m_pRootFrame,
		&pAniCtrl)))	// AniCtrl
		return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);



	_matrix		matTemp, matRotX, matRotY;

	D3DXMatrixRotationY(&matRotY, D3DXToRadian(-90));
	matTemp = matRotY;


	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CDynamicMesh::Render_Meshes(void)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * *pMeshContainer->ppFrameCombinedMatrix[i];
		}

		void*		pDestVtx = nullptr;
		void*		pSourVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pDestVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pSourVtx);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경 또한 동시에 수행)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, NULL, pDestVtx, pSourVtx);
		
		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

const	D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

Engine::_bool Engine::CDynamicMesh::Is_AnimationSetEnd(void)
{
	return m_pAniCtrl->Is_AnimationSetEnd();
}

void CDynamicMesh::Set_Animationset(const _uint & iIndex)
{

	if (nullptr == m_pAniCtrl)
		return;

	m_pAniIndex = iIndex;
	m_pAniCtrl->Set_Animationset(iIndex);
}

const _uint & CDynamicMesh::Get_AniIndex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniIndex;
}

void CDynamicMesh::Play_AnimationsetBoss(const _float & fTimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return;

	m_pAniCtrl->Play_Animationset(fTimeDelta);

	_matrix		matTemp, matRotX, matRotY;



	D3DXMatrixRotationY(&matRotY, D3DXToRadian(-90));
	matTemp = matRotY;

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);
	//Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
}



void CDynamicMesh::Play_Animationset(const _float & fTimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return;

	m_pAniCtrl->Play_Animationset(fTimeDelta);
	
	_matrix		matTemp, matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(-90));

	matTemp = matRotX * matRotY;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);
	//Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));

}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CDynamicMesh * CDynamicMesh::CreateBoss(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar * pFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_MeshesBoss(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	m_MeshContainerList.clear();
}

void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTranformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTranformationMatrix);
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			// 뼈의 이름을 얻어옴
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			// 이름과 일치하는 뼈를 찾음
			D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			
			pDerivedMeshContainer->ppFrameCombinedMatrix[i] = &pFindFrame->CombinedTranformationMatrix;
		}

		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

