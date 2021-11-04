#pragma once

#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Base.h"
#include <Management.h>

#include "Logo.h"


#include "TestCamera.h"
#include "MainCamera.h"
#include "BattleCamera.h"
#include "Player.h"
#include "Monster.h"
#include "BattleUI.h"
#include "StageUI.h"

#include "MiddleBossA.h"

#include "BattlePlate.h"
#include "MonsterA.h"
#include "Skybox.h"
#include "Terrain.h"


#include "Effecter.h"
#include "EffectParticle.h"

#include "Teleporter.h"
#include "TriggerBox.h"
#include "FakeTerrain.h"
#include "Gress.h"
#include "Tree.h"


USING(Engine)
//�̱��� �ƴ�
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Ready_MainApp();
	_uint Update_MainApp();

private:
	HRESULT Setup_DefaultSetting();

private: //�ֻ��� ������Ÿ�� Call
	HRESULT HighLevel_Prototype_Caller();

private://�߰� ������Ÿ�� Call
	HRESULT GameObject_Prototype_Caller(CManagement * _pManagement);
	HRESULT Component_Prototype_Caller(CManagement * _pManagement);
	HRESULT Texture_Prototype_Caller(CManagement * _pManagement);
	HRESULT Status_Comp_Prototype(CManagement * _pManagement);
	HRESULT Etc_Prototype_Caller(CManagement * _pManagement);


#pragma region ���� ������Ÿ��

private://GameObject ����  Prototype
	HRESULT Player_Prototype(CManagement * _pManagement);
	HRESULT Monster_Prototype(CManagement * _pManagement);
	HRESULT BattlePlate_Prototype(CManagement * _pManagement);
	HRESULT Camera_Prototype(CManagement * _pManagement);
	HRESULT UI_Prototype(CManagement * _pManagement);
	HRESULT BATTLE_UI_Prototype(CManagement * _pManagement);
	HRESULT Terrain_Prototype(CManagement * _pManagement);
	HRESULT SkyBox_Prototype(CManagement * _pManagement);

	HRESULT Effecter_Prototype(CManagement * _pManagerment);
	HRESULT Particle_Prototype(CManagement * _pManagerment);

	HRESULT MiddleBossABullet_Prototype(CManagement* _pManagement);
	HRESULT Item_Prototype(CManagement* _pManagement);

	HRESULT TriggerBox_Prototype(CManagement* _pManagement);
	HRESULT Teleporter_Prototype(CManagement* _pManagement);
	HRESULT Gress_Prototype(CManagement* _pManagement);
	HRESULT Tree_Prototype(CManagement* _pManagement);
	HRESULT FakeTerrain_Prototype(CManagement* _pManagement);


private://Component ���� Prototype
	HRESULT Transform_Prototype(CManagement * _pManagement);
	HRESULT VIBuffer_Prototype(CManagement * _pManagement);
	HRESULT Font_Prototype(CManagement * _pManagement);

private://Texture ���� Prototype
	HRESULT Player_Texture_Prototype(CManagement * _pManagement);
	HRESULT Player_2DTexture_Prototype(CManagement* _pManagement);

	HRESULT Monster_Texture_Prototype(CManagement * _pManagement);
	HRESULT BattlePlate_Texture_Prototype(CManagement * _pManagement);
	HRESULT UI_Texture_Prototype(CManagement * _pManagement);
	HRESULT Terrain_Texture_Prototype(CManagement * _pManagement);
	HRESULT SkyBox_Texture_Prototype(CManagement * _pManagement);
	HRESULT Item_Texture_Prototype(CManagement* _pManagement);
	HRESULT BackGround_Texture_Prototype(CManagement* _pManagement);


	//Ȥ�ó� �;
	HRESULT CLEAR_PROTOTYPE(CManagement* _pManagement);

	HRESULT Gress_Texture_Prototype_Han(CManagement * _pManagement);

#pragma endregion

private : //���޿�
	HRESULT Monster_Texture_Prototype_JDY(CManagement * _pManagement);

public:
	static CMainApp* Create();
	virtual void Free() override;

private:
	CManagement*	m_pManagement = nullptr;

	_Device			m_pDevice = nullptr;
	LPD3DXFONT		m_pFont = nullptr;
	LPD3DXSPRITE	m_pSprite = nullptr;


};


#endif // !__MAINAPP_H__