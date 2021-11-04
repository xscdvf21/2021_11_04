#include "stdafx.h"
#include "Vacuum.h"
#include "VacuumBomb.h"
#include "VacuumCircle.h"
#include "VacuumMine.h"
#include "VacuumStone.h"
#include "VacuumFieldPart.h"
#include "VacuumFieldPart2.h"
#include "VacuumFieldPart3.h"
#include "VacuumFieldPart4.h"
#include "VacuumFieldPart5.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "BoundingBox.h"
#include "Cody.h"
#include "May.h"
#include "MayCamera.h"
#include "CodyCamera.h"


//생성 할 뼈이름 Neck (돌멩이)
//LeftHandBottomLips1 //서클
//RightHandBottomLips1 // 서클
//RightHandJaw, LeftHandJaw (마인) 가로 16 세로 6(한팔당 3줄) z축 왼쪽 24 오른쪽 -24 총 48 x축 +25 ~ -12 그런데 아마 x축은 +12 - 12 정도쓰는듯. 
CVacuum::CVacuum(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_fMineX(0.f), m_fMineX2(9.f), m_fMineX3(18.f)
	, m_fMineX4(0.f), m_fMineX5(-9.f), m_fMineX6(-18.f)
	, m_fMineZ(-24.f)
	, m_fMineZ3(-21.f)
	, m_iStoneCount(0)
	, m_bStun(false), m_bStunFirst(false), m_bCreateTrigger(false)
	, m_bCodyRide(false), m_bMayRide(false)
	, m_bAniStop(false), m_fDelta(0.f)
{
}

CVacuum::~CVacuum()
{
}

HRESULT CVacuum::Ready_Object(void * pArg)
{
	m_iHP = 10;
	m_iHPMax = 10;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	if (nullptr != pArg)
	{

	}

	m_vNextPos = { m_fMineX, 0.f, m_fMineZ };
	m_vNextPos2 = { m_fMineX2, 0.f, m_fMineZ };
	m_vNextPos3 = { m_fMineX3, 0.f, m_fMineZ3 };
	m_vNextPos4 = { m_fMineX4, 0.f, m_fMineZ };
	m_vNextPos5 = { m_fMineX5, 0.f, m_fMineZ };
	m_vNextPos6 = { m_fMineX6, 0.f, m_fMineZ3 };



	m_pMeshCom->Set_Animationset(0);
	Create_Bomb(pArg);

	return S_OK;
}

_int CVacuum::Update_Object(const _float & fTimeDelta)
{
	if (!m_bTrigger)
	{
		_vec3 vPos = m_pTransformCom->Get_Position();
		_vec3 vTriggerPos = { vPos.x, vPos.y + 10.f, vPos.z };

		m_pTrigger[TriggerID::Vacuum] = m_pTrigger[TriggerID::Vacuum] = CTrigger::CreateSphereTirgger(m_pScene, this, m_pTransformCom->Get_Position(), 7.f, TRIGGER_VACUUMBOSS_VACUUM, FilterGroup::eDefaultTrigger);
		m_pTrigger[TriggerID::Vacuum]->Set_Pos(vTriggerPos);

		// 코디 트리거
		m_pTrigger[TriggerID::CodyRide] = CTrigger::CreateSphereTirgger(m_pScene, this, _vec3(15.705f, 0.0f, 4.727f), 3.f, TRIGGER_VACUUM_BOSS_CODY);
		m_pTrigger[TriggerID::CodyRide]->Set_Interactable(false);

		m_pTrigger[TriggerID::MayRide] = CTrigger::CreateSphereTirgger(m_pScene, this, _vec3(16.261f, 0.0f, -3.83614f), 3.f, TRIGGER_VACUUM_BOSS_MAY);
		m_pTrigger[TriggerID::MayRide]->Set_Interactable(false);

		m_bTrigger = true;
	}

	for (auto& iter = m_vecActiveBomb.begin(); iter != m_vecActiveBomb.end();)
	{
		if (OBJ_DEAD == (*iter)->Update_Object(fTimeDelta))
		{
			m_vecBomb.emplace_back(*iter);
			iter = m_vecActiveBomb.erase(iter);
			continue;
		}
		++iter;
	}

	m_fDelta = fTimeDelta;
	if (m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_75 ||
		m_iAniIndex == animID::VacuumBoss_End_EyePoppedOut_Enter ||
		m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_Preview)
	{
		m_fDelta = m_fDelta / 2;
	}
	if (m_iHP <= 0)
	{
		m_bStun = true;
	}

	if (m_bDead)
		return OBJ_DEAD;

	if (Engine::Key_Down(DIK_O))
	{
		m_iHP = 0;
		/*	m_bPartUp = true;*/

	}
	if (Engine::Key_Down(DIK_P))
	{
		m_bPartDown = true;
	}


	if (m_bDead)
		return OBJ_DEAD;

	if (m_bPartUp)
		PartUp(fTimeDelta);

	if (m_bPartDown)
		PartDown(fTimeDelta);

	Effect_Dead(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);


	if (!m_bStun)
		Move(fTimeDelta);
	else if (m_bStun)
		Stun(fTimeDelta);


	if (!m_bAniStop)
		m_pMeshCom->Play_Animationset(m_fDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuum::LateUpdate_Object(const _float & fTimeDelta)
{
	for (auto& iter = m_vecActiveBomb.begin(); iter != m_vecActiveBomb.end();)
	{
		if (OBJ_DEAD == (*iter)->LateUpdate_Object(fTimeDelta))
		{
			m_vecBomb.emplace_back(*iter);
			iter = m_vecActiveBomb.erase(iter);
			continue;
		}
		++iter;
	}

	if (m_bStart)
	{
		Create_UI(L"VB_UI");
		m_bStart = false;
		return 0;
	}

	Update_UI(fTimeDelta);

	return _int();
}

void CVacuum::Render_Object(const _int & iIndex)
{
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

void* CVacuum::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	Set_Damage(-1);
	return nullptr;
}

HRESULT CVacuum::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_VB, L"VacuumBoss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-90.f));
	m_pTransformCom->Set_Pos(_vec3(30.f, 0.f, 0.f));

	return S_OK;
}

HRESULT CVacuum::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CVacuum::Move(const _float & fTimeDelta)
{
	m_iAniIndex = m_pMeshCom->Get_AniIndex();

	Engine::CTransform*		pMayTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));
	if (!pMayTransformCom || !pCodyTransformCom)
		return;
	_vec3 vMayPos; //메이 위치
	_vec3 vCodyPos; //코디 위치
	_vec3 vPos; //현재 몬스터(자신)	위치.
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	vMayPos = pMayTransformCom->Get_Position();
	vCodyPos = pCodyTransformCom->Get_Position();
	vPos = m_pTransformCom->Get_Position();

	_vec3 vMayDir;	//메이와의 거리
	_vec3 vCodyDir; //코디와의 거리

	_float fMayDir;
	_float fCodyDir;

	vMayDir = vMayPos - vPos;
	vCodyDir = vCodyPos - vPos;

	fMayDir = D3DXVec3Length(&vMayDir);
	fCodyDir = D3DXVec3Length(&vCodyDir);


	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	if (fMayDir < fCodyDir)
	{
		m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
	}
	else if (fCodyDir <= fMayDir)
	{
		m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
	}
	if (!m_pTargetPlayer)
		return;
	if (!m_bPattrunSelect)
	{
		m_pMeshCom->Set_Animationset(8);
		m_fPattrunDelta += fTimeDelta;
	}

	if (m_bPartPatturn && m_fPattrunDelta > 20.f)
		//if (m_bPartPatturn && m_fPattrunDelta > 13.f)
	{
		if (!m_bDownOne)
		{
			m_bPartDown = true;
			m_bDownOne = true;
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Force_Ride_Off();
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Force_Ride_Off();
		}
	}
	if (m_bPartPatturn && m_fPattrunDelta > 27.f)
	{
		m_bBombCheck = false;
		if (m_iPatturnSelect != 5)
			m_iPatturnSelect++;
		m_fPattrunDelta = 0.f;
		m_bPartPatturn = false;
		m_bDownOne = false;
		m_bPattrunSelect = true;
	}

	if (!m_bPartPatturn && m_fPattrunDelta > 3.f)
	{
		m_bBombCheck = false;
		if (m_iPatturnSelect != 5)
			m_iPatturnSelect++;
		m_fPattrunDelta = 0.f;
		m_bPattrunSelect = true;
	}


	//if (!m_bPattrunSelect)
	//{
	//	m_bBombCheck = false;
	//	m_iPatturnSelect = rand() % 5 + 1;
	//	m_bPattrunSelect = true;
	//}

	if (m_bPattrunSelect)
	{
		//돌멩이
		if (m_iPatturnSelect == 1)
		{
			m_fStoneTime += fTimeDelta;

			m_bSound[Sound_Stone] = false;

			if (m_fStoneTime > 1.f)
			{
				if (!m_bSound[Sound_Stone])
				{
					Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Stone);
					Engine::PlaySoundW(L"Vacuum_StoneReady.wav", CSoundMgr::CHANNELID::Vacuum_Stone, 0.2f);

					m_bSound[Sound_Stone] = true;
				}


				CTransform*		pTargetTransCom = static_cast<CTransform*>(m_pTargetPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

				_vec3 vTargetPos;
				vTargetPos = pTargetTransCom->Get_Position();
				m_pMeshCom->Set_Animationset(11);

				CLayer*				pLayer = nullptr;
				pLayer = Engine::Get_Layer(L"GameObject");
				Engine::CGameObject*		pGameObject = nullptr;

				const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("Neck");

				_matrix matBoneWorld, matWorld;
				matBoneWorld = pFrame->CombinedTranformationMatrix;
				matWorld = *(m_pTransformCom->Get_WorldMatrix());

				matWorld = matBoneWorld * matWorld;

				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"VacuumStone %d", m_iStoneCount);



				_float fX;
				_float fZ;
				_float fAngle;
				_float fScale;
				_float fRotX;
				_float fRotY;
				_float fRotZ;

				CRandoms CRandom;
				fX = CRandom(-5.f, 5.f);
				fZ = CRandom(-5.f, 5.f);
				fScale = CRandom(0.004f, 0.007f);
				fAngle = CRandom(0.f, 360.f);
				fRotX = CRandom(-1.f, 1.f);
				fRotY = CRandom(-1.f, 1.f);
				fRotZ = CRandom(-1.f, 1.f);

				OBJINFO tTemp;
				tTemp.vPos = { matWorld._41, matWorld._42, matWorld._43 };
				tTemp.vScale = { fScale, fScale, fScale };
				tTemp.vAngle = { fAngle,fAngle,fAngle };

				vTargetPos.x = vTargetPos.x + fX;
				vTargetPos.z = vTargetPos.z + fZ;

				pGameObject = CVacuumStone::Create(m_pGraphicDev, &tTemp, vTargetPos);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );

				m_iStoneCount++;

				wsprintf(tagTemp, L"VacuumStone %d", m_iStoneCount);

				vTargetPos = pTargetTransCom->Get_Position();


				fX = CRandom(-10.f, 10.f);
				fZ = CRandom(-10.f, 10.f);
				fScale = CRandom(0.004f, 0.007f);
				fAngle = CRandom(0.f, 360.f);
				fRotX = CRandom(-1.f, 1.f);
				fRotY = CRandom(-1.f, 1.f);
				fRotZ = CRandom(-1.f, 1.f);


				tTemp.vPos = { matWorld._41, matWorld._42, matWorld._43 };
				tTemp.vScale = { fScale, fScale, fScale };
				tTemp.vAngle = { fAngle,fAngle,fAngle };

				vTargetPos.x = vTargetPos.x + fX;
				vTargetPos.z = vTargetPos.z + fZ;

				pGameObject = CVacuumStone::Create(m_pGraphicDev, &tTemp, vTargetPos);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );

				m_iStoneCount++;


				m_fStoneTime = 0.f;
			}

			if (m_iStoneCount >= 20)
			{
				m_iStoneCount = 0; m_bPattrunSelect = false;

				return;
			}
		}

		//마인 패턴
		if (m_iPatturnSelect == 2)
		{
			m_fMineTime += fTimeDelta;
			//마인 완성
			if (!m_bRandomNum)
			{
				for (i = 0; i < 6; ++i)
				{
					data[i] = rand() % 6 + 1;
					for (sub_i = 0; sub_i < i; ++sub_i)
					{
						if (data[i] == data[sub_i])
						{
							i--;
							break;
						}
					}
				}

				m_bRandomNum = true;
			}
			if (!m_bMineSelect)
			{
				m_iMineSelect = data[m_iMinePatturnCount];
				m_iMinePatturnCount++;
				m_bMineSelect = true;
			}

			if (m_iMinePatturnCount > 6)
			{
				m_fMineTime = 0.f;
				m_bMineSelect = false;
				m_pMeshCom->Set_Animationset(11);
				m_iMinePatturnCount = 0;
				m_bRandomNum = false;
				m_bPattrunSelect = false;
				return;
			}
			m_fMineCreateTime += fTimeDelta;
			m_pMeshCom->Set_Animationset(25);

			CLayer*				pLayer = nullptr;
			pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;


			if (m_iMineSelect == 1)
			{
				//첫시작 왼팔
				if (m_fMineCreateTime < 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX += m_fMineDis;
					if (m_fMineX > 6.f)
					{
						m_fMineX = 0.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos = { m_fMineX , 0.f, m_fMineZ };

					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos = { m_fMineX  , 0.f, m_fMineZ };
					}
				}
				else if (m_fMineCreateTime > 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX += m_fMineDis;
					if (m_fMineX > 6.f)
					{
						m_fMineX = 0.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos = { m_fMineX, 0.f, m_fMineZ };
					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos = { m_fMineX , 0.f, m_fMineZ };
						//m_fMineTime = 0.f;
						m_bMineSelect = false;
						m_fMineCreateTime = 0.f;
						m_pMeshCom->Set_Animationset(11);
					}
				}
			}

			if (m_iMineSelect == 2)
			{
				//첫시작 왼팔
				if (m_fMineCreateTime < 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos2); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX2 += m_fMineDis;
					if (m_fMineX2 > 15.f)
					{
						m_fMineX2 = 9.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos2 = { m_fMineX2 , 0.f, m_fMineZ };

					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos2 = { m_fMineX2  , 0.f, m_fMineZ };
					}
				}
				else if (m_fMineCreateTime > 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos2); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX2 += m_fMineDis;
					if (m_fMineX2 > 15.f)
					{
						m_fMineX2 = 9.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos2 = { m_fMineX2, 0.f, m_fMineZ };
					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos2 = { m_fMineX2 , 0.f, m_fMineZ };
						//m_fMineTime = 0.f;
						m_bMineSelect = false;
						m_fMineCreateTime = 0.f;
						m_pMeshCom->Set_Animationset(11);
					}
				}
			}

			if (m_iMineSelect == 3)
			{
				//첫시작 왼팔
				if (m_fMineCreateTime < 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos3); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX3 += m_fMineDis;
					if (m_fMineX3 > 24.f)
					{
						m_fMineX3 = 18.f;
						m_fMineZ3 += m_fMineDis;
					}

					m_vNextPos3 = { m_fMineX3 , 0.f, m_fMineZ3 };

					if (m_fMineZ3 > 21.f) //초기화 단계
					{
						m_fMineZ3 = -21.f;
						m_vNextPos3 = { m_fMineX3  , 0.f, m_fMineZ3 };
					}
				}
				else if (m_fMineCreateTime > 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos3); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX3 += m_fMineDis;
					if (m_fMineX3 > 24.f)
					{
						m_fMineX3 = 18.f;
						m_fMineZ3 += m_fMineDis;
					}

					m_vNextPos3 = { m_fMineX3, 0.f, m_fMineZ3 };
					if (m_fMineZ3 > 21.f) //초기화 단계
					{
						m_fMineZ3 = -21.f;
						m_vNextPos3 = { m_fMineX3 , 0.f, m_fMineZ3 };
						//m_fMineTime = 0.f;
						m_bMineSelect = false;
						m_fMineCreateTime = 0.f;
						m_pMeshCom->Set_Animationset(11);
					}
				}
			}

			if (m_iMineSelect == 4)
			{
				//첫시작 왼팔
				if (m_fMineCreateTime < 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos4); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX4 += -m_fMineDis;
					if (m_fMineX4 < -6.f)
					{
						m_fMineX4 = 0.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos4 = { m_fMineX4 , 0.f, m_fMineZ };

					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos4 = { m_fMineX4  , 0.f, m_fMineZ };
					}
				}
				else if (m_fMineCreateTime > 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos4); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX4 += -m_fMineDis;
					if (m_fMineX4 < -6.f)
					{
						m_fMineX4 = 0.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos4 = { m_fMineX4, 0.f, m_fMineZ };
					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos4 = { m_fMineX4 , 0.f, m_fMineZ };
						//m_fMineTime = 0.f;
						m_bMineSelect = false;
						m_fMineCreateTime = 0.f;
						m_pMeshCom->Set_Animationset(11);
					}
				}
			}

			if (m_iMineSelect == 5)
			{
				//첫시작 왼팔
				if (m_fMineCreateTime < 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos5); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX5 += -m_fMineDis;
					if (m_fMineX5 < -15.f)
					{
						m_fMineX5 = -9.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos5 = { m_fMineX5 , 0.f, m_fMineZ };

					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos5 = { m_fMineX5  , 0.f, m_fMineZ };
					}
				}
				else if (m_fMineCreateTime > 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos5); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX5 += -m_fMineDis;
					if (m_fMineX5 < -15.f)
					{
						m_fMineX5 = -9.f;
						m_fMineZ += m_fMineDis;
					}

					m_vNextPos5 = { m_fMineX5, 0.f, m_fMineZ };
					if (m_fMineZ > 24.f) //초기화 단계
					{
						m_fMineZ = -24.f;
						m_vNextPos5 = { m_fMineX5 , 0.f, m_fMineZ };
						//m_fMineTime = 0.f;
						m_bMineSelect = false;
						m_fMineCreateTime = 0.f;
						m_pMeshCom->Set_Animationset(11);
					}
				}
			}

			if (m_iMineSelect == 6)
			{
				//첫시작 왼팔
				if (m_fMineCreateTime < 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos6); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX6 += -m_fMineDis;
					if (m_fMineX6 < -24.f)
					{
						m_fMineX6 = -18.f;
						m_fMineZ3 += m_fMineDis;
					}

					m_vNextPos6 = { m_fMineX6 , 0.f, m_fMineZ3 };

					if (m_fMineZ3 > 21.f) //초기화 단계
					{
						m_fMineZ3 = -21.f;
						m_vNextPos6 = { m_fMineX6  , 0.f, m_fMineZ3 };
					}
				}
				else if (m_fMineCreateTime > 0.8f)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandJaw");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumMine %d", m_iMineCount);

					_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

					pGameObject = CVacuumMine::Create(m_pGraphicDev, vTemp, m_vNextPos6); //생성위치, 떨어질위치.
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iMineCount++;
					m_fMineX6 += -m_fMineDis;
					if (m_fMineX6 < -24.f)
					{
						m_fMineX6 = -18.f;
						m_fMineZ3 += m_fMineDis;
					}

					m_vNextPos6 = { m_fMineX6, 0.f, m_fMineZ3 };
					if (m_fMineZ3 > 21.f) //초기화 단계
					{
						m_fMineZ3 = -21.f;
						m_vNextPos6 = { m_fMineX6 , 0.f, m_fMineZ3 };
						//m_fMineTime = 0.f;
						m_bMineSelect = false;
						m_fMineCreateTime = 0.f;
						m_pMeshCom->Set_Animationset(11);
					}
				}
			}
			//LeftHandJaw, RightHandJaw


		}

		//붐(공격할수있는 폭탄).
		if (m_iPatturnSelect == 5)
		{
			m_fBombTime += fTimeDelta;
			m_bSound[Sound_Stone] = false;
			if (m_fBombTime > 1.f)
			{

				if (!m_bSound[Sound_Stone])
				{
					Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Stone);
					Engine::PlaySoundW(L"Vacuum_StoneReady.wav", CSoundMgr::CHANNELID::Vacuum_Stone, 0.2f);

					m_bSound[Sound_Stone] = true;
				}


				CTransform*		pTargetTransCom = static_cast<CTransform*>(m_pTargetPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

				_vec3 vTargetPos;
				vTargetPos = pTargetTransCom->Get_Position();
				m_pMeshCom->Set_Animationset(11);

				CLayer*				pLayer = nullptr;
				pLayer = Engine::Get_Layer(L"GameObject");
				Engine::CGameObject*		pGameObject = nullptr;

				const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("Neck");

				_matrix matBoneWorld, matWorld;
				matBoneWorld = pFrame->CombinedTranformationMatrix;
				matWorld = *(m_pTransformCom->Get_WorldMatrix());

				matWorld = matBoneWorld * matWorld;

				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"VacuumBomb %d", m_iBombCount);

				_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };
				_float fX;
				_float fZ;
				/*	fX = rand()*/
				CRandoms CRandom;
				fX = CRandom(-5.f, 5.f);
				fZ = CRandom(-5.f, 5.f);

				vTargetPos.x = vTargetPos.x + fX;
				vTargetPos.z = vTargetPos.z + fZ;
				auto* pBomb = m_vecBomb.front();
				pBomb->Set_Move(vTemp, vTargetPos);
				m_vecActiveBomb.emplace_back(pBomb);
				m_vecBomb.pop_front();

				m_iBombCount++;
				m_iBombMaxCount++;

				wsprintf(tagTemp, L"VacuumBomb %d", m_iBombCount);

				vTargetPos = pTargetTransCom->Get_Position();
				fX = CRandom(-5.f, 5.f);
				fZ = CRandom(-5.f, 5.f);

				vTargetPos.x = vTargetPos.x + fX;
				vTargetPos.z = vTargetPos.z + fZ;

				m_vecBomb.front()->Set_Move(vTemp, vTargetPos);
				m_vecActiveBomb.emplace_back(m_vecBomb.front());
				m_vecBomb.pop_front();

				m_iBombMaxCount++;
				m_iBombCount++;
				m_fBombTime = 0.f;
			}

			if (m_iBombMaxCount >= 10)
			{
				m_bPartPatturn = true;
				m_bPartUp = true;
				m_bBombCheck = true;
				m_iBombMaxCount = 0;
				m_fBombTime = 0.f;
				m_bPattrunSelect = false;
				return;
			}
		}

		//양손 
		if (m_iPatturnSelect == 4)
		{
			m_pMeshCom->Set_Animationset(5);
			m_fTwoHandCircleTime += fTimeDelta;

			CLayer*				pLayer = nullptr;
			pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			if (!m_bSound[Sound_Double])
			{
				Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Double);
				Engine::PlaySoundW(L"Vacuum_Double.wav", CSoundMgr::CHANNELID::Vacuum_Double);

				m_bSound[Sound_Double] = true;
			}


			if (m_fTwoHandCircleTime > 1.f)
			{

				if (m_iTwoHandCircleMax == 0)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandBottomLips1");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_vec3 vPos;
					vPos = { matWorld._41, matWorld._42 + 1.5f, matWorld._43 };

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumCircle %d", m_iTwoHandCircle);

					pGameObject = CVacuumCircle::Create(m_pGraphicDev, vPos);
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iTwoHandCircle++;
					m_iTwoHandCircleMax++;
				}
				if (m_iTwoHandCircleMax == 1)
				{

					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandBottomLips1");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_vec3 vPos;
					vPos = { matWorld._41, matWorld._42 + 1.2f, matWorld._43 + 0.5f };

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumCircle %d", m_iTwoHandCircle);

					pGameObject = CVacuumCircle::Create(m_pGraphicDev, vPos);
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iTwoHandCircleMax++;
					m_iTwoHandCircle++;

				}
			}

			if (m_iTwoHandCircleMax >= 2)
			{
				m_iTwoHandCircleMax = 0;
				m_fTwoHandCircleTime = 0.f;
				m_bPattrunSelect = false;
				m_bSound[Sound_Double] = false;
				return;
			}
		}

		//한손
		if (m_iPatturnSelect == 3)
		{
			m_pMeshCom->Set_Animationset(6);
			m_fOneHandCircleTime += fTimeDelta;

			CLayer*				pLayer = nullptr;
			pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			if (!m_bSound[Sound_Single])
			{
				Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Single);
				Engine::PlaySoundW(L"Vacuum_Single.wav", CSoundMgr::CHANNELID::Vacuum_Single);

				m_bSound[Sound_Single] = true;
			}

			if (m_fOneHandCircleTime > 1.f)
			{
				if (m_iOneHandCircleMax == 0)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandBottomLips1");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_vec3 vPos;
					vPos = { matWorld._41, matWorld._42 + 1.5f, matWorld._43 };

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumOneHandCircle %d", m_iOneHandCircle);

					pGameObject = CVacuumCircle::Create(m_pGraphicDev, vPos);
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iOneHandCircle++;
					m_iOneHandCircleMax++;
				}
			}

			if (m_fOneHandCircleTime > 2.5f)
			{
				if (m_iOneHandCircleMax == 1)
				{
					const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHandBottomLips1");

					_matrix matBoneWorld, matWorld;
					matBoneWorld = pFrame->CombinedTranformationMatrix;
					matWorld = *(m_pTransformCom->Get_WorldMatrix());

					matWorld = matBoneWorld * matWorld;

					_vec3 vPos;
					vPos = { matWorld._41, matWorld._42 + 1.2f, matWorld._43 + 0.5f };

					_tchar tagTemp[MAX_PATH];
					wsprintf(tagTemp, L"VacuumOneHandCircle %d", m_iOneHandCircle);

					pGameObject = CVacuumCircle::Create(m_pGraphicDev, vPos);
					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

					m_iOneHandCircle++;
					m_iOneHandCircleMax++;
				}
			}

			if (m_iOneHandCircleMax >= 2)
			{
				m_iOneHandCircleMax = 0;
				m_fOneHandCircleTime = 0.f;
				m_bPattrunSelect = false;
				m_bSound[Sound_Single] = false;
				return;
			}
		}
	}
}

void CVacuum::Stun(const _float & fTimeDelta)
{
	if (false == m_bTriggerStart)
	{
		m_pTrigger[CVacuum::MayRide]->Set_Interactable(true);
		m_pTrigger[CVacuum::CodyRide]->Set_Interactable(true);
		m_bTriggerStart = true;
	}

	m_iAniIndex = m_pMeshCom->Get_AniIndex();
	CCody*	pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CMay*	pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");

	if (pCody)
		m_bCodyRide = pCody->Get_RideEnd();
	//if(pMay)//메이타는거되면 이거해줘야함.
		//m_bMayRide = pMay->Get_RideEnd();

	//이거 대신에 타고있을때 bool값 던져서 둘다 트루면 고고
	if (Key_Down(DIK_7))
	{
		m_bMayRide = true;
		m_bCodyRide = true;
	}

	if (m_bCodyRide && m_bMayRide)
	{
		Dead(fTimeDelta);
		return;
	}

	if (!m_bStunFirst)
	{
		m_pMeshCom->Set_Animationset(animID::VacuumBoss_Stunned_Enter);
		if (m_iAniIndex == animID::VacuumBoss_Stunned_Enter && m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_pMeshCom->Set_Animationset(animID::VacuumBoss_Stunned, 1.0f, 0.05f);
			m_bStunFirst = true;

			//트리거 생성.
			if (!m_bCreateTrigger)
			{
				const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LeftHandBend");
				const _matrix*	m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;
				const _matrix*  m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();

				_matrix matWorld;
				matWorld = *m_pParentBoneMatrix * *m_pParentWorldMatrix;
				_vec3 vPos = { matWorld._41, matWorld._42, matWorld._43 };
				m_pTrigger[TriggerID::LeftHand] = CTrigger::CreateBoxTrigger(m_pScene, L"LeftHandBend", vPos, 1.f, TRIGGER_VACUUM_LEFT, FilterGroup::eInteractItem);


				pFrame = m_pMeshCom->Get_FrameByName("RightHandBend");
				const _matrix*	m_pParentBoneMatrix2 = &pFrame->CombinedTranformationMatrix;
				const _matrix*  m_pParentWorldMatrix2 = m_pTransformCom->Get_WorldMatrix();

				_matrix matWorld2;
				matWorld2 = *m_pParentBoneMatrix2 * *m_pParentWorldMatrix2;
				vPos = { matWorld2._41, matWorld2._42, matWorld2._43 };

				m_pTrigger[TriggerID::RightHand] = CTrigger::CreateBoxTrigger(m_pScene, L"RightHandBend", vPos, 1.f, TRIGGER_VACUUM_RIGHT, FilterGroup::eInteractItem);
				m_bCreateTrigger = true;
			}
		}
	}

}

void CVacuum::Dead(const _float & fTimeDelta)
{
	if (m_iAniIndex == animID::VacuumBoss_Stunned)
		m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_ArmsOverride_Preview);

	//if (m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_Preview && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_ArmsOverride_50, 1.f, 0.1f);
	//}
	//else if (m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_50 && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_ArmsOverride_75);
	//}
	//else if (m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_75 && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_EyePoppedOut);
	//}
	//else if (m_iAniIndex == animID::VacuumBoss_End_EyePoppedOut && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_EyePoppedOut_Enter);
	//}
	//else if (m_iAniIndex == animID::VacuumBoss_End_EyePoppedOut_Enter && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_Death);
	//}
	//else if (m_iAniIndex == animID::VacuumBoss_End_Death && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_bAniStop = true;
	//}


	if (m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_Preview && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_ArmsOverride_75, 1.f, 0.1f);
	}
	else if (m_iAniIndex == animID::VacuumBoss_End_ArmsOverride_75 && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_EyePoppedOut);
	}
	else if (m_iAniIndex == animID::VacuumBoss_End_EyePoppedOut && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_EyePoppedOut_Enter);
	}
	else if (m_iAniIndex == animID::VacuumBoss_End_EyePoppedOut_Enter && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bDeadEffect = true;
		m_pMeshCom->Set_Animationset(animID::VacuumBoss_End_Death);
	}
	else if (m_iAniIndex == animID::VacuumBoss_End_Death && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bAniStop = true;
	}


}


void CVacuum::Create_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_STATIC;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossName = pGameObject->Is<CImageUI>();
	m_pBossName->Set_Image(L"BossName");
	m_pBossName->Set_Size(_vec3{ 185.f, 24.f, 0.f });
	m_pBossName->Set_Pos(_vec3{ 180.f, -65.f, 0.f });
	m_pBossName->Set_RenderID(RENDER_UI);
	m_pBossName->Set_PassIndex(2);
	m_pBossName->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossName", pGameObject), );
	m_pBossName->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossHp = pGameObject->Is<CImageUI>();
	m_pBossHp->Set_Image(L"HP");
	m_pBossHp->Set_Size(_vec3{ 1600.f, 50.f, 0.f });
	m_pBossHp->Set_Pos(_vec3{ 160.f, -90.f, 0.f });
	m_pBossHp->Set_RenderID(RENDER_MENUUI);
	m_pBossHp->Set_PassIndex(4);
	m_pBossHp->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHP", pGameObject), );
	m_pBossHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossHpbg = pGameObject->Is<CImageUI>();
	m_pBossHpbg->Set_Image(L"HP");
	m_pBossHpbg->Set_Size(_vec3{ 1600.f, 50.f, 0.f });
	m_pBossHpbg->Set_Pos(_vec3{ 160.f, -90.f, 0.f });
	m_pBossHpbg->Set_RenderID(RENDER_UI);
	m_pBossHpbg->Set_PassIndex(5);
	m_pBossHpbg->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHPBG", pGameObject), );
	m_pBossHpbg->AddRef();

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CVacuum::Update_UI(const _float & fTimeDelta)
{
	HP_UI(fTimeDelta);
}

void CVacuum::HP_UI(const _float & fTimeDelta)
{
	if (m_iHP <= 0)
	{
		m_pBossHp->Set_Visible(false);
		m_pBossHpbg->Set_Visible(false);
		m_pBossName->Set_Visible(false);
	}

	m_pBossHp->Set_Amount((_float)m_iHP / (_float)m_iHPMax);
}

void CVacuum::Effect_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadEffect)
		return;

	m_fDeadTime += fTimeDelta;
	m_fDeadEffect += fTimeDelta;


	CMayCamera* pMayCam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
	CCodyCamera* pCodyCam = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");

	if (m_fDeadEffect > 0.2f)
	{
		StopNPlaySound(L"Rocket_Dead.wav", CSoundMgr::CHANNELID::Space_Missile_Dead);
		pMayCam->Set_Shake();
		pCodyCam->Set_Shake();
		_vec3 FixPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-7.f, 7.f), CRandoms()(-7.f, 7.f) + 3.f, CRandoms()(-7.f, 7.f));


		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(10.f, 10.f, 10.f);
		tParticle.vEndSize = _vec3(10.f, 10.f, 10.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 255.f };
		tParticle.vEndColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 0.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 1.f;

		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.vDir = { 0.f, 0.f, 0.f };

		tParticle.vPos = FixPos;
		Engine::Emit_ImageParticle(tParticle, 23, 4);


		Engine::Set_AnimState(tParticle, 8, 8, 1.f);
		tParticle.fLifeTime = 1.f;
		tParticle.vStartSize = _vec3(8.f, 8.f, 8.f);
		tParticle.vEndSize = _vec3(8.f, 8.f, 8.f);
		tParticle.vStartColor = { 0.f ,0.f, 0.f, 255.f };
		tParticle.vEndColor = { 0.f ,0.f, 0.f, 155.f };
		tParticle.vPos = FixPos;
		Engine::Emit_ImageParticle(tParticle, 29, 4);
		tParticle.vStartSize = _vec3(6.f, 6.f, 6.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		Engine::Emit_ImageParticle(tParticle, 30, 4);
		tParticle.vStartColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 255.f * 3.5f };
		tParticle.vEndColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 0.f };
		tParticle.vStartSize = _vec3(6.f, 6.f, 6.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		Engine::Emit_ImageParticle(tParticle, 30, 4);
		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
		Engine::Emit_ImageParticle(tParticle, 29, 4);


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vEndSize = _vec3(0.001f, 0.001f, 0.001f);

		tParticle2.vStartColor = { 255.f * 2, 130.f * 2, 85.f * 2.f, 255.f * 5 };
		tParticle2.vEndColor = { 255.f * 2, 130.f * 2, 85.f * 2.f, 255.f * 5 };
		tParticle2.bBill = false;
		tParticle2.bGravity = true;
		tParticle2.fGravityForce = 1.5f;
		tParticle2.vPos = FixPos;

		for (_int i = 0; i < 30; ++i)
		{
			tParticle2.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.5f;
			tParticle2.vRot = { D3DXToRadian(CRandoms()(1, 360)), D3DXToRadian(CRandoms()(1, 360)), D3DXToRadian(CRandoms()(1, 360)) };
			_float fRand = CRandoms()(10, 30) * 0.0001f;
			tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle2.fVelocity = _float(CRandoms()(1, 10) + 2.f);
			tParticle2.vDir = { CRandoms()(-100, 100) * 0.1f, CRandoms()(100, 2000) * 0.01f, CRandoms()(-100, 100) * 0.1f };
			Engine::Emit_MeshParticle(tParticle2, L"BreakIce1", 13);
		}


		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);

		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle3.fRotateSpeed = 0.f;

		tParticle3.vStartColor = { 255.f * 2, 130.f * 2, 85.f * 2, 255.f * 2.5f };
		tParticle3.vEndColor = { 255.f, 130.f, 85.f, 50.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		for (_int i = 0; i < 30; ++i)
		{
			_float fRand = CRandoms()(50, 100) * 0.003f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle3.fVelocity = CRandoms()(0, 10) * 0.1f + 2.5f;
			tParticle3.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
			tParticle3.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
			tParticle3.vPos = FixPos + _vec3(CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f);
			Engine::Emit_ImageParticle(tParticle3, 15, 11);
		}
		m_fDeadEffect = 0.f;
	}

	if (m_fDeadTime > 10.f)
	{
		m_fDeadTime = 0.f;
		m_bDeadEffect = false;
	}
}

CVacuum * CVacuum::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CVacuum*	pInstance = new CVacuum(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuum::Free(void)
{
	for (auto& pBomb : m_vecActiveBomb)
		Safe_Release(pBomb);
	for (auto& pBomb : m_vecBomb)
		Safe_Release(pBomb);

	Safe_Release(m_pBossHp);
	Safe_Release(m_pBossHpbg);
	Safe_Release(m_pBossName);

	CEnermy::Free();
}



void CVacuum::Create_Bomb(void * pArg)
{
	CVacuumBomb* pGameObject = nullptr;
	_vec3 vPos = { 0.f,-10000.f,0.f };
	for (int i = 0; i < 20; ++i)
	{
		pGameObject = CVacuumBomb::Create(m_pGraphicDev, vPos, vPos);
		NULL_CHECK_RETURN(pGameObject, );
		m_vecBomb.emplace_back(pGameObject);
	}
}

#pragma region 스테이지 파츠 업 다운

void CVacuum::All_FrontDoorOpen(_bool bState)
{
	CVacuumFieldPart4*	pPart4_1 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_1");
	CVacuumFieldPart4*	pPart4_2 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_2");

	pPart4_1->Set_MoveOpenCheck(true);
	pPart4_2->Set_MoveOpenCheck(true);
	CVacuumFieldPart5*	pHead = (CVacuumFieldPart5*)Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_2");
	if (bState)
		pHead->Set_MoveUpCheckFront(bState);

}

void CVacuum::All_BackDoorOpen()
{
	CVacuumFieldPart4*	pPart4_3 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_3");
	CVacuumFieldPart4*	pPart4_4 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_4");

	pPart4_3->Set_MoveOpenCheck(true);
	pPart4_4->Set_MoveOpenCheck(true);
}

void CVacuum::All_FrontDoorCloes()
{
	CVacuumFieldPart4*	pPart4_1 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_1");
	CVacuumFieldPart4*	pPart4_2 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_2");

	pPart4_1->Set_MoveCloseCheck(true);
	pPart4_2->Set_MoveCloseCheck(true);
}

void CVacuum::All_BackDoorCloes()
{
	CVacuumFieldPart4*	pPart4_3 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_3");
	CVacuumFieldPart4*	pPart4_4 = (CVacuumFieldPart4*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart4_4");

	pPart4_3->Set_MoveCloseCheck(true);
	pPart4_4->Set_MoveCloseCheck(true);
}


void CVacuum::All_PartUp()
{
	CVacuumFieldPart*	pPart1 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_1");
	CVacuumFieldPart*	pPart2 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_2");
	CVacuumFieldPart2*	pPart2_1 = (CVacuumFieldPart2*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart2_1");
	CVacuumFieldPart3*	pPart3_1 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_1");
	CVacuumFieldPart3*	pPart3_2 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_2");
	CVacuumFieldPart5*	pHeadPart = (CVacuumFieldPart5*)Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_1");

	pPart1->Set_MoveUpCheck(true);
	pPart2->Set_MoveUpCheck(true);
	pPart2_1->Set_MoveUpCheck(true);
	pPart3_1->Set_MoveUpCheck(true);
	pPart3_2->Set_MoveUpCheck(true);
	pHeadPart->Set_MoveUpCheck(true);
}

void CVacuum::All_PartDown()
{
	CVacuumFieldPart*	pPart1 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_1");
	CVacuumFieldPart*	pPart2 = (CVacuumFieldPart*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart_2");
	CVacuumFieldPart2*	pPart2_1 = (CVacuumFieldPart2*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart2_1");
	CVacuumFieldPart3*	pPart3_1 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_1");
	CVacuumFieldPart3*	pPart3_2 = (CVacuumFieldPart3*)Engine::Get_GameObject(L"GameObject", L"VacuumFieldPart3_2");
	CVacuumFieldPart5*	pHeadPart = (CVacuumFieldPart5*)Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_1");
	CVacuumFieldPart5*	pHead = (CVacuumFieldPart5*)Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_2");

	pPart1->Set_MoveDownCheck(true);
	pPart2->Set_MoveDownCheck(true);
	pPart2_1->Set_MoveDownCheck(true);
	pPart3_1->Set_MoveDownCheck(true);
	pPart3_2->Set_MoveDownCheck(true);
	pHeadPart->Set_MoveDownCheck(true);
	pHead->Set_MoveDownCheckFront(true);
}

void CVacuum::PartUp(const _float& fTimeDelta)
{

	m_fPartUpDelta += fTimeDelta;


	if (!m_bSound[Sound_PartUp])
	{
		Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Part);
		Engine::PlaySoundW(L"Play_World_Shed_Vacuum_Platform_BossBack_Down.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Part, 0.2f);

		m_bSound[Sound_PartUp] = true;
	}
	if (0 == m_iPartUpSelect && m_fPartUpDelta > 0.f)
	{
		All_BackDoorOpen();
		m_iPartUpSelect++;
	}

	if (1 == m_iPartUpSelect && m_fPartUpDelta > 0.5f)
	{
		All_PartUp();
		m_iPartUpSelect++;
	}

	if (2 == m_iPartUpSelect && m_fPartUpDelta > 4.5f)
	{
		All_FrontDoorOpen(true);
		m_iPartUpSelect++;
	}

	if (3 == m_iPartUpSelect && m_fPartUpDelta > 6.5f)
	{
		All_FrontDoorCloes();
		m_iPartUpSelect++;

		m_fPartUpDelta = 0.f;
		m_iPartUpSelect = 0;
		m_bPartUp = false;
		m_bSound[Sound_PartUp] = false;
	}

}

void CVacuum::PartDown(const _float& fTimeDelta)
{
	m_fPartDownDelta += fTimeDelta;

	if (!m_bSound[Sound_PartDown])
	{
		Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Part);
		Engine::PlaySoundW(L"Play_World_Shed_Vacuum_Platform_BossBack_Down.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Part, 0.2f);

		m_bSound[Sound_PartDown] = true;
	}

	if (0 == m_iPartDownSelect && m_fPartDownDelta > 0.f)
	{
		All_FrontDoorOpen(false);
		m_iPartDownSelect++;
	}

	if (1 == m_iPartDownSelect && m_fPartDownDelta > 0.5f)
	{
		All_PartDown();
		m_iPartDownSelect++;
	}

	if (2 == m_iPartDownSelect && m_fPartDownDelta > 2.0f)
	{
		All_FrontDoorCloes();
		m_iPartDownSelect++;

	}
	if (3 == m_iPartDownSelect && m_fPartDownDelta > 6.5f)
	{
		All_BackDoorCloes();
		m_iPartDownSelect++;

		m_fPartDownDelta = 0.f;
		m_iPartDownSelect = 0;
		m_bPartDown = false;
		m_bSound[Sound_PartDown] = false;
	}

}

#pragma endregion 

