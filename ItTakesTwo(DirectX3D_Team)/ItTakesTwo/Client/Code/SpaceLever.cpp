#include "stdafx.h"
#include "SpaceLever.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Fan.h"


CSpaceLever::CSpaceLever(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSpaceLever::~CSpaceLever(void)
{
}

HRESULT CSpaceLever::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;
	m_vOrgAngle = m_tObjInfo.vAngle;
	
	switch (m_ePlayer)
	{
	case CSpaceLever::CODY:
		m_fLimitAngle = D3DXToRadian(95.0f);
		break;
	case CSpaceLever::MAY:
		m_fLimitAngle = D3DXToRadian(88.0f);
		break;
	}


	// 0이면 왼쪽 1이면 오른쪽
	m_eDir = LEFT;

	_float3 vLook, vPos;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	_matrix  matRotY;
	D3DXMatrixRotationY(&matRotY, m_tObjInfo.vAngle.y);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotY);

	vPos = m_pTransformCom->Get_Position();

	m_vLookingAtDir = -vLook;

	return S_OK;
}

Engine::_int CSpaceLever::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, m_eTriggerType);
		m_bStart = false;
	}

	

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CSpaceLever::LateUpdate_Object(const _float & fTimeDelta)
{
	Move(fTimeDelta);
	return _int();
}

void CSpaceLever::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	// Lever
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

void CSpaceLever::Reset_State()
{
	m_pTransformCom->Set_Pos(m_vStartPos);
	m_pTransformCom->Set_Angle(m_vStartAngle);
	m_bReSet = false;
	m_bWork = false;
}

void CSpaceLever::Set_Dir(DIR eDir)
{
	if (m_eDir == LEFT)
		m_eDir = RIGHT;
	else
		m_eDir = LEFT;

	m_bMove = true;
}

void CSpaceLever::Move(const _float& TimeDelta)
{

		//switch (m_eDir)
		//{
		//case CSpaceLever::LEFT:
		//	m_pTransformCom->Set_AddAngleX(D3DXToRadian(m_fAngle) * 2.0f * TimeDelta);
		//	break;
		//case CSpaceLever::RIGHT:
		//	m_pTransformCom->Set_AddAngleX(D3DXToRadian(m_fAngle) * -2.0f * TimeDelta);
		//	break;
		//}

	if (m_bWork)
	{
		m_pTransformCom->Set_AddAngleX(D3DXToRadian(-90.0f * (TimeDelta / m_fRotTime)));
		if (m_pTransformCom->Get_Angle().x <= m_vOrgAngle.x - m_fLimitAngle)
		{
			m_bWork = false;
			m_bComplete = true;
		}
	}
	else if (m_bReSet)
	{
		m_pTransformCom->Set_AddAngleX(D3DXToRadian(90.0f * (TimeDelta / m_fRotTime)));
		if (m_pTransformCom->Get_Angle().x >= m_vOrgAngle.x)
		{
			m_bReSet = false;
		}
	}

	m_pTransformCom->Update_Component(TimeDelta);
}

void CSpaceLever::Set_RotTime(_float fRotTime)
{
	m_fRotTime = fRotTime;
	m_bWork = true;
}

void * CSpaceLever::On_Active(void * pCaller)
{
	Engine::Get_GameObject(L"GameObject", L"SpacePlatform")->On_Active(pCaller);

	return nullptr;
}



void* CSpaceLever::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	Engine::Get_GameObject(L"GameObject", L"SpacePlatform")->On_Trigger_Active(pTrigger,pCaller);
	return nullptr;
}

CSpaceLever* CSpaceLever::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vNormal, _vec3 vTargetPos, TRIGGERTYPE eTriggerType, void* pArg, PlayerType eType)
{
	CSpaceLever*	pInstance = new CSpaceLever(pGraphicDev);

	pInstance->m_ePlayer = eType;
	pInstance->m_eTriggerType = eTriggerType;
	pInstance->m_vNormal = vNormal;
	pInstance->m_vPoint = vTargetPos;
	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpaceLever::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSpaceLever::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Lever"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Angle(tTemp.vAngle);
	m_vStartAngle = tTemp.vAngle;
	m_vStartPos = tTemp.vPos;

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	

	return S_OK;
}

HRESULT CSpaceLever::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
