#ifndef __NAVIMGR_H__

#include "Engine_Define.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CNaviMesh;

END

class CNaviMgr : public CBase
{

	DECLARE_SINGLETON(CNaviMgr)

private:
	explicit CNaviMgr();
	virtual ~CNaviMgr();

public:
	HRESULT	Add_NaviMesh(_int iStageIdx, _uint iType, CNaviMesh* pNaviMesh);
	CNaviMesh* Get_NaviMesh(_int iStageIdx, _uint iType);


public:
	virtual void Free() override;

private:
	map<_int, map<_uint, Engine::CNaviMesh*>> m_mapNaviList;



};



#define __NAVIMGR_H__
#endif // !__NAVIMGR_H__


