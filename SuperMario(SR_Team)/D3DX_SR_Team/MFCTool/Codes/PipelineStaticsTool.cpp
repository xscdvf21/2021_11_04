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
	/* ī�޶��� ����� right, up, look, position�� ���� ����� �����Ѵ�. */
	_float3 vRight, vUp, vLook;

	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	if (FAILED(Setup_StateMatrix(pOut, &vRight, &vUp, &vLook, pEye)))
		return E_FAIL;

	/* ������ ���� ī�޶� ����� ����ķ� ������ָ� ������� �ȴ�. */
	D3DXMatrixInverse(pOut, 0, pOut);

	return S_OK;
}

HRESULT CPipelineStaticsTool::Setup_ProjectionMatrix(_float4x4 * pOut, _float fFovY, _float fAspect, _float fNear, _float fFar)
{
	D3DXMatrixIdentity(pOut);

	_float fScale = 1.f / tanf(fFovY * 0.5f);

	/*
	Scale���ҵ��� �þ߰� 90�� ȯ������ ������� ���������̽� �󿡼�
	���� �þ߰��� ǥ���ϱ����� �������� �ٿ��� �÷ȴ� �ϵ����ϴ� ���Ҵ�.
	*/
	pOut->_11 = fScale / fAspect; /* ����Ʈ�� ��ȯ�� �� ������ �ְ��� �������� �̸� ��� ���ѳ��´�. */
	pOut->_22 = fScale;

	/*
	�� �����̽��� Near ~ Far ������ ���� 0 ~ Far ������ ��ȯ�ϴ� ����.
	���� Z�����⸦ ���� �� Near ~ Far�� ������ 0.f ~ 1.f ������ �Ǿ�� �ϴϱ�.
	���� 0.f ~ 1.f ������ ��ȯ�� Z���� Z���ۿ� ��ϵȴ�.
	*/
	pOut->_33 = fFar / (fFar - fNear);
	pOut->_43 = -(fNear * fFar) / (fFar - fNear);

	/*
	��������� ���� ��������� w�ڸ��� ���� ���Ͱ� �����ִ� z���� �����ϱ����� ��� ����
	�Ѹ���� ���� Z�����⸦ ���ؼ� Z���� ����.
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

	// Z������
	_float z = vec4.x * pMatrix->_14 + vec4.y * pMatrix->_24 + vec4.z * pMatrix->_34 + vec4.w * pMatrix->_44;
	(*pOut) /= z;
}
