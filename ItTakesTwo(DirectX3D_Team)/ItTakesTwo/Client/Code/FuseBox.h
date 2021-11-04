#ifndef __FuseBox_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "FuseStatic.h"
#include "FuseLight.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

const	_uint						g_iFuseCount = 3;
class CFuseBox : public CGameObject
{

private:
	explicit		 CFuseBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CFuseBox();

public:
	virtual HRESULT Ready_Object(void * pArg, _bool bRun = false);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public:
	virtual void*	On_Trigger_Active(void* pTrigger,void* pCaller) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	void			Insert_Fuse(const _uint& iIndex);
	_bool			Check_AllReady();

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CFuseStatic*				m_pFuse[g_iFuseCount];
	CFuseLight*					m_pFuseLight[g_iFuseCount];

private :
	_bool						m_bAllReady = false;
	CTrigger*					m_pSlotTrigger[g_iFuseCount];
	_bool						m_bSlot[g_iFuseCount]{};
	_float						m_fDeltaTime;

public:
	static CFuseBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun = false);
	virtual void Free(void)		override;
};


#define __FuseBox_H__
#endif
