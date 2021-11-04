#ifndef __TOYSWORD_H__
#define __TOYSWORD_H__

#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CTexture;
class CCollider;
class CCalculator;
class CShader;

END

class CToySword : public CPlayer
{
private:
	explicit CToySword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToySword(void);

public:
	HRESULT					Ready_Object(void* pArg);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;
	virtual void			Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex = 0) override;

private:
	HRESULT					Add_Component(void* pArg);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:	
	void					Set_Active(_bool active) { m_bActive = active; }
	void					Set_Render(_bool render) { m_bRenderState = render; }

private:
	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentBoneMatrix2 = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;
	_matrix					m_pPosMatrix;
	_matrix					m_matTrans;
	_float					m_fDeltaTime;
	_vec3					m_vDir;
	_bool					m_bColl;
	_uint					m_iFlag = 0;
	_bool					m_bStart = true;
	_bool					m_bActive = false;
	_vec4					m_vColor{};
	_bool					m_bColor = false;
	_bool					m_bRenderState = true;

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTexture*		m_pTextureCom2 = nullptr;
	Engine::CTexture*		m_pEffectCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CToySword*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg =nullptr);

private:
	virtual void Free(void) override;

private:
	_float4x4				m_matColWorld;

};

#endif // Hat_h__
