#include "stdafx.h"
#include "HPUI.h"
#include "Boss.h"
#include "SoundMgr.h"
#include "SP_Potion.h"
#include "HP_Potion.h"
#include "Inventory.h"
#include "Export_Fucntion.h"


CHPUI::CHPUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CHPUI::~CHPUI(void)
{
}



HRESULT CHPUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;

}

_int CHPUI::Update_Object(const _float & TimeDelta)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	Engine::CGameObject::Update_Object(TimeDelta);

	CManagement*	pManagement = CManagement::GetInstance();

	CBoss*			pBoss = dynamic_cast<CBoss*>(pManagement->Get_GameObject(L"GameLogic", L"Boss"));

	m_bVictory = pBoss->Get_Victory();

	if (true == m_bVictory)
	{
		m_fVictory += TimeDelta;
		
	}

	if (m_iSelectItem < 0)
	{
		m_iSelectItem = 1;
	}
	else if (m_iSelectItem > 1)
	{
		m_iSelectItem = 0;
	}
	if (Engine::Key_Down(DIK_Q))
	{
		m_iSelectItem--;
	}
	else if (Engine::Key_Down(DIK_E))
	{
		m_iSelectItem++;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

void CHPUI::Render_Object()
{
	//HP바 프레임
	Render_UI_Hp_Frame();
	Render_UI_HP_Bar();
	Render_UI_Stemina_Bar();
	//아이템 셀렉 프레임.
	Render_UI_Item_Frame();

	if (m_bVictory && m_fVictory <= 5.f)
	{
		if (false == m_bVictorySound)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::EFFECT);
			CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Hurt4.wav", CSoundMgr::CHANNELID::EFFECT);

			m_bVictorySound = true;
		}

		Rneder_UI_Victory();

	}

	
	//아이템 물약


	switch (m_iSelectItem)
	{
	case 0:
		Render_UI_Item_HP();
		break;
	case 1:
		Render_UI_Item_SP();
		break;
	}




}

HRESULT CHPUI::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	// Texture
	pComponent = m_pTextureCom[UIID_FRAME] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_Frame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture1", pComponent);

	pComponent = m_pTextureCom[UIID_ITEMBACK] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_Item_Back"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture2", pComponent);

	pComponent = m_pTextureCom[UIID_HP_POTION] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_UI_Item2Test"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture3", pComponent);

	pComponent = m_pTextureCom[UIID_ITEM_HPEND] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_UI_Item2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture4", pComponent);

	pComponent = m_pTextureCom[UIID_HPBAR] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_Hp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture5", pComponent);

	pComponent = m_pTextureCom[UIID_STEMINABAR] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_UI_Stemina"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture6", pComponent);

	pComponent = m_pTextureCom[UIID_VICTORY] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_UI_Victory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture7", pComponent);

	pComponent = m_pTextureCom[UIID_SP_POTION] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_UI_Stemina"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture8", pComponent);


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

void CHPUI::Render_UI_Hp_Frame()
{

	m_fX = 400.f;
	m_fY = 180.f;

	m_fSizeX = 800.f;
	m_fSizeY = 350.f;


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



	m_pTextureCom[UIID_FRAME]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}
void CHPUI::Render_UI_HP_Bar()
{

	CManagement* pManagement = CManagement::GetInstance();

	CGameObject*	pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

	m_tINFOPLAYER = pPlayer->Get_INFOPLAYER();



	//m_fX = 405.f;
	m_fX = 305.f - ((540.f / 2) * (((_float)m_tINFOPLAYER.iMaxHP - (_float)m_tINFOPLAYER.iHP) / (_float)m_tINFOPLAYER.iMaxHP));
	m_fY = 30.f;




	//m_fSizeX = 540.f;
	m_fSizeX = 540.f * ((_float)m_tINFOPLAYER.iHP / (_float)m_tINFOPLAYER.iMaxHP);
	m_fSizeY = 7.f;

	
	//_float3(1.f - ((265.f / 2) * (((float)m_tObjInfo.iMaxHp - (float)m_tObjInfo.iHp) / (float)m_tObjInfo.iMaxHp))

	//pCamera->Set_Ortho_For_UI(_float3(265.f * ((float)m_tObjInfo.iHp / (float)m_tObjInfo.iMaxHp), 10.f, 1.f),
	//	_float3(1.f - ((265.f / 2) * (((float)m_tObjInfo.iMaxHp - (float)m_tObjInfo.iHp) / (float)m_tObjInfo.iMaxHp)), 240.f, 1.f));




	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX  * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



	m_pTextureCom[UIID_HPBAR]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


}
void CHPUI::Render_UI_Item_Frame()
{

	m_fX = 700.f;
	m_fY = 550.f;

	m_fSizeX = 200.f;
	m_fSizeY = 100.f;


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


	m_pTextureCom[UIID_ITEMBACK]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CHPUI::Render_UI_Item_HP()
{

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	CInventory*		pInven = dynamic_cast<CInventory*>(pManagement->Get_GameObject(L"GameLogic", L"Inventory"));
	CGameObject*	pHP_Postion = dynamic_cast<CHP_Potion*>(pManagement->Get_GameObject(L"GameLogic", L"HP_Potion"));

	/*m_iItemCount = pInven->Get_ITEMCount();*/
	m_iItemCount = pHP_Postion->Get_PotionCount();
	m_ItemType = UIID_HP_POTION;

	if (m_iItemCount <= 0)
		return;

	wsprintf(m_szFPS, L"%d", m_iItemCount);

	m_ItemType = UIID_HP_POTION;

	Engine::Render_Font(L"Font_Jinji", m_szFPS, &_vec2(710.f, 560.f), D3DXCOLOR(0.8f, 0.12f, 0.25f, 1.f));

	m_fX = 700.f;
	m_fY = 550.f;

	m_fSizeX = 40.f;
	m_fSizeY = 40.f;


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


	m_pTextureCom[UIID_HP_POTION]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CHPUI::Render_UI_Item_SP()
{

	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return;

	CInventory*		pInven = dynamic_cast<CInventory*>(pManagement->Get_GameObject(L"GameLogic", L"Inventory"));

	CGameObject*	pSP_Postion = dynamic_cast<CSP_Potion*>(pManagement->Get_GameObject(L"GameLogic", L"SP_Potion"));

	//m_iSPCount = pInven->Get_SPCount();

	m_iSPCount = pSP_Postion->Get_PotionCount();
	m_ItemType = UIID_SP_POTION;
	if (m_iSPCount <= 0)
		return;

	wsprintf(m_szFPS, L"%d", m_iSPCount);

	m_ItemType = UIID_SP_POTION;

	Engine::Render_Font(L"Font_Jinji", m_szFPS, &_vec2(710.f, 560.f), D3DXCOLOR(0.8f, 0.12f, 0.25f, 1.f));


	m_fX = 700.f;
	m_fY = 550.f;

	m_fSizeX = 40.f;
	m_fSizeY = 40.f;


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


	m_pTextureCom[UIID_SP_POTION]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

void CHPUI::Render_UI_Stemina_Bar()
{

	m_fX = 305.f - ((540.f / 2) * (((_float)100 - (_float)m_tINFOPLAYER.fSp) / (_float)100));
	m_fY = 41.f;




	//m_fSizeX = 540.f;
	m_fSizeX = 540.f * ((_float)m_tINFOPLAYER.fSp / (_float)100);
	m_fSizeY = 7.f;



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



	m_pTextureCom[UIID_STEMINABAR]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CHPUI::Rneder_UI_Victory()
{



	m_fX = 400.f;
	m_fY = 300.f;

	m_fSizeX = 400.f;
	m_fSizeY = 300.f;


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


	m_pTextureCom[UIID_VICTORY]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

CHPUI * CHPUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHPUI*	pInstance = new CHPUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CHPUI::Free(void)
{

	Engine::CGameObject::Free();
}
