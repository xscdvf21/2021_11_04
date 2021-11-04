#ifndef __DoubleHoze_H__
#define __DoubleHoze_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Trigger.h"
#include "VacuumFieldPart5.h"
#include "VacuumHeadHoze.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;

END

class CDoubleHoze : public Engine::CGameObject
{
private:
	explicit CDoubleHoze(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDoubleHoze(void);

	bool SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _int& iIndex);
public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	HRESULT				Ready_Object(const _bool& bIstwoWay, void* pArg = nullptr);
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;


	virtual void*			On_Trigger_Active(void* pTrigger, void* pCaller) override;
	virtual void*			On_Active(void* pCaller) override;

	void Load_Path(const _wstr& tagPath);


private:

	HRESULT Add_Component(const _bool& bIsTwoWay, void* pArg = nullptr);
	void	Create_DynamicPart();
public:


public:
	static CDoubleHoze* Create(LPDIRECT3DDEVICE9 pGraphicDev, const vector<_wstr>& tagPath, const _bool& bIstwoWay = false, void* pArg = nullptr);
private:
	CStaticMesh*			m_pMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

public:
private:
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	_bool					m_bIsTwoWay = false;
	_bool					m_bFlip = false;
	vector<vector<_vec3>*>	m_vecTotalPos;
	vector<_vec3>			m_vecPos;
	vector<_vec3>			m_vecRvPos;
	vector<_float>			m_vecPoint;
	CVacuumHeadHoze*		m_pVacuum[2] = { nullptr,nullptr };

private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
