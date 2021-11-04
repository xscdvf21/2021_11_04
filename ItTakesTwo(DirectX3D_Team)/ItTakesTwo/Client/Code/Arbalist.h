#ifndef __ARBALIST_H__

#include "Enermy.h"
#include "Cone.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CArbalist : public CEnermy
{

	enum animID {
		Idle,                                     //0	frame:[31]
		Walk,                                     //1	frame:[19]
		Attack_Start,                             //2	frame:[21]
		Attack,                                   //3	frame:[16]
		Attack_Idle,                              //4	frame:[31]
		Damage,                                   //5	frame:[15]
		Dead,                                     //6	frame:[21]
		End
	};


private:
	explicit CArbalist(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArbalist();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public :
	void			Set_Move(_bool bMove) { m_bMove = bMove; }
private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

private:
	void			PhysicsCreate();
public:
	void			Set_Damage(_int iDamage) override;

private:
	void			Hit_State(const _float& fTimeDelta);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	class CCrossBow*			m_pCrossBow = nullptr;
	class CArrow*				m_pArrow = nullptr;

	_bool						m_bCodySelect = false;
	_bool						m_bMaySelect = false;

	_float						m_fTurnSpeed; //�� ���ǵ�
	_float						m_fAngle;	 //�÷��̾���� ����

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iRandom;
	_uint						m_iAniNum = 0; //���� ����ǰ��ִ� �ִϸ��̼� ��ȣ ����

	_bool						m_bAttackStart = false; //���� ��ŸƮ�� �ѹ��� �ϱ�����
	_float						m_fAttackTime = 0.f; //���� ������ �ֱ� ����.

	_tchar						m_tArrowTag[MAX_PATH] = L""; //���� �̸��� �������־ ȭ�� �±׸� �����ֱ�����.
	_uint						m_iArrowCount = 0; //ȭ���±׸� �����̱�����.
	_float						m_fTimeDelta;
	CCone*						m_pCone = nullptr;

private:
	void						Create_Cone(const _tchar* tagLayer);


private :
	_bool						m_bMove;
public:
	static CArbalist*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual void Free(void)		override;
};

#define __ARBALIST_H__
#endif


