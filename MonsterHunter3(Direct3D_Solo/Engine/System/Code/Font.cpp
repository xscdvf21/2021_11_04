#include "Font.h"

USING(Engine)

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CFont::~CFont(void)
{

}

HRESULT Engine::CFont::Ready_Font(const _tchar* pFontType,
									const _uint& iWidth,
									const _uint& iHeight,
									const _uint& iWeight)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = DEFAULT_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);
	Font_Desc.Width  = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT	rc{ _long(pPos->x), _long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}


void Engine::CFont::Render_Font3D(const _tchar* pString, const _vec2* pPos, const _matrix _PlayerWorld, D3DXCOLOR Color)
{

	_matrix		matBill, matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vec3));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vec3));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vec3));


	_matrix matRotX, matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRotX);

	D3DXMatrixRotationX(&matRotX, D3DXToRadian(180));



	matWorld._11 = 0.01f;
	matWorld._22 = 0.01f;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	matWorld *= matRotX;
	matWorld *= matBill;

	matWorld._41 = _PlayerWorld._41 + 1.f;
	matWorld._42 = _PlayerWorld._42 + 1.f;
	matWorld._43 = _PlayerWorld._43;



	/*matWorld = matBill * matWorld;*/
	//matWorld._41 -= 1.f;
	//matWorld._42 += 10.f;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), NULL, DT_NOCLIP, Color);

	m_pSprite->End();
}




CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight)
{
	CFont*	pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		MSG_BOX("Font Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CFont::Free(void)
{
	Engine::Safe_Release(m_pFont);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pGraphicDev);
}

