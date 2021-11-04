#include "ParticleBuffer.h"

USING(Engine)

Engine::CParticleBuffer::CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CParticleBuffer::CParticleBuffer(const CParticleBuffer& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CParticleBuffer::~CParticleBuffer(void)
{

}

HRESULT Engine::CParticleBuffer::Ready_Buffer(void)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXPARTICLE);
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwFVF = FVF_PARTICLE;

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
		D3DUSAGE_DYNAMIC|D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY, // 정적 버퍼 사용 시 숫자 0(D3DUSAGE_DYNAMIC : 파티클을 생성할 때)
		m_dwFVF,
		D3DPOOL_DEFAULT, // 일반적으로 정적 버퍼 사용 시 Managed, 동적버퍼 사용 시 Default 사용
		&m_pVB, nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB, nullptr), E_FAIL);

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
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

void Engine::CParticleBuffer::Render_Buffer(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, (DWORD)m_fSize);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, (DWORD)0.2);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, (DWORD) 5.0);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, 0);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, 0);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, 1);


	
}



HRESULT Engine::CParticleBuffer::Ready_Buffer(const _bool& bScreen, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{

	Ready_Buffer();


	return S_OK;
}

Engine::CParticleBuffer* Engine::CParticleBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _bool& bScreen /*=false */, const _float& fX/*=0*/, const _float& fY/*=0*/, const _float& fSizeX/*=1920*/, const _float& fSizeY/*=1080*/)
{
	CParticleBuffer*		pInstance = new CParticleBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(bScreen, fX, fY, fSizeX, fSizeY)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CParticleBuffer::Change_Size(const _float& fSpeed, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
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

CComponent * CParticleBuffer::Clone(void* pArg)
{
	return new CParticleBuffer(*this);
}

Engine::_mat Engine::CParticleBuffer::Get_Scale()
{
	_mat matScale;
	D3DXMatrixScaling(&matScale, m_vCurrentSize.z - m_vCurrentSize.x, m_vCurrentSize.w - m_vCurrentSize.y, 1.f);
	return matScale;
}

Engine::_mat Engine::CParticleBuffer::Get_Pos()
{
	_mat matTrans;
	D3DXMatrixTranslation(&matTrans, (m_vCurrentSize.x + m_vCurrentSize.z)*0.5f - (WINCX >> 1), (m_vCurrentSize.y + m_vCurrentSize.w)*0.5f - (WINCY >> 1), 0.f);
	return matTrans;
}

_vec2 Engine::CParticleBuffer::Update_Buffer(const _float& fDeltaTime)
{
	if (!m_bChange)
		return _vec2{ m_vCurrentSize.z - m_vCurrentSize.x,m_vCurrentSize.w - m_vCurrentSize.y };
	if (D3DXVec4Length(&(m_vTargetSize - m_vCurrentSize)) < D3DXVec4Length(&m_vMoveSize) / m_fChangeSpeed)
	{
		m_vCurrentSize = m_vTargetSize;
		m_bChange = false;
	}
	else
	{

		m_vCurrentSize += m_vMoveSize / m_fChangeSpeed*fDeltaTime;
	}
	return _vec2{ m_vCurrentSize.z - m_vCurrentSize.x,m_vCurrentSize.w - m_vCurrentSize.y };

	//VTXTEX*		pVertex = NULL;

	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	////pVertex[0].vPosition = _vec3(m_vCurrentSize.x - 0.5f, m_vCurrentSize.y - 0.5f, 0.f);

	////pVertex[1].vPosition = _vec3((m_vCurrentSize.x + m_vCurrentSize.z) - 0.5f, m_vCurrentSize.y - 0.5f, 0.f);

	////pVertex[2].vPosition = _vec3((m_vCurrentSize.x + m_vCurrentSize.z) - 0.5f, (m_vCurrentSize.y + m_vCurrentSize.w) - 0.5f, 0.f);

	////pVertex[3].vPosition = _vec3(m_vCurrentSize.x - 0.5f, (m_vCurrentSize.y + m_vCurrentSize.w) - 0.5f, 0.f);

	//pVertex[0].vPosition = _vec3(m_vTargetSize.x - 0.5f, m_vTargetSize.y - 0.5f, 0.f);

	//pVertex[1].vPosition = _vec3((m_vTargetSize.x + m_vTargetSize.z) - 0.5f, m_vTargetSize.y - 0.5f, 0.f);

	//pVertex[2].vPosition = _vec3((m_vTargetSize.x + m_vTargetSize.z) - 0.5f, (m_vTargetSize.y + m_vTargetSize.w) - 0.5f, 0.f);

	//pVertex[3].vPosition = _vec3(m_vTargetSize.x - 0.5f, (m_vTargetSize.y + m_vTargetSize.w) - 0.5f, 0.f);

	//m_pVB->Unlock();
}

void Engine::CParticleBuffer::Free(void)
{
	CVIBuffer::Free();
}

