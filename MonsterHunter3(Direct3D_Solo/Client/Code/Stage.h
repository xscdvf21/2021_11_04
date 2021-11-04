#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Base.h"
#include "define.h"


#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Player.h"
#include "Stone.h"
#include "Sword.h"
#include "Effect.h"
#include "UI.h"
#include "Tree.h"
#include "MonsterJagi.h"
#include "Boss.h"
#include "Bosstail.h"
#include "HPUI.h"
#include "Weapon.h"
#include "Iru.h"
#include "Blood.h"
#include "Wind.h"
#include "BulletWind.h"
#include "HitPlayer.h"
#include "HP_Potion.h"
#include "SP_Potion.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo(void);


	HRESULT			Load_Navi();

private :
	_bool			m_bBossSoundOne = false; //사운드바꾸는걸 한번만 하기위해.
	_bool			m_bBossSound = false; //보스랑 만났을때 BGM교체를 위해.
	
public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

#endif // Stage_h__
