#ifndef __StaticHoze_H__
#define __StaticHoze_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Trigger.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;

END

class CStaticHoze : public Engine::CGameObject
{
private:
	explicit CStaticHoze(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticHoze(void);

	bool SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _int& iIndex);
public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	 HRESULT				Ready_Object(const _wstr& tagName,const _int& bIstwoWay,void* pArg = nullptr) ;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;


	virtual void*			On_Trigger_Active(void* pTrigger, void* pCaller) override;

	virtual void* On_Active(void* pCaller) override;
public:
	void					Set_Power(const _float& fPower) { m_fPower = fPower; }
public:
	const _float&			Get_Power() { return m_fPower; }
	const _float&			Get_Duration() { return m_fDuration; }
public:
	void					Set_ShootData(const _float& fPower, const _float& fDuration, const _float& fSpeed) { m_fSpeed = fSpeed; m_fDuration = fDuration; m_fPower = fPower; }
	void					Load_Path(const _wstr& tagPath, const _bool& bBack = false);
	void					Set_Active();
private:

	HRESULT					Add_Component(const _wstr& tagName,const _int& bIsTwoWay, void* pArg = nullptr);

public:


public:
	static CStaticHoze* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _wstr& tagName,const vector<_wstr>& tagPath,const _int& Way=false, const _bool& bBack = false,void* pArg = nullptr);
	private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CDynamicMesh*			m_pMeshHead[2] = { nullptr,nullptr };
	CTransform*				m_pTransformCom[2] = { nullptr,nullptr };
public:
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	_int					m_iWay = false;
	vector<_vec3>			m_vecPos;
	vector<vector<_vec3>*>	m_vecOut;
	vector<_float>			m_vecPoint;
	_float					m_fDuration = 1.f;
	_float					m_fPower = 20.f;
	_float					m_fSpeed = 1.f;
	size_t					m_iInterval;
	_int					m_iPathCount;
	_bool					m_bActive = false;

private:
	class CBlow*			m_pBlow = nullptr;
	_float					m_fEffectTime = 0.f;


private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
