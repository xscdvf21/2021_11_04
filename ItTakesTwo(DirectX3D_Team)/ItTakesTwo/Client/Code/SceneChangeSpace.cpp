#include "stdafx.h"
#include "SceneChangeSpace.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Logo.h"
#include "Nozzle.h"
#include "Cody.h"
#include "May.h"
#include "CodyCamera.h"
#include "MayCamera.h"


CSceneChangeSpace::CSceneChangeSpace(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSceneChangeSpace::~CSceneChangeSpace(void)
{
}

_bool CSceneChangeSpace::Fade_Out(const _float& fTimeDelta)
{
	if (m_pUI->Is_ColorEnd())
		return true;
	m_pUI->Update_Object(fTimeDelta);
	m_pUI->LateUpdate_Object(fTimeDelta);
	return false;
}

HRESULT CSceneChangeSpace::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;
	return S_OK;
}

Engine::_int CSceneChangeSpace::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_vPos, m_vScale.x, m_vScale.y, m_vScale.z, TRIGGERTYPE::TRIGGER_SCENE_CHANGE);
		m_pTrigger->Set_Interactable(false);
		m_bStart = false;
	}


	if ((m_bSceneChange[0] && m_bSceneChange[1]) || (m_bInstance && (m_bSceneChange[0] || m_bSceneChange[1])))
	{
		if (m_pUI)
			m_pUI->Change_Color_From_To(1.f, { 0.f,0.f,0.f,0.f }, { 0.f,0.f,0.f,255.f });

		switch (m_tObjInfo.eSceneID)
		{
		case RESOURCE_VF:
			break;
		case RESOURCE_VF2:
			break;
		case RESOURCE_VF3:
			break;
		case RESOURCE_VB:
			break;
		case RESOURCE_CF:
			break;
		case RESOURCE_CB:
			break;
		case RESOURCE_SF:
		case RESOURCE_SF2:
		case RESOURCE_SF3:
			PlaySoundW(L"SceneChange.wav", Engine::CSoundMgr::CHANNELID::Scene_Change, 0.7f);
			break;
		case RESOURCE_SB:
			break;
		default:
			break;
		}

		if (!m_bSetScenePos)
		{

			CHANGE_SCENE(m_pGraphicDev, m_eNextScene, OBJ_NOEVENT);
		}
		else
		{
			CHANGE_SCENE_POS(m_pGraphicDev, m_eNextScene, m_vNextScenePos, OBJ_NOEVENT);
		}
		return SCENE_CHANGE;

	}
	return OBJ_NOEVENT;
}

_int CSceneChangeSpace::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CSceneChangeSpace::Render_Object(const _int& iIndex)
{
}

void* CSceneChangeSpace::On_Trigger_Active(void* pTrigger, void* pCaller)
{

	_wstr tagObj = ToObj<CGameObject>(pCaller)->Get_ObjTag();
	CGameObject* pPlayer = nullptr;
	if (tagObj == L"Cody")
	{
		m_bSceneChange[0] = true;
		pPlayer = Engine::Get_GameObject(L"GameObject", tagObj.c_str());
		if (pPlayer)
			pPlayer->Is<CPlayerValue>()->Set_ReadySceneChange();
	}
	else if (tagObj == L"May")
	{
		m_bSceneChange[1] = true;
		pPlayer = Engine::Get_GameObject(L"GameObject", tagObj.c_str());
		if (pPlayer)
			pPlayer->Is<CPlayerValue>()->Set_ReadySceneChange();
	}
	return nullptr;
}

void* CSceneChangeSpace::On_Active(void* pCaller)
{
	if (!m_bEnd)
	{
		return nullptr;
	}
	else
		m_bEnd = false;


	m_pTrigger->Set_Interactable(true);

	switch (m_tObjInfo.eSceneID)
	{
	case RESOURCE_VF:
		break;
	case RESOURCE_VF2:
		Event_VF2(pCaller);
		break;
	case RESOURCE_VF3:
		break;
	case RESOURCE_VB:
		break;
	case RESOURCE_CF:
		Event_TO_CB(pCaller);
		break;
	case RESOURCE_CB:
		break;
	case RESOURCE_SF:
		Event_TO_SF(pCaller);
		break;
	case RESOURCE_SF2:
		Event_TO_SF2(pCaller);
		break;
	case RESOURCE_SF3:
		Event_TO_SF3(pCaller);
		break;
	case RESOURCE_SB:
		Event_TO_SB(pCaller);
		break;
	default:
		break;
	}
	return nullptr;
}

void CSceneChangeSpace::Set_Next_Scene_StartPos(const _vec3 vPos)
{
	m_bSetScenePos = true;
	m_vNextScenePos = vPos;
}

void CSceneChangeSpace::Event_VF2(void* pCaller)
{
	_vec3 vPos = static_cast<CGameObject*>(pCaller)->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_Position();
	CTransform* pCody = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", COMPONENTID::ID_DYNAMIC)->Is<CTransform>();
	CTransform* pMay = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", COMPONENTID::ID_DYNAMIC)->Is<CTransform>();
	CCamera* pCamera;
	if (D3DXVec3Length(&(vPos - pCody->Get_Position())) <
		D3DXVec3Length(&(vPos - pMay->Get_Position())))
	{
		pCamera = static_cast<CCamera*>(Engine::Get_GameObject(L"GameObject", L"CodyCamera"));
		CCodyCamera* pCodyCamera = pCamera->Is<CCodyCamera>();
		pCodyCamera->Set_Mode(CAMERAMODE::CAMERA_FIX);
		pCodyCamera->Set_Fix(m_vPos, m_vPos + _vec3{ 0.f,20.f,-3.f });
	}
	else
	{
		pCamera = static_cast<CCamera*>(Engine::Get_GameObject(L"GameObject", L"MayCamera"));
		CMayCamera* pMayCamera = pCamera->Is<CMayCamera>();
		pMayCamera->Set_Mode(CAMERAMODE::CAMERA_FIX);
		pMayCamera->Set_Fix(m_vPos, m_vPos + _vec3{ 0.f,20.f,-3.f });

	}
	m_tObjInfo.vPos = m_vPos;
	m_tObjInfo.vPos.y -= 5.f;
	m_tObjInfo.vAngle = { 0.f,0.f,D3DXToRadian(-90.f) };
	Engine::Get_Layer(L"GameObject")->Add_GameObject(L"Nozzle", CNozzle::Create(m_pGraphicDev, pCamera, &m_tObjInfo));
}

void CSceneChangeSpace::Event_TO_SF2(void * pCaller)
{
}

void CSceneChangeSpace::Event_TO_SF3(void * pCaller)
{
}

void CSceneChangeSpace::Event_TO_SF(void * pCaller)
{

}

void CSceneChangeSpace::Event_TO_SB(void * pCaller)
{
}

void CSceneChangeSpace::Event_TO_CB(void * pCaller)
{
}


CSceneChangeSpace* CSceneChangeSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eNext, const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg /*= nullptr*/)
{
	CSceneChangeSpace*	pInstance = new CSceneChangeSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_Next_Scene(eNext);
	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fX, fY, fZ);
	return pInstance;
}

CSceneChangeSpace* CSceneChangeSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eNext, const _vec3& vPos, const _float& fHalf, void* pArg /*= nullptr*/)
{
	CSceneChangeSpace*	pInstance = new CSceneChangeSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_Next_Scene(eNext);
	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fHalf);
	return pInstance;
}

void CSceneChangeSpace::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSceneChangeSpace::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;

	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	//Temp.eSceneID = RESOURCE_STATIC;
	//m_pUI = CImageUI::Create(m_pGraphicDev,&Temp,false);
	//NULL_CHECK_RETURN(m_pUI, E_FAIL);
	//m_pUI->Set_RenderID(RENDER_UI);
	//m_pUI->Set_Image(L"White_Bg");
	return S_OK;
}