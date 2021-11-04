#ifndef __BABOONLASER_H__


#include "Enermy.h"


BEGIN(Engine)

class CStaticMesh;
class CCollider;
class CCalculator;
class CShader;
class CTexture;

END

class CBaboonLaser : public CEnermy
{
	enum SoundID
	{
		Sound_Effect,
		Sound_End,
	};


private:
	explicit CBaboonLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaboonLaser();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, CGameObject* pTarget = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	void			Move(const _float& fTimeDelta);
public:
	void					Set_Active(_bool active) { m_bActive = active; }
	void					Set_Index(_int index) { m_iCount = index; }
	void					Set_Amount(_float amount) { m_fAmount = amount; }
	void					Set_Pos(const _vec3& vPos) { m_pTransformCom->Set_Pos(vPos); }
	void					Set_Dir(const _vec3& vDir) { m_pTransformCom->Set_Angle(vDir); }
	_bool					Get_Active() { return m_bActive; }

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

private:
	_vec3					m_vPos;
	_float					m_fAngle;


private:
	_bool						m_bSound[Sound_End] = { false };

public:
	static CBaboonLaser*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, CGameObject* pTarget = nullptr);
	virtual void						Free(void)			override;

private:
	_mat					m_matColWorld;
	_float					m_fTimeDelta = 0.f;
	_int					m_iCount = 0;
	_float					m_fAmount = 1.f;

	CGameObject*			m_pTarget;

	_vec3					m_vStartPos;
	_vec3					m_vEndPos;

	_float					m_fLifeTime;

	_vec3					m_vTargetPos;
};

#define __BABOONLASER_H__
#endif

