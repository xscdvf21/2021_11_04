#include "stdafx.h"
#include "Nozzle.h"
#include "MiniVacuumBody.h"
#include "Export_Function.h"




CNozzle::CNozzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CNozzle::~CNozzle()
{

}

HRESULT CNozzle::Ready_Object(_uint iBodyCount, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	if (nullptr != pArg)
	{

	}





	return S_OK;
}

_int CNozzle::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);


	if (m_pTransformCom->Get_Position().y < m_vStartPos.y + 8 && m_bStart)
	{
		m_pTargetCamera->Set_Shake();
		m_pTransformCom->Set_AddPos({ 0.f,0.1f,0.f });
	}
	else
	{
		m_bStart = false;
		m_pTargetCamera->Set_Mode(CAMERAMODE::CAMERA_DEFAULT);
	}

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CNozzle::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CNozzle::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CNozzle::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Nozzle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Angle(tTemp.vAngle);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_vStartPos = tTemp.vPos;
	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}


HRESULT CNozzle::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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


CNozzle* CNozzle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CCamera* pCamera, void* pArg /*= nullptr*/)
{
	CNozzle*	pInstance = new CNozzle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(0, pArg)))
	{
		Safe_Release(pInstance);
	}
	pInstance->Set_TargetCamera(pCamera);
	return pInstance;
}

void CNozzle::Free(void)
{
	Engine::CGameObject::Free();
}
