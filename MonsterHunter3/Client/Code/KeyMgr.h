#ifndef __KEYMGR_H__

#include "Engine_Define.h"
#include "Base.h"
#include "define.h"
BEGIN(Engine)

END
#define VK_MAX 0xFF
class CKeyMgr final : public CBase
{
	
	DECLARE_SINGLETON(CKeyMgr)
private:
	explicit CKeyMgr();
	virtual ~CKeyMgr(void);

public:
	void Key_Update();
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);


private:
	bool				m_bState[VK_MAX];

public:
	virtual void Free() override;
};




#define __KEYMGR_H__
#endif // !__KEYMGR_H__


