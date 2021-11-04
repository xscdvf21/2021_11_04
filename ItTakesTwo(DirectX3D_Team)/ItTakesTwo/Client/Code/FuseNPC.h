#ifndef __FUSENPC_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "ImageUI.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CFuseNPC : public CGameObject
{
public:
	enum animID {
		FuseCrab_Carried,                         //0	frame:[41]
		FuseCrab_Dazed,                           //1	frame:[41]
		FuseCrab_Mh,                              //2	frame:[25]
		FuseCrab_Taunt,                           //3	frame:[131]
		Shed_Awakening_CrawlSpace_FuseRunInHole_FuseCrab,//4	frame:[61]
		Shed_Awakening_DoubleInteractSaw_FuseGoToShelf_FuseCrab,//5	frame:[159]
		Shed_Awakening_DoubleJumpDash_FuseDoubleeJump_FuseCrab,//6	frame:[88]
		Shed_Awakening_Drillbazz_WallSlidemove_Cog_FuseCrab,//7	frame:[85]
		Shed_Awakening_Drillbazz_WallSlidemove_FuseCrab,//8	frame:[64]
		Shed_Awakening_FuseSocket_JumpOut_FuseCrab1,//9	frame:[939]
		Shed_Awakening_FuseSocket_JumpOut_FuseCrab2,//10	frame:[939]
		Shed_Awakening_FuseSocket_JumpOut_FuseCrab3,//11	frame:[939]
		Shed_Awakening_GlassJar_Fusebump_FuseCrab,//12	frame:[57]
		Shed_Awakening_Saw_FuseDoubleJump_FuseCrab,//13	frame:[113]
		Shed_Awakening_Slide_FuseSlide_FuseCrab,  //14	frame:[146]
		Shed_Awakening_Walljump_FuseJump_FuseCrab,//15	frame:[128]
		Shed_Awakening_Walljump_ToDoubleJump_FuseCrab,//16	frame:[46]
		End
	};

private:
	explicit		 CFuseNPC(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CFuseNPC();

public:
	virtual HRESULT Ready_Object(void * pArg, _uint iFlag, _uint iEvent, _bool bRun = false);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			MoveUse(const _float& fTimeDelta);
	void			Fuse_Jump(const _float& fTimeDelta);
	void			Fuse_Slide(const _float& fTimeDelta);
	void			Fuse_JumpStart(const _float& fTimeDelta);

private:
	void			FuseEffect();

public:
	_bool			Get_Carried() { return m_bCarried; }

public:
	void			Set_Carried(_tchar* tagTargetName);// { m_bCarried = true; }
	void			Set_Drop();// { m_bDrop = true; }
	void			Set_TriggerOff();
	void			Set_Work() { m_bWork = true; }
	void			EventStart();

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private :
	CTrigger*					m_pTrigger = nullptr;
	_bool						m_bRun; //도망갈놈인지 아닌지.
	_bool						m_bUse; //발전기에 사용되었는지.

	_uint						m_iAniIndex;
	_uint						m_iPattrunStart; //다음패턴들 실행하기 위해.  1씩 ++해줄것임.
	_float						m_fDeltaTime;
	_bool						m_bCarried = false;
	_bool						m_bRotation = false;
	_bool						m_bOverlap = false;
	_bool						m_bRotationStop = false;
	TCHAR						m_tagTargetName[MAX_PATH] = L"";
	
	_float4x4					m_matCombinedMatrix;
	_bool						m_bDrop = false;
	_bool						m_bWork = false;
	_uint						m_iFlag = 0;
	_bool						m_bPlay = false;
	_uint						m_iEvent = 0;
	_bool						m_bJump = false;
	_float						m_fFlyTime = 0.0f;
	_bool						m_bSlide = false;
	_bool						m_bSlideStart = false;
	_bool						m_bFuseDrop = false;

	_bool						m_bJumpStart = false;
	_bool						m_bSlideJump = false;
	_vec3						m_vSlideJumpStart{};

public:
	static CFuseNPC*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iFlag, _uint iEvent, void * pArg, _bool bRun = false);
	virtual void Free(void)		override;

};


#define __FUSENPC_H__
#endif

