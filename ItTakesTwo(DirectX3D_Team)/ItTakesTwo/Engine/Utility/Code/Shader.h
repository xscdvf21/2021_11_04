#ifndef Shader_h__
#define Shader_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader(void);

public:
	LPD3DXEFFECT			Get_EffectHandle(void) { return m_pEffect; }
public:
	HRESULT					Ready_Shader(const _tchar* pFilePath);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXEFFECT			m_pEffect;		// 쉐이더 대표 컴객체
	LPD3DXBUFFER			m_pErrMsg;		// hlsl 언어를 컴파일 이후 발생하는 에러 메세지 상태를 저장하기 위한 메모리 공간

public:
	static CShader*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual CComponent*		Clone(void* pArg = nullptr);
	virtual void			Free(void);

};

END
#endif // Shader_h__