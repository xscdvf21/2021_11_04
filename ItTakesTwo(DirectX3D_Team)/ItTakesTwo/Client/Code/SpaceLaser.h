#ifndef __SPACELASER_H__
#define __SPACELASER_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CShader;
class CTexture;

END

class CSpaceLaser : public CEnermy
{
private:
	explicit CSpaceLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceLaser(void);

public:
	HRESULT					Ready_Object(void* pArg, _float fAngle = -90.f);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;


private:
	HRESULT					Add_Component(void* pArg);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	const _bool&			Get_Active(){ return m_bActive; }
public:
	void					Set_Active(_bool active) { m_bActive = active; }
	void					Set_Index(_int index) { m_iCount = index; }
	void					Set_Amount(_float amount) { m_fAmount = amount; }
	void					Set_2DMap() { m_b3DMap = false; }

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

private :
	_vec3					m_vPos;
	_float					m_fAngle;
public:
	static CSpaceLaser*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg, _float fAngle = -90.f);

private:
	virtual void Free(void) override;

private:
	_float4x4				m_matColWorld;
	_float					m_fTimeDelta = 0.f;
	_int					m_iCount = 0;
	_float					m_fAmount = 0.f;
	_bool					m_b3DMap = true;

};

#endif // __SPACELASER_H__
