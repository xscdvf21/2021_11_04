#include "stdafx.h"

#include "VacuumFieldPart.h"
#include "VacuumFieldPart2.h"
#include "VacuumFieldPart3.h"
#include "VacuumFieldPart4.h"
#include "VacuumFieldPart5.h"

#include "Vacuum.h"


#include "Export_Function.h"


CVacuumFieldPart4::CVacuumFieldPart4(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_iIndex(0)
{
}

CVacuumFieldPart4::~CVacuumFieldPart4()
{
}

HRESULT CVacuumFieldPart4::Ready_Object(_bool bOpen, _uint iIndex, void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	

	OBJINFO tTemp;
	if (nullptr != pArg)
	{
	
		memcpy(&tTemp, pArg, sizeof(OBJECTINFO));

	}
	m_bOpen = bOpen;
	m_iIndex = iIndex;
	
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_vStartPos = m_pTransformCom->Get_Position();
	m_vStartPos.y = 0;
	return S_OK;
}

_int CVacuumFieldPart4::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);


	if (true == m_bMoveOpen)
	{
		MoveOpen(fTimeDelta);
	}
	if (true == m_bMoveClose)
	{
		MoveClose(fTimeDelta);
	}

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuumFieldPart4::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	PxQuat cQuat = PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f });
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget({ vPos.x,vPos.y,vPos.z ,cQuat });
	return _int();
}

void CVacuumFieldPart4::Render_Object(const _int & iIndex)
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

CVacuumFieldPart4 * CVacuumFieldPart4::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bOpen, _uint iIndex, void* pArg)
{
	CVacuumFieldPart4*	pInstance = new CVacuumFieldPart4(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(bOpen, iIndex, pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumFieldPart4::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CVacuumFieldPart4::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"VacuumPlatform_Part04"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhys = Engine::Get_Physics();
	PxQuat cQuat = PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f });
	PxRigidDynamic* pBody = pPhys->createRigidDynamic(PxTransform(tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z, cQuat));
	m_pRigidActor = pBody;
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxTriangleMeshGeometry tGeo{};
	tGeo.triangleMesh = m_pMeshCom->Get_PxMesh();
	tGeo.scale = PxMeshScale(0.01f);

	PxShape* pShape = pPhys->createShape(tGeo, *pPhys->createMaterial(0.5f, 0.5f, 0.f), true);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidBody = pShape;
	m_pRigidActor->attachShape(*pShape);
	return S_OK;
}

HRESULT CVacuumFieldPart4::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CVacuumFieldPart4::MoveClose(const _float & fTimeDelta)
{
	//All_PartDown();
	//CVacuumFieldPart5*	pObj = (CVacuumFieldPart5*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldHead_1");

	//CTransform*			pHeadTrans = (CTransform*)Engine::Get_Component(L"GameObject", L"VacuumFieldHead_1", L"Com_Transform", COMPONENTID::ID_DYNAMIC);

	//_vec3 vHeadPos;
	//vHeadPos = pHeadTrans->Get_Position();


	_vec3 vRight;
	_vec3 vPos;


	m_pTransformCom->Get_Info(INFO_LOOK, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	if (D3DXVec3Length(&(m_pTransformCom->Get_Position() - m_vEndPos)) > 1.3f)
	{
		//여기서 셋포스 건드려줘야할듯. 다끝난 상황.
		if (!m_bOpen)
		{

			if (m_iIndex == 1)
			{
				_vec3 vSetPos;
				vSetPos = { -13.8f, 0.05f, -0.65f };
				m_pTransformCom->Set_Pos(vSetPos);
			}

			if (m_iIndex == 3)
			{
				_vec3 vSetPos;
				vSetPos = { -23.8f, 0.05f, -0.65f };
				m_pTransformCom->Set_Pos(vSetPos);
			}


		}
		else if (m_bOpen)
		{
			if (m_iIndex == 2)
			{
				_vec3 vSetPos;
				vSetPos = { -13.8f, 0.05f, 0.65f };
				m_pTransformCom->Set_Pos(vSetPos);
			}
			if (m_iIndex == 4)
			{
				_vec3 vSetPos;
				vSetPos = { -23.8f, 0.05f, 0.65f };
				m_pTransformCom->Set_Pos(vSetPos);
			}
		}


		m_bMoveClose = false;
	}

	//if (vHeadPos.y < -1.f)
	//{
		if (!m_bOpen)
		{

			m_pTransformCom->Move_Pos(&vRight, -2.f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_Pos(&vRight, 2.f, fTimeDelta);
		}
	//}

}

void CVacuumFieldPart4::MoveUpEndClose(const _float & fTimeDelta)
{

	//_vec3 vRight;
	//_vec3 vPos;


	//m_pTransformCom->Get_Info(INFO_LOOK, &vRight);
	//D3DXVec3Normalize(&vRight, &vRight);


	//if (D3DXVec3Length(&(m_pTransformCom->Get_Position() - m_vEndPos)) > 1.3f)
	//{
	//	return;
	//}

	//if (!m_bOpen)
	//{

	//	m_pTransformCom->Move_Pos(&vRight, -2.f, fTimeDelta);
	//}
	//else
	//{
	//	m_pTransformCom->Move_Pos(&vRight, 2.f, fTimeDelta);
	//}
}

void CVacuumFieldPart4::All_PartUp()
{
	CVacuumFieldPart*	pPart1 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_1");
	CVacuumFieldPart*	pPart2 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_2");
	CVacuumFieldPart2*	pPart2_1 = (CVacuumFieldPart2*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart2_1");
	CVacuumFieldPart3*	pPart3_1 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_1");
	CVacuumFieldPart3*	pPart3_2 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_2");
	CVacuumFieldPart5*	pHeadPart = (CVacuumFieldPart5*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldHead_1");

	pPart1->Set_MoveUpCheck(true);
	pPart2->Set_MoveUpCheck(true);
	pPart2_1->Set_MoveUpCheck(true);
	pPart3_1->Set_MoveUpCheck(true);
	pPart3_2->Set_MoveUpCheck(true);
	pHeadPart->Set_MoveUpCheck(true);
}

void CVacuumFieldPart4::All_PartDown()
{
	CVacuumFieldPart*	pPart1 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_1");
	CVacuumFieldPart*	pPart2 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_2");
	CVacuumFieldPart2*	pPart2_1 = (CVacuumFieldPart2*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart2_1");
	CVacuumFieldPart3*	pPart3_1 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_1");
	CVacuumFieldPart3*	pPart3_2 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_2");
	CVacuumFieldPart5*	pHeadPart = (CVacuumFieldPart5*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldHead_1");

	pPart1->Set_MoveDownCheck(true);
	pPart2->Set_MoveDownCheck(true);
	pPart2_1->Set_MoveDownCheck(true);
	pPart3_1->Set_MoveDownCheck(true);
	pPart3_2->Set_MoveDownCheck(true);
	pHeadPart->Set_MoveDownCheck(true);
}

void CVacuumFieldPart4::MoveOpen(const _float & fTimeDelta)
{
	
	//CVacuum* pVacuum = (CVacuum*)Engine::Get_GameObject(L"GameObject", L"VacuumBoss");

	//m_bOpenMove = pVacuum->Get_Bomb();

	//if (m_bOpenMove)
	//{

	_vec3 vRight;
	_vec3 vPos;


	m_pTransformCom->Get_Info(INFO_LOOK, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	//다열림.
	if (D3DXVec3Length(&(m_pTransformCom->Get_Position() - m_vStartPos)) > 1.3f)
	{
		
		m_vEndPos = m_pTransformCom->Get_Position();
		//All_PartUp();
		m_bMoveOpen = false;
			
	}

	if (false == m_bOpen)
	{
		m_pTransformCom->Move_Pos(&vRight, 2.f, fTimeDelta);
	}
	else
	{
		m_pTransformCom->Move_Pos(&vRight, -2.f, fTimeDelta);
	}

}
