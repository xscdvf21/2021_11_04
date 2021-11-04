#include "Light.h"

USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_iIndex(0)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

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

	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);


	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(-0.5f, -0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4(ViewPort.Width - 0.5f, -0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(-0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
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

	//m_pGraphicDev->SetLight(iIndex, pLightInfo);
	//m_pGraphicDev->LightEnable(iIndex, TRUE);


	return S_OK;
}

void CLight::Render_Light(LPD3DXEFFECT & pEffect)
{
	if (!m_bRender)
		return;
	if (m_bStart)
	{
		m_bStart = false;
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
		pEffect->SetVector("g_vLightPos", &_vec4(m_tLightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_tLightInfo.Range);
	}
	_uint iPass = 0;

	if (D3DLIGHT_DIRECTIONAL == m_tLightInfo.Type)
	{
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
		pEffect->SetFloat("g_fPower", 1.f);
		iPass = 0;
	}

	else if (D3DLIGHT_POINT == m_tLightInfo.Type)
	{
		pEffect->SetVector("g_vLightPos", &_vec4(m_tLightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_tLightInfo.Range);
		pEffect->SetFloat("g_fPower", 50.f);
		iPass = 1;
	}

	else if (D3DLIGHT_SPOT == m_tLightInfo.Type)
	{
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
		pEffect->SetVector("g_vLightPos", &_vec4(m_tLightInfo.Position, 1.f));
		pEffect->SetFloat("g_fPower", 50.f);
		pEffect->SetFloat("g_fRange", m_tLightInfo.Range);
		iPass = 2;
	}

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);
	//pEffect->SetFloat("g_fPower", 1.f);
	pEffect->SetFloat("g_fCosOuter", 1.f);
	pEffect->SetFloat("g_fCosInner", 0.5f);

	//float		g_fCosOuter;
	//float		g_fCosInner;

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);

	pEffect->CommitChanges();

	pEffect->BeginPass(iPass);
	pEffect->CommitChanges();

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(FVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
}

CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*	pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pGraphicDev);
}

