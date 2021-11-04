#ifndef __ESCAPE_SHUTTLE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CShader;

END

class CEscape_Shuttle : public CGameObject
{
	typedef enum MeshID
	{
		Shuttle,
		ShuttleDoor,
		Shuttle_end
	}MESHID;

	typedef enum ShuttleID
	{
		ShuttleMove,
		ShuttleDoorOpen,
		ShuttleDoorClose,
		ShuttleCutScene,
		End
	}SHUTTLE;

private:
	explicit CEscape_Shuttle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEscape_Shuttle();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr, _bool bState = false);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;


	virtual _mat Get_EventPos() override;

public:
	void				Set_ShuttleStack(_int i) { m_iShuttleStack += i; }
	void				Set_CutScene() { m_bCutScene = true; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex, const _int& iMeshNum = 100);

private:
	void				Move(const _float& fTimeDelta);
	void				DoorOpen(const _float& fTimeDelta);
	void				DoorClose(const _float& fTimeDelta);
	_int				ShuttleUpMove(const _float& fTimeDelta);
	void				CutScene(const _float& fTimeDelta);
private :
	_bool				m_bMove[SHUTTLE::End] = { false };
	CTrigger*			m_pTrigger = nullptr;
	

	_uint				m_iShuttleStack = 0;
private:
	Engine::CStaticMesh*			m_pMeshCom[MESHID::Shuttle_end];
	Engine::CTransform*				m_pTransformCom[MESHID::Shuttle_end];
	Engine::CShader*				m_pShaderCom = nullptr;


	_bool							m_bState = false;
	_bool							m_bCutScene = false;
	_float							m_fOpenSpeed = D3DXToRadian(2);
	_float							m_fAngle = 90.f;
	_float							m_fCurrentAngle = 0.f;

	_float							m_fSpeed = 1.f;
	_float							m_fShuttleTime = 0.f;

	_float							m_fOpenDoorTime = 0.f;

	CLight*							m_pLight[2];

public:
	static CEscape_Shuttle*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _bool bState = false);
	virtual void						Free(void)		override;
};

#define __ESCAPE_SHUTTLE_H__
#endif

