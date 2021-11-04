#pragma once
#include "GameObject.h"
USING(Engine)
class CEffectParticle :	public CGameObject
{
private:
	explicit CEffectParticle(LPDIRECT3DDEVICE9 pDevice);
	explicit CEffectParticle(const CEffectParticle& other);
	virtual ~CEffectParticle() = default;


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CEffectParticle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CGameObject * CloneToClone(CEffectParticle* Clone);
	virtual void Free() override;

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	HRESULT Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex);

#pragma region set
public:
	void Set_Position(D3DXVECTOR3 _vStartPoint);
	void Set_Scale(D3DXVECTOR3 _vScaleStart, D3DXVECTOR3 _vScaleEnd);
	void Set_Color(D3DXVECTOR3 _vColorStart, D3DXVECTOR3 _vColorEnd, bool _bMulti);
	void Set_Rotation(int _eAxis, float _Rot);
	void Set_Jump(bool _bJump, float _fJumpPower, D3DXVECTOR3 _vShot);
	void Set_Accel(float _fAccel) { m_fAccel = _fAccel; }
	void Set_LifeTime(float _fLifeTime) { m_fLifeTime = _fLifeTime; }
	void Set_Start(){ m_bStart = true; }

	void Set_vStartPoint(_float3 _vPos)
	{
		m_vStartPoint = _vPos;
		Set_Position(m_vStartPoint);
	}
#pragma endregion
public:
	bool Get_Dead() { m_bDead; }



private:
	void Ready_Color();
	void Ready_Scale();

	void Update_Color(float _fDeltaTime);
	void Update_Scale(float _fDeltaTime);
	void Update_Rotation(float _fDeltaTime);
	void Update_Trans(float _fDeltaTime);
	void Jumping(float _fDeltaTime);

#pragma region 변수
private:
	bool m_bDead = false;
	//사이즈용
	D3DXVECTOR3	m_vScaleStart;//
	D3DXVECTOR3	m_vScaleEnd;//
	D3DXVECTOR3 m_vScaleAdd;

								//컬러용
	bool	m_bMultiColor;//
	D3DXVECTOR3	m_vColorStart;//
	D3DXVECTOR3 m_vColorEnd;//
	D3DXVECTOR3 m_vColorAdd;

								//회전용
	float		m_fRotation;
	enum RotationAxis { X, Y, Z, XY, XZ, YZ, XYZ, NUN };
	RotationAxis e_Axis;//

							//라이프타임.
	float	m_fLifeTime;//

							//중력 여부.
	bool			m_bJumpStart;
	bool			m_bJump;//
	float			m_fJumpPowerY;//
	float			m_fJumpPowerXZ;
	float			m_fJumpTime;
	D3DXVECTOR3		m_vShotting;//
	D3DXVECTOR3		m_vJumpStartPoint;
	D3DXVECTOR3		m_vJumpingPoint;

	D3DXVECTOR3		m_vStartPoint;

	bool			m_bStart;
	bool			m_bStop = false;

	float			m_fAccel;
#pragma endregion
};

