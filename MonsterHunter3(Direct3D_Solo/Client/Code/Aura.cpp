#include "stdafx.h"
#include "Aura.h"
#include "Player.h"
#include "Export_Fucntion.h"

CAura::CAura(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CAura::~CAura()
{
}

HRESULT CAura::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(2.f, 3.f, 2.f));
	m_bBuff = true;

	return S_OK;
}

_int CAura::Update_Object(const _float & fTimeDelta)
{

	m_fLiftDelTime += fTimeDelta;
	
	if (m_fLiftDelTime > m_fLifeTime)
	{
		return OBJ_DEAD;
	}


	m_fFrame += 30.f * fTimeDelta;

	if (30.f < m_fFrame)
		m_fFrame = 0.f;

	
	CManagement*		pManagement = CManagement::GetInstance();
	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform",COMPONENTID::ID_DYNAMIC));
	_vec3				vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);


	CCamera*		pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));
	_vec3	vEye;
	_vec3	vAt;
	_vec3	vUp;

	pCamera->Get_EyeAtUp(&vEye, &vAt, &vUp);


	_vec3  vPos2;
	vPos2 = vEye - vAt;

	D3DXVec3Normalize(&vPos2, &vPos2);
	
	vPos2 = vPos2 * 3.f;
	

	m_pTransformCom->Set_Pos(vPlayerPos + vPos2);


	Engine::CGameObject::Update_Object(fTimeDelta);

	_matrix		matBill, matView, matWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));


	_vec3 vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_BLACK, this);

	return OBJ_NOEVENT;

}

void CAura::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CAura::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Effect_Aura"));
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

CAura * CAura::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAura*	pInstance = new CAura(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CAura::Free(void)
{
	Engine::CGameObject::Free();
}
