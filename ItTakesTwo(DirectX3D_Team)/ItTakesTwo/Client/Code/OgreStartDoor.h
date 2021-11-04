#ifndef __OGRESTARTDOOR_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

enum SoundID
{
	BGM,
	Sound_End
};

class COgreStartDoor : public CGameObject
{
private:
	explicit COgreStartDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COgreStartDoor();

public :
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public :
	void			Set_MoveStart(_bool _bStart) { m_bMoveStart = _bStart; }

private:
	HRESULT			Add_Component(void* pArg =nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
private :
	void			Move(const _float& fTimeDelta);
	void			TriggerCreate();

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private :
	CTrigger*						m_pTrigger = nullptr;
	_bool							m_bCreateTrigger;
private :
	_bool							m_bMoveStart;
	_float							m_fSpeed;

	_bool							m_bSound[Sound_End] = { false };
public:
	static COgreStartDoor*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;


};

#define __OGRESTARTDOOR_H__
#endif

