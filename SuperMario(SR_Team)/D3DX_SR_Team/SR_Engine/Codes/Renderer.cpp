#include "..\Headers\Renderer.h"
#include "GameObject.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer(/*LPDIRECT3DDEVICE9 pDevice*/)
//: m_pDevice(pDevice)
{
	//Safe_AddRef(m_pDevice);
}


HRESULT CRenderer::Set_Device(_Device pDevice, LPD3DXSPRITE pSprite, LPD3DXFONT pFont)
{
	if (nullptr == pDevice)
		return E_FAIL;

	m_pDevice = pDevice;
	m_pSprite = pSprite;
	m_pFont = pFont;

	Safe_AddReference(m_pDevice);
	Safe_AddReference(m_pSprite);
	Safe_AddReference(m_pFont);

	return S_OK;
}

/* 매 프레임마다 렌더러에 렌더링 대상을 추가 */
HRESULT CRenderer::Add_RenderList(ERenderPriority ePriority, CGameObject* pGameObject)
{
	if (ERenderPriority::End <= ePriority ||
		nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[(_uint)ePriority].push_back(pGameObject);
	Safe_AddReference(pGameObject);

	return S_OK;
}


/* 매 프레임마다 렌더링함 */
HRESULT CRenderer::Render_RenderList(HWND hWND /*= nullptr*/)
{
	if (nullptr == m_pDevice)
	{
		return E_FAIL;
	}

	if (FAILED(m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0)))
	{	
	return E_FAIL;
	}

	if (FAILED(m_pDevice->BeginScene()))
		return E_FAIL;

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NoAlpha()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_HalfAlpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(m_pDevice->EndScene()))
		return E_FAIL;

	if (FAILED(m_pDevice->Present(nullptr, nullptr, hWND, nullptr)))
		return E_FAIL;

	return S_OK;

}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_GameObjects[(_uint)ERenderPriority::Priority])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)ERenderPriority::Priority].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NoAlpha()
{
	for (auto& pGameObject : m_GameObjects[(_uint)ERenderPriority::NoAlpha])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)ERenderPriority::NoAlpha].clear();

	return S_OK;
}
//알파 블랜딩 필수... 왠만하면 쓸일 없기를.
HRESULT CRenderer::Render_HalfAlpha()
{

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
	{
		return E_FAIL;
	}
	//알파블랜딩 옵션 적용
	if (FAILED(m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD)))
	{
		return E_FAIL;
	}
	//Source픽셀(=그려야 할 픽셀) 의 혼합 비율 설정
	if (FAILED(m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
	{
		return E_FAIL;
	}
	//Dest픽셀의 혼합 비율 설정
//D3DRS_DESTBLEND: Dest픽셀(이미 그려져있는 픽셀)
//D3DBLEND_INVSRCALPHA: 혼합비율이며 1 - D3DBLEND_SRCALPHA. 0~1범위.
//ex) D3DBLEND_SRCALPHA(0.7f, 0.7f, 0.7f, 0.7f) -> D3DBLEND_INVSRCALPHA(0.3f, 0.3f, 0.3f, 0.3f)
//*/
	if (FAILED(m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
	{
		return E_FAIL;
	}

	for (auto& pGameObject : m_GameObjects[(_uint)ERenderPriority::HalfAlpha])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)ERenderPriority::HalfAlpha].clear();


	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//알파 테스팅
HRESULT CRenderer::Render_Alpha()
{
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		return E_FAIL;

	/* 알파 기준값 설정 */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
		return E_FAIL;

	/* 알파 테스팅 함수 수행. */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		return E_FAIL;


	for (auto& pGameObject : m_GameObjects[(_uint)ERenderPriority::Alpha])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)ERenderPriority::Alpha].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}
//와이어프레임 세팅 필요
HRESULT CRenderer::Render_Wireframe()
{
	//연산 많이 처먹으니까 오브젝트 하나도 없으면 생략
	if (m_GameObjects[(_uint)ERenderPriority::WireFrame].empty())
	{
		return S_OK;
	}

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (auto& pGameObject : m_GameObjects[(_uint)ERenderPriority::WireFrame])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)ERenderPriority::WireFrame].clear();

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}
//알파 테스팅 먹임
HRESULT CRenderer::Render_UI()
{
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		return E_FAIL;

	/* 알파 기준값 설정 */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
		return E_FAIL;

	/* 알파 테스팅 함수 수행. */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		return E_FAIL;


	for (auto& pGameObject : m_GameObjects[(_uint)ERenderPriority::UI])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)ERenderPriority::UI].clear();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

void CRenderer::Free()
{
	for (_uint i = 0; i < (_uint)ERenderPriority::End; ++i)
	{
		for (auto& pGameObject : m_GameObjects[i])
		{
			Safe_Release(pGameObject);
		}

		m_GameObjects[i].clear();
	}
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
}
