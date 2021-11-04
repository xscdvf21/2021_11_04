#ifndef _BOSSTIMEBULLET_H__

#include "GameObject.h"
#include "define.h"
#include "Base.h"
#include "Player.h"


BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCollider;
class CSphere;
class CCalculator;


END



class CBossTimeBullet : public CGameObject
{
private:
	explicit CBossTimeBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossTimeBullet(void);


public:
	virtual HRESULT Ready_Object(void) override;
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			SetUp_Sphere(void);

	_bool			Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag, const _float& fTimeDelta);

public :
	_vec3			Get_PlayerDir() { return m_vDir; }


private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CSphere*		m_pSphere = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;


	const _matrix*			m_pParentBoneMatrix_Sphere;
	const _matrix*			m_pParentWorldMatrix_Sphere = nullptr;
	_matrix					m_pFinalMatrix_Sphere;

	_vec3					m_vBossLook = { 0.f, 0.f, 0.f }; //������ �����.
	_vec3					m_vDir = { 0.f, 0.f, 0.f }; //���ư� ���⺤��. //�÷��̾ ���� ����.

	_bool					m_bCollOne = false;
	
	_float fDelta = 0.f;
	_bool		m_bBulletWind = false;  //���� �� ���� 5�� �ҷ�����. ���󰥶� �����ϱ� ����.
	_vec3		m_vWindPos;				//�ҷ� ���� ���� �������, ������Ʈ���� �����⶧���� ����ܰ迡�� ���ڸ� ����������.
public:
	static CBossTimeBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f,0.f });

private:
	virtual void		Free(void) override;

};


#define _BOSSTIMEBULLET_H__
#endif // !_BOSSTIMEBULLET_H__






