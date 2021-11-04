#include "stdafx.h"
#include "ChessDoorSwitch.h"
#include "ChessOpenDoor.h"
#include "ChessBridge.h"
#include "Export_Function.h"

CChessDoorSwitch::CChessDoorSwitch(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bMoveDoor(false)
	, m_bCreateTrigger(false), m_bFireCreate(false), m_bMoveBridge(false), m_bCrusherMove(false)
	, m_eButtonID(BUTTON_END)
{
}

CChessDoorSwitch::~CChessDoorSwitch()
{
}

HRESULT CChessDoorSwitch::Ready_Object(BUTTONID _eButton, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(_eButton, pArg), E_FAIL);


	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_eButtonID = _eButton;


	return S_OK;
}

_int CChessDoorSwitch::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateTrigger)
	{
		TriggerCreate();
	}
	switch (m_eButtonID)
	{
	case BUTTON_DOOR:
		MoveDoor(fTimeDelta);
		break;
	case BUTTON_FIRE:
		CreateFire(fTimeDelta);
		break;
	case BUTTON_BRIDGE:
		MoveBridge(fTimeDelta);
		break;
	case BUTTON_CRUSHER:
		MoveCrusher(fTimeDelta);
		break;
	default:
		break;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CChessDoorSwitch::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CChessDoorSwitch::Render_Object(const _int & iIndex)
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

void CChessDoorSwitch::TriggerCreate()
{
	switch (m_eButtonID)
	{
	case BUTTON_DOOR:
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5f, TRIGGER_CHESS_DOORBUTTON, FilterGroup::eButton);
		break;
	case BUTTON_FIRE:
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5f, TRIGGER_CHESS_FIREBUTTON, FilterGroup::eButton);
		break;
	case BUTTON_BRIDGE:
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5f, TRIGGER_CHESS_BRIDGE, FilterGroup::eButton);
		break;
	case BUTTON_CRUSHER:
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5f, TRIGGER_CHESS_CRUSHERBUTTON, FilterGroup::eButton);
		break;
	default:
		break;
	}

	m_bCreateTrigger = true;
}


void CChessDoorSwitch::MoveCrusher(const _float & fTimeDelta)
{
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

	CChessBridge*	pBridge = (CChessBridge*)Engine::Get_GameObject(L"GameObject", L"ChessBridge");
	if (m_bCrusherMove)
	{

		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 1.f, fTimeDelta);
		if (vPos.y <= -0.2f)
		{
			m_pTransformCom->Set_Pos(vPos);
		
		}
	}
	else if (!m_bCrusherMove)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 1.f, fTimeDelta);
		if (vPos.y >= 0.f)
		{
			m_pTransformCom->Set_Pos(vPos);
		
		}
	}
}

HRESULT CChessDoorSwitch::Add_Component(BUTTONID _eButton, void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessDoorSwitch"));
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
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	return S_OK;
}

HRESULT CChessDoorSwitch::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CChessDoorSwitch::MoveDoor(const _float & fTimeDelta)
{
	if (true == m_bMoveDoor)
	{
		_vec3 vPos;
		vPos = m_pTransformCom->Get_Position();
		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 1.f, fTimeDelta);

		m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());
		if (vPos.y <= -0.2f)
		{
			m_pTransformCom->Set_Pos(vPos);
			CChessOpenDoor*		pDoor = (CChessOpenDoor*)Engine::Get_GameObject(L"GameObject", L"ChessOpenDoor");
			pDoor->Set_Open(true);
		}
	}
}

void CChessDoorSwitch::CreateFire(const _float & fTimeDelta)
{

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

	if (m_bFireCreate)
	{
		
		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 1.f, fTimeDelta);
		if (vPos.y <= -0.2f)
		{	
			//여기서 파이어 기둥 소환하면 될듯.
			m_pTransformCom->Set_Pos(vPos);
		}
	}
	else if (!m_bFireCreate)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 1.f, fTimeDelta);
		if (vPos.y >= 0.f)
		{
			m_pTransformCom->Set_Pos(vPos);
		}
	}
}

void CChessDoorSwitch::MoveBridge(const _float & fTimeDelta)
{
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

	CChessBridge*	pBridge = (CChessBridge*)Engine::Get_GameObject(L"GameObject", L"ChessBridge");
	if (m_bMoveBridge)
	{

		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 1.f, fTimeDelta);
		if (vPos.y <= -0.2f)
		{
			m_pTransformCom->Set_Pos(vPos);
			pBridge->Set_Move(true);
		}
	}
	else if (!m_bMoveBridge)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 1.f, fTimeDelta);
		if (vPos.y >= 0.f)
		{
			m_pTransformCom->Set_Pos(vPos);
			pBridge->Set_Move(false);
		}
	}
}
CChessDoorSwitch * CChessDoorSwitch::Create(LPDIRECT3DDEVICE9 pGraphicDev, BUTTONID _eButton, void * pArg)
{
	CChessDoorSwitch*	pInstance = new CChessDoorSwitch(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_eButton,pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChessDoorSwitch::Free(void)
{
	Engine::CGameObject::Free();
}
