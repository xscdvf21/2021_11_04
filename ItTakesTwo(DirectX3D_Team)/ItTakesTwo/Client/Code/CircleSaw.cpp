#include "stdafx.h"
#include "CircleSaw.h"

#include "Export_Function.h"
#include "FuseNPC.h"

CCircleSaw::CCircleSaw(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CCircleSaw::~CCircleSaw()
{
}

HRESULT CCircleSaw::Ready_Object(void * pArg, _bool bRun)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_pMeshCom->Set_Animationset(0);

	return S_OK;
}

_int CCircleSaw::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGERTYPE::TRIGGER_SHOOT);
		m_bStart = false;
	}

	if (m_bWork == true)
	{
		if (false == m_bPlaySoundWork)
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"CircleSawWork.wav", CSoundMgr::CHANNELID::Vacuum_CircleSaw_Work, g_fEffecctSound);
			m_bPlaySoundWork = true;
		}
		m_pMeshCom->Set_Animationset(0);
		m_pMeshCom->Play_Animationset(fTimeDelta);
	}

	if (false == m_bStop && true == m_bWork && true == m_bMay && true == m_bCody)
		m_bMove = true;

	if (Engine::Key_Down(DIK_R))
	{
		m_bMove = true;
	}

	if (false == m_bStop && true == m_bMove && m_pTransformCom->Get_Position().y >= 42.82f)
	{
		m_bMove = false;
		m_bStop = true;
		m_vAttachCodyPos += m_vGoalPos - m_pTransformCom->Get_Position();
		m_vAttachMayPos += m_vGoalPos - m_pTransformCom->Get_Position();
		m_pTransformCom->Set_Pos(m_vGoalPos);
	}
	else if (false == m_bStop && true == m_bMove)
	{
		if (false == m_bMovestart)
		{
			m_bMovestart = true;
			StopNPlaySound(L"VF_GetLastFuse.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);
			Engine::Get_GameObject(L"GameObject", L"FuseNPC1")->Is<CFuseNPC>()->EventStart();
		}

		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		m_fSpeed += 50.0f * fTimeDelta * fTimeDelta;
		m_vAttachCodyPos += vLook * m_fSpeed;
		m_vAttachMayPos += vLook * m_fSpeed;
		m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + vLook * m_fSpeed);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CCircleSaw::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCircleSaw::Render_Object(const _int & iIndex)
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

HRESULT CCircleSaw::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = tTemp;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"CircleSaw"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Angle(tTemp.vAngle);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CCircleSaw::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void * CCircleSaw::On_Active(void * pCaller)
{
	m_bWork = true;

	if (false == m_bPlaySound)
	{
		CSoundMgr::GetInstance()->StopNPlaySound(L"CircleSawStart.wav", CSoundMgr::CHANNELID::Vacuum_CircleSaw, g_fEffecctSound);
		m_bPlaySound = true;
	}
	return nullptr;
}

CCircleSaw * CCircleSaw::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun)
{
	CCircleSaw*	pInstance = new CCircleSaw(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, bRun)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCircleSaw::Free(void)
{
	Engine::CGameObject::Free();
}
