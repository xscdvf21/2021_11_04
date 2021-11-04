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
	m_pAniCtrl = nullptr;
	if (rhs.m_pAniCtrl->Get_AniCtrl())
		m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

void Engine::CDynamicMesh::Render_Meshes(void)
{
	m_pAniCtrl->Get_AniCtrl()->AdvanceTime(0.0f, NULL);
	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90)));

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

void Engine::CDynamicMesh::Render_PhysMeshes(LPD3DXEFFECT& pEffect, _uint iPassIndex /*= 0*/, vector<PxRigidDynamic*> vecMat, const _int& iDir)
{
	if (m_pAniCtrl)
		m_pAniCtrl->Get_AniCtrl()->AdvanceTime(0.0f, nullptr);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90)));

	_uint		iPass = iPassIndex;
	_mat matDir;

	D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90));
	switch (iDir)
	{
	case 0:
		D3DXMatrixRotationY(&matDir, D3DXToRadian(0));
		break;
	case 1:
		D3DXMatrixRotationY(&matDir, D3DXToRadian(90));
		break;
	case 2:
		D3DXMatrixRotationY(&matDir, D3DXToRadian(180));
		break;
	case 3:
		D3DXMatrixRotationY(&matDir, D3DXToRadian(-90));
		break;
	default:
		break;
	}

	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;
		PxTransform tCurTransform, tPrvTransform;
		//PxTransform tTransform = vecMat[0]->getGlobalPose();
		D3DXMATRIX matOrigin, matCur;
		//D3DXMatrixIdentity(&matOrigin);
		//memcpy(&matOrigin._41, &tTransform.p, sizeof(_vec3));

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			tCurTransform = vecMat[i]->getGlobalPose();

			PxMat44 mat = PxMat44(tCurTransform);
			D3DXMatrixIdentity(&matCur);
			int size = sizeof(PxMat44);
			memcpy(&matCur, &mat, sizeof(PxMat44));
			_mat local = matCur;
			local._41 *= 100;
			local._42 *= 100;
			local._43 *= 100;


			_mat ppFrameCombinedMatrix = *pMeshContainer->ppFrameCombinedMatrix[i];
			_mat pFrameOffsetMatrix = pMeshContainer->pFrameOffsetMatrix[i];

			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * matTemp* matDir*local;

		}

		void*		pDestVtx = nullptr;
		void*		pSourVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pDestVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pSourVtx);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경 또한 동시에 수행)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, NULL, pDestVtx, pSourVtx);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			pEffect->BeginPass(iPass);

			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);

			//if (pMeshContainer->ppNmTexture[i] != nullptr)
			pEffect->SetTexture("g_NormalTexture", pMeshContainer->ppNmTexture[i]);

			//if (pMeshContainer->ppMmTexture[i] != nullptr)
			pEffect->SetTexture("g_MaterialTexture", pMeshContainer->ppMmTexture[i]);

			//if (pMeshContainer->ppEmTexture[i] != nullptr)
			pEffect->SetTexture("g_EmissiveTexture", pMeshContainer->ppEmTexture[i]);

			//if (pMeshContainer->ppSmTexture[i] != nullptr)
			pEffect->SetTexture("g_SpecularTexture", pMeshContainer->ppSmTexture[i]);

			pEffect->CommitChanges();

			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			pEffect->EndPass();
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex,const _bool& bPhys)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath, wSceneIndex, bPhys);
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


	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90)));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void CDynamicMesh::Render_Meshes(LPD3DXEFFECT & pEffect, _uint iPassIndex)
{
	m_pAniCtrl->Get_AniCtrl()->AdvanceTime(0.0f, NULL);
	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90)));

	_uint		iPass = iPassIndex;



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
		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{

			pEffect->BeginPass(iPass);

			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
			//if (pMeshContainer->ppNmTexture[i] != nullptr)
			pEffect->SetTexture("g_NormalTexture", pMeshContainer->ppNmTexture[i]);

			//if (pMeshContainer->ppMmTexture[i] != nullptr)
			pEffect->SetTexture("g_MaterialTexture", pMeshContainer->ppMmTexture[i]);

			//if (pMeshContainer->ppEmTexture[i] != nullptr)
			pEffect->SetTexture("g_EmissiveTexture", pMeshContainer->ppEmTexture[i]);

			//if (pMeshContainer->ppSmTexture[i] != nullptr)
			pEffect->SetTexture("g_SpecularTexture", pMeshContainer->ppSmTexture[i]);
			pEffect->SetTexture("g_VTFTexture", pMeshContainer->pVTFTexture);

			pEffect->CommitChanges();

			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			pEffect->EndPass();
		}

	}
}

void Engine::CDynamicMesh::Render_Meshes_VTF(LPD3DXEFFECT& pEffect, _uint iPassIndex /*= 0*/)
{
	m_pAniCtrl->Get_AniCtrl()->AdvanceTime(0.0f, NULL);
	_mat matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90)));

	_uint		iPass = iPassIndex;
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		LPD3DXBONECOMBINATION pBoneComb =
			reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		for (int j = 0; j < (int)pMeshContainer->NumAttributeGroups; j++)
		{

			for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
			{

				int iMatrixIndex = pBoneComb[j].BoneId[i];
				if (iMatrixIndex != UINT_MAX)
				{
					pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[iMatrixIndex] *
						*pMeshContainer->ppFrameCombinedMatrix[iMatrixIndex];
				}
			}

			D3DLOCKED_RECT lockRect = { 0, };
			LPDIRECT3DTEXTURE9& VTFTex = pMeshContainer->pVTFTexture;

			if (FAILED(VTFTex->LockRect(0, &lockRect, nullptr, 0)))
				return;
			memcpy(lockRect.pBits, pMeshContainer->pRenderingMatrix, sizeof(_mat)*pMeshContainer->dwNumBones);

			VTFTex->UnlockRect(0);

			m_pGraphicDev->SetTexture(D3DVERTEXTEXTURESAMPLER0, pMeshContainer->pVTFTexture);


			int mtrlIndex = pBoneComb[j].AttribId;

			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[mtrlIndex]);
			//if (pMeshContainer->ppNmTexture[i] != nullptr)
			pEffect->SetTexture("g_NormalTexture", pMeshContainer->ppNmTexture[mtrlIndex]);

			//if (pMeshContainer->ppMmTexture[i] != nullptr)
			pEffect->SetTexture("g_MaterialTexture", pMeshContainer->ppMmTexture[mtrlIndex]);

			//if (pMeshContainer->ppEmTexture[i] != nullptr)
			pEffect->SetTexture("g_EmissiveTexture", pMeshContainer->ppEmTexture[mtrlIndex]);

			//if (pMeshContainer->ppSmTexture[i] != nullptr)
			pEffect->SetTexture("g_SpecularTexture", pMeshContainer->ppSmTexture[mtrlIndex]);
			pEffect->BeginPass(iPass);
			pEffect->CommitChanges();

			pMeshContainer->MeshData.pMesh->DrawSubset(mtrlIndex);

			pEffect->EndPass();
			//LPDIRECT3DVERTEXBUFFER9 pVB;
			//pMeshContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
			//m_pGraphicDev->SetStreamSource(0, pVB, 0, pMeshContainer->MeshData.pMesh->GetNumBytesPerVertex());
			//m_pGraphicDev->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_POINTLIST, 0, pMeshContainer->MeshData.pMesh->GetNumVertices());
			//pVB->Release();

		}
	}
}

void Engine::CDynamicMesh::Render_VTF(LPD3DXEFFECT& pEffect, _uint iPassIndex /*= 0*/)
{
	//SetUp_VTFMatrices();

	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;
		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * *pMeshContainer->ppFrameCombinedMatrix[i];
		}

		D3DLOCKED_RECT lockRect = { 0, };
		LPDIRECT3DTEXTURE9 VTFTex = pMeshContainer->pVTFTexture;
		if (FAILED(VTFTex->LockRect(0, &lockRect, nullptr, 0)))
			return;
		memcpy(lockRect.pBits, pMeshContainer->pRenderingMatrix, sizeof(_mat)*pMeshContainer->dwNumBones);
		VTFTex->UnlockRect(0);

		m_pGraphicDev->SetTexture(D3DVERTEXTEXTURESAMPLER0, pMeshContainer->pVTFTexture);
		//pEffect->SetTexture("g_VTFTexture", VTFTex);
		pEffect->CommitChanges();
		LPDIRECT3DVERTEXBUFFER9 pVB;
		pMeshContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
		m_pGraphicDev->SetStreamSource(0, pVB, 0, pMeshContainer->MeshData.pMesh->GetNumBytesPerVertex());
		m_pGraphicDev->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_POINTLIST, 0, pMeshContainer->MeshData.pMesh->GetNumVertices());
		pVB->Release();


	}
}

_bool CDynamicMesh::Find_Electric(const char * pFileName)
{
	_uint	iLength = _uint(strlen(pFileName));

	for (_uint i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'Z')
				return true;
		}
	}

	return false;
}

void Engine::CDynamicMesh::SetUp_VTFMatrices()
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		LPD3DXBONECOMBINATION pBoneComb =
			reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		for (int j = 0; j < (int)pMeshContainer->NumAttributeGroups; j++)
		{

			for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
			{

				int iMatrixIndex = pBoneComb[j].BoneId[i];
				if (iMatrixIndex != UINT_MAX)
				{
					pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[iMatrixIndex] *
						*pMeshContainer->ppFrameCombinedMatrix[iMatrixIndex];
				}
			}
		}

		D3DLOCKED_RECT lockRect = { 0, };
		LPDIRECT3DTEXTURE9& VTFTex = pMeshContainer->pVTFTexture;

		if (FAILED(VTFTex->LockRect(0, &lockRect, nullptr, 0)))
			return;
		memcpy(lockRect.pBits, pMeshContainer->pRenderingMatrix, sizeof(_mat)*pMeshContainer->dwNumBones);

		VTFTex->UnlockRect(0);
	}

}

const	D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);


}

_bool CDynamicMesh::Is_AnimationSetEnd(_float minusTime, _bool bPosReset)
{
	return m_pAniCtrl->Is_AnimationSetEnd(minusTime, bPosReset);
}

Engine::_ulong Engine::CDynamicMesh::Get_FrameCount(const _int& iIndex)
{
	auto iter = m_MeshContainerList.begin();
	for (_int i = 0; i < iIndex; ++i)
		iter++;

	return (*iter)->dwNumBones;
}

void CDynamicMesh::Set_Animationset(const _uint & iIndex, _float fspeed, _float fSyncTime, _float fDelay)
{
	if (nullptr == m_pAniCtrl)
		return;

	m_iAniIndex = iIndex;
	m_pAniCtrl->Set_Animationset(iIndex, fspeed, fSyncTime, fDelay);
}

void CDynamicMesh::Play_Animationset(const _float & fTimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return;

	m_pAniCtrl->Play_Animationset(fTimeDelta);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(-90)));

}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName, wSceneIndex)))
		Safe_Release(pInstance);

	return pInstance;
}
CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex, const _bool& bPhys)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName, wSceneIndex, bPhys)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CDynamicMesh::Clone(void* pArg)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		if (m_pRootFrame)
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

void CDynamicMesh::Get_FrameMatrix()
{
	//D3DXMESHCONTAINER_DERIVED*		pMeshContainer = m_MeshContainerList.back();
	//pMeshContainer->pFrameOffsetMatrix[i] * *pMeshContainer->ppFrameCombinedMatrix[i];
}



