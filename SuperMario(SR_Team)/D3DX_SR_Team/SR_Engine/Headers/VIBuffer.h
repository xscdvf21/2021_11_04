#pragma once

#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	enum class eVertexType
	{
		Color,
		Textrue,
		Wireframe
	};


protected:
	explicit CVIBuffer(_Device pDevice);
	explicit CVIBuffer(const CVIBuffer& other);
	virtual ~CVIBuffer() = default;


public:
	virtual HRESULT Ready_Component_Prototype() PURE;
	virtual HRESULT Ready_Component(void* pArg = nullptr) PURE;
	virtual HRESULT Render_VIBuffer() PURE;
	//셋 트랜스폼 굳이 여따 안박아도 됨

public:
	eVertexType Get_VertexType();
	void		Set_VertexType(eVertexType eType);
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color,_uint iIndex = 0) PURE;
	void* Get_VertexData();
	void* Get_IndexData();
	HRESULT	Set_VertexData(void* _vertexData);
	//virtual void Set_IndexData(void* _indexData);

public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

protected:
	_Device m_pDevice;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	_uint m_iVertexSize;	//버텍스 "개당" 메모리 크기
	_uint m_iVertexCount;	//버텍스 개수
	_uint m_iFVF;			//FVF 방식
	_uint m_iTriCount;		//폴리곤 개수

	LPDIRECT3DINDEXBUFFER9 m_pIB;
	_uint m_iIndexSize; // 인덱스 개당 메모리 크기
	D3DFORMAT m_IndexFmt; // 인덱스 포맷. 16비트 or 32비트

	void* m_pVertexArr;		//버텍스 정보 저장
	void* m_pIndexArr;		//인덱스 정보 저장
	_bool		m_IsClone;
	eVertexType m_eVType;	//버텍스 타입
};

END

#endif // !__VIBUFFER_H__