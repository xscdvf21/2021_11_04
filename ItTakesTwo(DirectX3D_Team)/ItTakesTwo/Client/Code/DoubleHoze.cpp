#include "stdafx.h"
#include "DoubleHoze.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Logo.h"
#include <fstream>


CDoubleHoze::CDoubleHoze(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CDoubleHoze::~CDoubleHoze(void)
{
}

bool CDoubleHoze::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	D3DXMatrixScaling(&matWorld, 0.01f, 0.01f, 0.01f);
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

HRESULT CDoubleHoze::Ready_Object(void* pArg)
{

	return S_OK;
}

HRESULT CDoubleHoze::Ready_Object(const _bool& bIstwoWay, void* pArg /*= nullptr*/)
{
	FAILED_CHECK_RETURN(Add_Component(bIstwoWay, pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CDoubleHoze::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		if (m_pVacuum[0])
		{
			m_pVacuum[0]->Set_Suck(true);
			m_pVacuum[0]->Set_Scene(m_pScene);
			auto vecBody = m_pVacuum[0]->Get_HozeBody();
			m_pScene->addAggregate(*m_pVacuum[0]->Get_Hoze());
			for (auto pBody : vecBody)
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);
		}
		if (m_pVacuum[1])
		{
			m_pVacuum[1]->Set_Scene(m_pScene);

			m_pVacuum[1]->Set_Suck(false);
			auto vecBody = m_pVacuum[1]->Get_HozeBody();

			m_pScene->addAggregate(*m_pVacuum[1]->Get_Hoze());
			for (auto pBody : vecBody)
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);
		}

		m_bStart = false;
	}
	CGameObject::Update_Object(fTimeDelta);
	for (_int i = 0; i < 2; ++i)
	{
		if (m_pVacuum[i])
			m_pVacuum[i]->Update_Object(fTimeDelta);
	}

	return OBJ_NOEVENT;
}

_int CDoubleHoze::LateUpdate_Object(const _float & fTimeDelta)
{
	for (_int i = 0; i < 2; ++i)
	{
		if (m_pVacuum[i])
			m_pVacuum[i]->LateUpdate_Object(fTimeDelta);
	}
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return _int();
}

void CDoubleHoze::Render_Object(const _int& iIndex)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;
	_matrix			matWorld, matView, matProj;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 6);

	pEffect->End();

	Safe_Release(pEffect);
}

void* CDoubleHoze::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	m_vecTotalPos.clear();
	if (m_bFlip)
	{
		m_vecTotalPos.emplace_back(m_pVacuum[1]->Get_IOPath());
		m_vecTotalPos.emplace_back(&m_vecPos);
		m_vecTotalPos.emplace_back(m_pVacuum[0]->Get_IOPath());
	}
	else
	{
		m_vecTotalPos.emplace_back(m_pVacuum[0]->Get_IOPath());
		m_vecTotalPos.emplace_back(&m_vecRvPos);
		m_vecTotalPos.emplace_back(m_pVacuum[1]->Get_IOPath());
	}


	return &m_vecTotalPos;
}

void CDoubleHoze::Load_Path(const _wstr& tagPath)
{
	wifstream rd;
	rd.open(tagPath.c_str());
	_wstr Path;
	TCHAR* szBuf;
	_wstr wPos;
	TCHAR* buffer;
	TCHAR* Pos;
	double dwPos;

	while (!rd.eof())
	{
		getline(rd, Path);

		szBuf = new TCHAR[Path.length() + 1];
		lstrcpy(szBuf, Path.c_str());
		Pos = wcstok_s(szBuf, L",", &buffer);
		while (Pos)
		{
			wPos = Pos;
			wPos.erase(remove(wPos.begin(),
				wPos.end(), '['), wPos.end());
			wPos.erase(remove(wPos.begin(),
				wPos.end(), ']'), wPos.end());

			dwPos = _ttof(wPos.c_str());
			m_vecPoint.emplace_back((_float)dwPos*0.01f);

			Pos = wcstok_s(nullptr, L",", &buffer);
		}

		Safe_Delete_Array(szBuf);
	}
	rd.close();

	for (size_t i = 0; i < m_vecPoint.size(); i += 3)
	{
		m_vecPos.emplace_back(_vec3{ (_float)m_vecPoint[i],(_float)m_vecPoint[i + 2],(_float)m_vecPoint[i + 1] });
	}
	for (size_t i = m_vecPoint.size()-3; i >0 ; i -= 3)
	{
		m_vecRvPos.emplace_back(_vec3{ (_float)m_vecPoint[i],(_float)m_vecPoint[i + 2],(_float)m_vecPoint[i + 1] });
	}
	m_vecPoint.clear();






}

void* CDoubleHoze::On_Active(void* pCaller)
{
	m_bFlip = !m_bFlip;
	if (m_pVacuum[0])
		m_pVacuum[0]->Set_Suck(!m_bFlip);
	if (m_pVacuum[1])
		m_pVacuum[1]->Set_Suck(m_bFlip);
	Engine::StopNPlaySound(L"Play_Gameplay_Vehicle_VacuumHubSmall_ChangeDirection.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Start, 0.5f);
	Engine::StopNPlaySound(L"VF2_ReverseElectric.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);
	return nullptr;
}

HRESULT CDoubleHoze::Add_Component(const _bool& bIsTwoWay /*= nullptr*/, void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;

	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = tTemp;
	m_bIsTwoWay = bIsTwoWay;
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"StaticHoze2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform1", pComponent);
	m_pTransformCom->Set_Scale(0.01f);
	m_pTransformCom->Set_Pos(2.f, 0.f, 0.f);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(0.f, 0.f, 0.f);
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(0.01f, 0.01f, 0.01f));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName((char*)this);



	return S_OK;
}


void CDoubleHoze::Create_DynamicPart()
{
	m_tObjInfo.vPos = m_vecPos.back();
	m_tObjInfo.vPos.x += 0.3f;

	// LEFT
	m_pVacuum[0] = CVacuumHeadHoze::Create(m_pGraphicDev, 3,2, 30.f,2.f, &m_tObjInfo,this);
	m_pVacuum[0]->Set_UpForce(1.7f);
	m_tObjInfo.vPos = m_vecPos.front();
	m_tObjInfo.vPos.z += 1.5f;
	// RIGHT
	m_pVacuum[1] = CVacuumHeadHoze::Create(m_pGraphicDev, 0,2, 33.f, 2.f, &m_tObjInfo, this);
	m_pVacuum[1]->Set_UpForce(1.5f);

}

CDoubleHoze* CDoubleHoze::Create(LPDIRECT3DDEVICE9 pGraphicDev, const vector<_wstr>& tagPath, const _bool& bIstwoWay, void* pArg /*= nullptr*/)
{
	CDoubleHoze*	pInstance = new CDoubleHoze(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(bIstwoWay, pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	for (size_t i = 0; i < tagPath.size(); ++i)
	{
		pInstance->Load_Path(tagPath[i]);
	}
	pInstance->Create_DynamicPart();

	return pInstance;
}

void CDoubleHoze::Free(void)
{
	Engine::CGameObject::Free();
	for (_int i = 0; i < 2; ++i)
	{
		if (m_pVacuum[i])
			Safe_Release(m_pVacuum[i]);
	}
}