#include "stdafx.h"
#include "UnitSkill.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"
#include "StartBeforeKeyMgr.h"
#include "SoundMgr.h"
#include "Key_Manager.h"
#include "Effect.h"
#include "Animation_IMP.h"
#include "NormalEffect.h"

CUnitSkill::CUnitSkill()
{
}


CUnitSkill::~CUnitSkill()
{
}

HRESULT CUnitSkill::Ready_GameObject()
{
	m_iSkillOne = 50;
	m_iSkillTwo = 120;
	m_pTextureKey = L"UnitSkill";
	return S_OK;
}

int CUnitSkill::Update_GameObject()
{
	Key_Check();
	return 0;
}

void CUnitSkill::LateUpdate_GameObject()
{
}

void CUnitSkill::Render_GameObject()
{
	if (static_iAllCheck == 3)
	{
		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, m_iSceneNum);
		const TEXINFO*	pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, 2);
		const TEXINFO*	pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, 3);


		const TEXINFO*	pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 4);
		if (nullptr == pTexInfo)
		{
			ERR_MSG(L"UnitInfo.Cpp, TexInfo가 오류.");
			return;
		}

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		_matrix matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 300.f, 500.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 600.f, 450.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		if (m_iSceneNum == 0)
		{
			TCHAR szChar5[64] = L"";
			swprintf_s(szChar5, L"체력을 1/3 만큼 회복합니다. ");
			D3DXMatrixTranslation(&matTrans, 25.f, 30.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szChar5, lstrlen(szChar5), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_iSceneNum == 1)
		{
			TCHAR szChar6[64] = L"";
			swprintf_s(szChar6, L"체력을 전부 회복합니다.");
			D3DXMatrixTranslation(&matTrans, 25.f, 30.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szChar6, lstrlen(szChar6), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"정신력 (EN)");
		D3DXMatrixTranslation(&matTrans, 630.f, 480.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar2, L"%d / %d",m_tInfo.iEnergy, m_tInfo.iMaxEnergy);
		D3DXMatrixTranslation(&matTrans, 630.f, 510.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar2, lstrlen(szChar2), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szChar3[64] = L"";
		swprintf_s(szChar3, L"근성     %d", m_iSkillOne);
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar3, lstrlen(szChar3), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar4[64] = L"";
		swprintf_s(szChar4, L"초 근성     %d", m_iSkillTwo);
		D3DXMatrixTranslation(&matTrans, 350.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar4, lstrlen(szChar4), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CUnitSkill::Release_GameObject()
{
}

void CUnitSkill::Key_Check()
{

	if (CStartBeforeKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{

		m_iSceneNum++;
		if (m_iSceneNum > 1)
		{
			m_iSceneNum = 0;
		}
	}

	if (CStartBeforeKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{

		m_iSceneNum--;
		if (m_iSceneNum < 0)
		{
			m_iSceneNum = 1;
		}
	}

	if (m_bUnitSkillCheck && static_iAllCheck == 3 && m_iSceneNum == 0 && CKey_Manager::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{

			if (!lstrcmp(m_pTextureUnit, _rList->Get_TextureUnitKey()))
			{
				m_tInfo.iMaxHp = _rList->Get_Info().iMaxHp;
				m_tInfo.iHp = _rList->Get_Info().iHp;

				if (0 > m_tInfo.iEnergy - m_iSkillOne)
				{
					_rList->Set_UnitInfo(m_tInfo);
					_rList->Set_static_iAllCheck(0);
					_rList->Set_iSceneNum(0);

					for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
					{
						_rList->Set_static_iAllCheck(0);
						_rList->Set_iSceneNum(0);
					}
					CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);
					break;
				}

				if (m_tInfo.iMaxHp == m_tInfo.iHp)
				{
					_rList->Set_UnitInfo(m_tInfo);
					_rList->Set_static_iAllCheck(0);
					_rList->Set_iSceneNum(0);

					for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
					{
						_rList->Set_static_iAllCheck(0);
						_rList->Set_iSceneNum(0);
					}
					CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);
					break;
				}
				int iTempHealHp = m_tInfo.iMaxHp / 3;

				if (m_tInfo.iMaxHp <= iTempHealHp + m_tInfo.iHp)
				{
					m_tInfo.iHp = m_tInfo.iMaxHp;
					m_tInfo.iEnergy -= m_iSkillOne;
				}
				else
				{
					m_tInfo.iHp = m_tInfo.iHp + iTempHealHp;
					m_tInfo.iEnergy -= m_iSkillOne;
				}
				_rList->Set_UnitInfo(m_tInfo);
				_rList->Set_static_iAllCheck(0);
				_rList->Set_iSceneNum(0);

				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
				{
					_rList->Set_static_iAllCheck(0);
					_rList->Set_iSceneNum(0);
				}
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);

				CEffect_IMP * pAni = new CAnimation_IMP;
				pAni->Ready_Effect_IMP();
				dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"UnitHeal", { 0.f, 9.f });
				_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
				CEffect* pNormal = new CNormalEffect(pAni);
				pNormal->Ready_GameObject();
				pNormal->Set_Pos(m_tInfo.vPos);
				CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Heal.wav", CSoundMgr::MONSTER);
			}
		}

		m_bUnitMenuCheck = false;
		static_iAllCheck = 0;
	}

	if (m_bUnitSkillCheck && static_iAllCheck == 3 && m_iSceneNum == 1 && CKey_Manager::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (!lstrcmp(m_pTextureUnit, _rList->Get_TextureUnitKey()))
			{
				m_tInfo.iMaxHp = _rList->Get_Info().iMaxHp;
				m_tInfo.iHp = _rList->Get_Info().iHp;

				if (0 > m_tInfo.iEnergy - m_iSkillTwo)
				{
					_rList->Set_UnitInfo(m_tInfo);
					_rList->Set_static_iAllCheck(0);
					_rList->Set_iSceneNum(0);

					for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
					{
						_rList->Set_static_iAllCheck(0);
						_rList->Set_iSceneNum(0);
					}
					CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);
					break;
				}
			
				if (m_tInfo.iMaxHp == m_tInfo.iHp)
				{
					_rList->Set_UnitInfo(m_tInfo);
					_rList->Set_static_iAllCheck(0);
					_rList->Set_iSceneNum(0);

					for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
					{
						_rList->Set_static_iAllCheck(0);
						_rList->Set_iSceneNum(0);
					}
					CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);
					break;
				}
				m_tInfo.iHp = m_tInfo.iMaxHp;
				m_tInfo.iEnergy -= m_iSkillTwo;
			

				_rList->Set_UnitInfo(m_tInfo);
				_rList->Set_static_iAllCheck(0);
				_rList->Set_iSceneNum(0);

				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
				{
					_rList->Set_static_iAllCheck(0);
					_rList->Set_iSceneNum(0);
				}
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);

				CEffect_IMP * pAni = new CAnimation_IMP;
				pAni->Ready_Effect_IMP();
				dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"UnitHeal", { 0.f, 11.f });
				CEffect* pNormal = new CNormalEffect(pAni);
				pNormal->Ready_GameObject();
				pNormal->Set_Pos(m_tInfo.vPos);
				CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Heal.wav", CSoundMgr::MONSTER);
			}

		}

		m_bUnitMenuCheck = false;
		static_iAllCheck = 0;
	}

}
