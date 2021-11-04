#include "stdafx.h"
#include "Lever.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Fan.h"


CLever::CLever(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CLever::~CLever(void)
{
}

HRESULT CLever::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	// 0이면 왼쪽 1이면 오른쪽
	m_eDir = LEFT;

	m_vecFanList.reserve(Temp.eRenderType);

	_float3 vLook, vPos;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	m_pTransformCom->Set_AddAngleY(m_tObjInfo.vAngle.y);
	m_pBaseTransformCom->Set_AddAngleY(m_tObjInfo.vAngle.y);
	_matrix  matRotY;
	D3DXMatrixRotationY(&matRotY, m_tObjInfo.vAngle.y);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotY);

	vPos = m_pTransformCom->Get_Position();

	m_vPoint = vPos + vLook * 0.84f;
	m_vLookingAtDir = -vLook;

	return S_OK;
}

Engine::_int CLever::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vPos, m_tObjInfo.vScale.x, m_tObjInfo.vScale.y, m_tObjInfo.vScale.z, TRIGGERTYPE::TRIGGER_LEVER);
		m_bStart = false;
	}

	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CLever::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CLever::Render_Object(const _int& iIndex)
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

	// Base
	_matrix matWorld;
	m_pBaseTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	m_pBascMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

void CLever::Set_Dir(DIR eDir)
{
	if (m_eDir == LEFT)
		m_eDir = RIGHT;
	else
		m_eDir = LEFT;

	size_t iSize = m_vecFanList.size();
	for (size_t i = 0; i < m_vecFanList.size(); ++i)
	{
		CFan* pFan = static_cast<CFan*>(Engine::Get_GameObject(L"GameObject", m_vecFanList[i].c_str()));
		pFan->Set_Up(!pFan->Get_Up());
	}

	m_bMove = true;
}

void CLever::Move(const _float& TimeDelta)
{
	if (m_bMove)
	{
		switch (m_eDir)
		{
		case CLever::LEFT:
			m_pTransformCom->Set_AddAngleX(D3DXToRadian(m_fAngle) * 2.0f * TimeDelta);
			break;
		case CLever::RIGHT:
			m_pTransformCom->Set_AddAngleX(D3DXToRadian(m_fAngle) * -2.0f * TimeDelta);
			break;
		}

		if (m_pTransformCom->Get_Angle().x >= D3DXToRadian(60.0f))
		{
			m_pTransformCom->Set_AngleX(D3DXToRadian(60.0f));
			m_bMove = false;
		}
		else if (m_pTransformCom->Get_Angle().x <= -D3DXToRadian(60.0f))
		{
			m_pTransformCom->Set_AngleX(-D3DXToRadian(60.0f));
			m_bMove = false;
		}
	}
	/*CDynamicMesh* pTargetMesh = (CDynamicMesh*)Engine::Get_Component(L"GameObject", m_tagTargetName, L"Com_Mesh", ID_STATIC);
	CTransform* pTransform = (CTransform*)Engine::Get_Component(L"GameObject", m_tagTargetName, L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pTargetMesh, );

	const D3DXFRAME_DERIVED* pBone = pTargetMesh->Get_FrameByName("Align");
	NULL_CHECK_RETURN(pBone, );

	_float4x4 matBone = pBone->CombinedTranformationMatrix;
	const _float4x4* matPlayer = pTransform->Get_WorldMatrix();

	m_pTransformCom->Set_Scale(_float3(1.0f, 1.0f, 1.0f));
	m_pTransformCom->Set_AngleY(D3DXToRadian(90.0f));
	m_matCombinedMatrix = matBone * *matPlayer;

	_float4x4 matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	matWorld *= matBone * *matPlayer;
	m_pTrigger->Set_Pos(PxMat44(matWorld).getPosition());*/
}

void CLever::Add_FanList(wstring wstrFanName)
{
	m_vecFanList.emplace_back(wstrFanName);
}

CLever* CLever::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CLever*	pInstance = new CLever(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLever::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CLever::Add_Component(void* pArg)
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

	// BaseCStaticMesh
	pComponent = m_pBascMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"LeverBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_BaseMesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AngleX(D3DXToRadian(60.0f));
	m_pTransformCom->Set_AngleY(D3DXToRadian(90.0f));

	// BaseTransform
	pComponent = m_pBaseTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_BaseTransform", pComponent);
	m_pBaseTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pBaseTransformCom->Set_AngleY(D3DXToRadian(90.0f));
	_float3 vBasePos = tTemp.vPos;
	vBasePos.y -= 0.3f;
	m_pBaseTransformCom->Set_Pos(vBasePos);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CLever::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
