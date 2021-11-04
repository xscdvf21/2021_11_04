#include "framework.h"
#include "BattlePlate.h"
#include "Monster.h"
#include "BattlePlateCircle.h"
#include "BattlePlateSlide.h"
#include "BattlePlateSelectLine.h"
#include "BattlePlateSelectCube.h"

#include "DropGoomba.h"

CBattlePlate::CBattlePlate(_Device pDevice)
	:CGameObject(pDevice)
	, m_iRemainAttackCount(0)
	, m_iRemainCount(0)
	, m_iRemainMonster(0)
	, m_iRow(0)
	, m_iCol(0)
	, m_fRemainTimer(0)
	, m_fNeedTime(0)
	, m_fNowNeedTimer(0)
	, m_bStopTimerEnable(false)
	, eBattleState(EBattleState::ROTATEMODE)
	, eBattleMode(EModeType::ROTATE)
	, eAPos(EPosisition::NONE)
	, eWeapon(EWeaponType::DEFAULT)
	,eBattleEffect(EBattlePlateEffect::NONE)
	,m_bGameStart(false)
	, m_bSelectMoveCircle(false)
	, m_fDefenceFlowTime(0.f)
	, m_fDefenceTime(2.f)
	, m_iCamCol(11)
{
	ZeroMemory(&arrBattlePlate, sizeof(arrBattlePlate));
	//ZeroMemory(&arrBattlePlateOrigin, sizeof(arrBattlePlateOrigin));
}

CBattlePlate::CBattlePlate(const CBattlePlate& other)
	:CGameObject(other)		//필요하면 other  기반으로 바꾸시오.
	, m_iRemainAttackCount(0)
	, m_iRemainCount(0)
	, m_iRemainMonster(0)
	, m_iRow(0)
	, m_iCol(0)
	, m_fRemainTimer(0)
	, m_fNeedTime(0)
	, m_fNowNeedTimer(0)
	, m_bStopTimerEnable(false)
	, eBattleState(EBattleState::ROTATEMODE)
	, eBattleMode(EModeType::ROTATE)
	, eAPos(EPosisition::NONE)
	, eWeapon(EWeaponType::DEFAULT)
	, eBattleEffect(EBattlePlateEffect::NONE)
	, m_bGameStart(false)
	, m_bSelectMoveCircle(false)
	, m_fDefenceFlowTime(0.f)
	, m_fDefenceTime(2.f)
	, m_iCamCol(11)
{
	ZeroMemory(&arrBattlePlate, sizeof(arrBattlePlate));
	//ZeroMemory(&arrBattlePlateOrigin, sizeof(arrBattlePlateOrigin));
}

HRESULT CBattlePlate::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
	{
		return E_FAIL;
	}



	return S_OK;
}

HRESULT CBattlePlate::Ready_GameObject(void* pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
	{
		return E_FAIL;
	}


	m_pTransformCom->Initialize_vPos(_float3(-12.8f, -1.f, -12.8f));;
	m_pTransformCom->Initialize_vScale(_float3(0.2f, 0.0f, 0.2f));
	m_pTransformCom->Initialize_fSpeed(0.f);
	m_pTransformCom->Initialize_fRotate(0.f);		//플레이트 자체를 돌릴일 없음


	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_uint CBattlePlate::Update_GameObject(_float fDeltaTime)
{
	//왜 업데이트 전에 돌리느냐? : 게임 시작 직전이라서요
	//대충 유니티에서 'start()' 와 같은 포지션

	if (m_bGameStart != true)
	{
		auto pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return 0;

		m_fRemainTimer = m_iRemainMonster * 5.f;

		//시작할때 원본 플레이트 복사해감.
		//Copy_PlateState(arrBattlePlateOrigin, arrBattlePlate);

		//이펙트 추가
		//pBattlePlateCircle = static_cast<CBattlePlateCircle*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate", 1));
		//Safe_AddReference(pBattlePlateCircle);
		//pBattlePlateSlide = static_cast<CBattlePlateSlide*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate", 2));
		//Safe_AddReference(pBattlePlateSlide);
		//pBattlePlateSelectLine = static_cast<CBattlePlateSelectLine*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate", 3));	
		//Safe_AddReference(pBattlePlateSelectLine);
		//pBattlePlateSelectCube = static_cast<CBattlePlateSelectCube*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate", 4));
		//Safe_AddReference(pBattlePlateSelectCube);

		//이펙트 끝
		m_iRemainCount = 1;

		m_bGameStart = true;
	}

	CGameObject::Update_GameObject(fDeltaTime);


	//애들 문제는 아님... 그러면?
	Update_BattlePlateVisual(fDeltaTime);

	switch (eBattleState)
	{
	case EBattleState::ROTATEMODE:
		if (FAILED(Control_Plate(fDeltaTime)))
		{
			return 0;
		}
		break;
	case EBattleState::TARGETSELECTMODE:
		if (FAILED(Search_Target())) 
		{
			return 0;
		}
		break;
	case EBattleState::ATTACK:		
		if (FAILED(Hit_Target())) 
		{
			return 0;
		}
		break;
	case EBattleState::ENEMYATTACK:
	{
		m_iRemainEnemyBattle = (m_iRemainMonster / 4) + 1;

		Enemy_Turn(fDeltaTime);
	}
		break;
	case EBattleState::REROLL:
		//일단 리롤 없음
		break;
	case EBattleState::GAMESET:
		Battle_Result();
		break;
	case EBattleState::RELEASE:
		//게임 나가기
		break;
	default:
		break;
	}
	
	
	//이펙트 세팅부분
	//pBattlePlateCircle->Set_avIndex(m_iRow);
	//pBattlePlateSlide->Set_avIndex(m_iCol);
	//pBattlePlateSelectLine->Set_avIndex(m_iCol);
	//pBattlePlateSelectCube->Set_avIndex(m_iCol);

	//if (eBattleState == EBattleState::ROTATEMODE)
	//{
	//	if (m_bSelectMoveCircle == true && (int)(m_fRemainTimer * 10) % 5 == 0)
	//	{
	//		//그럼 출력 안함.
	//	}
	//	else{
	//		switch (eBattleMode)
	//		{
	//		case EModeType::ROTATE:
	//			pBattlePlateCircle->Set_Visual();
	//			break;
	//		case EModeType::SLIDE:
	//			pBattlePlateSlide->Set_Visual();
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}
	//if (eBattleState == EBattleState::TARGETSELECTMODE)
	//{
	//	switch (eWeapon)
	//	{
	//	case EWeaponType::BOOTS:
	//		pBattlePlateSelectLine->Set_Visual();
	//		break;
	//	case EWeaponType::HAMMER:
	//		pBattlePlateSelectCube->Set_Visual();
	//		break;
	//	case EWeaponType::DEFAULT:
	//		pBattlePlateSelectLine->Set_Visual();
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//이펙트 세팅부분 끝
	return _uint();
}
_uint CBattlePlate::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	//이펙트 처리

	return m_pTransformCom->Update_Transform();
}

HRESULT CBattlePlate::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;



	const TCHAR* aTexture = L"x : %d, y : %d, monsterNumber : %d";
	TCHAR anTexture[256] = L"";
	swprintf_s(anTexture, aTexture, m_iRow, m_iCol, m_iRemainMonster);


	SetWindowText(g_hWnd, anTexture);

	return S_OK;
}

CBattlePlate* CBattlePlate::Create(_Device pDevice)
{
	CBattlePlate* pInstance = new CBattlePlate(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create BattlePlate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBattlePlate::Clone(void* pArg)
{
	CBattlePlate* pInstance = new CBattlePlate(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone BattlePlate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattlePlate::Free()
{
	Clear_PlateState(arrBattlePlate);
	//Clear_PlateState(arrBattlePlateOrigin);	

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	//Safe_Release(pBattlePlateCircle);
	//Safe_Release(pBattlePlateSlide);
	//Safe_Release(pBattlePlateSelectLine);
	//Safe_Release(pBattlePlateSelectCube);

	CGameObject::Free();
}

HRESULT CBattlePlate::Add_Component()
{
	//대충 바닥 버텍스 버퍼 추가하는 코드
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Terrain",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	//대충 텍스쳐 추가하는 코드
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattlePlate",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

_uint CBattlePlate::Get_RemainCircle()
{
	return m_iRemainCount;
}

_uint CBattlePlate::Get_RemainAttackCount()
{
	return m_iRemainAttackCount;
}

_float CBattlePlate::Get_RemainTimer()
{
	return m_fRemainTimer;
}

HRESULT CBattlePlate::Add_Monster(const wstring& _pMonsterObject, _uint posY, _uint posX)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//스테이지 (프로토타입 위치 / 스테이지별 재생산)에서 배틀 씬으로 뽑아오기.


	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		_pMonsterObject,
		(_uint)ESceneType::Battle,
		L"Layer_BattlePlate_Monster")))
	{
		return E_FAIL;
	}
		arrBattlePlate[posY][posX] = dynamic_cast<CSuperMonster*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate_Monster", m_iRemainMonster));
		Safe_AddReference(arrBattlePlate[posY][posX]);

	m_iRemainMonster++;	

	return S_OK;
}
//일단 킵
HRESULT CBattlePlate::Add_Monster_Group(const wstring& _pMonsterObject, _uint posY, _uint posX)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//스테이지 (프로토타입 위치 / 스테이지별 재생산)에서 배틀 씬으로 뽑아오기.

	if (arrBattlePlate[0][posX] != nullptr)
	{
		PRINT_LOG(L"Error", L"이미 몬스터가 존재하는 위치입니다.");
		return S_OK;
	}

	for (int i = 0; i < 4; ++i) {
		if (FAILED(pManagement->Clone_GameObject_ToLayer(
			(_uint)ESceneType::Static,
			_pMonsterObject,
			(_uint)ESceneType::Battle,
			L"Layer_BattlePlate_Monster")))
		{
			return E_FAIL;
		}

		arrBattlePlate[i][posX] = dynamic_cast<CSuperMonster*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate_Monster", m_iRemainMonster));

		Safe_AddReference(arrBattlePlate[i][posX]);

		m_iRemainMonster++;
	}


	return S_OK;
}



HRESULT CBattlePlate::Control_Plate(_float fDeltaTime)
{
	//일단 남은 시간을 깎아먹어요
	m_fRemainTimer -= fDeltaTime;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//1. 선택 회수 다 썼으면			-아직 안 넣음
	//2. 정렬 성공했으면
	//3. 시간 초과 나면
	//4. 완료 키 누르면
	//= 타겟 셀렉트 모드로

	if ((m_iRemainCount == 0
		||Caculate_Position() == true
		|| m_fRemainTimer < 0.1f
		|| pManagement->Key_Down(KEY_Z)) && eBattleState == EBattleState::ROTATEMODE)
	{
		//카메라 위치에 현재 선택된 Column을 맞춤
		m_iCol = m_iCamCol;

		m_iRemainAttackCount = (m_iRemainMonster / 4) + 1;

		eBattleState = EBattleState::TARGETSELECTMODE;
	}

	if (m_bSelectMoveCircle != true) 
	{
		if (pManagement->Key_Down(KEY_Q))
		{
			eBattleMode = EModeType::ROTATE;
		}
		if (pManagement->Key_Down(KEY_E))
		{
			eBattleMode = EModeType::SLIDE;
		}
		if (pManagement->Key_Down(KEY_X))
		{
			m_bSelectMoveCircle = true;
		}
	}
	else if (m_bSelectMoveCircle == true)
	{
		if (pManagement->Key_Down(KEY_X))
		{
			m_bSelectMoveCircle = false;
			m_iRemainCount--;
		}
	}

	eAPos = EPosisition::NONE;
	//키 중립화
	if (pManagement->Key_Up(KEY_UP))
	{
		eAPos = EPosisition::UP;
	}
	if (pManagement->Key_Up(KEY_DOWN))
	{
		eAPos = EPosisition::DOWN;
	}
	if (pManagement->Key_Up(KEY_LEFT))
	{
		eAPos = EPosisition::LEFT;
	}
	if (pManagement->Key_Up(KEY_RIGHT))
	{
		eAPos = EPosisition::RIGHT;
	}

	switch (eBattleMode)
	{
	case EModeType::ROTATE:
		if (m_bSelectMoveCircle != true)
		{
			Select_Spin_Circle();
		}
		else
		{
			Move_Spin_Circle();
		}
		break;
	case EModeType::SLIDE:
		if (m_bSelectMoveCircle != true)
		{
			Select_Spin_Slide();
		}
		else
		{
			Move_Spin_Slide();
		}
		break;
	default:
		if (m_bSelectMoveCircle != true)
		{
			Select_Spin_Circle();
		}
		else
		{
			Move_Spin_Circle();
		}
		break;
	}

	return S_OK;
}
//일단 공격부는 빼고
HRESULT CBattlePlate::Search_Target()
{
	if (m_iRemainMonster == 0)
	{
		eBattleState = EBattleState::GAMESET;
	}

	if (m_iRemainAttackCount == 0)
	{
		eBattleState = EBattleState::ENEMYATTACK;
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	eAPos = EPosisition::NONE;

	if (pManagement->Key_Down(KEY_LEFT))
	{
		if (m_iCol == 0)
		{
			m_iCol = 12;
		}
		m_iCol--;

		eAPos = EPosisition::LEFT;
	}
	else if (pManagement->Key_Down(KEY_RIGHT))
	{
		m_iCol++;
		m_iCol %= 12;

		eAPos = EPosisition::RIGHT;
	}

	if (pManagement->Key_Down(KEY_UP)|| pManagement->Key_Down(KEY_DOWN))
	{
		if (eWeapon != EWeaponType::BOOTS)
		{
			eWeapon = EWeaponType::BOOTS;
		}
		else if (eWeapon != EWeaponType::HAMMER)
		{
			eWeapon = EWeaponType::HAMMER;
		}
	}


	if (pManagement->Key_Down(KEY_X))
	{
		Hit_Target();
	}

	

	return S_OK;
}

_bool CBattlePlate::Caculate_Position()
{
	//1. 직선 판정
	_bool bExist;
	_uint iLineCount;
	_bool bLineThemUp[12] = {};
	_bool bCubeThemUp[12] = {};

	for (_uint i = 0; i < 12; ++i)
	{
		bExist = false;
		iLineCount = 0;
		for (_uint j = 0; j < 4; ++j)
		{
			if (bExist == true && iLineCount == 2 && j > 1 && arrBattlePlate[j][i] == nullptr)			//해머 형 조건에 만족하는 경우
			{
				;
			}
			else if (bExist == true && arrBattlePlate[j][i] == nullptr)									//라인에 1개체 이상 있고 비어있는 칸이 있는경우
			{
				return false;
			}
			else if (bExist == true && arrBattlePlate[j][i] != nullptr && (iLineCount != j))			//라인에 1개체 이상 있고, 자리에 몬스터가 있으나 개수가 맞지않은 경우.
			{
				return false;
			}
			else if (bExist != true && j != 0 && arrBattlePlate[j][i] != nullptr)						//라인의 첫번째 몬스터가 0번째 칸이 아닐때
			{
				return false;
			}
			else if (arrBattlePlate[j][i] != nullptr)													//그냥 몬스터가 있을때
			{
				bExist = true;
				++iLineCount;
			}
		}
		if (iLineCount == 4)
		{
			bLineThemUp[i] = true;
		}
	}
	//2. 사각 판정

	_bool bLeft = false;
	_bool bRight = false;
	for (int i = 0; i < 12; ++i)
	{
		bLeft = false;
		bRight = false;

		if (bCubeThemUp[i] == true || bLineThemUp[i] == true)
		{
			continue;
		}

		if (arrBattlePlate[0][i] != nullptr && arrBattlePlate[1][i] != nullptr)
		{
			bLeft = true;
		}
		if (arrBattlePlate[0][(i + 1) % 12] != nullptr && arrBattlePlate[1][(i + 1) % 12] != nullptr)
		{
			bRight = true;
		}
		if (bLeft == true && bRight == true)
		{
			bCubeThemUp[i] = true;
			bCubeThemUp[(i + 1) % 12] = true;
		}
	}

	//3. 판정 정리
	for (_uint i = 0; i < 12; ++i)
	{
		if (arrBattlePlate[0][i] != nullptr && (bLineThemUp[i] == false && bCubeThemUp[i] == false))			//첫번째자리에 몬스터가 있음에도 라인클리어나 큐브클리어가 되지 않은 경우
		{
			return false;
		}
	}

	return true;
}

void CBattlePlate::Select_Spin_Circle()
{
	switch (eAPos)
	{
	case EPosisition::UP:
		++m_iRow;
		if (m_iRow == 4)
		{
			m_iRow = 0;
		}
		break;
	case EPosisition::DOWN:
		if (m_iRow == 0)
		{
			m_iRow = 4;
		}
		--m_iRow;
		break;
	default:
		break;
	}
	
}

void CBattlePlate::Select_Spin_Slide()
{
	switch (eAPos)
	{
	case EPosisition::LEFT:
		if (m_iCol == 0)
		{
			m_iCol = 12;
		}
		--m_iCol;
		break;
	case EPosisition::RIGHT:
		++m_iCol;
		if (m_iCol == 12)
		{
			m_iCol = 0;
		}
		break;
	default:
		break;
	}
}

void CBattlePlate::Move_Spin_Circle()
{
	CSuperMonster* pPointer;
	switch (eAPos)
	{
	case EPosisition::LEFT:
		pPointer = arrBattlePlate[m_iRow][0];
		for (int i = 1; i < 12; i++)
		{
			arrBattlePlate[m_iRow][i - 1] = arrBattlePlate[m_iRow][i];
		}
		arrBattlePlate[m_iRow][11] = pPointer;
		eAPos = EPosisition::NONE;
		break;
	case EPosisition::RIGHT:
		pPointer = arrBattlePlate[m_iRow][11];
		for (int i = 1; i < 12; i++)
		{
			arrBattlePlate[m_iRow][12 - i] = arrBattlePlate[m_iRow][12 - i - 1];
		}
		arrBattlePlate[m_iRow][0] = pPointer;
		eAPos = EPosisition::NONE;
		break;
	default:
		break;
	}
	pPointer = nullptr;
}

//3 2 1 0 - 0 1 2 3 구조
void CBattlePlate::Move_Spin_Slide()
{
	CSuperMonster* pPointer;
	switch (eAPos)
	{
	case EPosisition::UP:
		pPointer = arrBattlePlate[3][m_iCol];

		for (int i = 1; i < 4; i++)
		{
			arrBattlePlate[4 - i][m_iCol] = arrBattlePlate[4 - i - 1][m_iCol];
		}
		arrBattlePlate[0][m_iCol] = arrBattlePlate[0][(m_iCol + 6) % 12];		//중앙 지점 옮기기
		for (int i = 1; i < 4; i++)
		{
			arrBattlePlate[i - 1][(m_iCol + 6) % 12] = arrBattlePlate[i][(m_iCol + 6) % 12];
		}
		arrBattlePlate[3][(m_iCol + 6) % 12] = pPointer;

		eAPos = EPosisition::NONE;
		break;
	case EPosisition::DOWN:
		pPointer = arrBattlePlate[3][(m_iCol + 6) % 12];
		for (int i = 1; i < 4; i++)			//2번째 지점 밀어내기
		{
			arrBattlePlate[4 - i][(m_iCol + 6) % 12] = arrBattlePlate[4 - i - 1][(m_iCol + 6) % 12];
		}
		arrBattlePlate[0][(m_iCol + 6) % 12] = arrBattlePlate[0][m_iCol];
		for (int i = 1; i < 4; i++)
		{
			arrBattlePlate[i - 1][m_iCol] = arrBattlePlate[i][m_iCol];
		}
		arrBattlePlate[3][m_iCol] = pPointer;


		eAPos = EPosisition::NONE;
		break;
	default:
		break;
	}
}

HRESULT CBattlePlate::Hit_Target()
{
	switch (eWeapon)
	{
	case EWeaponType::BOOTS:
		Hit_Boots();
		break;
	case EWeaponType::HAMMER:
		Hit_Hammer();
		break;
	case EWeaponType::DEFAULT:
		Hit_Boots();
		break;
	default:
		break;
	}
	m_iRemainAttackCount--;

	eBattleState = EBattleState::TARGETSELECTMODE;
	return S_OK;
}

void CBattlePlate::Hit_Boots()
{
	for (_uint i = 0; i < 4; i++)
	{
		if (arrBattlePlate[i][m_iCol] != nullptr)
		{
			arrBattlePlate[i][m_iCol]->Hurt_HP(5);

			if (arrBattlePlate[i][m_iCol]->Get_Health_Status().iHP == 0)
			{
				arrBattlePlate[i][m_iCol]->Set_Dead();
				//Safe_Release(arrBattlePlate[i][m_iCol]);		//?

				m_iRemainMonster--;
				arrBattlePlate[i][m_iCol] = nullptr;
			}
		}
	}
}

void CBattlePlate::Hit_Hammer()
{
	//X0
//00
	if (arrBattlePlate[0][m_iCol] != nullptr)
	{
		arrBattlePlate[0][m_iCol]->Hurt_HP(5);

		if (arrBattlePlate[0][m_iCol]->Get_Health_Status().iHP == 0)
		{
			arrBattlePlate[0][m_iCol]->Set_Dead();
			//Safe_Release(arrBattlePlate[0][m_iCol]);		//?

			m_iRemainMonster--;
			arrBattlePlate[0][m_iCol] = nullptr;
		}
	}
	//0X
	//00

	if (arrBattlePlate[1][m_iCol] != nullptr)
	{
		arrBattlePlate[1][m_iCol]->Hurt_HP(5);

		if (arrBattlePlate[1][m_iCol]->Get_Health_Status().iHP == 0)
		{
			arrBattlePlate[1][m_iCol]->Set_Dead();
			Safe_Release(arrBattlePlate[1][m_iCol]);		//?

			m_iRemainMonster--;
			arrBattlePlate[1][m_iCol] = nullptr;
		}
	}

	//00
	//X0
	if (arrBattlePlate[0][(m_iCol + 1) % 12] != nullptr)
	{
		arrBattlePlate[0][(m_iCol + 1) % 12]->Hurt_HP(5);

		if (arrBattlePlate[0][(m_iCol + 1) % 12]->Get_Health_Status().iHP == 0)
		{
			arrBattlePlate[0][(m_iCol + 1) % 12]->Set_Dead();
			Safe_Release(arrBattlePlate[0][(m_iCol + 1) % 12]);		//?

			m_iRemainMonster--;
			arrBattlePlate[0][(m_iCol + 1) % 12] = nullptr;
		}
	}
	//00
	//0X

	if (arrBattlePlate[1][(m_iCol + 1) % 12] != nullptr)
	{
		arrBattlePlate[1][(m_iCol + 1) % 12]->Hurt_HP(5);

		if (arrBattlePlate[1][(m_iCol + 1) % 12]->Get_Health_Status().iHP == 0)
		{
			arrBattlePlate[1][(m_iCol + 1) % 12]->Set_Dead();
			Safe_Release(arrBattlePlate[1][(m_iCol + 1) % 12]);		//?

			m_iRemainMonster--;
			arrBattlePlate[1][(m_iCol + 1) % 12] = nullptr;
		}
	}
}

void CBattlePlate::Enemy_Turn(float fDeltaTime)
{
	while (m_iRemainEnemyBattle > 0)
	{
		switch (Enemy_Attack_Phase(fDeltaTime))
		{
		case 0: //공격회수 다 쓴 경우
			//eBattleState = EBattleState::REROLL; 원래 리롤임
			eBattleState = EBattleState::ROTATEMODE;
			break;
		case 1: //아직 공격 회수가 남은 경우 + 공격 안함
			m_fDefenceFlowTime = m_fDefenceTime;
			//아무것도 업음
			break;
		case 2: //아직 공격 회수가 남은 경우 + 공격중
			Enemy_Attack(fDeltaTime);
			break;
		case 3: //아직 공격 회수가 남은 경우 + 공격 끝남
			m_iRemainEnemyBattle--;
			break;

		default:
			break;
		}
	}

	eBattleState = EBattleState::ROTATEMODE;
	m_iRemainCount = (m_iRemainMonster / 4) + 1;
}

_uint CBattlePlate::Enemy_Attack_Phase(float fDeltaTime)
{
	if (m_iRemainEnemyBattle == 0 && m_bNowAttack != true)
	{
		return 0;
	}
	if (m_bNowAttack == true && m_fDefenceFlowTime > 0.01f)
	{
		return 2;
	}
	else if (m_bNowAttack == true)
	{
		m_fDefenceFlowTime = m_fDefenceTime;
		m_bNowAttack = false;
		return 3;
	}
	else if (m_bNowAttack != true)
	{
		m_bNowAttack = true;
		return 1;
	}
	return 0;
}

void CBattlePlate::Enemy_Attack(float fDeltaTime)
{
	m_fDefenceFlowTime -= fDeltaTime * m_fDefenceTime;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	dynamic_cast<CDropGoomba*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Monster_Object", 0))->Set_Drop();

	if (pManagement->Key_Down(KEY_X) && m_fDefenceFlowTime < 1.0f)
	{
		//가드 대미지
		m_fDefenceFlowTime = 0.f;
	} 
	else if (m_fDefenceFlowTime < 0.1f)
	{
		//일반 대미지
		m_fDefenceFlowTime = 0.f;
	}

}

void CBattlePlate::Battle_Result()
{
	eBattleState = EBattleState::RELEASE;
	PRINT_LOG(L"End", L"배틀 끝!");
}

void CBattlePlate::Update_BattlePlateVisual(_float fDeltaTime)
{
	_float fRotate;

	_mat matWorld, matRotY, matParent;

	for (_uint x = 0; x < 12; ++x)
	{
		fRotate = 30.f * x;

		for (_uint y = 0; y < 4; ++y)
		{
			if (arrBattlePlate[y][x] != nullptr)
			{
				_float3 _vPos = { y * 2.1f + 4.2f,0.f,0.f };

				D3DXMatrixRotationY(&matRotY, D3DXToRadian(fRotate));		//회전 먹이고
				D3DXMatrixTranslation(&matParent, 0.f, 0.f, 0.f);			//부모 위치 먹이고
				matWorld = matRotY * matParent;
				D3DXVec3TransformCoord(&_vPos, &_vPos, &matWorld);			//좌표 받아서

				arrBattlePlate[y][x]->Set_Position(_vPos);					//세팅합니다
			}
		}
	}
}



void CBattlePlate::Copy_PlateState(CSuperMonster* (*dst)[12], CSuperMonster* (*origin)[12])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			Safe_Release(dst[i][j]);
			dst[i][j] = origin[i][j];
			Safe_AddReference(dst[i][j]);			
		}
	}
}

void CBattlePlate::Clear_PlateState(CSuperMonster* (*dst)[12])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (dst[i][j] != nullptr) 
			{
				dst[i][j]->Set_Dead();
				Safe_Release(dst[i][j]);
				dst[i][j] = nullptr;
			}
		}
	}
}

//윽 야매코드 또 쓰네
void CBattlePlate::Set_CamCol(int a)
{
	if (m_iCamCol == 0 && a == -1) {
		m_iCamCol = 12;
	}

	m_iCamCol = m_iCamCol + a + 12;
	m_iCamCol %= 12;
}
