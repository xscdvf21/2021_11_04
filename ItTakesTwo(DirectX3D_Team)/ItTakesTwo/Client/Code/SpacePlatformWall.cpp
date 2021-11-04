#include "stdafx.h"
#include "SpacePlatformWall.h"
#include "Export_Function.h"


CSpacePlatformWall::CSpacePlatformWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpacePlatformWall::~CSpacePlatformWall()
{
}

HRESULT CSpacePlatformWall::Ready_Object(void * pArg)
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

_int CSpacePlatformWall::Update_Object(const _float & fTimeDelta)
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

_int CSpacePlatformWall::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpacePlatformWall::Render_Object(const _int & iIndex)
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

void CSpacePlatformWall::Reset_State()
{
	m_bMove = false;
	m_fTime = 0.f;
	m_pTransformCom->Set_Pos(m_vStartPos);

}

HRESULT CSpacePlatformWall::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Move_Wall"));
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


	m_vStartPos = tTemp.vPos;

	return S_OK;
}

HRESULT CSpacePlatformWall::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CSpacePlatformWall * CSpacePlatformWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpacePlatformWall*	pInstance = new CSpacePlatformWall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpacePlatformWall::Free(void)
{
	Engine::CGameObject::Free();
}

void CSpacePlatformWall::Move(const _float & fTimeDelta)
{
	if (!m_bMove)
		return;
	m_fTime+=fTimeDelta;
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vGap= m_vGoalPos - vPos;
	if (D3DXVec3Length(&vGap) < 1.f)
	{
		m_bMove = false;
		//m_pTransformCom->Set_Pos(m_vGoalPos);
		return;
	}
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vGap);
	m_pTransformCom->Move_Pos(&vDir, 3.f, fTimeDelta);

}
