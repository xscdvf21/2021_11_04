#include "stdafx.h"
#include "ImageUI.h"
#include "Texture.h"
#include "RcScreen.h"
#include "RcTex.h"
#include "Export_Function.h"




CImageUI::CImageUI(LPDIRECT3DDEVICE9 pDevice)
	: CUI(pDevice)
{
	Set_Color(_vec4{ 255.f,255.f,255.f,255.f });
	m_pVIBufferCom = nullptr;

}

CImageUI::CImageUI(const CImageUI & other)
	: CUI(other)
{
	m_pTextureCom = other.m_pTextureCom;
	m_pTransformCom = other.m_pTransformCom;
	m_pVIBufferCom = other.m_pVIBufferCom;
	m_bChangeStart_Visible = false;
	m_bChangeStart_Pos = false;
	m_bChangeStart_Size = false;
	m_bChangeStart = false;
	m_fTime = 0.f;
	m_fCurTime = 0.f;
	Set_Color(_vec4{ 255.f, 255.f, 255.f, 255.f });
	m_eRenderID = other.m_eRenderID;

}
HRESULT CImageUI::Ready_Object(void* pArg)
{
	m_pVIBufferCom = Engine::CRcScreen::Create(m_pGraphicDev, 0, 0, WINCX, WINCY);

	//m_pVIBufferCom = std::static_pointer_cast<Engine::CRectTexVIBuffer>(Engine::CRectTexVIBuffer::Create(m_pDevice, true));
	m_eRenderID = RENDER_UI;

	Add_Component(pArg);
	return S_OK;
}

Engine::_int CImageUI::Update_Object(const _float& TimeDelta)
{
	_uint iEvent = OBJ_NOEVENT;
	if (Is_Dead())
		return OBJ_DEAD;
	iEvent = CUI::Update_Object(TimeDelta);
	Change_Color(TimeDelta);
	Change_Size(TimeDelta);
	Change_Pos(TimeDelta);
	if (m_pTransformCom)
		m_pTransformCom->Update_Component(TimeDelta);

	if (m_bChangeStart_Visible)
	{
		m_fTime_Visible -= TimeDelta;
		if (m_fTime_Visible < 0)
		{
			this->Set_Visible(false);
			m_bChangeStart_Visible = false;
		}
	}

	if (m_bAni)
	{
		++m_iAniIndex;// = (_int)((m_iAniX * m_iAniY) * m_fAniSpeed);
		if (m_iAniIndex >= (m_iAniX * m_iAniY))
			m_iAniIndex = 0;
	}

	return iEvent;
}

Engine::_int CImageUI::LateUpdate_Object(const _float& TimeDelta)
{
	_uint iEvent = OBJ_NOEVENT;

	m_fTimeDelta += TimeDelta;
	m_fCoolDeltaTime = TimeDelta;

	if (Is_Visible())
	{
		Engine::Add_RenderGroup(m_eRenderID, this);
	}

	return iEvent;
}

void CImageUI::Render_Object(const _int& iIndex /*= 0*/)
{
	if (!Is_Visible())
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (!pEffect) return;
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	pEffect->BeginPass(m_iPass);

	if (m_pVIBufferCom)
		m_pVIBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CImageUI::Ready_GameObject(void* pArg, _bool bCheck)
{
	if (!bCheck)
		m_pVIBufferCom = Engine::CRcScreen::Create(m_pGraphicDev, 0, 0, WINCX, WINCY);
	else
		m_pVIBufferCom = Engine::CRcTex::Create(m_pGraphicDev, true);
	m_bOrth = bCheck;
	Add_Component(pArg);
	return S_OK;
}

CImageUI*  CImageUI::Create(LPDIRECT3DDEVICE9 pDevice, void* pArg, _bool bCheck)
{
	CImageUI* pInstance = new CImageUI(pDevice);
	if (FAILED(pInstance->Ready_GameObject(pArg, bCheck)))
	{
		Safe_Delete(pInstance);
		pInstance = nullptr;
	}
	return pInstance;
}

HRESULT CImageUI::Add_Component(void* pArg)
{
	CComponent* pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_eSceneID = tTemp.eSceneID;
	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_vec3{ 1280.f, 720.f, 0.f });
	m_pTransformCom->Set_Pos(_vec3{ 0.f, 0.f, 0.f });

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CImageUI::Set_Image(_wstr TextureName)
{
	if (m_pTextureCom)
	{
		Safe_Release(m_pTextureCom);
	}
	m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(m_eSceneID, TextureName.c_str()));

	return S_OK;
}

void CImageUI::Set_Color(_float Red, _float Green, _float Blue, _float Alpha)
{
	if (Red > 255)
		Red = 255;
	if (Red <= 0)
		Red = 0;
	if (Green > 255)
		Green = 255;
	if (Green <= 0)
		Green = 0;
	if (Blue > 255)
		Blue = 255;
	if (Blue <= 0)
		Blue = 0;
	if (Alpha > 255)
		Alpha = 255;
	if (Alpha <= 0)
		Alpha = 0;
	m_vStartColor = { (_float)Red,(_float)Green, (_float)Blue, (_float)Alpha };

	m_vColor = _vec4{ Red ,Green ,Blue,Alpha };
}
void CImageUI::Set_Color(_vec4 vColor)
{

	if (vColor.x > 255)
		vColor.x = 255;
	if (vColor.x <= 0)
		vColor.x = 0;
	if (vColor.y > 255)
		vColor.y = 255;
	if (vColor.y <= 0)
		vColor.y = 0;
	if (vColor.z > 255)
		vColor.z = 255;
	if (vColor.z <= 0)
		vColor.z = 0;
	if (vColor.w > 255)
		vColor.w = 255;
	if (vColor.w <= 0)
		vColor.w = 0;

	m_vStartColor = { (_float)vColor.x,(_float)vColor.y, (_float)vColor.z, (_float)vColor.w };
	m_vColor = vColor;
}

void CImageUI::Set_Pos(const _vec3 & vPos)
{
	m_vFlatPos = vPos;
	_vec3 vScale = _vec3(m_pTransformCom->Get_Scale().x * 0.5f, -m_pTransformCom->Get_Scale().y * 0.5f, 0.f);
	vScale += vPos;
	vScale += _vec3(-WINCX * 0.5f, WINCY * 0.5f, 0.f);
	m_pTransformCom->Set_Pos(vScale);
}

void CImageUI::Set_AddPos(const _vec3 & vPos)
{
	m_vStartPos = vPos;
	m_vEndPos = vPos;
	m_pTransformCom->Set_AddPos(vPos);
}

void CImageUI::Set_RoT(ROTATION eType, const _float & fAngle)
{
	m_pTransformCom->Rotation(eType, fAngle);
}

void CImageUI::Set_Dir(ROTATION eType, const _float & fAngle)
{
	_vec3 vDir;
	switch (eType)
	{
	case Engine::ROT_X:
		vDir = { 1.f,0.f,0.f };
		break;
	case Engine::ROT_Y:
		vDir = { 0.f,1.f,0.f };
		break;
	case Engine::ROT_Z:
		vDir = { 0.f,0.f,1.f };
		break;
	default:
		return;
		break;
	}
	m_pTransformCom->Set_Angle(vDir*fAngle);
}

_vec3 CImageUI::Get_Pos()
{
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vScale = _vec3(m_pTransformCom->Get_Scale().x * 0.5f, -m_pTransformCom->Get_Scale().y * 0.5f, 0.f);
	vPos += _vec3(WINCX * 0.5f, -WINCY * 0.5f, 0.f);
	vPos -= vScale;

	return vPos;
}

void CImageUI::Set_Size(const _vec3 & vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

void CImageUI::Change_Color_To(_float Time, _float Red, _float Green, _float Blue, _float Alpha)
{
	m_bChangeStart = true;
	m_fTime = Time;
	m_fCurTime = 0.f;
	m_vEndColor = { Red ,Green ,Blue,Alpha };
}

//RGBA
void CImageUI::Change_Color_From_To(_float Time, _vec4 vFrom, _vec4 vTo)
{
	m_bChangeStart = true;
	m_fTime = Time;
	m_fCurTime = 0.f;
	m_vEndColor = { vTo.x ,vTo.y ,vTo.z,vTo.w };
	m_vStartColor = { vFrom.x ,vFrom.y ,vFrom.z,vFrom.w };
	Set_Color(m_vStartColor);
}

void CImageUI::Change_Color(_float Time)
{

	if (m_bChangeStart)
	{
		m_fCurTime += Time;
		Set_Color(
			(((m_vStartColor.x + ((((m_vEndColor.x) - (m_vStartColor.x)) / m_fTime)*m_fCurTime)))),
			(((m_vStartColor.y + ((((m_vEndColor.y) - (m_vStartColor.y)) / m_fTime)*m_fCurTime)))),
			(((m_vStartColor.z + ((((m_vEndColor.z) - (m_vStartColor.z)) / m_fTime)*m_fCurTime)))),
			(((m_vStartColor.w + ((((m_vEndColor.w) - (m_vStartColor.w)) / m_fTime)*m_fCurTime)))));
		if (m_fTime < m_fCurTime)
		{
			m_bChangeStart = false;
			m_fCurTime = 0.f;
		}
	}
}

void CImageUI::Change_Size(_float Time)
{
	if (m_bChangeStart_Size)
	{

		m_fCurTime_Size += Time;
		Set_Size
		({
			(((m_vStartSize.x + ((((m_vEndSize.x) - (m_vStartSize.x)) / m_fTime_Size)*m_fCurTime_Size)))),
			(((m_vStartSize.y + ((((m_vEndSize.y) - (m_vStartSize.y)) / m_fTime_Size)*m_fCurTime_Size)))),
			(((m_vStartSize.z + ((((m_vEndSize.z) - (m_vStartSize.z)) / m_fTime_Size)*m_fCurTime_Size))))
		});
		if (m_fTime_Size < m_fCurTime_Size)
		{
			m_bChangeStart_Size = false;
			m_fCurTime_Size = 0.f;
			Set_Size(m_vEndSize);
		}
	}
}

void CImageUI::Change_Size_To(_float Time, _vec3 vToSize)
{
	m_bChangeStart_Size = true;
	m_fTime_Size = Time;
	m_vEndSize = vToSize;
}

void CImageUI::Change_Size_From_To(_float Time, _vec3 vFromSize, _vec3 vToSize)
{
	m_bChangeStart_Size = true;
	m_fTime_Size = Time;
	m_vEndSize = vToSize;
	m_vStartSize = vFromSize;
	Set_Size(m_vStartSize);
}

void CImageUI::Change_Pos(_float Time)
{
	if (m_bChangeStart_Pos)
	{
		m_fCurTime_Pos += Time;
		Set_Pos
		({
			(((m_vStartPos.x + ((((m_vEndPos.x) - (m_vStartPos.x)) / m_fTime_Pos)*m_fCurTime_Pos)))),
			(((m_vStartPos.y + ((((m_vEndPos.y) - (m_vStartPos.y)) / m_fTime_Pos)*m_fCurTime_Pos)))),
			(((m_vStartPos.z + ((((m_vEndPos.z) - (m_vStartPos.z)) / m_fTime_Pos)*m_fCurTime_Pos))))
		});
		if (m_fTime_Pos < m_fCurTime_Pos)
		{
			m_bChangeStart_Pos = false;
			m_fCurTime_Pos = 0.f;
		}
	}
}

void CImageUI::Change_Pos_To(_float Time, _vec3 vToPos)
{
	m_bChangeStart_Pos = true;
	m_fTime_Pos = Time;
	m_vEndPos = vToPos;
}

void CImageUI::Change_Pos_From_To(_float Time, _vec3 vFromPos, _vec3 vToPos)
{
	m_bChangeStart_Pos = true;
	m_fTime_Pos = Time;
	m_vStartPos = vFromPos;

	m_vEndPos = vToPos;
	Set_Pos(m_vStartPos);
}

void CImageUI::Move_Pos(const _vec3 * pDir, const _float & fSpeed, const _float & fTimeDelta)
{
	/*m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;*/
}

void CImageUI::Visible_WhileTime(_float Time)
{
	m_fTime_Visible = Time;
	this->Set_Visible(true);
	m_bChangeStart_Visible = true;
}

void CImageUI::Set_AniState(_bool bAni, _int x, _int y, _float fSpeed /*= 1.f*/)
{
	m_iAniX = x;
	m_iAniY = y;
	m_iAniIndex = 0;
	m_bAni = true;
	m_fAniSpeed = fSpeed;
}

void CImageUI::Set_CoolTime(_float Time)
{
	m_fAmount = 0.f;
	m_fCoolTime = 1.f / Time;
}

HRESULT CImageUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_mat			matWorld, matView, matProj;

	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	if (m_bOrth)
	{
		D3DXMatrixIdentity(&matView);
		D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);
	}
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	if (m_bAni)
	{
		pEffect->SetVector("g_vAniInfo", &_vec4{ (_float)m_iAniX,(_float)m_iAniY,(_float)(m_iAniIndex % m_iAniX) ,(_float)(m_iAniIndex / m_iAniX) });
	}

	if (m_iPass == 1)
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 1);
	else if (m_iPass == 4)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 3);
		pEffect->SetFloat("g_Amount", m_fAmount);
	}
	else if (m_iPass == 6)
	{
		m_fAmount += m_fCoolDeltaTime * m_fCoolTime;
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 7);
		pEffect->SetFloat("g_Amount", m_fAmount);
	}
	else if (m_iPass == 9)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 19);
		pEffect->SetFloat("g_Amount", m_fAmount);
	}
	else if (m_iPass == 10)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 20);
		pEffect->SetFloat("g_Amount", m_fAmount);
	}
	else if (m_iPass == 11)
	{
		m_fAmount += 0.001f;
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 7);
		pEffect->SetFloat("g_Amount", m_fAmount);
	}
	else if (m_iPass == 14)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_AddTexture", 17);
		pEffect->SetFloat("g_Amount", m_fAmount);
	}



	pEffect->SetVector("g_vColor", &m_vColor);
	pEffect->SetFloat("g_TimeDelta", m_fTimeDelta);
	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_iTextureIndex);

	return S_OK;
}


void CImageUI::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	CGameObject::Free();
}

