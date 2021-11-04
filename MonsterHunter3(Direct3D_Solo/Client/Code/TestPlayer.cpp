#include "stdafx.h"
#include "TestPlayer.h"

#include "Export_Fucntion.h"


CTestPlayer::CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTestPlayer::~CTestPlayer(void)
{

}


HRESULT CTestPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CTestPlayer::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CTestPlayer::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTestPlayer* CTestPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestPlayer*	pInstance = new CTestPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTestPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTestPlayer::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);
	
	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CTestPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(Engine::INFO_UP, &m_vDir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&m_vDir, 5.f, fTimeDelta);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&m_vDir, -5.f, fTimeDelta);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f * fTimeDelta));
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(-90.f * fTimeDelta));
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(90.f * fTimeDelta));
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(-90.f * fTimeDelta));
	}

}

