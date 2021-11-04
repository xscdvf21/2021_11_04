#include "stdafx.h"
#include "Cog.h"
#include "Export_Function.h"



CCog::CCog(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCog::~CCog()
{
}

void CCog::Move(const _float & fTimeDelta)
{
	if (fabs(m_fValue - m_fRotValue) < 0.2f)
		return;
	_vec3 vAngle = m_pTransformCom->Get_Angle();

	m_pTransformCom->Set_AngleZ(m_fRotValue + ((((m_fValue)-(m_fRotValue)) / m_fTotalTime)*fTimeDelta));

}

HRESULT CCog::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	return S_OK;
}

_int CCog::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
	}

	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CCog::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCog::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);			//	1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	m_pMeshCom->Render_Meshes(pEffect, 8);	//	pEffect->BeginPass(0);
											//	스태틱매쉬8
											//	다이나믹6


	pEffect->End();

	Safe_Release(pEffect);
}

void* CCog::On_Active(void* pCaller)
{
	PxVec3 vDir = static_cast<CGameObject*>(pCaller)->Get_Up();
	if (m_bMove || m_vDir == ToVec3(vDir))
		return nullptr;

	m_bMove = true;
	m_vDir = ToVec3(vDir);
	m_vGoal = m_vStartPos + -m_vDir * m_fDist;
	return nullptr;
}

HRESULT CCog::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, tTemp.tagMesh));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);




	return S_OK;
}

HRESULT CCog::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

CCog * CCog::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CCog*	pInstance = new CCog(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCog::Free(void)
{
	Engine::CGameObject::Free();
}
