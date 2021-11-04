#include "stdafx.h"
#include "ToolTerrain.h"

#include "Export_Function.h"
#include "..\Engine\Resources\Code\Texture.h"


CToolTerrain::CToolTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CToolTerrain::~CToolTerrain(void)
{
}

HRESULT CToolTerrain::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	return S_OK;
}

Engine::_int CToolTerrain::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return OBJ_NOEVENT;
}

void CToolTerrain::Render_Object(const _int& iIndex)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//m_pTextureCom->Set_Texture(m_TextureIndex);

	FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CToolTerrain* CToolTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CToolTerrain*	pInstance = new CToolTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CToolTerrain::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CToolTerrain::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	VECVTX Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(VECVTX));
	}

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CNormalTerrainTex*>(Engine::Clone_Resource(0, L"Buffer_NormalTerrainTex", pArg));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(0, Temp.tagTexture));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(Temp.tagTexture, pComponent);
	
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	return S_OK;
}

HRESULT CToolTerrain::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(tMtrlInfo));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}

HRESULT CToolTerrain::Change_Texture(_uint iIndex)
{
	m_TextureIndex = iIndex;

	return S_OK;
}
