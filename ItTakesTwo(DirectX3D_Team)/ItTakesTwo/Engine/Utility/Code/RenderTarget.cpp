#include "RenderTarget.h"

USING(Engine)

Engine::CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pTargetSurface(nullptr)
	, m_pOldTargetSurface(nullptr)
	, m_pTargetTexture(nullptr)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CRenderTarget::~CRenderTarget(void)
{

}

HRESULT Engine::CRenderTarget::Ready_RenderTarget(const _uint& iWidth,
	const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	if (FAILED(D3DXCreateTexture(m_pGraphicDev,
		iWidth,
		iHeight,
		1,	// mipmap
		D3DUSAGE_RENDERTARGET,
		Format,
		D3DPOOL_DEFAULT,
		&m_pTargetTexture)))
		return E_FAIL;

	m_ClearColor = Color;

	// 텍스처로부터 렌더타겟 요소를 추출한다. 첫번째 : 만약 밉맵을 만들었을 경우 레벨 지정이 가능하나 우리는 하나만 만들어서 0번으로 지정
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);
	Safe_Release(m_pTargetTexture);

	return S_OK;
}

void Engine::CRenderTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);

}

void CRenderTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
}

void CRenderTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldTargetSurface);

	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldTargetSurface);

	Safe_Release(m_pOldTargetSurface);
}

HRESULT CRenderTarget::Ready_DebugBuffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // 정적 버퍼 사용 시 숫자 0(D3DUSAGE_DYNAMIC : 파티클을 생성할 때)
		FVF_SCREEN,
		D3DPOOL_MANAGED, // 일반적으로 정적 버퍼 사용 시 Managed, 동적버퍼 사용 시 Default 사용
		&m_pVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, NULL), E_FAIL);


	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(fX - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4((fX + fSizeX) - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4((fX + fSizeX) - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(fX - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
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

void CRenderTarget::Render_DebugBuffer(void)
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(FVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}



void Engine::CRenderTarget::SetUp_OnShader(LPD3DXEFFECT& pEffect, const char* pConstantTable)
{
	pEffect->SetTexture(pConstantTable, m_pTargetTexture);
}
void Engine::CRenderTarget::SetUp_OnRegister(_int RegisterIndex)
{
	_int sampler = 0;
	switch (RegisterIndex)
	{
	case 0:
		sampler = D3DVERTEXTEXTURESAMPLER0;
		break;
	case 1:
		sampler = D3DVERTEXTEXTURESAMPLER1;
		break;
	case 2:
		sampler = D3DVERTEXTEXTURESAMPLER2;
		break;
	case 3:
		sampler = D3DVERTEXTEXTURESAMPLER3;
		break;
	default:
		return;
		break;
	}
		m_pGraphicDev->SetTexture(sampler, m_pTargetTexture);
		m_pGraphicDev->SetSamplerState(sampler, D3DSAMPLERSTATETYPE::D3DSAMP_MINFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_POINT);
		m_pGraphicDev->SetSamplerState(sampler, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_POINT);
		m_pGraphicDev->SetSamplerState(sampler, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXTUREFILTERTYPE::D3DTEXF_NONE);
		m_pGraphicDev->SetSamplerState(sampler, D3DSAMPLERSTATETYPE::D3DSAMP_ADDRESSU, D3DTEXTUREADDRESS::D3DTADDRESS_WRAP);
		m_pGraphicDev->SetSamplerState(sampler, D3DSAMPLERSTATETYPE::D3DSAMP_ADDRESSV, D3DTEXTUREADDRESS::D3DTADDRESS_WRAP);
}

CRenderTarget* Engine::CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget*		pInstance = new CRenderTarget(pGraphicDev);

	if (FAILED(pInstance->Ready_RenderTarget(iWidth, iHeight, Format, Color)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRenderTarget::Free(void)
{

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pOldTargetSurface);
	Safe_Release(m_pTargetSurface);
	Safe_Release(m_pGraphicDev);

}

