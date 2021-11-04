#include "stdafx.h"
#include "BulletBust.h"
#include "SoundMgr.h"


#include "Export_Fucntion.h"

CBulletBust::CBulletBust(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBulletBust::~CBulletBust()
{
}

HRESULT CBulletBust::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	
	m_pTransformCom->Set_Pos(vPos);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::EFFECT);
	CSoundMgr::Get_Instance()->PlaySoundW(L"FireExp.wav", CSoundMgr::CHANNELID::EFFECT);

	return S_OK;
}

_int CBulletBust::Update_Object(const _float & fTimeDelta)
{



	m_fFrame += 32.f * fTimeDelta;

	if (m_fFrame >= 32.f)
	{

		return OBJ_DEAD;
	}


	Engine::CGameObject::Update_Object(fTimeDelta);

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
	D3DXMatrixScaling(&matScale, 15.f, 15.f, 1.f);


	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pTransformCom->Set_WorldMatrix(&(matScale * matBill * matWorld));


	_vec3 vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_BLACK, this);

	return OBJ_NOEVENT;
}

void CBulletBust::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();

}

HRESULT CBulletBust::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Effect_Bust"));
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

CBulletBust * CBulletBust::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Pos)
{
	CBulletBust*	pInstance = new CBulletBust(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Pos)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBulletBust::Free(void)
{
	Engine::CGameObject::Free();
}
