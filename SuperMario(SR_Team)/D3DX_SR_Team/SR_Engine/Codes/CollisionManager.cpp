#include"CollisionManager.h"
#include "Transform.h"

Engine::CCollisionManager::CCollisionManager()
{
}


void Engine::CCollisionManager::Collision_RectEx_Push(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{	//미는 대상.
	for (auto& Dst : _Dst) // 대상의 메모리를 받고.
	{	// 밀리는 대상.
		for (auto& Src : _Src) // 대상의 메모를 받고.
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY, &fZ)) // check_Rect를 함.
			{
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y < Src->Get_Transform()->Get_Position().y)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fX > fZ && fY > fZ)	
				{
					if (Dst->Get_Transform()->Get_Position().z < Src->Get_Transform()->Get_Position().z)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fZ > fX && fY > fX)		
				{
					if (Dst->Get_Transform()->Get_Position().x < Src->Get_Transform()->Get_Position().x)
						Src->Set_X(fX);
					else
						Src->Set_X(-fX);
				}
			}
		}
	}
}

bool Engine::CCollisionManager::Check_Rect(CGameObject * _pDst, CGameObject * _pSrc, float * _x, float* _y, float * _z)
{
	float fX = fabs(_pDst->Get_Transform()->Get_Position().x - _pSrc->Get_Transform()->Get_Position().x);
	float fY = fabs(_pDst->Get_Transform()->Get_Position().y - _pSrc->Get_Transform()->Get_Position().y);
	float fZ = fabs(_pDst->Get_Transform()->Get_Position().z - _pSrc->Get_Transform()->Get_Position().z);
	// 반지름 합
	float fCX = ((_pDst->Get_Transform()->Get_AbsScale().x + _pSrc->Get_Transform()->Get_AbsScale().x) *0.5f);
	float fCY = ((_pDst->Get_Transform()->Get_AbsScale().y + _pSrc->Get_Transform()->Get_AbsScale().y) *0.5f);
	float fCZ = ((_pDst->Get_Transform()->Get_AbsScale().z + _pSrc->Get_Transform()->Get_AbsScale().z) *0.5f);

	if (fCX > fX && fCY> fY && fCZ > fZ )
	{
		*_x = fCX - fX;
		*_y = fCY - fY;
		*_z = fCZ - fZ;
		return true;
	}
	return false;

}

void Engine::CCollisionManager::Collision_RectEx_PlayerAndMonster(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY, &fZ))
			{
				Dst->Set_PlayerAndMonster();
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y < Src->Get_Transform()->Get_Position().y)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fX > fZ && fY > fZ)
				{
					if (Dst->Get_Transform()->Get_Position().z < Src->Get_Transform()->Get_Position().z)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fZ > fX && fY > fX)
				{
					if (Dst->Get_Transform()->Get_Position().x < Src->Get_Transform()->Get_Position().x)
						Src->Set_X(fX);
					else
						Src->Set_X(-fX);
				}
			}
		}
	}
}

void Engine::CCollisionManager::Collision_RectEx_PlayerAndCoin(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY, &fZ))
			{
				Src->Set_Dead();
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y < Src->Get_Transform()->Get_Position().y)
						Src->Set_Z(fZ);
					else
						Src->Set_Z(-fZ);
				}
				else if (fX > fZ && fY > fZ)
				{
					if (Dst->Get_Transform()->Get_Position().z < Src->Get_Transform()->Get_Position().z)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fZ > fX && fY > fX)
				{
					if (Dst->Get_Transform()->Get_Position().x < Src->Get_Transform()->Get_Position().x)
						Src->Set_X(fX);
					else
						Src->Set_X(-fX);
				}
			}
		}
	}
}

void Engine::CCollisionManager::Collision_RectEx_BulletAndMonster(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY, &fZ))
			{
				Dst->Set_Dead();
				Src->Set_BulletAndMonster();
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y < Src->Get_Transform()->Get_Position().y)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fX > fZ && fY > fZ)
				{
					if (Dst->Get_Transform()->Get_Position().z < Src->Get_Transform()->Get_Position().z)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fZ > fX && fY > fX)
				{
					if (Dst->Get_Transform()->Get_Position().x < Src->Get_Transform()->Get_Position().x)
						Src->Set_X(fX);
					else
						Src->Set_X(-fX);
				}
			}
		}
	}
}

void Engine::CCollisionManager::Collision_RectEx_BulletAndPlayer(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY, &fZ))
			{
				Dst->Set_Dead();
				Src->Set_BulletAndPlayer();
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y < Src->Get_Transform()->Get_Position().y)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fX > fZ && fY > fZ)
				{
					if (Dst->Get_Transform()->Get_Position().z < Src->Get_Transform()->Get_Position().z)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fZ > fX && fY > fX)
				{
					if (Dst->Get_Transform()->Get_Position().x < Src->Get_Transform()->Get_Position().x)
						Src->Set_X(fX);
					else
						Src->Set_X(-fX);
				}
			}
		}
	}
}

void Engine::CCollisionManager::Collision_RectEx_BattleMonsterAndPlayer(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Rect(Dst, Src, &fX, &fY, &fZ))
			{
				Dst->Set_Dead();
				Dst->Set_BattleMonsterAndPlayer();
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y < Src->Get_Transform()->Get_Position().y)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fX > fZ && fY > fZ)
				{
					if (Dst->Get_Transform()->Get_Position().z < Src->Get_Transform()->Get_Position().z)
						Src->Set_Z(fZ);

					else
						Src->Set_Z(-fZ);
				}
				else if (fZ > fX && fY > fX)
				{
					if (Dst->Get_Transform()->Get_Position().x < Src->Get_Transform()->Get_Position().x)
						Src->Set_X(fX);
					else
						Src->Set_X(-fX);
				}
			}
		}
	}
}





void Engine::CCollisionManager::Collision_Terrain(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	_uint iCollChecker = 0;
	for (auto& Dst : _Dst) // 대상의 메모리를 받고.
	{
		for (auto& Src : _Src) // 대상의 메모를 받고.
		{

			if (Check_Terrain_Scaffolding(Dst, Src))
			{
				++iCollChecker;
				Dst->Set_SinkholeCheck(false);
			}

			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Check_Terrain(Dst, Src, &fX, &fY, &fZ)) // check_Rect를 함.
			{
				if (fX > fY && fZ > fY)
				{
					if (Dst->Get_Transform()->Get_Position().y > Src->Get_Transform()->Get_Position().y)
					{
						++iCollChecker;

						_float fValue = Dst->Get_Transform()->Get_Position().y + fY;
						
						Dst->Set_StartPosY(fValue);
						Dst->Set_SinkholeCheck(false);
					}
					else
					{
						Dst->Set_Y(-fY);
					}
				}
				else if (fX > fZ && fY > fZ)	// z 충돌
				{
					if (Dst->Get_Transform()->Get_Position().z > Src->Get_Transform()->Get_Position().z)
					{
						Dst->Set_Z(fZ);
					}
					else
					{
						Dst->Set_Z(-fZ);
					}
				}
				else if (fZ > fX && fY > fX)
				{
					if (Dst->Get_Transform()->Get_Position().x > Src->Get_Transform()->Get_Position().x)
					{
						Dst->Set_X(fX);
					}
					else
					{
						Dst->Set_X(-fX);
					}
				}//충돌 이벤트 끝
			}//Check_Terrain 끝

			_float ffValue = Dst->Get_Transform()->Get_Position().y;
		}//src 끝
		
		_float ffValue = Dst->Get_StartPosY();

		if (0 == iCollChecker)
		{
			Dst->Set_StartPosY(0.f);
			Dst->Set_SinkholeCheck(true);
		}
			

	}//dst 끝
}

bool Engine::CCollisionManager::Check_Terrain(CGameObject * _pDst, CGameObject * _pSrc, float * _x, float* _y, float * _z )
{
	//대상과의 거리.
	float fX = fabs(_pDst->Get_Transform()->Get_Position().x - _pSrc->Get_Transform()->Get_Position().x);
	float fY = fabs(_pDst->Get_Transform()->Get_Position().y - _pSrc->Get_Transform()->Get_Position().y);
	float fZ = fabs(_pDst->Get_Transform()->Get_Position().z - _pSrc->Get_Transform()->Get_Position().z);

	// 반지름 합
	float fCX = (float)((_pDst->Get_Transform()->Get_AbsScale().x + _pSrc->Get_Transform()->Get_AbsScale().x) * 0.5);
	float fCY = (float)((_pDst->Get_Transform()->Get_AbsScale().y + _pSrc->Get_Transform()->Get_AbsScale().y) * 0.5);
	float fCZ = (float)((_pDst->Get_Transform()->Get_AbsScale().z + _pSrc->Get_Transform()->Get_AbsScale().z) * 0.5);


	if (fCX > fX && fCZ > fZ && fCY > fY)
	{
		*_x = fCX - fX;
 		*_y	= fCY - fY;
 		*_z = fCZ - fZ;
		return true;
	}
	return false;
}

bool Engine::CCollisionManager::Check_Terrain_Scaffolding(CGameObject * _pDst, CGameObject * _pSrc)
{
	float fX = fabs(_pDst->Get_Transform()->Get_Position().x - _pSrc->Get_Transform()->Get_Position().x);
	float fY = fabs((_pDst->Get_Transform()->Get_Position().y - 0.5f) - _pSrc->Get_Transform()->Get_Position().y);
	float fZ = fabs(_pDst->Get_Transform()->Get_Position().z - _pSrc->Get_Transform()->Get_Position().z);

	float fCX = (float)(((_pDst->Get_Transform()->Get_AbsScale().x * 0.5f) + _pSrc->Get_Transform()->Get_AbsScale().x)*0.5f);
	float fCY = (float)(((_pDst->Get_Transform()->Get_AbsScale().y *0.5f) + _pSrc->Get_Transform()->Get_AbsScale().y)*0.5f);
	float fCZ = (float)(((_pDst->Get_Transform()->Get_AbsScale().z * 0.5f) + _pSrc->Get_Transform()->Get_AbsScale().z)*0.5f);

	if (fCX > fX && fCZ > fZ && fCY > fY)
	{
		return true;
	}
	return false;
}

//양방향 이벤트.
void Engine::CCollisionManager::Collision_Event_Bidirectional(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst) 
	{	
		for (auto& Src : _Src) 
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Collision_Sphrase(Dst, Src, &fX, &fY,&fZ))
			{
				Dst->Set_Event(true);
				Src->Set_Event(true);
			}
		}
	}

}

//단방향 이벤트
void Engine::CCollisionManager::Collision_Event_Unidirectional(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst)//이벤트를 가하는 대상.
	{	
		for (auto& Src : _Src) // 이벤트를 당하는 대상.
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Collision_Sphrase(Dst, Src, &fX,&fY, &fZ))
			{
				Src->Set_Event(true);
			}
		}
	}
}

//원충돌로 밀기.  // 위 아래는 막음.
void Engine::CCollisionManager::Collision_Sphrase_Push(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src)
{
	for (auto& Dst : _Dst) // 미는 대상.
	{
		for (auto& Src : _Src) // 밀리는 대상.
		{
			float fX = 0.f, fY = 0.f, fZ = 0.f;
			if (Collision_Sphrase(Dst, Src, &fX, &fY, &fZ))
			{
				float fDir = D3DXVec3Length(&(Dst->Get_Transform()->Get_Position() - Src->Get_Transform()->Get_Position()));
				float fRad = (float)((int)(Dst->Get_Transform()->Get_AbsScale().x + Src->Get_Transform()->Get_AbsScale().x) >> 1); 
				// 상하좌우 비율이 같아야 깨끗하게 들어감.

				_float3 vSrcDir = Dst->Get_Transform()->Get_Position() - Src->Get_Transform()->Get_Position();
				D3DXVec3Normalize(&vSrcDir, &vSrcDir);

				vSrcDir.y = 0.f;

				Src->Get_Transform()->AddPostion(vSrcDir* (fRad - fDir));
			}
		}
	}
}



bool Engine::CCollisionManager::Collision_Sphrase(CGameObject * _pDst, CGameObject * _pSrc, float * _x, float * _y, float * _z)
{
	_float fDir = D3DXVec3Length(&(_pDst->Get_Transform()->Get_Position() - _pSrc->Get_Transform()->Get_Position()));

	float fCX = (float)((int)(_pDst->Get_Transform()->Get_AbsScale().x + _pSrc->Get_Transform()->Get_AbsScale().x) >> 1);
	float fCY = (float)((int)(_pDst->Get_Transform()->Get_AbsScale().y + _pSrc->Get_Transform()->Get_AbsScale().y) >> 1);
	float fCZ = (float)((int)(_pDst->Get_Transform()->Get_AbsScale().z + _pSrc->Get_Transform()->Get_AbsScale().z) >> 1);

	if (fCX > fDir && fCY> fDir && fCZ > fDir)
	{
		*_x = fCX - fDir;
		*_y = fCY - fDir;
		*_z = fCZ - fDir;
		return true;
	}
	return false;

}

