#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)
enum class EAxis
{
	X, Y, Z
};

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDescription
	{
		_float3 vScale, vRotate, vPos;
		_mat	matWorld;
		_float	fSpeed, fRotate;

	}TRANSFORM_DESC;
private :
	explicit CTransform(_Device pDevice);
	explicit CTransform(const CTransform& other);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
public:
	HRESULT Set_Transform();
	_uint Update_Transform();

public://Set
	void			Set_Position(_float3 _vPos);
	void			Set_AxisScale(EAxis _eAxis, _float _fValue);
	void			Set_Scale(_float3& _vScale);
	void			Set_Rotation(_float3& _Rot);
	void			Set_Rotation(EAxis eAxis, float _Rot);
	void			Set_WorldMatrix(const _float4x4& _matWorld);
	void			Set_PosY(_float _fY) { m_TransformDesc.vPos.y = _fY; }
	void			Set_XScale(bool _bInverce);

public://Initialize Set
#pragma region Ready 단계에서 딱 한번만 쓰는 함수.
	void			Initialize_vScale(const _float3 _vScals);
	void			Initialize_vRotate(const _float3 _vRotate);
	void			Initialize_vPos(const _float3 _vPos);
	void			Initialize_matWorld(const _float4x4 _matWorld);
	void			Initialize_fSpeed(const _float _fSpeed);
	void			Initialize_fRotate(const _float _fRotate);
#pragma endregion

public://Get
	const _float3&	Get_Position() const;
	const _float	Get_Rotate() const;
	const _mat&		Get_WorldMatrix() const;
	const _float3   Get_Scale()const;
	const _float&	Get_vScale(EAxis _eAxis)const;



public:
	//근데 얘 애초에 자동으로 돌지 않았나?
	void Go_Straight(_float fDeltaTime);
	void Go_AxisStraight(EAxis _eAxis, _float fDeltaTime);
	void Go_Side(_float fDeltaTime);
	void Go_Jump(_float fDeltaTime);
	void Rotate(EAxis eAxis, _float fDeltaTime);



//충돌을 위한 함수 추가.
	const _float3 Get_AbsScale() const
	{ return _float3(fabs(m_TransformDesc.vScale.x), fabs(m_TransformDesc.vScale.y), fabs(m_TransformDesc.vScale.z)); }

	void AddPostion(_float3 vPos);

public:
	static CTransform* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	TRANSFORM_DESC	m_TransformDesc;

};

END

#endif