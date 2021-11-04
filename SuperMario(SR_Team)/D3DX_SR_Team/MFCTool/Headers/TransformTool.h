#pragma once
#include "ComponentTool.h"
#include "PipelineStaticsTool.h"

BEGIN(Engine)
enum class EAxis
{
	X, Y, Z, XY, XZ, YZ, XYZ
};
END

USING(Engine)
class CTransformTool :
	public CComponentTool
{
public:
	typedef struct tagTransformDesc
	{
		_float3 vScale, vRotate, vPosition;
		_float4x4 matWorld;
		_float fSpeedPerSecond, fRotatePerSecond;
	}TRANSFORM_DESC;

private:
	explicit CTransformTool(LPDIRECT3DDEVICE9 pDevice);
	explicit CTransformTool(const CTransformTool& other);
	virtual ~CTransformTool() = default;

public:
	const _float3& Get_Position() const;
	const _float4x4& Get_WorldMatrix() const;
	const _float3& Get_Scale()const;
	const _float3& Get_Rotation()const;

public:
	void Set_Position(const _float3& vPos);
	void Set_WorldMatrix(const _float4x4& matWorld);
	void Set_Scale(const _float3& vScale);
	void Set_RelativeScale(const _float3 & vScale);
	void Set_Rotation(const _float3& vRotation);

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;



public:
	_uint Update_Transform();
	HRESULT Set_Transform();

public:
	const _float3 Get_AbsScale() const
	{
		return _float3(fabs(m_TransformDesc.vScale.x), fabs(m_TransformDesc.vScale.y), fabs(m_TransformDesc.vScale.z));
	}


public:
	void Go_Straight(_float fDeltaTime);
	void Go_Side(_float fDeltaTime);
	void Rotate(EAxis eAxis, _float fDeltaTime);
	void Rotate_Quick(EAxis eAxis, _float _fAngle);
	void Move_ToDirection(const _float3& vMoveDir, _float fDeltaTime);

public:
	static CTransformTool* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponentTool * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	TRANSFORM_DESC	m_TransformDesc;
	
};

