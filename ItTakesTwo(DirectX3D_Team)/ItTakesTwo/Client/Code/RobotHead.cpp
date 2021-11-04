#include "stdafx.h"
#include "RobotHead.h"
#include "MiniVacuumBody.h"
#include "Export_Function.h"


CRobotHead::CRobotHead(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRobotHead::~CRobotHead()
{

}

HRESULT CRobotHead::Ready_Object(_uint iBodyCount, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	if (nullptr != pArg)
	{

	}
	m_pMeshCom->Set_Animationset((4));

	return S_OK;
}

_int CRobotHead::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	//m_pMeshCom->Play_Animationset(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CRobotHead::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CRobotHead::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 4);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CRobotHead::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"RobotHead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Angle(tTemp.vAngle);
	m_pTransformCom->Set_Scale(tTemp.vScale);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}


HRESULT CRobotHead::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);
	pEffect->CommitChanges();

	return S_OK;
}


CRobotHead* CRobotHead::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg /*= nullptr*/)
{
	CRobotHead*	pInstance = new CRobotHead(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(0, pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRobotHead::Free(void)
{
	Engine::CGameObject::Free();
}