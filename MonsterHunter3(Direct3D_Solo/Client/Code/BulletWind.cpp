#include "stdafx.h"
#include "BulletWind.h"

#include "Export_Fucntion.h"

CBulletWind::CBulletWind(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBulletWind::~CBulletWind()
{
}

HRESULT CBulletWind::Ready_Object(_vec3 vDir,_vec3 vPos, _float fDir)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	_vec3 vSetDir = vDir;

	m_vMoveDir = vDir;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
	D3DXVec3Normalize(&vSetDir, &vSetDir);

	vSetDir = vSetDir * -1.f;
	vSetDir = vSetDir * fDir;

	_vec3 vSetPos = vPos + vSetDir;
	m_pTransformCom->Set_Pos(vSetPos);

	return S_OK;
}

_int CBulletWind::Update_Object(const _float & TimeDelta)
{


	m_pTransformCom->Move_Pos(&m_vMoveDir, 30.f, TimeDelta);

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



	D3DXMatrixScaling(&matScale, 10.f, 10.f, 1.f);

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pTransformCom->Set_WorldMatrix(&(matScale * matBill * matWorld));


	_vec3 vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	if (vPos.y <= 0.f)
	{
		return OBJ_DEAD;
	}

	Compute_ViewZ(&vPos);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_BLACK, this);


	return OBJ_NOEVENT;
}

void CBulletWind::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CBulletWind::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Effect_BulletWind"));
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

CBulletWind * CBulletWind::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vDir, _vec3 vPos, _float fDir)
{
	CBulletWind*	pInstance = new CBulletWind(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vDir, vPos, fDir)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBulletWind::Free(void)
{
	Engine::CGameObject::Free();
}
