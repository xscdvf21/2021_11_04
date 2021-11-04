#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(void);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);

public:
	enum eTransformMode
	{
		MODE_EULER,
		MODE_AXIS,
		MODE_FREEANGLE,
	};
public:
	void			Get_Info(INFO eType, _vec3* pInfo);
	const _matrix*	Get_WorldMatrix(void);
	void			Get_WorldMatrix(_matrix* pMatrix) const;
	_vec3			Get_Position() { return m_vInfo[INFO_POS]; }
	_vec3			Get_Scale() { return m_vScale; }
	_vec3			Get_Angle() { return m_vAngle; }
	_vec3			Get_RevAngle() { return m_vRevAngle; }
	_vec3			Get_Parent() { return m_vParent; }

	_vec3			Get_Look() { return m_vLook; }
	void			Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);
	void			Rotation(ROTATION eType, const _float& fAngle);
	void			Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	void			Chase_TargetY(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

public: // Set
	void			Set_Pos(PxTransform vPos) { m_vInfo[INFO_POS] = _vec3(vPos.p.x, vPos.p.y, vPos.p.z); }
	void			Set_Pos(_vec3 vPos) { m_vInfo[INFO_POS] = vPos; }
	void			Set_Pos(PxVec3 vPos) { m_vInfo[INFO_POS] = (_vec3&)vPos; }
	void			Set_Pos(_float fX, _float fY, _float fZ) { m_vInfo[INFO_POS].x = fX; m_vInfo[INFO_POS].y = fY; m_vInfo[INFO_POS].z = fZ; }
	void			Set_AddPos(_vec3 vPos) { m_vInfo[INFO_POS] += vPos; }
	void			Set_PosY(_float fY) { m_vInfo[INFO_POS].y = fY; }
	void			Set_PosZ(_float fZ) { m_vInfo[INFO_POS].z = fZ; }
	void			Set_Scale(const _float& fScale) { m_vScale = { fScale ,fScale ,fScale }; }
	void			Set_Scale(_vec3 vScale) { m_vScale = vScale; }
	void			Set_Scale(_float fX, _float fY, _float fZ) { m_vScale.x = fX; m_vScale.y = fY; m_vScale.z = fZ; }
	void			Set_AddScale(_vec3 vScale) { m_vScale += vScale; }
	void			Set_AngleX(_float fX) { m_vAngle.x = fX; }
	void			Set_AngleY(_float fY) { m_vAngle.y = fY; }
	void			Set_AngleZ(_float fZ) { m_vAngle.z = fZ; }
	void			Set_AddAngleX(_float fX) { m_vAngle.x += fX; }
	void			Set_AddAngleY(_float fY) { m_vAngle.y += fY; }
	void			Set_AddAngleZ(_float fZ) { m_vAngle.z += fZ; }
	void			Set_AddAngle(_vec3 vAngle) { m_vAngle += vAngle; }
	void			Set_Angle(_vec3 vAngle) { m_vAngle = vAngle; }
	void			Set_ParentMatrix(const _matrix& pParent) { m_matWorld *= pParent; }
	void			Set_WorldMatrix(const _matrix& pMatrix) { m_matWorld = pMatrix; }
	void			Set_WorldMatrix(const PxTransform& pMatrix);

	//공전
	void			Set_RevAngleX(_float fX) { m_vRevAngle.x = fX; }
	void			Set_RevAngleY(_float fY) { m_vRevAngle.y = fY; }
	void			Set_RevAngleZ(_float fZ) { m_vRevAngle.z = fZ; }
	void			Set_RevAddAngleX(_float fX) { m_vRevAngle.x += fX; }
	void			Set_RevAddAngleY(_float fY) { m_vRevAngle.y += fY; }
	void			Set_RevAddAngleZ(_float fZ) { m_vRevAngle.z += fZ; }
	void			Set_RevAddAngle(_vec3 vAngle) { m_vRevAngle += vAngle; }
	void			Set_RevAngle(_vec3 vAngle) { m_vRevAngle = vAngle; }

	void			Set_Parent(PxTransform vPos) { m_vParent = _vec3(vPos.p.x, vPos.p.y, vPos.p.z); }
	void			Set_Parent(_vec3 vPos) { m_vParent = vPos; }
	void			Set_Parent(_float fX, _float fY, _float fZ) { m_vParent.x = fX; m_vParent.y = fY; m_vParent.z = fZ; }
	void			Set_Mode(const _int& iMode) { m_iMode = iMode; }

	void			Set_Axis(const _vec3& vDir) { m_vAxis = vDir; }
	void			Set_RotationMatrix(const _mat& vDir) { m_matRot= vDir; }
	void			Set_Up(_vec3* vDir) { m_vUp = *vDir; }
	void			Set_Right(_vec3* vDir) { m_vRight = *vDir; }
	void			Set_Look(_vec3* vDir) { m_vLook = *vDir; }
public:
	HRESULT			Ready_Transform(void);
	virtual _int	Update_Component(const _float& fTimeDelta);

private:
	const _matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);

private:
	_vec3			m_vUp;
	_vec3			m_vRight;
	_vec3			m_vLook;
	_vec3			m_vAxis;
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;

	_vec3			m_vAngle;
	_vec3			m_vRevAngle;
	_vec3			m_vParent;
	_matrix			m_matWorld;

	//자유로운 회전용
	_matrix			m_matRot;
	////
	_qtnian			m_qQuat;
	_qtnian			m_RevqQuat;
	_int			m_iMode=0;
public:
	static CTransform*		Create(void);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
private:
	virtual void	Free(void);

};



END
#endif // Transform_h__
