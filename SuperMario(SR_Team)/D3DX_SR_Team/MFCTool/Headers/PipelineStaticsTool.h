#pragma once
#ifndef __PipeTool_H__
#define __PipeTool_H__
#include "Base.h"

USING(Engine)

class CPipelineStaticsTool
{
private:
	CPipelineStaticsTool();
	~CPipelineStaticsTool();

public:
	static HRESULT Setup_StateMatrix(_float4x4* pOut, const _float3* pRight, const _float3* pUp, const _float3* pLook, const _float3* pPosition);
	static HRESULT Setup_WorldMatrix(_float4x4* pOut, const _float3* pScale, const _float3* pRotate, const _float3* pPosition, const _float3* pRevol = nullptr, const _float4x4* pParent = nullptr);
	static HRESULT Setup_ViewMatrix(_float4x4* pOut, const _float3* pEye, const _float3* pAt, const _float3* pUp);
	static HRESULT Setup_ProjectionMatrix(_float4x4* pOut, _float fFovY, _float fAspect, _float fNear, _float fFar);
	static HRESULT MyVec3RotationX(_float3* pOut, const _float3* pIn, _float fRadian);
	static HRESULT MyVec3RotationY(_float3* pOut, const _float3* pIn, _float fRadian);
	static HRESULT MyVec3RotationZ(_float3* pOut, const _float3* pIn, _float fRadian);
	static void MyTransformCoord(_float3* pOut, const _float3* pIn, const _float4x4* pMatrix);
};


#endif // !__PipeTool_H__
