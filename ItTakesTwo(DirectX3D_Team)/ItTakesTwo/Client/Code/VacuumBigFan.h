#ifndef VacuumBigFan_h__
#define VacuumBigFan_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CVacuumBigFan : public Engine::CGameObject
{
private:
	explicit CVacuumBigFan(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumBigFan(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

public:
	virtual void*			On_Attach(void* pCaller) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void					Set_Sound() { m_bSoundFan = true; }
private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CStaticMesh*	m_pSylinderMeshCom[4];
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	_float					m_fMoveAngle = 0.0f;
	_vec3					m_vResult{};

public:
	static CVacuumBigFan*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	_bool					m_bPlaySound = false;
	_bool					m_bSoundFan = false;


private:
	virtual void Free(void) override;

private:
	_float					m_fVolume = 0.0f;
	class CTransform*		m_pCodyTransform = nullptr;
	class CTransform*		m_pMayTransform = nullptr;
};

#endif // __FAN_H__
