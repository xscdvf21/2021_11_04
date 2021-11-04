#include "stdafx.h"
#include "ChessTile.h"

#include "Export_Function.h"


IMPLEMENT_SINGLETON(CChessTile)

CChessTile::CChessTile()
	:m_iBossHp(500)
{
}


CChessTile::~CChessTile()
{
	Free();
}

void CChessTile::Ready_ChessTile()
{


	m_fX = -9.8f;
	m_fY = -9.8f;
	m_iTileIndex = 0;
	for (int i = 0; i < CHESSTILE_Y; ++i)
	{
		_float3 vPos;

		for (int j = 0; j < CHESSTILE_X; ++j)
		{
			CHESSTILE tTemp;
			tTemp.TileIndex = m_iTileIndex;
			tTemp.vPos = _float3(m_fY + i * 2.8f , 0.0f, m_fX + j * 2.8f);
			m_vecChessTile.emplace_back(tTemp);

			m_iTileIndex++;
		}
	}
}

const CHESSTILE CChessTile::Get_TileIndex(_uint iIndex)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (iIndex >= CHESSTILE_X*CHESSTILE_Y)
		iIndex =CHESSTILE_X*CHESSTILE_Y - 1;
	return m_vecChessTile[iIndex];
}

const CHESSTILE  CChessTile::Get_Index(_vec3 _vec)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	_vec3 vPos = _vec;

	vPos.x += 9.8f;
	vPos.z += 9.8f;

	if (vPos.x < 0 || vPos.z < 0)
		return m_vecChessTile[0];

	float		iX = (float)vPos.x / CHESSTILE_CX;
	float		iY = (float)vPos.z / CHESSTILE_CY;

	float		fIndex = iX * CHESSTILE_X + iY;

	
	int iIndex = int(fIndex);
	if (iIndex >= CHESSTILE_X*CHESSTILE_Y)
		iIndex = CHESSTILE_X*CHESSTILE_Y - 1;
	return m_vecChessTile[iIndex];
}


void CChessTile::Set_Damage(_int iHp)
{
	m_iBossHp += iHp;
	if (m_iBossHp <= 0)
		m_iBossHp = 0;
}

void CChessTile::Free()
{
	
}
