#include "stdafx.h"
#include "ButtonUI.h"
#include "Texture.h"
#include "RcScreen.h"
#include "Logo.h"

#include "Export_Function.h"


CButtonUI::CButtonUI(LPDIRECT3DDEVICE9 pDevice)
	: CUI(pDevice)
{
	Set_Color(_vec4{ 255.f,255.f,255.f,255.f });

}

CButtonUI::CButtonUI(const CButtonUI & other)
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
	Set_Color(_vec4{ 255.f,255.f,255.f,255.f });

}
HRESULT CButtonUI::Ready_Object(void* pArg /*= nullptr*/)
{
	m_pVIBufferCom = Engine::CRcScreen::Create(m_pGraphicDev, true);

	Add_Component();
	return S_OK;
}

Engine::_int CButtonUI::Update_Object(const _float& TimeDelta)
{
	_uint iEvent = OBJ_NOEVENT;
	//if (!m_bEnd)
	//{

	//	_tchar szBuf[255] = L"", szY[255] = L"";

	//	GetPrivateProfileString(m_tagNickName.c_str(), L"PosX", L"0.f", szBuf, 255, L"../../Pos.ini");
	//	GetPrivateProfileString(m_tagNickName.c_str(), L"PosY", L"0.f", szY, 255, L"../../Pos.ini");
	//	_vec3 vPos = _vec3{ (_float)_ttof(szBuf) ,(_float)_ttof(szY),0.f };
	//	m_pTransformCom->Set_Pos(vPos);
	//	GetPrivateProfileString(m_tagNickName.c_str(), L"SizeX", L"0.f", szBuf, 255, L"../../Pos.ini");
	//	GetPrivateProfileString(m_tagNickName.c_str(), L"SizeY", L"0.f", szY, 255, L"../../Pos.ini");
	//	vPos = _vec3{ (_float)_ttof(szBuf) ,(_float)_ttof(szY),0.f };
	//	m_pTransformCom->Set_Scale(vPos);
	//}


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
	Click_Check();

	if (m_bClick)
	{
		//Engine::Change_Scene();
		//return SCENE_CHANGE;
	}
	return iEvent;
}

Engine::_int CButtonUI::LateUpdate_Object(const _float& TimeDelta)
{
	_uint iEvent = OBJ_NOEVENT;


	if (Is_Visible())
		Engine::Add_RenderGroup(RENDER_UI, this);

	return iEvent;
}

void CButtonUI::Render_Object(const _int& iIndex /*= 0*/)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (!pEffect) return;
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	if (m_bAlpha)
		pEffect->BeginPass(2);
	else
		pEffect->BeginPass(4);
	if (m_pVIBufferCom)
		m_pVIBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

CButtonUI* CButtonUI::Create(LPDIRECT3DDEVICE9 pDevice, _bool bClickable /*= true*/,_bool bEnd)
{
	CButtonUI* pInstance =new CButtonUI(pDevice);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Delete(pInstance);
		return pInstance;
	}
	pInstance->Set_Clickable(bClickable);
	pInstance->Set_End(bEnd);
	if (!bClickable)
		pInstance->Set_Alpha(false);
	return pInstance;
}

HRESULT CButtonUI::Add_Component()
{
	CComponent* pComponent = nullptr;


	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_vec3{ 1280.f, 720.f, 0.f });
	m_pTransformCom->Set_Pos(_vec3{ 0.f,0.f,0.f });

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

void CButtonUI::Click_Check()
{
	if (!m_bClickable)
		return;
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	ptMouse.y = WINCY - ptMouse.y;
	_vec3   vPos = m_pTransformCom->Get_Position();
	_vec3	vScale = m_pTransformCom->Get_Scale();
	vPos.x += WINCX >> 1;
	vPos.y += WINCY >> 1;
	RECT tButton{};

	tButton.left = (_long)(vPos.x - (vScale.x/2));
	tButton.right = (_long)(vPos.x + (vScale.x / 2));
	tButton.top = (_long)(vPos.y - (vScale.y / 2));
	tButton.bottom = (_long)(vPos.y + (vScale.y / 2));

	if (PtInRect(&tButton, ptMouse))
	{
		m_bHover = true;
		if (Engine::Key_Down(DIM_LB))
		{
			m_bClick = true;

			//Engine::SetUp_Management(CLogo::Create(SCENE_STAGE));
			return;
		}

	}
	else
	{
		m_bHover = false;
		m_bClick = false;
	}
}

HRESULT CButtonUI::Set_Image(_wstr TextureName)
{
	if (m_pTextureCom)
	{
		Safe_Release(m_pTextureCom);
	}
	m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, TextureName.c_str()));

	return S_OK;
}

void CButtonUI::Set_Color(_float Red, _float Green, _float Blue, _float Alpha)
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

		m_vColor = _vec4{ Red ,Green ,Blue,Alpha  };
	
	

}
void CButtonUI::Set_Color(_vec4 vColor)
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

void CButtonUI::Set_Pos(const _vec3 & vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CButtonUI::Set_Size(const _vec3 & vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}


void CButtonUI::Change_Color_To(_float Time, _float Red, _float Green, _float Blue, _float Alpha)
{
	m_bChangeStart = true;
	m_fTime = Time;
	m_fCurTime = 0.f;
	m_vEndColor = { Red ,Green ,Blue,Alpha };
}

//RGBA
void CButtonUI::Change_Color_From_To(_float Time, _vec4 vFrom, _vec4 vTo)
{
	m_bChangeStart = true;
	m_fTime = Time;
	m_fCurTime = 0.f;
	m_vEndColor = { vTo.x ,vTo.y ,vTo.z,vTo.w };
	m_vStartColor = { vFrom.x ,vFrom.y ,vFrom.z,vFrom.w };
	Set_Color(m_vStartColor);

}
void CButtonUI::Change_Color(_float Time)
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

void CButtonUI::Change_Size(_float Time)
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
		}


	}
}

void CButtonUI::Change_Size_To(_float Time, _vec3 vToSize)
{
	m_bChangeStart_Size = true;
	m_fTime_Size = Time;
	m_vEndSize = vToSize;
}

void CButtonUI::Change_Size_From_To(_float Time, _vec3 vFromSize, _vec3 vToSize)
{
	m_bChangeStart_Size = true;
	m_fTime_Size = Time;
	m_vEndSize = vToSize;
	m_vStartSize = vFromSize;
	Set_Size(m_vStartSize);
}

void CButtonUI::Change_Pos(_float Time)
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

void CButtonUI::Change_Pos_To(_float Time, _vec3 vToPos)
{
	m_bChangeStart_Pos = true;
	m_fTime_Pos = Time;
	m_vEndPos = vToPos;
}

void CButtonUI::Change_Pos_From_To(_float Time, _vec3 vFromPos, _vec3 vToPos)
{
	m_bChangeStart_Pos = true;
	m_fTime_Pos = Time;
	m_vEndPos = vToPos;
	m_vStartPos = vFromPos;
	Set_Pos(m_vStartPos);
}

void CButtonUI::Visible_WhileTime(_float Time)
{
	m_fTime_Visible = Time;
	this->Set_Visible(true);
	m_bChangeStart_Visible = true;
}

HRESULT CButtonUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_mat			matWorld, matView, matProj;

	matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	if (m_pTextureCom)
		m_pTextureCom->Set_Texture(pEffect, "g_BassTexture", (_uint)m_bHover);
	else
		return E_FAIL;
	pEffect->CommitChanges();
	return S_OK;
}



void CButtonUI::Free()
{

}

