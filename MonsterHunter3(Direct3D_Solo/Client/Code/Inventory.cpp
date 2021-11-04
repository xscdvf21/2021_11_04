#include "stdafx.h"
#include "Inventory.h"
#include "Player.h"
#include "HPUI.h"
#include "HP_Potion.h"
#include "SP_Potion.h"
#include "Export_Fucntion.h"


CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	m_CursorfX = 524.f;
	m_CursorfY = 161.f;

	return S_OK;
}

_int CInventory::Update_Object(const _float & TimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();

	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));


	if (nullptr == pPlayer)
		return E_FAIL;

	m_bInvenSee = pPlayer->Get_Inventory();

	if (true == m_bInvenSee)
		Render_KeyInput();
	else if(false == m_bInvenSee)
		m_iInvenIndex = 0;


	if (m_vecInven.empty())
	{

		CManagement*	pManagement = CManagement::GetInstance();

		CGameObject*	pPotion = pManagement->Get_GameObject(L"GameLogic", L"HP_Potion");
		CGameObject*	pSP_Potion = pManagement->Get_GameObject(L"GameLogic", L"SP_Potion");

		if (nullptr == pPotion)
			return 0;

		m_vecInven.push_back(pPotion);
		m_vecInven.push_back(pSP_Potion);


	}
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	Engine::CGameObject::Update_Object(TimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;

}

void CInventory::Render_Object()
{

	if (!m_bInvenSee)
		return;


	Render_InvenFrame();
	Render_InvenHP();
	Render_InvenSP();
	Render_Cursor();

}

HRESULT CInventory::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom[INVEN_FRAME] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_UI_Invectory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);


	// Texture
	pComponent = m_pTextureCom[INVEN_HP] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_UI_Item2Test"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture1", pComponent);

	// Texture
	pComponent = m_pTextureCom[INVEN_SP] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_UI_Stemina"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture2", pComponent);


	// Texture
	pComponent = m_pTextureCom[INVEN_CURSOR] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_UI_Cursor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture3", pComponent);
	

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	return S_OK;
}

void CInventory::Render_KeyInput()
{

	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return;


	CHPUI*			pHPUI = dynamic_cast<CHPUI*>(pManagement->Get_GameObject(L"UI", L"CHPUI"));


	if (m_iInvenIndex > 1)
	{
		m_iInvenIndex = 0;
	}
	else if (m_iInvenIndex < 0)
	{
		m_iInvenIndex = 0;
	}

	if (Engine::Key_Down(DIK_RIGHT))
	{
		m_iInvenIndex++;
	}
	else if (Engine::Key_Down(DIK_LEFT))
	{
		m_iInvenIndex--;
	}

	if (Engine::Key_Down(DIK_RETURN))
	{
		_uint iCountTemp = m_vecInven[m_iInvenIndex]->Get_MaxPotionCount();

		if (iCountTemp < 5)
		{
			return;
		}
		else
		{

			iCountTemp -= 5;

			if (m_iInvenIndex == 0)
			{

				CGameObject*	pHPObj = dynamic_cast<CHP_Potion*>(pManagement->Get_GameObject(L"GameLogic", L"HP_Potion"));
				_uint Temp = 4;
				pHPObj->Set_PotionCount(Temp);
				m_iSelectPotionCount = 5;
			}
			else if (m_iInvenIndex == 1)
			{
				CGameObject*	pSPObj = dynamic_cast<CSP_Potion*>(pManagement->Get_GameObject(L"GameLogic", L"SP_Potion"));
				_uint Temp = 3;
				pSPObj->Set_PotionCount(Temp);
				m_iSelectSPCount = 5;

			}
			pHPUI->Set_ItemSelect(m_iInvenIndex);
			m_vecInven[m_iInvenIndex]->Set_MaxPotionCount(iCountTemp);
		}
	}
	

	switch (m_iInvenIndex)
	{
	case 0:
		m_CursorfX = 524.f;
		m_CursorfY = 161.f;
		break;
	case 1:
		m_CursorfX = 560.f;
		m_CursorfY = 161.f;
		break;
	}
}

void CInventory::Render_InvenFrame()
{
	m_fX = 650.f;
	m_fY = 180.f;

	m_fSizeX = 300.f;
	m_fSizeY = 200.f;

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



	m_pTextureCom[INVEN_FRAME]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CInventory::Render_InvenHP()
{
	_uint iCountTemp = m_vecInven[0]->Get_MaxPotionCount();

	if (iCountTemp == 0)
		return;

	m_fX = 524.f; //첫번째 칸 포스값.
	m_fY = 161.f;

	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



	m_pTextureCom[INVEN_HP]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


}

void CInventory::Render_InvenSP()
{
	_uint iCountTemp = m_vecInven[1]->Get_MaxPotionCount();

	if (iCountTemp == 0)
		return;


	m_fX = 560.f;
	m_fY = 161.f;

	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



	m_pTextureCom[INVEN_SP]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

void CInventory::Render_Cursor()
{
	m_fX = m_CursorfX; //첫번째 칸 포스값.
	m_fY = m_CursorfY;

	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



	m_pTextureCom[INVEN_CURSOR]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory*	pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CInventory::Free(void)
{
	Engine::CGameObject::Free();
}
