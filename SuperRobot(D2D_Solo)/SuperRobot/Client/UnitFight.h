#pragma once
#include "GameObject.h"
class CUnitFight :
	public CGameObject
{
public:
	explicit CUnitFight();
	virtual ~CUnitFight();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	
public :
	void FrameMove(float fSpeed);

private :
	FRAME m_tFrame;

	float m_fx;
	float m_fsize;
	float m_fSpeed;
	float m_fSpeed2;
	float m_fAngle;

	int	iIndex;

	float m_fTime;
	bool  m_bCheck;
	bool m_bHpCheck;
	bool m_bSoundCheck; //���� ���� ���� üũ
	bool m_bSoundCheck2; //���� ���� �� ������ ����.


};

