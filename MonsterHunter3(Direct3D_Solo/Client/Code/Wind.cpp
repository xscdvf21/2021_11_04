#include "stdafx.h"
#include "Wind.h"

#include "Player.h"
#include "Export_Fucntion.h"


CWind::CWind(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CWind::~CWind()
{
}

HRESULT CWind::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Pos(_vec3(30.f, 2.f, 220.f));

	return S_OK;
}

_int CWind::Update_Object(const _float & TimeDelta)
{
	m_fLiftDelTime += TimeDelta;

	if (m_fLiftDelTime > m_fLifeTime)
	{
		return OBJ_DEAD;
	}


	m_fFrame += 4.f * TimeDelta;

	if (4.f < m_fFrame)
		m_fFrame = 0.f;


	CManagement*		pManagement = CManagement::GetInstance();
	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
	_vec3				vPlayerPos;
	_vec3				vPlayerLook;

	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos.y = 1.f;
	pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_vec3 vBackLook;

	vBackLook = vPlayerLook * -1.f;

	vBackLook = vBackLook * 2.f;
	vPlayerPos += vBackLook;
	m_pTransformCom->Set_Pos(vPlayerPos);



	Engine::CGameObject::Update_Object(TimeDelta);

	_matrix		matBill, matView, matWorld, matScale;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);


	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vec3));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vec3));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vec3));

	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	D3DXMatrixScaling(&matScale, 3.f, 3.f, 1.f);

	m_pTransformCom->Set_WorldMatrix(&(matScale * matBill * matWorld));


	_vec3 vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_BLACK, this);

	return OBJ_NOEVENT;
}

void CWind::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
		
}

HRESULT CWind::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Effect_Wind"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

CWind * CWind::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWind*	pInstance = new CWind(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CWind::Free(void)
{
	Engine::CGameObject::Free();

}
