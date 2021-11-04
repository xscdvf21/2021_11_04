#ifndef FootL_h__
#define FootL_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CCollider;
class CCalculator;
class CShader;

END

class CFootL : public Engine::CGameObject
{
private:
	explicit CFootL(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFootL(void);

public:
	HRESULT					Ready_Object(const _uint& iFlag);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	void					Set_Active(_bool active) { m_bActive = active; }

private:
	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentBoneMatrix2 = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;
	_matrix					m_pPosMatrix;
	_matrix					m_matTrans;

	_vec3					m_vDir;
	_bool					m_bColl;
	_uint					m_iFlag = 0;
	_bool					m_bStart = true;
	_bool					m_bActive = false;

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CFootL*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;

};

#endif // FootL_h__
