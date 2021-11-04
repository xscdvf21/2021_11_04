#ifndef __ORB_H__
#define __ORB_H__

#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CCollider;
class CCalculator;
class CShader;

END

class COrb : public CPlayer
{
private:
	explicit COrb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COrb(void);

public:
	HRESULT					Ready_Object(char* pBoneName, void* pArg);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;


private:
	HRESULT					Add_Component(void* pArg);
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
	_bool					m_bActive = true;

	char					strBoneName[MAX_PATH] = "";

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static COrb*		Create(LPDIRECT3DDEVICE9 pGraphicDev, char* pBoneName, void* pArg);

private:
	virtual void Free(void) override;

private:
	_float4x4				m_matColWorld;

};

#endif // Hat_h__
