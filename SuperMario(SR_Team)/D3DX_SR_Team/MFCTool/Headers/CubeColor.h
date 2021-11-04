#pragma once
#include "GameObj.h"
#include "ManagementTool.h"
#include "TransformTool.h"
#include "VIBufferTool.h"
#include "VIBuffer_CubeColorTool.h"

class CCubeColor :
	public CGameObj
{
private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeColor(const CCubeColor& other);
	virtual ~CCubeColor() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObj * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	class CVIBufferTool*	m_pVIBufferCom = nullptr;
	class CTransformTool*	m_pTransformCom = nullptr;

public:
	HRESULT Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex = 0);

public:			//Tap03과 소통용.
	HRESULT	Set_Position(D3DXVECTOR3 _vStartPoint);
	HRESULT Set_Scale(D3DXVECTOR3 _vScaleStart, D3DXVECTOR3 _vScaleEnd);
	void Set_Color(D3DXVECTOR3 _vColorStart, D3DXVECTOR3 _vColorEnd, bool _bMulti);
	void Set_Rotation(bool _RotX, bool _RotY, bool _RotZ, float _Rot);
	void Set_Jump(bool _bJump, float _fJumpPower, D3DXVECTOR3 _vShot);
	void Set_Accel(float _fAccel) { m_fAccel = _fAccel; }
	void Set_LifeTime(float _fLifeTime) { m_fLifeTime = _fLifeTime; }
	void Set_Start() { m_bStart = true; }
	void Update_Color();
	void Update_Scale();
	void Update_Roation();
	bool Get_Dead() { m_bDead; }

public:
	void Save();

private:
	void Jumping();

public:
	//사이즈용
	D3DXVECTOR3	m_vScaleStart;//
	D3DXVECTOR3	m_vScaleEnd;//
	D3DXVECTOR3 m_vScaleAdd;

	D3DXVECTOR3	m_vSaveScaleStart;//★
	D3DXVECTOR3	m_vSaveScaleEnd;//★

	//컬러용
	bool	m_bMultiColor;//
	D3DXVECTOR3	m_vColorStart;//
	D3DXVECTOR3 m_vColorEnd;//
	D3DXVECTOR3 m_vColorAdd;

	bool	m_bSaveMultiColor;//★
	D3DXVECTOR3	m_vSaveColorStart;//★
	D3DXVECTOR3 m_vSaveColorEnd;//★
	
	//회전용
	float		m_fRotation;
	float		m_fSaveRotation;//★
	enum RotationAxis { X, Y, Z, XY, XZ, YZ, XYZ, NUN };
	RotationAxis e_Axis;//
	RotationAxis e_SaveAxis;//★

	//라이프타임.
	float	m_fLifeTime;//
	float	m_fSaveLifeTime;//★

	//중력 여부.
	bool			m_bJumpStart;
	bool			m_bJump;//
	float			m_fJumpPowerY;//
	float			m_fJumpPowerXZ;
	float			m_fJumpTime;
	D3DXVECTOR3		m_vShotting;//
	D3DXVECTOR3		m_vJumpStartPoint;
	D3DXVECTOR3		m_vJumpingPoint;

	bool			m_bSaveJump;//★
	float			m_fSaveJumpPowerY;//★
	D3DXVECTOR3		m_vSaveShotting;//★

	bool			m_bStart;
	bool			m_bStop = false;
	bool			m_bSave = false;

	float			m_fAccel;
	float			m_fSaveAccel;

};

