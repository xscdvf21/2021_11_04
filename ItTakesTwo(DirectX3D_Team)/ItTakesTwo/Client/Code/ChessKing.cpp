#include "stdafx.h"
#include "ChessKing.h"
#include "ChessTile.h"
#include "ChessPawn.h"
#include "ChessBishop.h"
#include "ChessKnight.h"
#include "ChessRook.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "ChessBossDoor.h"

//룩하고 나이츠 소환이구나.
CChessKing::CChessKing(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_bTram(false), m_bTramStart(true), m_fTramPower(0.f), m_fTramTime(0.f), m_fTramHeight(0.f), m_fDuration(0.9f), m_fFall(-3.f)
	, m_bFall(false), m_bAnglecheck(false)
{
}

CChessKing::~CChessKing()
{
}

HRESULT CChessKing::Ready_Object(void * pArg)
{
	m_fColDis = 1.5f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (nullptr != pArg)
	{

	}

	CHESSTILE temp;
	ZeroMemory(&temp, sizeof(CHESSTILE));
	temp = CChessTile::GetInstance()->Get_TileIndex(4);


	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90));
	m_pTransformCom->Set_Pos(temp.vPos);

	m_pMeshCom->Set_Animationset((animID::King_Idle));

	m_tShader.m_fRimDist = 5.f;
	m_tShader.m_iRimPower = 30;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	PhysicsCreate();
	return S_OK;
}

_int CChessKing::Update_Object(const _float & fTimeDelta)
{
	// HP가 0이하일 경우 실행
	if (CChessTile::GetInstance()->Get_BossHp() <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		Dead_State(fTimeDelta);

		m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		if (m_bDead)
		{

			CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
			pCamera->Set_Mode(CAMERA_DEFAULT);
			//pCamera->Set_Fix(_vec3(vCameraPos.x, vCameraPos.y + 3.f, vCameraPos.z), _vec3(vCameraEye.x + 4.f - m_fCameraAngle, vCameraEye.y + 13.f - (m_fCameraAngle *0.5f), vCameraEye.z + 8.f), true);
			pCamera->Set_Fov(D3DXToRadian(60.f));

			CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
			CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

			pCody->Set_CutScene(false);
			pCody->Set_Stencil(true);

			pMay->Set_CutScene(false);
			pMay->Set_Stencil(true);

			CChessBossDoor*			pDoor = (CChessBossDoor*)Engine::Get_GameObject(L"GameObject", L"ChessBossDoor");
			pDoor->Set_Move();

			return OBJ_DEAD;
		}
		return OBJ_NOEVENT;
	}


	CHESSTILE temp;
	ZeroMemory(&temp, sizeof(CHESSTILE));
	temp = CChessTile::GetInstance()->Get_Index(m_pTransformCom->Get_Position());


	Engine::CGameObject::Update_Object(fTimeDelta);

	Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	_vec3 vPos = m_pTransformCom->Get_Position();

	// 공격
	if (m_bAttAble)
		CCollisionMgr::GetInstance()->Add_EnermyWeaponList(this);

	// 피격
	CCollisionMgr::GetInstance()->Add_EnermyList(this);

	Tick_Damage(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	//PxTransform tTransform(vPos.x, m_pRigidActor->getGlobalPose().p.y, vPos.z);
	//
	//m_pRigidActor->setGlobalPose(tTransform);
	return OBJ_NOEVENT;
}

_int CChessKing::LateUpdate_Object(const _float & fTimeDelta)
{
	Hit_RimLight(fTimeDelta);

	_vec3 vPos = m_pTransformCom->Get_Position();
	m_pRigidActor->setGlobalPose(PxTransform(vPos.x, vPos.y + 1.0f, vPos.z));

	//m_pTransformCom->Set_Pos(vPos.x, m_pRigidActor->getGlobalPose().p.y, vPos.z);


	return _int();
}

void CChessKing::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 5);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	//const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Spine2");
	//_float4x4 matBoneMatrix = pBone->CombinedTranformationMatrix;

	//Spine2
	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CChessKing::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CB, L"Chess_King"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CChessKing::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);
	pEffect->SetBool("g_bRim", m_bRimLight);

	_matrix matViewInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matViewInv);
	D3DXMatrixInverse(&matViewInv, nullptr, &matViewInv);
	pEffect->SetVector("g_vCamPos", &_vec4(matViewInv._41, matViewInv._42, matViewInv._43, 1.f));

	pEffect->CommitChanges();
	return S_OK;
}

void CChessKing::Tick_Damage(const _float & fTimeDelta)
{
	if (!m_bTick)
		return;

	if (m_bTickFinite)
		m_fTickTime -= fTimeDelta;

	if (m_fTickTime <= 0.f)
		m_bTick = false;

	Set_Damage(-m_iTickDamge);
	m_bHit = true;
}

void CChessKing::JumpReady(const _vec3& vEndPos)
{
	m_vMoveTile = vEndPos;
}

void CChessKing::MoveJump(const _float & fTimeDelta, const _vec3& TargetPos)
{
	if (!m_bAnglecheck)
	{
		_float fStartPower = 0.f;
		_vec3 vLook;
		_vec3 vPos;
		_vec3 vRight;
		_vec3 vLeft;
		_vec3 vDir;
		_float fDir;

		vPos = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

		vLook.y = 0.f;
		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		vLeft = vRight * -1.f;
		vDir = m_vMoveTile - vPos;
		fDir = D3DXVec3Length(&vDir);
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir)));

		_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
		_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);

		if (fAngle >= D3DXToDegree(m_fTurnSpeed))
		{
			if (fRightAngle < 0 && fLeftAngle > 0)
			{
				m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
			}
			else if (fRightAngle > 0 && fLeftAngle < 0)
				m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
		}
	}

	if (fAngle <= D3DXToDegree(m_fTurnSpeed) || fAngle == 90.f)
		m_bAnglecheck = true;

	if (m_bAnglecheck)
	{

		if (m_bTramStart)
		{
			m_pMeshCom->Set_Animationset(animID::King_Jump);
			m_vSetPos = TargetPos;
			m_vTramDir = TargetPos - m_pTransformCom->Get_Position();
			D3DXVec3Normalize(&m_vTramDir, &m_vTramDir);

			m_fTramPower = ((m_vTramDir.y + 1.f) - m_pTransformCom->Get_Position().y - (m_fDuration * m_fDuration) * -GRAVITY) / m_fDuration;

			m_vTramDis = TargetPos - m_pTransformCom->Get_Position();
			m_vTramDis.y = 0.f;
			m_fTramHeight = m_pTransformCom->Get_Position().y;
			m_bTramStart = false;
		}

		m_fTramTime += fTimeDelta;

		_float fHeight = m_fTramHeight + ((m_fTramTime * m_fTramTime) * (-GRAVITY) / 1) + (m_fTramTime * m_fTramPower);

		_vec3 vDstPos = m_pTransformCom->Get_Position() + (m_vTramDir * D3DXVec3Length(&m_vTramDis) * fTimeDelta / m_fDuration);

		m_pTransformCom->Set_Pos(_vec3(vDstPos.x, fHeight, vDstPos.z));
		if (m_pTransformCom->Get_Position().y <= 0.f)
		{
			// Effect_Land
			Effect_Land();

			StopSound(CSoundMgr::CHANNELID::ChessKing);
			PlaySoundW(L"ChessKing_Jump0.wav", CSoundMgr::CHANNELID::ChessKing, 0.5f);

			m_pTransformCom->Set_Pos(m_vSetPos);
			m_fTramTime = 0.f;
			m_fJumpTestTime = 0.f;
			m_bTramStart = true;
			m_bAnglecheck = false;
			m_pMeshCom->Set_Animationset(animID::King_Idle);

			m_bAttAble = true;
		}
		else
		{
			// 콜리전 판정
			m_vecCollisionList.clear();
			m_bAttAble = false;
		}
	}
}

void CChessKing::Dead_State(const _float& fTimeDelta)
{
	if (!m_bSound[Death])
	{

		StopSound(CSoundMgr::CHANNELID::ChessKing);
		PlaySoundW(L"ChessKingQueen_Death.wav", CSoundMgr::CHANNELID::ChessKing);
		m_bSound[Death] = true;
	}

	m_pMeshCom->Set_Animationset(King_Dead);
	m_pMeshCom->Play_Animationset(fTimeDelta);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bDead = true;
	}
	else if (m_pMeshCom->Is_AnimationSetEnd(3.f))
	{

		CHESSTILE temp;
		ZeroMemory(&temp, sizeof(CHESSTILE));
		temp = CChessTile::GetInstance()->Get_TileIndex(43);
		m_pTransformCom->Set_Pos(temp.vPos);
		m_pTransformCom->Update_Component(fTimeDelta);
		//52
		_vec3 vCameraPos = CChessTile::GetInstance()->Get_TileIndex(42).vPos;
		m_pTransformCom->Set_AngleY(D3DXToRadian(-90.f));
		_vec3 vCameraEye = CChessTile::GetInstance()->Get_TileIndex(46).vPos;
		CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
		pCamera->Set_Mode(CAMERA_FIX);
		pCamera->Set_Fix(_vec3(vCameraPos.x, vCameraPos.y + 3.f, vCameraPos.z), _vec3(vCameraEye.x + 4.f - m_fCameraAngle, vCameraEye.y + 13.f - (m_fCameraAngle *0.5f), vCameraEye.z + 8.f), true);
		pCamera->Set_Fov(D3DXToRadian(25.f));

		m_fCameraAngle += 1.5f * fTimeDelta;


		CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
		CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

		pCody->Set_CutScene(true);
		pCody->Set_Stencil(false);

		pMay->Set_CutScene(true);
		pMay->Set_Stencil(false);
	}
}


void CChessKing::PhysicsCreate()
{
	//PhysX
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// 질량설정
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
}

void CChessKing::Set_Damage(_int iHp)
{

	CRandoms Random;
	_uint	iRandom = Random(1, 5);
	if (iRandom == 1)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessKing);
		Engine::PlaySoundW(L"ChessKingQueenHit0.wav", CSoundMgr::CHANNELID::ChessKing, 0.3f);
	}
	else if (iRandom == 2)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessKing);
		Engine::PlaySoundW(L"ChessKingQueenHit1.wav", CSoundMgr::CHANNELID::ChessKing, 0.3f);
	}
	else if (iRandom == 3)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessKing);
		Engine::PlaySoundW(L"ChessKingQueenHit2.wav", CSoundMgr::CHANNELID::ChessKing, 0.3f);
	}
	else if (iRandom == 4)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessKing);
		Engine::PlaySoundW(L"ChessKingQueenHit3.wav", CSoundMgr::CHANNELID::ChessKing, 0.3f);
	}
	else if (iRandom == 5)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessKing);
		Engine::PlaySoundW(L"ChessKingQueenHit4.wav", CSoundMgr::CHANNELID::ChessKing, 0.3f);
	}

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f), 5.f);
	Hit_Damage(1.5f);
	Hit_Rim();

	CChessTile::GetInstance()->Set_Damage(iHp);
}

void CChessKing::Move(const _float & fTimeDelta)
{
	m_iAniNum = m_pMeshCom->Get_AniIndex();

	m_fPaturnTime += fTimeDelta;
	m_fJumpTestTime += fTimeDelta;

	if (m_fJumpTestTime >= 5.f)
	{

		MoveTile(fTimeDelta);
		MoveJump(fTimeDelta, m_vMoveTile);
	}
	else
	{
		// 콜리전 판정
		m_bAttAble = false;
		m_vecCollisionList.clear();
	}


	if (m_fPaturnTime >= 10.f && m_iAniNum == animID::King_Idle)
	{

		m_iSelect = rand() % 2 + 1;
	}

	m_fJumpTest += fTimeDelta;
	Engine::CTransform*		pMayTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

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

	/*m_iSelect = 1;*/

	if (m_iSelect == 1)
	{
		m_pMeshCom->Set_Animationset(animID::King_Summon);
		if (m_iAniNum == animID::King_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{

			//나이트 생성구간.
			for (_uint i = 0; i <= 7; ++i)
			{
				_tchar tagTemp[MAX_PATH];
				_uint  iTemp;
				iTemp = i * 8 + 7;
				wsprintf(tagTemp, L"ChessKnight %d", iTemp);

				_uint iRandom = 0;

				iRandom = rand() % 7;

				if (iRandom != 0)
				{
					CLayer*				pLayer = nullptr;
					pLayer = Engine::Get_Layer(L"GameObject");
					Engine::CGameObject*		pGameObject = nullptr;


					pGameObject = CChessKnight::Create(m_pGraphicDev, iTemp);
					pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene());
					Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);
				}
			}

			m_iSelect = 0;
			m_fPaturnTime = 0.f;
			m_pMeshCom->Set_Animationset(animID::King_Idle);
		}
	}

	else if (m_iSelect == 2)
	{
		m_pMeshCom->Set_Animationset(animID::King_Summon);
		if (m_iAniNum == animID::King_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{

			//룩 생성구간.

			_tchar tagTemp[MAX_PATH];
			_uint  iTemp = 0;
			wsprintf(tagTemp, L"ChessRook %d", iTemp);

			_uint iRandomPos = 0;

			iRandomPos = rand() % 63;

			CLayer*				pLayer = nullptr;
			pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			pGameObject = CChessRook::Create(m_pGraphicDev, iRandomPos);
			pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene());
			Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


			m_iSelect = 0;
			m_fPaturnTime = 0.f;
			m_pMeshCom->Set_Animationset(animID::King_Idle);
		}
	}

}

void CChessKing::MoveTile(const _float & fTimeDelta)
{

	Engine::CTransform*		pMayTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

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

	//어차피 5초에 한번들어오기때문에 가까운애 추격.
	if (fMayDir < fCodyDir)
	{
		m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
	}
	else if (fCodyDir < fMayDir)
	{
		m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
	}


	vector<CHESSTILE> vecTemp;
	_uint iIndex;
	vPos = m_pTransformCom->Get_Position();

	CHESSTILE	tTemp;
	ZeroMemory(&tTemp, sizeof(CHESSTILE));
	tTemp = CChessTile::GetInstance()->Get_Index(vPos);
	iIndex = tTemp.TileIndex;
	vecTemp.emplace_back(tTemp);

	//킹은 이동력이 1임.
	for (_uint i = 1; i <= 1; ++i)
	{
		if (0 <= iIndex + i && 63 >= iIndex + i)
		{
			if ((iIndex + i) % 8 == 0)
				break;
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex + i);
			vecTemp.emplace_back(tTemp);
		}
	}

	for (_uint i = 1; i <= 1; ++i)
	{
		if (0 <= iIndex - i && 63 >= iIndex - i)
		{

			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex - i);
			vecTemp.emplace_back(tTemp);

			if ((iIndex - i) % 8 == 0)
				break;
		}
	}

	for (int i = 1; i <= 1; ++i)
	{
		if (0 <= iIndex - i * 8 && 63 >= iIndex - i * 8) //위쪽 이동 가능타일
		{
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex - i * 8);
			vecTemp.emplace_back(tTemp);

			int iTemp = iIndex - i * 8;
			for (int j = 1; j <= 1; j++)
			{
				if (0 <= iTemp - j && 63 >= iTemp - j)
				{

					tTemp = CChessTile::GetInstance()->Get_TileIndex(iTemp - j); //한칸 씩 위로 올라가면서 왼쪽 옆 타일도 포함.
					vecTemp.emplace_back(tTemp);
					if ((iTemp - j) % 8 == 0)
						break;
				}
			}

			for (int j = 1; j <= 1; j++) //한칸 씩 위로 올라가면서 오른쪽 옆 타일도 포함.
			{
				if (0 <= iTemp + j && 63 >= iTemp + j)
				{

					if ((iIndex + j) % 8 == 0)
						break;

					tTemp = CChessTile::GetInstance()->Get_TileIndex(iTemp + j);
					vecTemp.emplace_back(tTemp);


				}
			}

		}
	}

	/////////////////////////////아래로 내려가면서 양쪽 타일 /////////////////////////////////////////////////////////
	for (int i = 1; i <= 1; ++i)
	{
		if (0 <= iIndex + i * 8 && 63 >= iIndex + i * 8)
		{


			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex + i * 8);
			vecTemp.emplace_back(tTemp);

			int iTemp = iIndex + i * 8;

			for (int j = 1; j <= 1; j++)
			{
				if (0 <= iTemp - j && 63 >= iTemp - j)
				{

					tTemp = CChessTile::GetInstance()->Get_TileIndex(iTemp - j); //한칸 씩 위로 내려가면서 왼쪽 옆 타일도 포함.
					vecTemp.emplace_back(tTemp);
				}

				if ((iTemp - j) % 8 == 0)
					break;
			}

			for (int j = 1; j <= 1; j++)
			{
				if (0 <= iTemp + j && 63 >= iTemp + j)
				{
					if ((iIndex + j) % 8 == 0)
						break;
					tTemp = CChessTile::GetInstance()->Get_TileIndex(iTemp + j);  //한칸 씩 위로 올라가면서 오른쪽 옆 타일도 포함
					vecTemp.emplace_back(tTemp);
				}
			}
		}
	}

	if (m_pTargetPlayer != nullptr)
	{
		CTransform*		pTargetTransCom = static_cast<CTransform*>(m_pTargetPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

		_vec3	vTargetPos;
		_vec3	vTargetDir;

		vTargetPos = pTargetTransCom->Get_Position();

		_uint iIndex;
		_float fCurrentDir = 0.0f;
		_float fDir = 0.0f;
		_bool  bStart = false;
		for (auto i = 0; i < vecTemp.size(); ++i)
		{
			fCurrentDir = D3DXVec3Length(&(vTargetPos - vecTemp[i].vPos));
			// 구한거리
			if (!bStart || fDir >= fCurrentDir)
			{

				// 인덱스담고
				fDir = fCurrentDir;
				iIndex = i;

				bStart = true;
			}
		}
		vPos.y = 0;
		vecTemp[iIndex].vPos.y = 0;
		JumpReady(vecTemp[iIndex].vPos);
	}

}

void CChessKing::Effect_Land()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 7, 7, 1.5f);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(0.f, 0.f, 0.f);
	tParticle.vEndSize = _vec3(9.f, 9.f, 9.f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.vStartColor = { 250.f * 2.f, 235.f* 2.f, 0.f, 255.f };
	tParticle.vEndColor = { 250.f* 2.f, 235.f* 2.f, 0.f , 150.f };
	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 0.25f;
	tParticle.bBill = false;
	tParticle.vRot = { D3DXToRadian(90.f), 0.f, 0.f };

	tParticle.vDir = { 0.f, 0.f, 0.f };
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.2f, 0.f);
	Engine::Emit_ImageParticle(tParticle, 28, 18);

	CMainCamera* pCam = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCam->Set_Shake();
}

CChessKing * CChessKing::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CChessKing*	pInstance = new CChessKing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChessKing::Free(void)
{
	CEnermy::Free();
}
