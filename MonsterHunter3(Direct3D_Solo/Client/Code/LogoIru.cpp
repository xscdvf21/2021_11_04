#include "stdafx.h"
#include "LogoIru.h"
#include "SoundMgr.h"
#include "Logo.h"
#include "Export_Fucntion.h"

CLogoIru::CLogoIru(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)

{
}

CLogoIru::~CLogoIru()
{
}

HRESULT CLogoIru::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = _vec3(0.005f, 0.005f, 0.005f);
	m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(90));
	m_pTransformCom->Set_Pos(_vec3(-0.7f, 0.5f, 0.5f));

	/*m_pTransformCom->Set_Pos(_vec3(-0.7f, -0.7f, 0.5f));*/ //영상 찍을때 이위치로.

	m_pMeshCom->Set_Animationset(3);

	return S_OK;

}

_int CLogoIru::Update_Object(const _float & fTimeDelta)
{
	if (m_bLoading && !m_bLoadingEnd)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::IRU);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Hello0.wav", CSoundMgr::CHANNELID::IRU);
		m_pMeshCom->Set_Animationset(6);
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(90));
		m_bLoadingEnd = true;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CLogoIru::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	/*m_pNaviMeshCom->Render_NaviMesh();*/

	m_pMeshCom->Render_Meshes();

}

HRESULT CLogoIru::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;
	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Logo_Iru"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);


	return S_OK;
}

CLogoIru * CLogoIru::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoIru*	pInstance = new CLogoIru(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogoIru::Free(void)
{
	Engine::CGameObject::Free();
}
