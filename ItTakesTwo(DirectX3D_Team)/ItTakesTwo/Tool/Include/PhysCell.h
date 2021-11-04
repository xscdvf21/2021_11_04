#pragma once

#ifndef PhysCell_h__
#define PhysCell_h__

#include "Engine_Define.h"
struct Vertex
{
	_vec3 vPos;
	_uint iUseCount;
};
class CPhysCell
{
public:
	CPhysCell();
	virtual ~CPhysCell();

	list<Vertex> m_vecVertex;
};
#endif // PhysCell_h__

