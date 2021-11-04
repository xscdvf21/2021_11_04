#ifndef __VacuumHub_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END



class CVacuumHub : public Engine::CGameObject
{
public:
	explicit CVacuumHub(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumHub();

public:
	virtual HRESULT Ready_Object(_uint iBodyCount, void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;


private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);


private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_bool						m_bLayer = false; //레이어 다 생성후에 한번 바디 생성하기위해.

	vector<CGameObject*>		m_vecBody;

	_uint						m_iBodyCount;

public:
	static CVacuumHub*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;

};



#define __VacuumHub_H__
#endif


