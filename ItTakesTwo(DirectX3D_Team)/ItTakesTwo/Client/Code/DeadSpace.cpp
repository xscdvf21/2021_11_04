#include "stdafx.h"
#include "DeadSpace.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"

CDeadSpace::CDeadSpace(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_bTex(false)
{

}

CDeadSpace::~CDeadSpace(void)
{
}

HRESULT CDeadSpace::Ready_Object(void* pArg)
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

HRESULT CDeadSpace::Ready_Object(_tchar* tTexName, _bool bTex /*= false*/, void* pArg /*= nullptr*/)
{
	m_bTex = bTex;
	m_tTexName = tTexName;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

//#ifdef _DEBUG
	if (m_bTex)
	{
		m_pTransformCom->Set_Scale(m_tObjInfo.vScale);
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(m_tObjInfo.vAngle.x));
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(m_tObjInfo.vAngle.y));
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(m_tObjInfo.vAngle.z));
		m_pTransformCom->Set_Pos(m_tObjInfo.vPos);
	}
//#endif // _DEBUG
	return S_OK;
}

Engine::_int CDeadSpace::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	//m_bTex = g_bRenderBox;
	m_fTimeDelta += 0.0016f;


	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGERTYPE::TRIGGER_DEADSPACE);
		m_bStart = false;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CDeadSpace::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CDeadSpace::Render_Object(const _int& iIndex)
{
	if (m_bTex)
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		pEffect->AddRef();

		FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

		_uint	iPassMax = 0;

		pEffect->Begin(&iPassMax, 0);

		if (lstrcmp(m_tTexName, L""))
			m_pMeshCom->Render_Meshes(pEffect, m_iPass);

		pEffect->End();

		Safe_Release(pEffect);
	}
}



void * CDeadSpace::On_Active(void * pCaller)
{
	m_pTrigger->Set_Interactable(false);
	return nullptr;
}

CDeadSpace* CDeadSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* tTexName	/*= L""*/, _bool bTex /*= false*/, void* pArg /*= nullptr*/)
{
	CDeadSpace*	pInstance = new CDeadSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tTexName, bTex, pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDeadSpace::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CDeadSpace::Add_Component(void* pArg)
{

	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;

	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}

	if (lstrcmp(m_tTexName, L""))
	{
		// StaticMesh
		pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(Temp.eSceneID, m_tTexName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	}

	if (!lstrcmp(m_tTexName, L"Water"))
		m_iPass = 15;
	else if (!lstrcmp(m_tTexName, L"Lava"))
		m_iPass = 18;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(0, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	

	return S_OK;
}

HRESULT CDeadSpace::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_DeltaTime", m_fTimeDelta);
	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
	m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 46);

	pEffect->CommitChanges();

	return S_OK;
}
