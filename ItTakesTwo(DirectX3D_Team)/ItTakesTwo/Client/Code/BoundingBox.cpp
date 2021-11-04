#include "stdafx.h"
#include "BoundingBox.h"

CBoundingBox::CBoundingBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CBoundingBox::~CBoundingBox(void)
{
	
}

void CBoundingBox::Move_Pos(_vec3 vDir, const _float & fTimeDelta, _float fSpeed)
{
	m_vPos += vDir * fTimeDelta * fSpeed;
}

HRESULT CBoundingBox::Ready_Collider(_vec3 vScale, _vec4 argb)
{
	m_vScale = vScale;
	LPD3DXMESH texMesh;
	FAILED_CHECK_RETURN(D3DXCreateBox(m_pGraphicDev, 1.0f, 1.0f, 1.0f, &texMesh, nullptr), E_FAIL);
	texMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, m_pGraphicDev, &m_pBuffer);
	texMesh->Release();

	VTXTEX* pVerts;
	if (SUCCEEDED(m_pBuffer->LockVertexBuffer(0, (void **)&pVerts))) {

		// get vertex count
		int numVerts = m_pBuffer->GetNumVertices();

		// loop through the vertices
		for (int i = 0; i < numVerts; i++) {

			// calculate texture coordinates
			pVerts->vTexUV.x = asinf(pVerts->vNormal.x) / D3DX_PI + 0.5f;
			pVerts->vTexUV.y = asinf(pVerts->vNormal.y) / D3DX_PI + 0.5f;

			// go to next vertex
			pVerts++;
		}

		// unlock the vertex buffer
		m_pBuffer->UnlockVertexBuffer();
	}


	D3DXCreateTexture(m_pGraphicDev,
		5,
		5,
		1, // miplevel
		0,	// 텍스처의 용도
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTexture);

	D3DLOCKED_RECT LockRect;
	m_pTexture->LockRect(0, &LockRect, NULL, 0);

	for (_int i = 0; i < 5; ++i)
	{
		for (_int j = 0; j < 5; ++j)
		{
			_int	iIndex = i * 5 + j;

			*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB((_int)argb.x, (_int)argb.y, (_int)argb.z, (_int)argb.w);
		}
	}
	
	return S_OK;
}

void CBoundingBox::Render_Collider(_matrix pParentMatrix, _bool debug, LPD3DXEFFECT & pEffect)
{
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, pParentMatrix._41, pParentMatrix._42, pParentMatrix._43);
	
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &(matScale * matTrans));

	_uint	iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(6);
	if (m_pBuffer && debug)
	{
		m_pBuffer->DrawSubset(0);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CBoundingBox::Render_Collider(_vec3 vPos, _bool debug, LPD3DXEFFECT & pEffect)
{
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	NULL_CHECK(pEffect);
	pEffect->AddRef();

	pEffect->SetTexture("g_BasetTexture", m_pTexture);
	pEffect->SetMatrix("g_matWorld", &(matScale * matTrans));

	_uint	iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(8);
	if (m_pBuffer && debug)
	{
		m_pBuffer->DrawSubset(0);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CBoundingBox::Render_Collider(_bool debug, LPD3DXEFFECT & pEffect)
{
	D3DXQUATERNION qX, qY, qZ;
	_matrix matScale, matTrans, matQuat;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	
	D3DXQuaternionRotationAxis(&qX, &_vec3{ 1.f,0.f,0.f }, m_vAngle.x);
	D3DXQuaternionRotationAxis(&qY, &_vec3{ 0.f,1.f,0.f }, m_vAngle.y);
	D3DXQuaternionRotationAxis(&qZ, &_vec3{ 0.f,0.f,1.f }, m_vAngle.z);
	D3DXMatrixRotationQuaternion(&matQuat, &(qX*qY*qZ));

	NULL_CHECK(pEffect);
	pEffect->AddRef();

	pEffect->SetTexture("g_BasetTexture", m_pTexture);
	pEffect->SetMatrix("g_matWorld", &(matScale * matQuat* matTrans));

	_uint	iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(8);
	if (m_pBuffer && debug)
	{
		m_pBuffer->DrawSubset(0);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CBoundingBox* CBoundingBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vScale, _vec4 argb)
{
	CBoundingBox*	pInstance = new CBoundingBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(vScale, argb)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoundingBox::Free(void)
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pTexture);

	if (m_pBuffer)
		m_pBuffer->Release();
}

CComponent* CBoundingBox::Clone(void* pArg)
{
	return new CBoundingBox(*this);
}

