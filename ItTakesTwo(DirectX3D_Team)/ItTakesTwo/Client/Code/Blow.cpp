#include "stdafx.h"
#include "Blow.h"

CBlow::CBlow(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice), m_fTimeDelta(0.f)
{
	m_iChannelID = g_iChannelID++;
}

CBlow::~CBlow()
{
}

HRESULT CBlow::Ready_Object(void* pArg /*= nullptr*/)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	switch (m_iChannelID)
	{
	case 0:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow0;
		break;
	case 1:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow1;
		break;
	case 2:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow2;
		break;
	case 3:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow3;
		break;
	case 4:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow4;
		break;
	case 5:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow5;
		break;
	case 6:
		m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow6;
		break;
	}


	return S_OK;
}

Engine::_int CBlow::Update_Object(const _float& TimeDelta)
{
	m_fTimeDelta += TimeDelta;

	if (m_bStart)
	{
		m_pCodyTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_pMayTransform = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_bStart = false;
	}


CGameObject::Update_Object(TimeDelta);
return OBJ_NOEVENT;
}

Engine::_int CBlow::LateUpdate_Object(const _float & TimeDelta)
{
	CGameObject::LateUpdate_Object(TimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fEffectTime += TimeDelta;

	return OBJ_NOEVENT;
}

void CBlow::Render_Object(const _int& iIndex /*= 0*/)
{
	Play_Sound();
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMesh->Render_Meshes(pEffect, 2);

	pEffect->End();

	Safe_Release(pEffect);
}

CBlow* CBlow::Create(LPDIRECT3DDEVICE9 pDevice, void* pArg)
{
	CBlow* pInstance = new CBlow(pDevice);
	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	//	pInstance->Get_Transform()->Set_ParentMatrix(Parent);
	return pInstance;
}

void CBlow::Free()
{
	CGameObject::Free();
}

void CBlow::Set_Parent(const Engine::_mat& matParent)
{
	m_pTransformCom->Set_ParentMatrix(matParent);
}

HRESULT CBlow::Add_Component(void* pArg)
{
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CDynamicMesh
	CComponent* pComponent = nullptr;
	pComponent = m_pMesh = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"VacuumBlow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshBlow", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Gradient"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Texture2
	pComponent = m_pTextureCom2 = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture2", pComponent);

	//m_pTransformCom->Set_Scale(0.01f);


	return S_OK;
}


HRESULT CBlow::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	if (m_bUp)
		pEffect->SetFloat("g_TimeDelta", m_fTimeDelta);
	else
		pEffect->SetFloat("g_TimeDelta", -m_fTimeDelta);

	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 7);
	m_pTextureCom2->Set_Texture(pEffect, "g_Add2Texture", 38);
	m_pTextureCom2->Set_Texture(pEffect, "g_Add3Texture", 15);
	return S_OK;
}

void CBlow::Play_Sound()
{
	_matrix matWorld = *m_pTransformCom->Get_WorldMatrix();
	_vec3 vPos = _vec3(matWorld._41, matWorld._42, matWorld._43);

	NULL_CHECK_RETURN(m_pCodyTransform,);
	NULL_CHECK_RETURN(m_pMayTransform,);

	_float3 vDis1 = m_pCodyTransform->Get_Position() - vPos;
	_float3 vDis2 = m_pMayTransform->Get_Position() - vPos;

	_float fDis1 = D3DXVec3Length(&vDis1);
	_float fDis2 = D3DXVec3Length(&vDis2);

	if (fDis2 <= 35.0f || fDis1 <= 35.0f)
	{
		if (fDis1 < fDis2)
			m_fVolume = fDis1;
		else
			m_fVolume = fDis2;

		_float fSound = g_fEffecctSound * (1 - m_fVolume / 35.0f);
		CSoundMgr::GetInstance()->PlaySoundW(L"Blow.wav", m_eChannelID, fSound);
		CSoundMgr::GetInstance()->Set_Volum(fSound, m_eChannelID);
	}
	else
		CSoundMgr::GetInstance()->StopSound(m_eChannelID);
}


