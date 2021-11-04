#include "stdafx.h"
#include "BaboonBombBullet.h"
#include "BaboonGround.h"
#include "Export_Function.h"


CBaboonBombBullet::CBaboonBombBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev)
	, m_fScale(0.001f)
{
}

CBaboonBombBullet::~CBaboonBombBullet()
{
}

HRESULT CBaboonBombBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(90.f));

	PlaySoundOnce(L"LaserBomb.wav", Engine::CSoundMgr::CHANNELID::Space_Boss_Bomb, 0.8f);
	
	return S_OK;
}

_int CBaboonBombBullet::Update_Object(const _float & fTimeDelta)
{


	CBaboonGround*			pGround = (CBaboonGround*)Get_GameObject(L"GameObject", L"BaboonGround");
	CTransform*				pGroundTrans = (CTransform*)pGround->Get_Component(L"Com_Transform", ID_DYNAMIC);


	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_PosY(pGroundTrans->Get_Position().y + 0.5f);
	m_fScale += 0.002f;
	m_fTimeDelta += fTimeDelta;

	if (m_fScale > 0.17f)
	{
		m_fScale = 0.17f;
	}

	if (m_fTimeDelta > 2.5f)
		m_bDead = true;

	if (m_bDead)
		return OBJ_DEAD;



	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}


_int CBaboonBombBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CBaboonBombBullet::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 29);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CBaboonBombBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_SB, L"IceOrb"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(0, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	return S_OK;
}

HRESULT CBaboonBombBullet::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
	pEffect->SetFloat("g_DeltaTime", m_fTimeDelta);

	return S_OK;
}

void CBaboonBombBullet::Move(const _float & fTimeDelta)
{
}

CBaboonBombBullet * CBaboonBombBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBaboonBombBullet*	pInstance = new CBaboonBombBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaboonBombBullet::Free(void)
{
	CEnermy::Free();
}
