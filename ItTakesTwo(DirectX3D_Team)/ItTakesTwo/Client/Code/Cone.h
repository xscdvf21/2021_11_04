#ifndef Cone_h__
#define Cone_h__

#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CCollider;
class CCalculator;
class CShader;
class CTexture;

END

class CCone : public CPlayer
{
private:
	explicit CCone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCone(void);

public:
	HRESULT					Ready_Object(void* pArg);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;


private:
	HRESULT					Add_Component(void* pArg);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:	
	void					Set_Active(_bool active) { m_bActive = active; }
	void					Set_Index(_int index) { m_iCount = index; }
	void					Set_Amount(_float amount) { m_fAmount = amount; }

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

	char					strBoneName[MAX_PATH] = "";

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

public:
	static CCone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);

private:
	virtual void Free(void) override;

private:
	_float4x4				m_matColWorld;
	_float					m_fTimeDelta = 0.f;
	_int					m_iCount = 0;
	_float					m_fAmount = 0.f;
};

#endif // Cone_h__
