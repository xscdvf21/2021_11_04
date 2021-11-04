#ifndef __CHESSDOORSWITCH_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessDoorSwitch : public CGameObject
{
private:
	explicit			CChessDoorSwitch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CChessDoorSwitch();

public:
	virtual HRESULT		Ready_Object(BUTTONID _eButton, void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public :
	void				TriggerCreate();
public : //도어 발판
	void				Set_MoveDoor(_bool bMove) { m_bMoveDoor = bMove; }
	void				MoveDoor(const _float& fTimeDelta);
	_bool				IsColDoor() { return m_bMoveDoor; }
public : //불발판.
	void				Set_CreateFire(_bool bCreate) { m_bFireCreate = bCreate; }
	void				CreateFire(const _float& fTimeDelta);

public : //움직이는 다리 발판
	void				Set_MoveBridge(_bool bMove) { m_bMoveBridge = bMove; }
	void				MoveBridge(const _float& fTimeDelta);

public : //크러셔 시작 발판
	void				Set_CrusherMove(_bool bMove) { m_bCrusherMove = bMove; }
	_bool				Get_CrusherMove() { return m_bCrusherMove; }
	void				MoveCrusher(const _float& fTimeDelta);
private:
	HRESULT				Add_Component(BUTTONID _eButton, void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private :
	BUTTONID						m_eButtonID;
	_bool							m_bCreateTrigger;
	CTrigger*						m_pTrigger = nullptr;

private:
	_bool							m_bMoveDoor; //문 
private :
	_bool							m_bFireCreate; //불기둥
private:
	_bool							m_bMoveBridge; //다리
private :
	_bool							m_bCrusherMove; //크러셔 무브.

public:
	static CChessDoorSwitch*			Create(LPDIRECT3DDEVICE9 pGraphicDev,BUTTONID _eButton, void* pArg = nullptr);
	virtual void						Free(void)		override;

};
#define __CHESSDOORSWITCH_H__
#endif

