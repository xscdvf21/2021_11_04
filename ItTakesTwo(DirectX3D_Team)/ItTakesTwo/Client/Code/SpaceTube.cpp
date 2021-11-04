#include "stdafx.h"
#include "SpaceTube.h"

#include "Export_Function.h"
CSpaceTube::CSpaceTube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_eColor(TubeColor_End)
{
}

CSpaceTube::~CSpaceTube()
{
}

HRESULT CSpaceTube::Ready_Object(void * pArg, TubeColor _eColor)
{
	m_eColor = _eColor;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	return S_OK;
}

_int CSpaceTube::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		_vec3 vPos;
		vPos = m_pTransformCom->Get_Position();
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, _vec3(vPos.x ,vPos.y + 1.f, vPos.z), 3.f, 2.f ,3.f,  TRIGGER_SPACE_TUBE, FilterGroup::eDefaultTrigger);
		m_bStart = false;
	}

	Move(fTimeDelta);
	SizeChange(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceTube::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceTube::Render_Object(const _int & iIndex)
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

HRESULT CSpaceTube::Add_Component(void * pArg, TubeColor _eColor)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	_tchar tagTemp[MAX_PATH];
	if (m_eColor == TubeColor_Blue)
	{
		wsprintf(tagTemp, L"Tube_Blue");
	}
	else if (m_eColor == TubeColor_Orange)
	{
		wsprintf(tagTemp, L"Tube_Orange");
	}
	else if (m_eColor == TubeColor_Purple)
	{
		wsprintf(tagTemp, L"Tube_Purple");
	}

	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, tagTemp));
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

HRESULT CSpaceTube::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CSpaceTube::Move(const _float & fTimeDelta)
{
	if (m_eColor == TubeColor_Blue)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(2.f));
	}
	else if (m_eColor == TubeColor_Orange)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-1.5f));
	}
	else if (m_eColor == TubeColor_Purple)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(2.5f));
	}
}

void CSpaceTube::SizeChange(const _float & fTimeDelta)
{
	if (!m_bSizeChange)
		return;


	if (!m_bSoundStart)
	{
		StopSound(CSoundMgr::CHANNELID::Space_Tube);
		PlaySoundW(L"TubeJump.wav", CSoundMgr::CHANNELID::Space_Tube, 0.1f);

		m_bSoundStart = true;
	}

	_vec3 vScale;
	vScale = m_pTransformCom->Get_Scale();
	vScale -= _vec3(0.0005f, 0.0005f, 0.0005f);
	m_pTransformCom->Set_Scale(vScale);

	if (vScale.x <= 0.008f)
	{
		_vec3 Temp = { 0.01f, 0.01f, 0.01f };
		m_pTransformCom->Set_Scale(Temp);
		m_bSizeChange = false;
		m_bSoundStart = false;
	}

}

CSpaceTube * CSpaceTube::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, TubeColor _eColor)
{
	CSpaceTube*	pInstance = new CSpaceTube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, _eColor)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceTube::Free(void)
{
	Engine::CGameObject::Free();
}
