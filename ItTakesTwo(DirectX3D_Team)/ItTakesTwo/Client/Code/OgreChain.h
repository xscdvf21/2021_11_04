#ifndef __OGRECHAIN_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class COgreChain : public CGameObject
{
	enum animID {
		ToyChain_Break, //0
		End
	};

	enum SoundID
	{
		Sound_break,
		Sound_End
	};
private:
	explicit COgreChain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COgreChain();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;
	
public :
	void			Set_Chain(_bool _bState) { m_bMoveStart = _bState; }
	void			Set_Sound();	
public:
	void					Set_OgreCol(_bool _bCol) { m_bOgreCol = _bCol; }
	_bool					IsCol() { return m_bOgreCol; }

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
private:
	void			Move(const _float& fTimeDelta);

	

private:
	Engine::CDynamicMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bMoveStart;
	_uint							m_iAniIndex;
private:
	_bool							m_bSound[Sound_End] = { false };
	_uint							m_iSound[Sound_End] = { 0 };

	_bool							m_bOgreCol;
	CParticle*						m_pParticle = nullptr;
	CLight*							m_pLight = nullptr;

public:
	static COgreChain*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, void* pArg = nullptr);
	virtual void					Free(void)		override;

};


#define __OGRECHAIN_H__
#endif

