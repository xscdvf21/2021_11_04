#include "RcTex.h"

USING(Engine)

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcTex::~CRcTex(void)
{

}

HRESULT Engine::CRcTex::Ready_Buffer(void)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwFVF = FVF_TEX;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vNormal = _vec3{ 0.f,0.f,1.f };
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vNormal = _vec3{ 0.f,0.f,1.f };
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vNormal = _vec3{ 0.f,0.f,1.f };
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vNormal = _vec3{ 0.f,0.f,1.f };
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CRcTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}



HRESULT Engine::CRcTex::Ready_Buffer(const _bool& bScreen, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{

	Ready_Buffer();


	return S_OK;
}

Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _bool& bScreen /*=false */, const _float& fX/*=0*/, const _float& fY/*=0*/, const _float& fSizeX/*=1920*/, const _float& fSizeY/*=1080*/)
{
	CRcTex*		pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(bScreen, fX, fY, fSizeX, fSizeY)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRcTex::Change_Size(const _float& fSpeed, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{

	m_fChangeSpeed = fSpeed;
	m_vTargetSize.x = fX;
	m_vTargetSize.y = fY;
	m_vTargetSize.z = fSizeX;
	m_vTargetSize.w = fSizeY;

	m_vMoveSize = m_vTargetSize - m_vCurrentSize;
	//m_vCurrentSize = m_vTargetSize;
	m_bChange = true;
}

CComponent * CRcTex::Clone(void* pArg)
{
	return new CRcTex(*this);
}

void Engine::CRcTex::Set_Size(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	m_vCurrentSize = { fX,fY,fSizeX,fSizeY };
}

Engine::_vec4 Engine::CRcTex::Update_Buffer(const _float& fDeltaTime)
{
	if (!m_bChange)
		return m_vCurrentSize;
	if (D3DXVec4Length(&(m_vTargetSize - m_vCurrentSize)) < D3DXVec4Length(&m_vMoveSize) / m_fChangeSpeed)
	{
		m_vCurrentSize = m_vTargetSize;
		m_bChange = false;
	}
	else
	{

		m_vCurrentSize += m_vMoveSize / m_fChangeSpeed*fDeltaTime;
	}
	return m_vCurrentSize;
}

Engine::_mat Engine::CRcTex::Get_Scale()
{
	_mat matScale;
	D3DXMatrixScaling(&matScale, m_vCurrentSize.z - m_vCurrentSize.x, m_vCurrentSize.w - m_vCurrentSize.y, 1.f);
	return matScale;
}

Engine::_mat Engine::CRcTex::Get_Pos()
{
	_mat matTrans;
	D3DXMatrixTranslation(&matTrans, (m_vCurrentSize.x + m_vCurrentSize.z)*0.5f - (WINCX >> 1), (m_vCurrentSize.y + m_vCurrentSize.w)*0.5f - (WINCY >> 1), 0.f);
	return matTrans;
}

void Engine::CRcTex::Free(void)
{
	CVIBuffer::Free();
}

