#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"

bool CCollisionMgr::m_LeftPadoColl = false;
bool CCollisionMgr::m_RightPadoColl = false;
bool CCollisionMgr::m_UpPadoColl = false;
bool CCollisionMgr::m_DownPadoColl = false;


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

 void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				/*Dst->Set_Dead();*/
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY)
				{
					if (Dst->Get_Info().fY < Src->Get_Info().fY+20)
						Src->Set_PosY(fY);
					else
						Src->Set_PosY(-fY);
				}
				else
				{
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj* _Dst, CObj* _Src)
{
	// abs()
	// -> 절대값을 구해주는 함수

	// sqrtf()
	// -> 제곱근을 구해주는 함수

	float fX = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDia = sqrtf(fX * fX + fY * fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);

	return fCX > fDia;
}


bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	float fDisX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fDisY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

	if (fCX > fDisX && fCY > fDisY)
	{
		*_x = fCX - fDisX;
		*_y = fCY - fDisY;
		return true;
	}

	return false;
}

void CCollisionMgr::Collision_SpeedItem(list<CObj*>& _Dst, list<CObj*>& _Src) //스피드를 증가시키기위해,
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_BoolSpeed();
				Src->Set_Dead();
			}
		}
	}
}



void CCollisionMgr::Collision_BoomItem(list<CObj*>& _Dst, list<CObj*>& _Src) //폭탄 갯수,
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_BoolBoom();
				Src->Set_Dead();
			}
		}
	}
}


void CCollisionMgr::Collision_PadoItem(list<CObj*>& _Dst, list<CObj*>& _Src) //물폭탄 파워 증가
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_BoolPado();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_Boom(list<CObj*>& _Dst, list<CObj*>& _Src) //물폭탄 겹쳐있는지
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_BoomCheck();
			}
		}
	}
}

void CCollisionMgr::Collision_BoomWave(list<CObj*>& _Dst, list<CObj*>& _Src) //파도에 물폭탄
{

	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Src->Set_BoomWave();
			}
		}
	}
} 



void CCollisionMgr::Collision_PadoAndPlayer(list<CObj*>& _Dst, list<CObj*>& _Src) // 파도와 플레이어 충돌 시, 플레이어 죽음
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Src->Set_PlayerDeadTime();
				Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Collision_PadoAndBlock2(CObj *& _Dst, list<CObj*>& _Src) //집들과 충돌 시 파도 생성 안함
{
	RECT rc = {};


	for (auto& Src : _Src)
	{
		if (IntersectRect(&rc, &_Dst->Get_Rect(), &Src->Get_Rect()))
		{
			_Dst->Set_Dead();
			return 1;
		}
	}
}



bool CCollisionMgr::Collision_PadoAndBlock2(CObj*& _Dst, list<CObj*>& _YellowBlock
	, list<CObj*>& _RedBlock, list<CObj*>& _Tree, list<CObj*>& _House
	, list<CObj*>& _TownBlock) //노란블럭과 빨간블럭 한번에 쓰기위해,
{
	RECT rc = {};


		for (auto& YellowBlock : _YellowBlock)
		{
			if (IntersectRect(&rc, &_Dst->Get_Rect(), &YellowBlock->Get_Rect()))
			{
				_Dst->Set_Dead();
				YellowBlock->Set_Dead();
				return 1;
			}
		}

		for (auto& RedBlock : _RedBlock)
		{
			if (IntersectRect(&rc, &_Dst->Get_Rect(), &RedBlock->Get_Rect()))
			{
				_Dst->Set_Dead();
				RedBlock->Set_Dead();
				return 1;
			}
		}

		for (auto& TownBlock : _TownBlock)
		{
			if (IntersectRect(&rc, &_Dst->Get_Rect(), &TownBlock->Get_Rect()))
			{
				_Dst->Set_Dead();
				TownBlock->Set_Dead();
				return 1;
			}
		}


		for (auto& Tree : _Tree)
		{
			if (IntersectRect(&rc, &_Dst->Get_Rect(), &Tree->Get_Rect()))
			{
				_Dst->Set_Dead();
				return 1;
			}
		}

		for (auto& House : _House)
		{
			if (IntersectRect(&rc, &_Dst->Get_Rect(), &House->Get_Rect()))
			{
				_Dst->Set_Dead();
				return 1;
			}
		}


	

}


void CCollisionMgr::Collision_PadoAndBlock(list<CObj*>& _Dst, list<CObj*>& _Src) // 파도와 블럭이 충돌
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
				
			}
		}
	}
}

void CCollisionMgr::Collision_BoomAndPlayer(list<CObj*>& _Dst, list<CObj*>& _Src) // 물폭탄과 플레이어가 충돌 시 물풍선을 못 설치하게 하기위해
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Src->Set_BoomAndPlayer();
				
			}
		}
	}
}

void CCollisionMgr::Collision_PadoAndHouse(list<CObj*>& _Dst, list<CObj*>& _Src) // 파도와 집 충돌 시 파도 삭제
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Dead();
				m_RightPadoColl = true;
				
			}
		}
	}
}

void CCollisionMgr::Collision_PadoAndBoss(list<CObj*>& _Dst, list<CObj*>& _Src) // 파도와 보스 충돌 시, 플레이어 죽음
{
	RECT rc = {};
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Src->Set_PlayerDeadTime();
				/*Src->Set_Dead();*/
			}
		}
	}
}



