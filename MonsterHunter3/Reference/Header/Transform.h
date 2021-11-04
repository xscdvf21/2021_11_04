#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

enum class EAxis
{
	X, Y, Z
};

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(void);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);


public:
	void			Get_Info(INFO eType, _vec3* pInfo);
	const _matrix*	Get_WorldMatrix(void);
	void			Get_WorldMatrix( _matrix* pMatrix) const;
	void			Get_NRotWorldMatrix(_matrix* pMatrix) const;
	void			Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);
	void			Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void			Rotation(ROTATION eType, const _float& fAngle);

	void			Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	
	void			Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void			Set_Pos(const _vec3& vPos);
	
	void			Set_Scale(const _vec3& vScale);
	void			Set_Scale(const _float& fScaleX, _float& fScaleY, _float& fScaleZ);

	void			Set_Rotation(EAxis eAxis, float fRot);
	void			Add_Rotation(EAxis eAxis, float fRot, const _float& fDelta = 1);

	void			Set_ParentMatrix(const _matrix* pParent);
	void			Set_WorldMatrix(const _matrix * pMatrix);

public:
	HRESULT			Ready_Transform(void);
	virtual _int	Update_Component(const _float& fTimeDelta);

private:
	const _matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;
	_matrix			m_matNRotWorld;

public:
	static CTransform*		Create(void);
	virtual CComponent*		Clone(void);
private:
	virtual void	Free(void);

};



END
#endif // Transform_h__
