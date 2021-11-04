#include "stdafx.h"
#include "MiniVacuumHead.h"


#include "MiniVacuumHub.h"
#include "Export_Function.h"



CMiniVacuumHead::CMiniVacuumHead(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMiniVacuumHead::~CMiniVacuumHead()
{
}

HRESULT CMiniVacuumHead::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;

	
	if (nullptr != pArg)
	{

		memcpy(&tTemp, pArg, sizeof(OBJINFO));
		
	}
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Angle(tTemp.vAngle);
	m_pTransformCom->Set_Pos(tTemp.vPos);

	//m_pTransformCom->Set_AddPos(_vec3(0.f, 7.f, 0.f));


	m_pMeshCom->Set_Animationset((0));

	return S_OK;
}

_int CMiniVacuumHead::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);



	Get_EventPos();

	m_pMeshCom->Play_Animationset(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CMiniVacuumHead::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CMiniVacuumHead::Render_Object(const _int & iIndex)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CMiniVacuumHead::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CMiniVacuumHead::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	pEffect->CommitChanges();

	return S_OK;
}

CMiniVacuumHead * CMiniVacuumHead::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CMiniVacuumHead*	pInstance = new CMiniVacuumHead(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);


	return pInstance;
}

void CMiniVacuumHead::Free(void)
{
	Engine::CGameObject::Free();
}
