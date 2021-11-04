#include "stdafx.h"
#include "VacuumCircle.h"
#include "Cody.h"
#include "May.h"
#include "Export_Function.h"

CVacuumCircle::CVacuumCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_vScale(0.01f, 0.01f, 0.01f)
	, m_fCodyHitTime(0.f), m_fMayHitTime(0.f)
{
}

CVacuumCircle::~CVacuumCircle()
{
}

HRESULT CVacuumCircle::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Circle);
	Engine::PlaySoundW(L"Vacuum_Circle.wav", CSoundMgr::CHANNELID::Vacuum_Circle, 0.3f);

	return S_OK;
}

_int CVacuumCircle::Update_Object(const _float & fTimeDelta)
{
	m_vScale.x += 1.2f;
	m_vScale.y += 1.2f;
	m_vScale.z += 1.2f;
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));

	Damage(fTimeDelta);

	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > 4.5f)
		return OBJ_DEAD;

	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuumCircle::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CVacuumCircle::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(5);
							
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CVacuumCircle::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"VBCircle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CVacuumCircle::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);
	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 1);

	pEffect->SetVector("g_vColor", &_vec4(255.f * 1.8f, 255.f * 1.8f, 255.f * 1.8f, 255.f * 1.8f));



	return S_OK;
}

void CVacuumCircle::Damage(const _float & fTimeDelta)
{
	//m_fCodyHitTime += fTimeDelta;
	m_fMayHitTime += fTimeDelta;
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	CCody*	pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CTransform*	 pCodyTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

	_vec3 vCodyPos = pCodyTrans->Get_Position();
	_vec3 vDir = vPos - vCodyPos;
	_float fDir;
	fDir = D3DXVec3Length(&vDir);

	_float x, y;

	x = m_vScale.x -23.f;
	y = m_vScale.x -20.f;
 
	if (x < fDir && fDir < y && CPlayerValue::PL_JUMP != pCody->Get_PlayerState())
	{
		if (!m_bCodyHit)
		{
			pCody->Set_Damage(-1);
			m_bCodyHit = true;
		}

	}

	CMay*	pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
	CTransform*	 pMayTrans = (CTransform*)pMay->Get_Component(L"Com_Transform", ID_DYNAMIC);

	_vec3 vMayPos = pMayTrans->Get_Position();
	_vec3 vDir2 = vPos - vCodyPos;
	_float fDir2;
	fDir2 = D3DXVec3Length(&vDir2);

	if (x < fDir2 && fDir2 < y && CPlayerValue::PL_JUMP != pMay->Get_PlayerState())
	{

		if (!m_bMayHit)
		{
			
			pMay->Set_Damage(-1);
			m_bMayHit = true;
		}
		//if (m_fMayHitTime > 1.f)
		//{
		//	pMay->Set_Damage(-1);
		//	m_fMayHitTime = 0.f;
		//}
	}
}

CVacuumCircle * CVacuumCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CVacuumCircle*	pInstance = new CVacuumCircle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumCircle::Free(void)
{
	Engine::CGameObject::Free();
}