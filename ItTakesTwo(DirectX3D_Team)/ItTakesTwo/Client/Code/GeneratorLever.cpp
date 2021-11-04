#include "stdafx.h"
#include "GeneratorLever.h"
#include "MayCamera.h"
#include "Export_Function.h"
#include "TeleportSpace.h"


CGeneratorLever::CGeneratorLever(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGeneratorLever::~CGeneratorLever()
{
}

HRESULT CGeneratorLever::Ready_Object(void * pArg, _vec3 TriggerPos, _vec3 vStartPos, wstring _tag, _uint DirType)
{
	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_vTriggerScale = tTemp.vTriggerScale;
	m_vPushStartPos = vStartPos;
	m_vTriggerPos = TriggerPos;
	m_tagBattery = _tag;
	m_iTypeIndex = DirType;
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	return S_OK;
}

_int CGeneratorLever::Update_Object(const _float & fTimeDelta)
{

	if (Key_Down(DIK_8))
	{
		m_bField_CutScene = true;
	}

	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_vTriggerPos, m_vTriggerScale.x, m_vTriggerScale.y, m_vTriggerScale.z, TRIGGERTYPE::TRIGGER_SPACE_GENERATOR_LEVER);
		m_bStart = false;
	}

	Move(fTimeDelta);
	Field_CutScene(fTimeDelta);
	Field_CutScene2(fTimeDelta);
	if (m_bMoveEnd && !m_vecInteractActor.empty())
	{
		if (static_cast<CGameObject*>(m_vecInteractActor[0])->On_Active(this)) //배터리
		{
			static_cast<CGameObject*>(m_vecInteractActor[1])->On_Active(this); //
			if (m_vecInteractActor.size() >= 3)
			{
				static_cast<CGameObject*>(m_vecInteractActor[2])->On_Active(this); //
				static_cast<CTeleportSpace*>(m_vecInteractActor[3])->Set_Active();
			}
		}
	}
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CGeneratorLever::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CGeneratorLever::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);			//	1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	m_pMeshCom->Render_Meshes(pEffect, 8);	//	pEffect->BeginPass(0);
											//	스태틱매쉬8
											//	다이나믹6
	pEffect->End();

	Safe_Release(pEffect);
}

void CGeneratorLever::Move(const _float & fTimeDelta)
{
	if (!m_bMove)
		return;

	if (m_iTypeIndex == 0)
	{
		if (m_fAngle >= 0.f)
		{
			m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(5.f));
			m_fAngle -= D3DXToRadian(5.f);
		}
		else
		{
			if (!m_bSoundStart)
			{
				StopSound(CSoundMgr::CHANNELID::Effect);
				PlaySoundW(L"Generator_PushSuccess.wav", CSoundMgr::CHANNELID::Effect, 0.5f);

				m_bSoundStart = true;
			}


			m_bMove = false;
			m_bMoveEnd = true;
		}
	}
	else if (m_iTypeIndex == 1)
	{
		if (m_fAngle >= 0.f)
		{
			m_pTransformCom->Rotation(ROT_X, -D3DXToRadian(5.f));
			m_fAngle -= D3DXToRadian(5.f);
		}
		else
		{
			if (!m_bSoundStart)
			{
				StopSound(CSoundMgr::CHANNELID::Effect);
				PlaySoundW(L"Generator_PushSuccess.wav", CSoundMgr::CHANNELID::Effect, 0.5f);

				m_bSoundStart = true;
			}


			m_bMove = false;
			m_bMoveEnd = true;
		}
	}

}

void CGeneratorLever::MoveRetrun(const _float & fTimeDelta)
{
	//if (!m_bMoveReturn)
	//	return;

	//if (m_fAngle >= 0.f)
	//{
	//	m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(2.f));
	//	m_fAngle -= D3DXToRadian(2.f);
	//}
	//else if(m_fAngle <= 20.f)
	//{
	//	
	//}


}

void CGeneratorLever::Field_CutScene(const _float & fTimeDelta)
{
	if (!m_bField_CutScene)
		return;

	Engine::End_Split();
	CMayCamera*		pCamera = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");


	pCamera->Set_Mode(CAMERA_FIX);
	pCamera->Set_ReachTime(1.f);
	pCamera->Set_Fix(_vec3(193.5f, 10.f, 0.f), _vec3(150.f, 35.f, 0.f));

	_bool m_bMove = pCamera->Is_MoveEnd();
	if (m_bMove && !m_bCutSceneStart)
	{
		Engine::StopNPlaySound(L"GravityFly_Start.wav", CSoundMgr::CHANNELID::Space_GravityStart, 0.5f);

		m_bField_CutScene = false;
		m_bField_CutScene2 = true;
		m_bCutSceneStart = true;
		return;
	}
	m_bCutSceneStart = false;
	
}

void CGeneratorLever::Field_CutScene2(const _float & fTimeDelta)
{
	if (!m_bField_CutScene2)
		return;

	CMayCamera*		pCamera = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
	if (m_bCutSceneStart)
	{
	

		pCamera->Set_Mode(CAMERA_FIX);
		pCamera->Set_ReachTime(2.f);
		pCamera->Set_Fix(_vec3(193.5f, 30.f, 0.f), _vec3(140.f, 100.f, 0.f));
		pCamera->Set_ResetTimer(2.5f, false);
		m_bCutSceneStart = false;
	}



}

HRESULT CGeneratorLever::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"GeneratorLever"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

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

HRESULT CGeneratorLever::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CGeneratorLever * CGeneratorLever::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _vec3 TriggerPos, _vec3 StartPos, wstring _tag, _uint DirType, _vec3 vPushDir)
{
	CGeneratorLever*	pInstance = new CGeneratorLever(pGraphicDev);

	pInstance->m_vPushDir = vPushDir;
	if (FAILED(pInstance->Ready_Object(pArg, TriggerPos, StartPos, _tag, DirType)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGeneratorLever::Free(void)
{
	Engine::CGameObject::Free();
}
