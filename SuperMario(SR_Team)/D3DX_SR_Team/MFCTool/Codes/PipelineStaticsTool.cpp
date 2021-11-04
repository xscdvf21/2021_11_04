#include "stdafx.h"
#include "..\Headers\PipelineStaticsTool.h"


CPipelineStaticsTool::CPipelineStaticsTool()
{
}


CPipelineStaticsTool::~CPipelineStaticsTool()
{
}

HRESULT CPipelineStaticsTool::Setup_StateMatrix(_float4x4 * pOut, const _float3 * pRight, const _float3 * pUp, const _float3 * pLook, const _float3 * pPosition)
{
	if (!pOut || !pRight || !pUp || !pLook || !pPosition)
		return E_FAIL;

	D3DXMatrixIdentity(pOut);

	memcpy(&pOut->m[0][0], pRight, sizeof(_float3));
	memcpy(&pOut->m[1][0], pUp, sizeof(_float3));
	memcpy(&pOut->m[2][0], pLook, sizeof(_float3));
	memcpy(&pOut->m[3][0], pPosition, sizeof(_float3));

	return S_OK;
}

HRESULT CPipelineStaticsTool::Setup_WorldMatrix(_float4x4 * pOut, const _float3 * pScale, const _float3 * pRotate, const _float3 * pPosition, const _float3 * pRevol, const _float4x4 * pParent)
{
	if (!pOut || !pScale || !pRotate || !pPosition)
		return E_FAIL;

	D3DXMatrixIdentity(pOut);

	_float3 vRight = _float3(1.f, 0.f, 0.f);
	_float3 vUp = _float3(0.f, 1.f, 0.f);
	_float3 vLook = _float3(0.f, 0.f, 1.f);

	/* Scale */
	vRight.x *= pScale->x;
	vUp.y *= pScale->y;
	vLook.z *= pScale->z;

	/* Rotate */
	MyVec3RotationX(&vRight, &vRight, pRotate->x);
	MyVec3RotationY(&vRight, &vRight, pRotate->y);
	MyVec3RotationZ(&vRight, &vRight, pRotate->z);

	MyVec3RotationX(&vUp, &vUp, pRotate->x);
	MyVec3RotationY(&vUp, &vUp, pRotate->y);
	MyVec3RotationZ(&vUp, &vUp, pRotate->z);

	MyVec3RotationX(&vLook, &vLook, pRotate->x);
	MyVec3RotationY(&vLook, &vLook, pRotate->y);
	MyVec3RotationZ(&vLook, &vLook, pRotate->z);

	if (FAILED(Setup_StateMatrix(pOut, &vRight, &vUp, &vLook, pPosition)))
		return E_FAIL;

	if (pRevol)
	{
		_float3 vResult;
		for (_uint i = 0; i < 4; ++i)
		{
			memcpy(&vResult, &pOut->m[i][0], sizeof(_float3));

			MyVec3RotationX(&vResult, &vResult, pRevol->x);
			MyVec3RotationY(&vResult, &vResult, pRevol->y);
			MyVec3RotationZ(&vResult, &vResult, pRevol->z);

			memcpy(&pOut->m[i][0], &vResult, sizeof(_float3));
		}
	}

	if (pParent)
	{
		*pOut *= *pParent;
	}

	return S_OK;
}

HRESULT CPipelineStaticsTool::Setup_ViewMatrix(_float4x4 * pOut, const _float3 * pEye, const _float3 * pAt, const _float3 * pUp)
{
	/* 카메라의 월드상 right, up, look, position을 구해 행렬을 구성한다. */
	_float3 vRight, vUp, vLook;

	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	if (FAILED(Setup_StateMatrix(pOut, &vRight, &vUp, &vLook, pEye)))
		return E_FAIL;

	/* 위에서 만든 카메라 행렬을 역행렬로 만들어주면 뷰행렬이 된다. */
	D3DXMatrixInverse(pOut, 0, pOut);

	return S_OK;
}

HRESULT CPipelineStaticsTool::Setup_ProjectionMatrix(_float4x4 * pOut, _float fFovY, _float fAspect, _float fNear, _float fFar)
{
	D3DXMatrixIdentity(pOut);

	_float fScale = 1.f / tanf(fFovY * 0.5f);

	/*
	Scale원소들은 시야각 90도 환경으로 만들어진 투영스페이스 상에서
	여러 시야각을 표현하기위해 정점들을 줄였다 늘렸다 하도록하는 원소다.
	*/
	pOut->_11 = fScale / fAspect; /* 뷰포트로 변환될 때 스케일 왜곡을 막기위해 미리 축소 시켜놓는다. */
	pOut->_22 = fScale;

	/*
	뷰 스페이스의 Near ~ Far 범위의 값을 0 ~ Far 범위로 변환하는 원소.
	추후 Z나누기를 했을 때 Near ~ Far의 범위가 0.f ~ 1.f 범위가 되어야 하니깐.
	추후 0.f ~ 1.f 범위로 변환된 Z값은 Z버퍼에 기록된다.
	*/
	pOut->_33 = fFar / (fFar - fNear);
	pOut->_43 = -(fNear * fFar) / (fFar - fNear);

	/*
	투영행렬을 곱한 결과벡터의 w자리에 원래 벡터가 갖고있던 z값을 보관하기위한 행렬 원소
	한마디로 추후 Z나누기를 위해서 Z값을 보존.
	*/
	pOut->_34 = 1.f;
	pOut->_44 = 0.f;

	return S_OK;
}

HRESULT CPipelineStaticsTool::MyVec3RotationX(_float3 * pOut, const _float3 * pIn, _float fRadian)
{
	/*
	1	0	0	0
	0	c	s	0
	0	-s	c	0
	0	0	0	1
	*/
	_float3 vTemp = *pIn;

	pOut->y = vTemp.y * cosf(fRadian) + vTemp.z * -sinf(fRadian);
	pOut->z = vTemp.y * sinf(fRadian) + vTemp.z * cosf(fRadian);

	return S_OK;
}

HRESULT CPipelineStaticsTool::MyVec3RotationY(_float3 * pOut, const _float3 * pIn, _float fRadian)
{
	/*
	c	0	-s	0
	0	1	0	0
	s	0	c	0
	0	0	0	1
	*/
	_float3 vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fRadian) + vTemp.z * sinf(fRadian);
	pOut->z = vTemp.x * -sinf(fRadian) + vTemp.z * cosf(fRadian);

	return S_OK;
}

HRESULT CPipelineStaticsTool::MyVec3RotationZ(_float3 * pOut, const _float3 * pIn, _float fRadian)
{
	/*
	c	s	0	0
	-s	c	0	0
	0	0	1	0
	0	0	0	1
	*/
	_float3 vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fRadian) + vTemp.y * -sinf(fRadian);
	pOut->y = vTemp.x * sinf(fRadian) + vTemp.y * cosf(fRadian);

	return S_OK;
}

void CPipelineStaticsTool::MyTransformCoord(_float3 * pOut, const _float3 * pIn, const _float4x4 * pMatrix)
{
	_float4 vec4 = _float4(pIn->x, pIn->y, pIn->z, 1.f);

	pOut->x = vec4.x * pMatrix->_11 + vec4.y * pMatrix->_21 + vec4.z * pMatrix->_31 + vec4.w * pMatrix->_41;
	pOut->y = vec4.x * pMatrix->_12 + vec4.y * pMatrix->_22 + vec4.z * pMatrix->_32 + vec4.w * pMatrix->_42;
	pOut->z = vec4.x * pMatrix->_13 + vec4.y * pMatrix->_23 + vec4.z * pMatrix->_33 + vec4.w * pMatrix->_43;

	// Z나누기
	_float z = vec4.x * pMatrix->_14 + vec4.y * pMatrix->_24 + vec4.z * pMatrix->_34 + vec4.w * pMatrix->_44;
	(*pOut) /= z;
}
