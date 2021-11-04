#ifndef __BABOONGROUND_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CCalculator;
class CCollider;
class CShader;

END

class CBaboonGround : public CGameObject
{
	typedef enum SoundID
	{
		Ground_Up,
		Ground_Stop,
		End
	}SOUNDID;

private:
	explicit CBaboonGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaboonGround();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public:
	void				Set_MoveUp() { m_bUp = true; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private :
	_bool							m_bUp;
	_bool							m_bMoveDir;

	_vec3							m_vStartPos;
	_vec3							m_vMoveDir;

	_float							m_fUpDis;
	_float							m_fBombTime;


	_bool							m_bSoundStart[SOUNDID::End] = { false };
public:
	static CBaboonGround*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;
};

#define __BABOONGROUND_H__
#endif

