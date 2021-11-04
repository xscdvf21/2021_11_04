#include "stdafx.h"
#include "ChessQueen.h"
#include "ChessTile.h"
#include "ChessPawn.h"
#include "ChessBishop.h"
#include "ChessKing.h"
#include "Export_Function.h"
#include "May_Castle.h"
#include "Cody_Wizard.h"
#include "MainCamera.h"
#include "CollisionMgr.h"

CChessQueen::CChessQueen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_fPaturnTime(3.f)
	, m_bTram(false), m_bTramStart(true), m_fTramPower(0.f), m_fTramTime(0.f), m_fTramHeight(0.f), m_fDuration(0.9f), m_fFall(-3.f)
	, m_bFall(false), m_bAnglecheck(false)
{
}

CChessQueen::~CChessQueen()
{
}

HRESULT CChessQueen::Ready_Object(void * pArg)
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
	temp = CChessTile::GetInstance()->Get_TileIndex(3);


	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90));
	m_pTransformCom->Set_Pos(temp.vPos);

	PhysicsCreate();

	m_pMeshCom->Set_Animationset((animID::Queen_Idle));

	m_tShader.m_fRimDist = 5.f;
	m_tShader.m_iRimPower = 30;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	return S_OK;
}

_int CChessQueen::Update_Object(const _float & fTimeDelta)
{
	// HP가 0이하일 경우 실행
	if (CChessTile::GetInstance()->Get_BossHp() <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		Dead_State(fTimeDelta);

		m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

		if (m_bDead)
			return OBJ_DEAD;

		return OBJ_NOEVENT;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_EnermyList(this);

	Tick_Damage(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CChessQueen::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_Position();

	m_pRigidActor->setGlobalPose(PxTransform(vPos.x, vPos.y + 1.0f, vPos.z));

	Hit_RimLight(fTimeDelta);

	//m_pTransformCom->Set_Pos(m_pRigidActor->getGlobalPose());

	return _int();
}

void CChessQueen::Render_Object(const _int & iIndex)
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

	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CChessQueen::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CB, L"Chess_Queen"));
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


	CHESSTILE temp;
	ZeroMemory(&temp, sizeof(CHESSTILE));
	temp = CChessTile::GetInstance()->Get_TileIndex(20);


	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(temp.vPos);


	return S_OK;
}

HRESULT CChessQueen::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CChessQueen::Tick_Damage(const _float & fTimeDelta)
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

void CChessQueen::Move(const _float & fTimeDelta)
{
	m_iAniNum = m_pMeshCom->Get_AniIndex();

	m_fPaturnTime += fTimeDelta;
	m_fJumpTestTime += fTimeDelta;

	if (m_fJumpTestTime >= 5.f)
	{

		MoveTile(fTimeDelta);
		MoveStraight(fTimeDelta, m_vMoveTile);
	}

	if (m_fPaturnTime >= 10.f && m_iAniNum == animID::Queen_Idle)
	{
		m_iSelect = rand() % 2 + 1;
	}

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

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	if (m_fJumpTest >= 5.f)
	{
		pBody->addForce(PxVec3(0.f, 1.f, 0.f)* 75.f, PxForceMode::eIMPULSE);
		m_fJumpTest = 0.f;
	}

	if (m_iSelect == 1)
	{
		m_pMeshCom->Set_Animationset(animID::Queen_Summon);
		if (m_iAniNum == animID::Queen_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{

			//비숍.
			for (_uint i = 0; i <= 1; i++)
			{
				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"ChessBishop %d", i);

				_uint iRandomPos = 0;
				iRandomPos = rand() % 63;

				if (i == 0)
				{
					iRandomPos = (iRandomPos / 2) * 2;
				}
				else if (i == 1)
				{
					iRandomPos = (iRandomPos / 2) * 2 + 1;
				}

				CLayer*				pLayer = nullptr;
				pLayer = Engine::Get_Layer(L"GameObject");
				Engine::CGameObject*		pGameObject = nullptr;

				pGameObject = CChessBishop::Create(m_pGraphicDev, iRandomPos, i);
				pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene());
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

			}
			m_iSelect = 0;
			m_fPaturnTime = 0.f;
			m_pMeshCom->Set_Animationset(animID::Queen_Idle);
		}
	}
	else if (m_iSelect == 2)
	{
		m_pMeshCom->Set_Animationset(animID::Queen_Summon);
		if (m_iAniNum == animID::Queen_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{

			//폰
			for (_uint i = 0; i <= 1; i++)
			{
				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"ChessPawn %d", i);

				_uint iRandomPos = 0;

				iRandomPos = rand() % 63;

				if (i == 0)
				{
					iRandomPos = (iRandomPos / 2) * 2;
				}
				else if (i == 1)
				{
					iRandomPos = (iRandomPos / 2) * 2 + 1;
				}

				CLayer*				pLayer = nullptr;
				pLayer = Engine::Get_Layer(L"GameObject");
				Engine::CGameObject*		pGameObject = nullptr;

				pGameObject = CChessPawn::Create(m_pGraphicDev, iRandomPos,i );
				pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene());
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

			}
			m_iSelect = 0;
			m_fPaturnTime = 0.f;
			m_pMeshCom->Set_Animationset(animID::Queen_Idle);
		}
	}
}

void CChessQueen::MoveTile(const _float & fTimeDelta)
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
	CGameObject*	pObj = static_cast<CChessKing*>(Engine::Get_GameObject(L"GameObject", L"Chess_King"))->Get_Target();
	NULL_CHECK_RETURN(pObj, );

	if (pObj == nullptr)
	{
		if (fMayDir < fCodyDir)
		{
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
		else if (fCodyDir < fMayDir)
		{
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
		}
	}
	else if (pObj != nullptr)
	{
		if (typeid(*pObj) == typeid(CMayCastle))
		{
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
		}
		else if (typeid(*pObj) == typeid(CCodyWizard))
		{
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
	}

	
	
	vector<CHESSTILE> vecTemp;
	_int iIndex;
	vPos = m_pTransformCom->Get_Position();

	CHESSTILE	tTemp;
	ZeroMemory(&tTemp, sizeof(CHESSTILE));
	tTemp = CChessTile::GetInstance()->Get_Index(vPos);
	iIndex = tTemp.TileIndex;
	vecTemp.emplace_back(tTemp);
	
	//오른쪽 직선
	for (_int i = 1; i <= 8; ++i)
	{
		if (0 <= iIndex + i && 63 >= iIndex + i)
		{
			if ((iIndex + i) % 8 == 0)
				break;
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex + i);
			vecTemp.emplace_back(tTemp);
		}
	}
	//왼쪽 직선
	for (_int i = 1; i <= 8; ++i)
	{
		if (0 <= iIndex - i && 63 >= iIndex - i)
		{

			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex - i);
			vecTemp.emplace_back(tTemp);
			if ((iIndex - i) % 8 == 0)
				break;
		}
	}

	//위쪽 직선.
	for (_int i = 1; i <= 8; ++i)
	{
		if (0 <= iIndex - i * 8 && 63 >= iIndex - i * 8)
		{
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex - i * 8);
			vecTemp.emplace_back(tTemp);
			if ((iIndex - i) % 8 == 0)
				break;
		}
	}
	//아래쪽 직선.
	for (_int i = 1; i <= 8; ++i)
	{
		if (0 <= iIndex + i * 8 && 63 >= iIndex + i * 8)
		{

			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex + i * 8);
			vecTemp.emplace_back(tTemp);
			if ((iIndex - i) % 8 == 0)
				break;
		}
	}

	//왼쪽 대각선 위로.
	for (_int i = 1; i <= 8; ++i)
	{
		_int iTemp = iIndex - i * 9;
		if (0 <= iTemp - i && 63 >= iTemp - i)
		{
			if ((iTemp - i) % 8 <= 0)
				break;
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex - i * 9);
			vecTemp.emplace_back(tTemp);
		}
	}

	//오른쪽 대각선 위로.
	for (_int i = 1; i <= 8; ++i)
	{
		_int iTemp = iIndex - i *7;
		if (0 <= iIndex - i * 7 && 63 >= iIndex - i * 7)
		{
			if ((iTemp - i) % 8 <= 0)
				break;
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex - i * 7);
			vecTemp.emplace_back(tTemp);

		}
	}

	//왼쪽 대각선 아래
	for (_uint i = 1; i <= 8; ++i)
	{

		_int iTemp = iIndex + i * 7;
		if (0 <= iTemp - i && 63 >= iTemp - i)
		{

			//if ((iIndex + i) % 7 == 0)
			//	break;
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex + i * 7);
			vecTemp.emplace_back(tTemp);
			if ((iTemp + i) % 8 <= 0)
				break;
		}
	}

	//오른쪽 대각선 아래.,/
	for (_uint i = 1; i <= 8; ++i)
	{
		_int iTemp = iIndex + i * 9;
		if (0 <= iIndex + i * 9 && 63 >= iIndex + i * 9)
		{

			if ((iTemp + i) % 8 <= 0)
				break;
			tTemp = CChessTile::GetInstance()->Get_TileIndex(iIndex + i * 9);
			vecTemp.emplace_back(tTemp);

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

void CChessQueen::JumpReady(const _vec3 & vEndPos)
{
	m_vMoveTile = vEndPos;
}

void CChessQueen::MoveStraight(const _float & fTimeDelta, const _vec3 & TargetPos)
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
			//else if (fRightAngle == 0.f && fLeftAngle == 0.f)
			//{
			//	m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
			//}
		}
	}

	if (fAngle <= D3DXToDegree(m_fTurnSpeed))
		m_bAnglecheck = true;

	if (m_bAnglecheck)
	{

		if (m_bTramStart)
		{
			m_pMeshCom->Set_Animationset(animID::Queen_Move);
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

		//_float fHeight = m_fTramHeight + ((m_fTramTime * m_fTramTime) * (-GRAVITY) / 1) + (m_fTramTime * m_fTramPower);

		_vec3 vDstPos = m_pTransformCom->Get_Position() + (m_vTramDir * D3DXVec3Length(&m_vTramDis) * fTimeDelta / m_fDuration);

		m_pTransformCom->Set_Pos(_vec3(vDstPos.x, 0.f, vDstPos.z));

		_vec3 vDirTemp = m_vSetPos - m_pTransformCom->Get_Position();
		_float fDirTemp = D3DXVec3Length(&vDirTemp);
		if (fDirTemp <= 1.f)
		{
			m_pTransformCom->Set_Pos(m_vSetPos);
			m_fTramTime = 0.f;
			m_fJumpTestTime = 0.f;
			m_bTramStart = true;
			m_bAnglecheck = false;
			m_pMeshCom->Set_Animationset(animID::Queen_Idle);
		}
	}
}

void CChessQueen::Dead_State(const _float & fTimeDelta)
{
	m_pMeshCom->Set_Animationset(Queen_Dead);
	m_pMeshCom->Play_Animationset(fTimeDelta);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bDead = true;
	}
	else if (m_pMeshCom->Is_AnimationSetEnd(3.f))
	{
		CHESSTILE temp;
		ZeroMemory(&temp, sizeof(CHESSTILE));
		temp = CChessTile::GetInstance()->Get_TileIndex(33);
		m_pTransformCom->Set_Pos(temp.vPos);
		m_pTransformCom->Set_AngleY(D3DXToRadian(-270.f));
		m_pTransformCom->Update_Component(fTimeDelta);
	}
}

void CChessQueen::PhysicsCreate()
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

void CChessQueen::Set_Damage(_int iHp)
{
	CRandoms Random;
	_uint	iRandom = Random(1, 5);
	if (iRandom == 1)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessQueen);
		Engine::PlaySoundW(L"ChessKingQueenHit0.wav", CSoundMgr::CHANNELID::ChessQueen, 0.3f);
	}
	else if (iRandom == 2)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessQueen);
		Engine::PlaySoundW(L"ChessKingQueenHit1.wav", CSoundMgr::CHANNELID::ChessQueen, 0.3f);
	}
	else if (iRandom == 3)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessQueen);
		Engine::PlaySoundW(L"ChessKingQueenHit2.wav", CSoundMgr::CHANNELID::ChessQueen, 0.3f);
	}
	else if (iRandom == 4)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessQueen);
		Engine::PlaySoundW(L"ChessKingQueenHit3.wav", CSoundMgr::CHANNELID::ChessQueen, 0.3f);
	}
	else if (iRandom == 5)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessQueen);
		Engine::PlaySoundW(L"ChessKingQueenHit4.wav", CSoundMgr::CHANNELID::ChessQueen, 0.3f);
	}

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f), 5.f);
	Hit_Damage(1.5f);
	Hit_Rim();
	CChessTile::GetInstance()->Set_Damage(iHp);
}

CChessQueen * CChessQueen::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CChessQueen*	pInstance = new CChessQueen(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChessQueen::Free(void)
{
	CEnermy::Free();
}
