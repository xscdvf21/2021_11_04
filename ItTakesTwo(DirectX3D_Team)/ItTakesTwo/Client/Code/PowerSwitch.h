#ifndef __PowerSwitch_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "FuseStatic.h"
#include "ImageUI.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CPowerSwitch : public CGameObject
{

private:
	explicit		 CPowerSwitch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CPowerSwitch();

	void Move(const _float & fTimeDelta);
	_bool m_bMoveEnd = false;

public:
	virtual HRESULT Ready_Object(void * pArg, _bool bRun = false);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;
	
public:
	virtual void* On_Trigger_Active(void* pTrigger,void* pCaller) override;
	
	_bool			Is_End() { return m_bEnd; }
private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	void			Set_Switch(bool bState);
	_bool			Check_AllReady();

public:
	_bool			Get_AllReady() { return m_bAllReady; }

public:
	void			Set_Socket(_uint iIdx, _bool bState);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CTrigger* m_pTrigger[2];

private :
	_bool						m_bEnd = false;
	_float						m_fDeltaTime;

	_bool						m_bSocket[3];
	_bool						m_bAllReady = true;
	_bool						m_bLaunchFuse = false;
	_bool						m_bRev = false;
	_float						m_fRevTime = 0.0f;
	_bool						m_bShoot = false;

public:
	static CPowerSwitch*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun = false);
	virtual void Free(void)		override;

};


#define __PowerSwitch_H__
#endif
