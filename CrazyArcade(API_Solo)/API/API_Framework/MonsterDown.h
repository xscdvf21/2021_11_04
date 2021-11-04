#pragma once
#ifndef __CMONSTERDOWN_H__
#define __CMONSTERDOWN_H__

#include "Obj.h"
class CMonsterDown : public CObj
{
public:
	CMonsterDown();
	virtual ~CMonsterDown();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private :
	DWORD			m_dwTime;
};


#endif // !__CMONSTERDOWN_H__



