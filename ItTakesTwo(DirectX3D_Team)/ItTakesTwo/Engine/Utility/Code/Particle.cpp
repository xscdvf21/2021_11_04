#include "Particle.h"
#include "Camera.h"
#include "Export_Function.h"
#include "..\Client\Code\CodyCamera.h"
#include "..\Client\Code\MayCamera.h"

USING(Engine)

CParticle::CParticle(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_Object(void * pArg /* = nullptr*/)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CParticle::Update_Object(const _float& TimeDelta)
{
	if (m_tParticle.fCurrentTime > m_tParticle.fLifeTime || !m_tParticle.bActive || m_bDead)
	{
		m_tParticle.bActive = false;
		return OBJ_DEAD;
	}


	if (m_pTransformCom == nullptr)
		return 0;

	D3DXVec3Normalize(&m_tParticle.vDir, &m_tParticle.vDir);

	_float ScaleX = m_tParticle.vStartSize.x + ((((m_tParticle.vEndSize.x) - (m_tParticle.vStartSize.x)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);
	_float ScaleY = m_tParticle.vStartSize.y + ((((m_tParticle.vEndSize.y) - (m_tParticle.vStartSize.y)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);
	_float ScaleZ = m_tParticle.vStartSize.z + ((((m_tParticle.vEndSize.z) - (m_tParticle.vStartSize.z)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);

	m_pTransformCom->Set_Scale(ScaleX, ScaleY, ScaleZ);



	if (!m_tParticle.bRotAdd)
	{

		if (m_iPassIndex == 24)
		{
			m_pTransformCom->Set_AngleX(m_tParticle.vRot.x);
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(360.f) * TimeDelta * 2.f);
			//m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(360.f) * TimeDelta);
		}
		else if (m_iPassIndex == 33)
		{
			m_pTransformCom->Set_AngleY(m_tParticle.vRot.y);
			m_pTransformCom->Rotation(ROT_Z, -D3DXToRadian(90.f) * TimeDelta);
		}
		else
		{
			m_pTransformCom->Set_AngleX(m_tParticle.vRot.x);
			m_pTransformCom->Set_AngleY(m_tParticle.vRot.y);
			m_pTransformCom->Set_AngleZ(m_tParticle.vRot.z);
		}
	}
	else
	{
		m_pTransformCom->Set_AngleX(m_tParticle.fRotateSpeed * m_tParticle.vRot.x * m_tParticle.fCurrentTime);
		m_pTransformCom->Set_AngleY(m_tParticle.fRotateSpeed * m_tParticle.vRot.y * m_tParticle.fCurrentTime);
		m_pTransformCom->Set_AngleZ(m_tParticle.fRotateSpeed * m_tParticle.vRot.z * m_tParticle.fCurrentTime);
	}



	// Moving
	m_tParticle.vPos += m_tParticle.vDir * m_tParticle.fVelocity /**(m_ParticleDesc.fAcc*m_ParticleDesc.fCurrentTime)*/*TimeDelta;

	if (m_tParticle.bGravity)
		m_tParticle.vPos.y -= m_tParticle.fGravityForce * m_tParticle.fCurrentTime * m_tParticle.fCurrentTime;

	m_pTransformCom->Set_Pos(m_tParticle.vPos);

	m_tParticle.fCurrentTime += TimeDelta;

	_float ColorRED = m_tParticle.vStartColor.r + ((((m_tParticle.vEndColor.r) - (m_tParticle.vStartColor.r)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);
	_float ColorGreen = m_tParticle.vStartColor.g + ((((m_tParticle.vEndColor.g) - (m_tParticle.vStartColor.g)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);
	_float ColorBlue = m_tParticle.vStartColor.b + ((((m_tParticle.vEndColor.b) - (m_tParticle.vStartColor.b)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);
	_float ScaleAlpha = m_tParticle.vStartColor.a + ((((m_tParticle.vEndColor.a) - (m_tParticle.vStartColor.a)) / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime);
	m_vColor = _vec4(ColorRED, ColorGreen, ColorBlue, ScaleAlpha);

	if (m_tParticle.bAni)
		m_tParticle.iAniIndex = (_int)((m_tParticle.iAniX*m_tParticle.iAniY / m_tParticle.fLifeTime)*m_tParticle.fCurrentTime*m_tParticle.fAniSpeed);


	if (m_tParticle.bActive)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);
	Engine::CGameObject::Update_Object(TimeDelta);

	return OBJ_NOEVENT;
}

_int CParticle::LateUpdate_Object(const _float& TimeDelta)
{

	return _int();
}

void CParticle::Render_Object(const _int& iIndex)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();


	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);


	if (m_eMode != MESH)
	{
		pEffect->BeginPass(m_iPassIndex);
		pEffect->CommitChanges();
		m_pBufferCom->Render_Buffer();
		pEffect->EndPass();
	}
	else
		m_pMeshCom->Render_Meshes(pEffect, m_iPassIndex);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CParticle::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Resource(0, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(0, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CParticle::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	if (m_tParticle.bBill)
		pEffect->SetMatrix("g_matWorld", &Is_Billboarding(0));
	else
		pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetBool("g_bActive", m_tParticle.bActive);


	if (m_tParticle.bAni)
	{
		pEffect->SetVector("g_vAniInfo", &_vec4{ (_float)m_tParticle.iAniX,(_float)m_tParticle.iAniY,(_float)(m_tParticle.iAniIndex%m_tParticle.iAniX) ,(_float)(m_tParticle.iAniIndex / m_tParticle.iAniX) });
	}

	// Add Texture
	SetUp_AddTexture(pEffect);

	pEffect->SetVector("g_vColor", &m_vColor);
	pEffect->SetFloat("g_TimeDelta", m_tParticle.fCurrentTime);
	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_iTextureIndex);

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}

void CParticle::SetUp_AddTexture(LPD3DXEFFECT & pEffect)
{
	switch (m_iPassIndex)
	{
	case 6:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 3);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 4);
		return;

	case 7:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 2);
		return;

	case 8:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 7);
		return;

	case 9:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 2);
		return;

	case 10:
	{
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 12);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 13);
		m_pTextureCom->Set_Texture(pEffect, "g_Add3Texture", 14);
		m_fAmount += 0.03f;
		pEffect->SetFloat("g_Amount", m_fAmount);
	}
	return;

	case 11:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 10);
		return;

	case 12:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 16);
		return;

	case 14:
	{
		m_fAmount += 0.07f;
		pEffect->SetFloat("g_Amount", m_fAmount);

		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 17);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 19);
		m_pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 18);
		m_pTextureCom->Set_Texture(pEffect, "g_BurnTexture", 20);

		m_fDissolve += 0.03f;
		if (m_fDissolve >= 3.14f / 2.f)
			m_fDissolve = 3.14f / 2.f;

		_float fSin = sinf(m_fDissolve);
		pEffect->SetFloat("g_fDissolve", fSin);
	}
	return;

	case 15:
	{
		m_tParticle.fAmount -= 0.02f;
		pEffect->SetFloat("g_Amount", m_tParticle.fAmount);

		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 10);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 22);
	}
	return;

	case 16:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 25);
		return;

	case 17:
		pEffect->SetFloat("g_Amount", m_tParticle.fAmount);
		pEffect->SetFloat("g_fDissolve", m_tParticle.fAniSpeed);
		return;

	case 18:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
		return;

	case 19:
	{
		m_tShader.m_fRimDist = 0.1f;
		m_tShader.m_iRimPower = 1;
		m_tShader.m_vRimColor = _vec4(0.3f, 0.4f, 0.8f, 0.5f);

		pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
		pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
		pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);

		_matrix matViewInv;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matViewInv);
		D3DXMatrixInverse(&matViewInv, nullptr, &matViewInv);
		pEffect->SetVector("g_vCamPos", &_vec4(matViewInv._41, matViewInv._42, matViewInv._43, 1.f));
	}
	return;

	case 20:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 34);
		return;

	case 21:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 37);
		return;

	case 22:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
		return;

	case 23:
		pEffect->SetFloat("g_Amount", m_tParticle.fAmount);
		pEffect->SetFloat("g_fDissolve", m_tParticle.fAniSpeed);
		return;

	case 24:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
		return;

	case 26:
	{
		m_fAmount += 0.07f;
		pEffect->SetFloat("g_Amount", m_fAmount);

		m_pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 22);
		m_pTextureCom->Set_Texture(pEffect, "g_BurnTexture", 47);

		m_fDissolve += 0.03f;
		if (m_fDissolve >= 3.14f / 2.f)
			m_fDissolve = 3.14f / 2.f;

		_float fSin = sinf(m_fDissolve);
		pEffect->SetFloat("g_fDissolve", fSin);
	}
	return;

	case 27:
		pEffect->SetFloat("g_Amount", m_tParticle.fAmount);
		return;

	case 28:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 42);
		pEffect->SetFloat("g_Amount", m_fAmount);
		return;

	case 30:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 18);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 9);
		return;

	case 31:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
		return;

	case 33:
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 9);
		return;

	default:
		return;
	}

}

void Engine::CParticle::Set_AniState(PARTICLE& pParticle, _int x, _int y, _float fSpeed /*= 1.f*/)
{
	pParticle.iAniX = x;
	pParticle.iAniY = y;
	pParticle.iAniIndex = 0;
	pParticle.bAni = true;
	pParticle.fAniSpeed = fSpeed;

	//pParticle.iPass = ;
}
void CParticle::Set_SpawnData(PARTICLE & tDesc)
{
	//Reset_Particle(tDesc);

	m_tParticle = tDesc;
}

void CParticle::Change_Pass(_uint PassIndex)
{
	m_iPassIndex = PassIndex;
}

void CParticle::Change_Texture(_uint textureIndex)
{
	m_iTextureIndex = textureIndex;
}

void Engine::CParticle::Set_ParticlePos(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Update_Component(0.f);
}

CParticle* Engine::CParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	auto* pInstance = new CParticle(pDevice);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}


void Engine::CParticle::Set_Mesh(const _wstr& tagMesh)
{
	//m_pMeshCom = Engine::Clone_Resource(m_iSceneIndex, tagMesh.c_str())->Is<CStaticMesh>();
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Get_Resource(m_iSceneIndex, tagMesh.c_str()));
	NULL_CHECK_RETURN(pComponent, );
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	m_eMode = MESH;
	m_pTransformCom->Set_Scale(0.f, 0.f, 0.f);
}

_mat CParticle::Is_Billboarding(const _int& iIndex)
{
	_matrix		matBill, matView, matWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vec3));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vec3));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vec3));

	D3DXMatrixInverse(&matBill, NULL, &matBill);


	m_pTransformCom->Get_WorldMatrix(&matWorld);

	return matBill * matWorld;
}

void Engine::CParticle::Reset_Particle(PARTICLE& pParticle)
{
	//ZeroMemory(&pParticle, sizeof(PARTICLE));
	pParticle.fVelocity = 1.f;
	pParticle.vPos = { 0.f,0.f,0.f };
	pParticle.vDir = { 0.f,0.f,0.f };
	pParticle.tagTextrue = L"";
	pParticle.bActive = false;
	pParticle.fLifeTime = 5.f;
	pParticle.fCurrentTime = 0;
	pParticle.fCurSize = 0.f;
	pParticle.fAcc = 0.f;
	pParticle.fEndSize = 0.f;
	pParticle.fStartSize = 0.f;
	pParticle.vStartSize = { 0.f,0.f,0.f };
	pParticle.vEndSize = { 0.f,0.f,0.f };
	pParticle.vStartColor = { 0.f,0.f,0.f,1.f };
	pParticle.vEndColor = { 1.f,1.f,1.f,1.f };
	pParticle.vCurColor = { 0.f,0.f,0.f,0.f };
	pParticle.bBill = true;
	pParticle.fAmount = 0.f;
}

void  Engine::CParticle::Free()
{
	CGameObject::Free();
}
