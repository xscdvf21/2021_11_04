#include "stdafx.h"
#include "UnitFight.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"
#include "SoundMgr.h"
#include "Background.h"

#include "NormalEffect.h"
#include "NonAnimation_IMP.h"
#include "Animation_IMP.h"

CUnitFight::CUnitFight()
	: m_bSoundCheck(false)
	, m_bSoundCheck2(false)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CUnitFight::~CUnitFight()
{
}

HRESULT CUnitFight::Ready_GameObject()
{


	m_pTextureKey = L"UnitFight";
	
	m_fsize = 1.0f;
	m_fSpeed = 0.f;
	m_fSpeed2 = 0.f;
	m_fAngle = 0.f;
	m_fTime = 0.f;
	m_bHpCheck = false;

	m_tFrame = { 4.f, 9.f };
	return S_OK;
}

int CUnitFight::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUnitFight::LateUpdate_GameObject()
{
	FrameMove(1.f);
}

void CUnitFight::Render_GameObject()
{

	if (m_fTurnDelay == 0.f)
	{
		CGameObject*	pObj = new CBackground;
		pObj->Ready_GameObject();
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_BACKGROUND, pObj);


	}
	m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();
	if (m_fTurnDelay >= 1.5f)
	{
		if (static_iAllCheck == 8)
		{

			m_fx += 5;
			if (m_fx >= 800)
				m_fx = 0;

			//유닛 전투 씬 백 그라운드
			const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"UnitFight", L"UnitFightBackground", 0);
			const TEXINFO*	pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"UnitFight", L"UnitFightBackground", 0);

			//유닛 전투 씬 유닛 인포 적을 창
			const TEXINFO*	pTexInfo3 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitFightReady", 0);
			//유닛 전투 씬 유닛 대사와 사진 창.
			const TEXINFO*	pTexInfo10 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitFightReady", 1);

			////유닛 전투 씬 체력 공격자의 체력바.
			//UnitHpPersent = m_tInfo.iHp / m_tInfo.iMaxHp * 100;
			//const TEXINFO*	pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 0);

			////유닛 전투 씬 체력 방어자의 체력 바.
			//const TEXINFO*	pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 0);
			if (nullptr == pTexInfo)
			{
				ERR_MSG(L"UnitFight.Cpp, UnitFight.png, 즉 pTexInfo가 오류.");
				return;
			}
			//유닛 전투 씬 에서 유닛.


			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
			int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

			_matrix matScale,matRotateZ, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_fx, WINCY >> 1, 0.f);

			/*matWorld = matScale * matTrans;*/

			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(0.f, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, m_fx - 800, WINCY >> 1, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &_vec3(0.f, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, 0.f, 400.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo3->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


			//전투 시 공격자  대사 넣는곳.
			D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo10->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//유닛
		/*	float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
			float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
			D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));*/

			//유닛
			for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
			{
				const TEXINFO*	pTexInfo4;
				const TEXINFO*	pTexInfo5;
				int UnitHpPersent = m_tInfo.iHp * 100 / m_tInfo.iMaxHp;
				if (UnitHpPersent >= 100)
				{
						pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 0);
				}
				else if (UnitHpPersent < 100 && UnitHpPersent >= 80)
				{
						pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 1);
				}
				else if (UnitHpPersent < 80 && UnitHpPersent >= 60)
				{
						pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 2);
				}
				else if (UnitHpPersent < 60 && UnitHpPersent >= 40)
				{
						pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 3);
				}
				else if (UnitHpPersent < 40 && UnitHpPersent >= 1)
				{
						pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 4);
				}
				else 
						pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 5);

				int MonHpPersent = m_tMonInfo.iHp * 100 / m_tMonInfo.iMaxHp;
				if (MonHpPersent >= 100)
				{
						pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 0);
				}
				else if (MonHpPersent < 100 && MonHpPersent >= 80)
				{
						pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 1);
				}
				else if (MonHpPersent < 80 && MonHpPersent >= 60)
				{
						pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 2);
				}
				else if (MonHpPersent < 60 && MonHpPersent >= 40)
				{
						pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 3);
				}
				else if (MonHpPersent < 40 && MonHpPersent >= 1)
				{
						pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 4);
				}
				else
						pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 5);





				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureUnit, L"GunDamMK"))
				{

					//유닛 전투 씬 체력 공격자의 체력바.



					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans,670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);

					//유닛 전투 씬에서 유닛 공격 이펙트.
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", m_iSceneNum);


					//공격자 그림들.
					if (m_fSpeed <= 600)
					{
						

						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						//공격자 대사.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s ", m_tInfo.szName, m_tInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f) 
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f};
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}
						if (0 == m_iSceneNum)
						{
							//float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							//float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							//D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							//D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							//matWorld = matScale * matTrans;
							//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							cout << m_iSceneNum << endl;

							if (m_fSpeed >= 600)
							{

								//몬스터
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

								//방어자 작은 사진.
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								//방어자 대사
								
								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 800.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트
								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}
									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 0);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tMonInfo.iHp -= (m_tInfo.iAttack - m_tMonInfo.iArmor);
										if(m_tMonInfo.iHp <= 0)
											m_tMonInfo.iHp = 0;
										m_bHpCheck = true;
									}


									if (m_fTime >= 3.0f)
									{

										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												if (m_tMonInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_MonsterHP(m_tMonInfo);
												break;
												
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												_rList->Set_UnitTurn(true);
												_rList->Set_Attacker(false);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureUnit, L"MazingerZ"))
				{

					//유닛 전투 씬 체력 공격자의 체력바.



					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);

					//유닛 전투 씬에서 유닛 공격 이펙트.
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", m_iSceneNum);


					//공격자 그림들.
					if (m_fSpeed <= 600)
					{


						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						//공격자 대사.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s ", m_tInfo.szName, m_tInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"MazingerZ.wav", CSoundMgr::EFFECT);

							m_bSoundCheck = true;
						}


						if (0 <= m_iSceneNum)
						{
							m_iSceneNum++;
							if (m_iSceneNum == 14)
									m_iSceneNum = 0;


							if (m_fSpeed <= 600)
							{
								D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 330.f, (WINCY >> 1) - 150.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							}

		

							if (m_fSpeed >= 600)
							{

								//몬스터
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

								//방어자 작은 사진.
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1) - 30.f, (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								//방어자 대사

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 800.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{
									D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans,  330.f, (WINCY >> 1) - 80.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트
								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}
									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 0);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tMonInfo.iHp -= (m_tInfo.iAttack - m_tMonInfo.iArmor);
										if (m_tMonInfo.iHp <= 0)
											m_tMonInfo.iHp = 0;
										m_bHpCheck = true;
									}


									if (m_fTime >= 3.0f)
									{

										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												if (m_tMonInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_MonsterHP(m_tMonInfo);
												break;

											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												_rList->Set_UnitTurn(true);
												_rList->Set_Attacker(false);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureUnit, L"Getta"))
				{

					//유닛 전투 씬 체력 공격자의 체력바.



					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);

					//유닛 전투 씬에서 유닛 공격 이펙트.
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", m_iSceneNum);


					//공격자 그림들.
					if (m_fSpeed <= 600)
					{


						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						//공격자 대사.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s ", m_tInfo.szName, m_tInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"GettaBeam.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}
						if (0 == m_iSceneNum)
						{
							//float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							//float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							//D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							//D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							//matWorld = matScale * matTrans;
							//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(90));
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 230.f + m_fSpeed, (WINCY >> 1) - 30.f, 0.f);
							matWorld = matScale  * matRotateZ * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							cout << m_iSceneNum << endl;

							if (m_fSpeed >= 600)
							{

								//몬스터
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

								//방어자 작은 사진.
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								//방어자 대사

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 800.f;
								//나가는 총알
								if (m_fSpeed2 <= 500)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(90));
									D3DXMatrixTranslation(&matTrans, -80.f + m_fSpeed2, (WINCY >> 1) - 30.f, 0.f);
									matWorld = matScale * matRotateZ* matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트
								if (m_fSpeed2 >= 500)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"GettaBeamEnd.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GettaAttack", { 0.f, 5.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) - 30.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}
									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 0);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tMonInfo.iHp -= (m_tInfo.iAttack - m_tMonInfo.iArmor);
										if (m_tMonInfo.iHp <= 0)
											m_tMonInfo.iHp = 0;
										m_bHpCheck = true;
									}


									if (m_fTime >= 3.0f)
									{

										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												if (m_tMonInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_MonsterHP(m_tMonInfo);
												break;

											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												_rList->Set_UnitTurn(true);
												_rList->Set_Attacker(false);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureUnit, L"Aphrodite"))
				{
					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);

					//유닛 전투 씬에서 유닛 공격 이펙트.
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", m_iSceneNum);


					//공격자 그림들.
					if (m_fSpeed <= 600)
					{


						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						//공격자 대사.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s ", m_tInfo.szName, m_tInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"AprhoditeAttack.wav", CSoundMgr::EFFECT);

							//CEffect_IMP * pAni = new CAnimation_IMP;
							//pAni->Ready_Effect_IMP();
							//dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							//_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							//CEffect* pNormal = new CNormalEffect(pAni);
							//pNormal->Ready_GameObject();
							//pNormal->Set_Pos(temp);
							//pNormal->Set_Dead(true);
							//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}
						if (5 >= m_iSceneNum)
						{
							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 100.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 130.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 160.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 190.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_iSceneNum++;
						}

						if (6 == m_iSceneNum)
						{

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 100.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 130.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 160.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.F, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 190.f + m_fSpeed, (WINCY >> 1) + -50.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


							if (m_fSpeed >= 600)
							{

								//몬스터
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

								//방어자 작은 사진.
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								//방어자 대사

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 800.f;
								//나가는 총알
								if (m_fSpeed2 <= 500)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) -50.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -130.f + m_fSpeed2, (WINCY >> 1) - 50.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -160.f + m_fSpeed2, (WINCY >> 1) - 50.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -190.f + m_fSpeed2, (WINCY >> 1) - 50.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -220.f + m_fSpeed2, (WINCY >> 1) - 50.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트
								if (m_fSpeed2 >= 500)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}
									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 0);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tMonInfo.iHp -= (m_tInfo.iAttack - m_tMonInfo.iArmor);
										if (m_tMonInfo.iHp <= 0)
											m_tMonInfo.iHp = 0;
										m_bHpCheck = true;
									}


									if (m_fTime >= 3.0f)
									{

										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												if (m_tMonInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_MonsterHP(m_tMonInfo);
												break;

											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												_rList->Set_UnitTurn(true);
												_rList->Set_Attacker(false);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureUnit, L"Whitebase"))
				{
					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);

					//유닛 전투 씬에서 유닛 공격 이펙트.
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", m_iSceneNum);


					const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 10);
					const TEXINFO*	pTexInfo13 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 11);
					const TEXINFO*	pTexInfo14 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 12);

					//공격자 그림들.
					if (m_fSpeed <= 600)
					{


						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						//공격자 대사.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s ", m_tInfo.szName, m_tInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"WhitebaseAttack.wav", CSoundMgr::EFFECT);

							m_bSoundCheck = true;
						}
						if (9 >= m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							m_iSceneNum++;

						}

						if (10 >= m_iSceneNum)
						{
	


							D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) -20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 170.f + m_fSpeed, (WINCY >> 1) -20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo13->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 370.f + m_fSpeed, (WINCY >> 1) -20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo14->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//몬스터
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

								//방어자 작은 사진.
								const TEXINFO*	pTexInfo15 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo15->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								//방어자 대사

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 800.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									//D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, - 200.f + m_fSpeed2, (WINCY >> 1) - 30.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, - 130.f + m_fSpeed2, (WINCY >> 1) - 30.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo13->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans,  70.f + m_fSpeed2, (WINCY >> 1) - 30.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo14->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트
								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}
									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 0);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tMonInfo.iHp -= (m_tInfo.iAttack - m_tMonInfo.iArmor);
										if (m_tMonInfo.iHp <= 0)
											m_tMonInfo.iHp = 0;
										m_bHpCheck = true;
									}


									if (m_fTime >= 3.0f)
									{

										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												if (m_tMonInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_MonsterHP(m_tMonInfo);
												break;

											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												_rList->Set_UnitTurn(true);
												_rList->Set_Attacker(false);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 800.f;

						}
					}
				}
			}

			//몬스터
			for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
			{

				const TEXINFO*	pTexInfo4;
				const TEXINFO*	pTexInfo5;
				int UnitHpPersent = m_tInfo.iHp * 100 / m_tInfo.iMaxHp;
				if (UnitHpPersent >= 100)
				{
					pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 0);
				}
				else if (UnitHpPersent < 100 && UnitHpPersent >= 80)
				{
					pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 1);
				}
				else if (UnitHpPersent < 80 && UnitHpPersent >= 60)
				{
					pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 2);
				}
				else if (UnitHpPersent < 60 && UnitHpPersent >= 40)
				{
					pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 3);
				}
				else if (UnitHpPersent < 40 && UnitHpPersent >= 1)
				{
					pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 4);
				}
				else
					pTexInfo4 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 5);

				int MonHpPersent = m_tMonInfo.iHp * 100 / m_tMonInfo.iMaxHp;
				if (MonHpPersent >= 100)
				{
					pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 0);
				}
				else if (MonHpPersent < 100 && MonHpPersent >= 80)
				{
					pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 1);
				}
				else if (MonHpPersent < 80 && MonHpPersent >= 60)
				{
					pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 2);
				}
				else if (MonHpPersent < 60 && MonHpPersent >= 40)
				{
					pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 3);
				}
				else if (MonHpPersent < 40 && MonHpPersent >= 1)
				{
					pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 4);
				}
				else
					pTexInfo5 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitHPBar", 5);



				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster00"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster01"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster02"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster03"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster04"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster05"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster06"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster07"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster08"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}

				if (_rList->Get_Attacker() && !lstrcmp(m_pTextureMonster, L"Monster09"))
				{

					D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo5->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					////////////////////////////////공격자 폰트 ///////////////////////////
					D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szBuf[64] = L"";
					swprintf_s(szBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szLevel[64] = L"";
					swprintf_s(szLevel, L"LV   %d", m_tMonInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					//방어자 폰트///////////////////////////////////
					D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					TCHAR szMonBuf[64] = L"";
					swprintf_s(szMonBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
					D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


					D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

					TCHAR szMonLevel[64] = L"";
					swprintf_s(szMonLevel, L"LV   %d", m_tInfo.iLevel);
					D3DXMatrixTranslation(&matTrans, 670, 530.f, 0.f);
					CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
					CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
						szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					//떄리는 유닛
					const TEXINFO*	pTexInfo6 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 1);

					//유닛 전투 씬에서 유닛 공격 이펙트.	
					const TEXINFO*	pTexInfo7 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureMonster, L"Attack", m_iSceneNum);

					//유닛 전투 씬에서 작은 사진.
					const TEXINFO*	pTexInfo11 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", m_pTextureMonster, 2);

					if (m_fSpeed <= 600)
					{
						float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
						float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
						matWorld = matScale * matTrans;
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
						CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo11->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


						//공격자 폰트.
						TCHAR szUnitToking[64] = L"";
						swprintf_s(szUnitToking, L"%s %s", m_tMonInfo.szName, m_tMonInfo.szToking);
						D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
						CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
						CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
							szUnitToking, lstrlen(szUnitToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
					}

					if (m_fTurnDelay >= 4.0f)
					{
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Beam1.wav", CSoundMgr::EFFECT);

							CEffect_IMP * pAni = new CAnimation_IMP;
							pAni->Ready_Effect_IMP();
							dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKReady", { 0.f, 11.f });
							_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
							CEffect* pNormal = new CNormalEffect(pAni);
							pNormal->Ready_GameObject();
							pNormal->Set_Pos(temp);
							pNormal->Set_Dead(true);
							CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

							m_bSoundCheck = true;
						}

						if (0 == m_iSceneNum)
						{
							float fCenterX7 = float(pTexInfo7->tImageInfo.Width >> 1);
							float fCenterY7 = float(pTexInfo7->tImageInfo.Height >> 1);

							D3DXMatrixScaling(&matScale, m_fsize, m_fsize, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(fCenterX7, fCenterY7, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_fsize += 0.5f;
							if (m_fsize >= 5)
							{
								m_fsize = 0.1f;
								m_iSceneNum = 1;
							}
						}

						if (1 == m_iSceneNum)
						{
							m_fsize += 0.2f;
							if (0.8 <= m_fsize)
							{
								m_fsize = 0.8f;
							}


							D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
							D3DXMatrixTranslation(&matTrans, (WINCX >> 1) + 70.f + m_fSpeed, (WINCY >> 1) + 0.f, 0.f);
							matWorld = matScale * matTrans;
							CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
							CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

							if (m_fSpeed >= 600)
							{

								//맞는 유닛
								const TEXINFO*	pTexInfo8 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 1);
								const TEXINFO*	pTexInfo12 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 2);
								float fCenterX8 = float(pTexInfo8->tImageInfo.Width >> 1);
								float fCenterY8 = float(pTexInfo8->tImageInfo.Height >> 1);

								D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo8->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


								D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
								D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
								matWorld = matScale * matTrans;
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
								CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo12->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

								TCHAR szMonsterToking[64] = L"";
								swprintf_s(szMonsterToking, L"%s %s", m_tInfo.szName, m_tInfo.szToking2);
								D3DXMatrixTranslation(&matTrans, 150.f, 20.f, 0.f);
								CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
								CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
									szMonsterToking, lstrlen(szMonsterToking), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

								m_fSpeed2 += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;
								//나가는 총알
								if (m_fSpeed2 <= 400)
								{

									D3DXMatrixScaling(&matScale, m_fsize, 1.f, 0.f);
									D3DXMatrixTranslation(&matTrans, -100.f + m_fSpeed2, (WINCY >> 1) + 0.f, 0.f);
									matWorld = matScale * matTrans;
									CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo7->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
								}
								//총알 도착 후 터지는 이펙트

								if (m_fSpeed2 >= 400)
								{
									if (!m_bSoundCheck2)
									{
										CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
										CSoundMgr::Get_Instance()->PlaySoundW(L"Explosion1.wav", CSoundMgr::EFFECT);


										CEffect_IMP * pAni = new CAnimation_IMP;
										pAni->Ready_Effect_IMP();
										dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"GundamMKfinish", { 0.f, 6.f });
										_vec3 temp = { (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f };
										CEffect* pNormal = new CNormalEffect(pAni);
										pNormal->Ready_GameObject();
										pNormal->Set_Pos(temp);
										pNormal->Set_Dead(true);
										CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);



										m_bSoundCheck2 = true;
									}

									m_fTime += CTime_Manager::Get_Instance()->Get_TimeDelta();

									//const TEXINFO*	pTexInfo9 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(m_pTextureUnit, L"Attack", 2);

									//float fCenterX8 = float(pTexInfo9->tImageInfo.Width >> 1);
									//float fCenterY8 = float(pTexInfo9->tImageInfo.Height >> 1);

									//D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
									//D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) + 0.f, 0.f);
									//matWorld = matScale * matTrans;
									//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
									//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo9->pTexture, nullptr, &_vec3(fCenterX8, fCenterY8, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

									if (!m_bHpCheck)
									{
										m_tInfo.iHp -= (m_tMonInfo.iAttack - m_tInfo.iArmor);
										if (m_tInfo.iHp <= 0)
											m_tInfo.iHp = 0;
										m_bHpCheck = true;
									}
									if (m_fTime >= 3.0f)
									{
										m_iSceneNum = 0;
										static_iAllCheck = 0;
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
										{
											if (!lstrcmp(_rList->Get_TextureUnitKey(), m_pTextureUnit))
											{
												if (m_tInfo.iHp <= 0)
												{
													_rList->Set_Dead(true);
													break;
												}
												_rList->Set_UnitHP(m_tInfo);
											}
										}

										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
										{
											if (!lstrcmp(_rList->Get_TextureMonsterKey(), m_pTextureMonster))
											{
												_rList->Set_MonsterTurn(true); //왜 턴이 두번 돌지 ?
												_rList->Set_Attacker(false);
											}
										}


										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}
										for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
										{
											_rList->Set_iSceneNum(m_iSceneNum);
											_rList->Set_static_iAllCheck(static_iAllCheck);
										}

										m_bDead = true;
									}
								}
							}
							m_fSpeed += CTime_Manager::Get_Instance()->Get_TimeDelta() * 500.f;

						}
					}
				}
			}



			//float fCenterX6 = float(pTexInfo6->tImageInfo.Width >> 1);
			//float fCenterY6 = float(pTexInfo6->tImageInfo.Height >> 1);
			//D3DXMatrixScaling(&matScale, -1.0f, 1.f, 0.f);
			//D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);
			//matWorld = matScale * matTrans;
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo6->pTexture, nullptr, &_vec3(fCenterX6, fCenterY6, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			////////////////////////////////공격자 폰트 ///////////////////////////
			//D3DXMatrixTranslation(&matTrans, 20, 410.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	m_tInfo.szUnitName, lstrlen(m_tInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

			//TCHAR szBuf[64] = L"";
			//swprintf_s(szBuf, L"HP       %d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
			//D3DXMatrixTranslation(&matTrans, 20, 460.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	szBuf, lstrlen(szBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

			//D3DXMatrixTranslation(&matTrans, 20, 530.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	m_tInfo.szName, lstrlen(m_tInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

			//TCHAR szLevel[64] = L"";
			//swprintf_s(szLevel, L"LV   %d", m_tInfo.iLevel);
			//D3DXMatrixTranslation(&matTrans, 270, 530.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

			//////////////////////// 맞는자 폰트  ////////////////////////////////


			//D3DXMatrixTranslation(&matTrans, 420, 410.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	m_tMonInfo.szUnitName, lstrlen(m_tMonInfo.szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


			//TCHAR szMonBuf[64] = L"";
			//swprintf_s(szMonBuf, L"HP       %d / %d", m_tMonInfo.iHp, m_tMonInfo.iMaxHp);
			//D3DXMatrixTranslation(&matTrans, 420, 460.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	szMonBuf, lstrlen(szMonBuf), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


			//D3DXMatrixTranslation(&matTrans, 420, 530.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//	m_tMonInfo.szName, lstrlen(m_tMonInfo.szName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

			//TCHAR szMonLevel[64] = L"";
			//swprintf_s(szMonLevel, L"LV   %d", m_tMonInfo.iLevel);
			//D3DXMatrixTranslation(&matTrans, 570, 530.f, 0.f);
			//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			//szMonLevel, lstrlen(szMonLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
	}
}

void CUnitFight::Release_GameObject()
{
}

void CUnitFight::FrameMove(float fSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * CTime_Manager::Get_Instance()->Get_TimeDelta() * fSpeed;
	if (m_tFrame.fFrameStart >= m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 4.f;

	}
}
