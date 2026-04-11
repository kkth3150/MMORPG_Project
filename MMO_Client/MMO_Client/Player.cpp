#include "pch.h"
#include "Player.h"
#include "Img_Manager.h"
#include "Input_Manager.h"
#include "Camera.h"
#include "Map_Manager.h"
#include "ItemData_Potion.h"
#include "ItemData_Equipment.h"
#include "ItemData_Etc.h" 

CPlayer::CPlayer()
{
	m_iMaxExp = 100.f;
	m_iCurExp = m_iMaxExp;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

#pragma region Sprites :

	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_B.png", L"PLAYER_IDLE_B");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_LB.png", L"PLAYER_IDLE_LB");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_L.png", L"PLAYER_IDLE_L");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_LT.png", L"PLAYER_IDLE_LT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_T.png", L"PLAYER_IDLE_T");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_RT.png", L"PLAYER_IDLE_RT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_R.png", L"PLAYER_IDLE_R");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_RB.png", L"PLAYER_IDLE_RB");

#pragma endregion PLAYER IDLE

#pragma region Sprites :

	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_B.png", L"PLAYER_WALK_B");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_LB.png", L"PLAYER_WALK_LB");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_L.png", L"PLAYER_WALK_L");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_LT.png", L"PLAYER_WALK_LT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_T.png", L"PLAYER_WALK_T");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_RT.png", L"PLAYER_WALK_RT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_R.png", L"PLAYER_WALK_R");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_RB.png", L"PLAYER_WALK_RB");

#pragma endregion PLAYER WALK

#pragma region Sprites :

	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_B.png", L"PLAYER_WALK_B");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_LB.png", L"PLAYER_WALK_LB");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_L.png", L"PLAYER_WALK_L");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_LT.png", L"PLAYER_WALK_LT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_T.png", L"PLAYER_WALK_T");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_RT.png", L"PLAYER_WALK_RT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_R.png", L"PLAYER_WALK_R");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_RB.png", L"PLAYER_WALK_RB");

#pragma endregion PLAYER WALK

#pragma region Sprites :

	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_B.png", L"PLAYER_ATTACK_B");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_LB.png", L"PLAYER_ATTACK_LB");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_L.png", L"PLAYER_ATTACK_L");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_LT.png", L"PLAYER_ATTACK_LT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_T.png", L"PLAYER_ATTACK_T");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_RT.png", L"PLAYER_ATTACK_RT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_R.png", L"PLAYER_ATTACK_R");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/attack/attack_RB.png", L"PLAYER_ATTACK_RB");

#pragma endregion PLAYER ATTACK

	m_pInventory = new CInventory;
	m_pEquipment = new CEquipment;
	m_iHp = 100;
	m_iMaxHp = 100;
	m_iMp = 100;
	m_iMaxMp = 100;
	m_iAttack = 10;
	m_iDef = 5;

	Motion_Change(PLAYER_IDLE);
	Direction_Change(DIR_B);
	Set_Collider(0.2f, 0.2f);
	m_fSpeed = 1.f;
	m_tIsoInfo.fWorldX = 10.f;
	m_tIsoInfo.fWorldZ = 10.f;

}

int CPlayer::Update(float dt)
{
	Key_Input(dt);
	Update_ClickEffect(dt);
	__super::Update_Rect();
	__super::Move_Frame();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(float dt)
{
}

void CPlayer::Render(ID2D1RenderTarget* pRT)
{
	switch (m_eCurState)
	{
	case PLAYER_IDLE:   RenderIDLE(pRT);   break;
	case PLAYER_WALK:   RenderWALK(pRT);   break;
	case PLAYER_ATTACK: RenderATTACK(pRT); break;
	case PLAYER_HIT:    RenderHIT(pRT);    break;
	case PLAYER_DEAD:   RenderDEAD(pRT);   break;
	default: break;
	}

	Render_ClickEffect(pRT);
#ifdef GAME_DEBUG
	Debug_Render(pRT);
#endif
}

void CPlayer::Release(void)
{
	if (m_pInventory) { delete m_pInventory; m_pInventory = nullptr; }
	if (m_pEquipment) { delete m_pEquipment; m_pEquipment = nullptr; }
}


void CPlayer::Render_Sprite(ID2D1RenderTarget* pRT, ID2D1Bitmap* pBitmap)
{
	if (!pBitmap) return;

	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

	float fDrawX = tScreen.x - m_tIsoInfo.fCX / 2.f;
	float fDrawY = tScreen.y - m_tIsoInfo.fCY
		- m_tIsoInfo.fHeight + TILE_HALF_H;

	// 목적지 Rect
	D2D1_RECT_F destRect = D2D1::RectF(
		fDrawX,
		fDrawY,
		fDrawX + m_tIsoInfo.fCX,
		fDrawY + m_tIsoInfo.fCY
	);

	// 스프라이트 시트에서 현재 프레임 잘라내기
	float fSrcX = m_tIsoInfo.fCX * m_tFrame.iFrameStart;
	D2D1_RECT_F srcRect = D2D1::RectF(
		fSrcX,
		0.f,
		fSrcX + m_tIsoInfo.fCX,
		m_tIsoInfo.fCY
	);

	pRT->DrawBitmap(
		pBitmap,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);
}

void CPlayer::RenderIDLE(ID2D1RenderTarget* pRT)
{
	ID2D1Bitmap* pBitmap = nullptr;
	switch (m_eDir)
	{
	case DIR_B:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_B");  break;
	case DIR_LB: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_LB"); break;
	case DIR_L:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_L");  break;
	case DIR_LT: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_LT"); break;
	case DIR_T:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_T");  break;
	case DIR_RT: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_RT"); break;
	case DIR_R:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_R");  break;
	case DIR_RB: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_RB"); break;
	default: break;
	}
	Render_Sprite(pRT, pBitmap);
}

void CPlayer::RenderWALK(ID2D1RenderTarget* pRT)
{
	ID2D1Bitmap* pBitmap = nullptr;
	switch (m_eDir)
	{
	case DIR_B:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_B");  break;
	case DIR_LB: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_LB"); break;
	case DIR_L:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_L");  break;
	case DIR_LT: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_LT"); break;
	case DIR_T:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_T");  break;
	case DIR_RT: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_RT"); break;
	case DIR_R:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_R");  break;
	case DIR_RB: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_RB"); break;
	default: break;
	}
	Render_Sprite(pRT, pBitmap);
}

void CPlayer::RenderATTACK(ID2D1RenderTarget* pRT) { /* 추후 구현 */ }
void CPlayer::RenderHIT(ID2D1RenderTarget* pRT) { /* 추후 구현 */ }
void CPlayer::RenderDEAD(ID2D1RenderTarget* pRT) { /* 추후 구현 */ }


void CPlayer::Render_ClickEffect(ID2D1RenderTarget* pRT)
{
	if (!m_tClickEffect.bActive) return;

	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tClickEffect.fWorldX, m_tClickEffect.fWorldZ);

	float fRX = TILE_HALF_W * 0.4f * m_tClickEffect.fScale;
	float fRY = TILE_HALF_H * 0.4f * m_tClickEffect.fScale;
	if (fRX <= 1.f || fRY <= 1.f) return;

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F((float)tScreen.x, (float)tScreen.y),
		fRX, fRY
	);

	//color에서 알파만 Scale로 덮어쓰기
	D2D1_COLOR_F col = m_tClickEffect.color;
	col.a = m_tClickEffect.fScale; // 페이드아웃

	ID2D1SolidColorBrush* pBrush = nullptr;
	pRT->CreateSolidColorBrush(col, &pBrush);
	pRT->DrawEllipse(ellipse, pBrush, 2.f);
	pBrush->Release();
}

// ===================== 상태/방향 변경 =====================

void CPlayer::Motion_Change(PLAYER_STATE eState)
{
	m_eCurState = eState;

	switch (eState)
	{
	case PLAYER_IDLE:
		m_tIsoInfo.fCX = 160.f;
		m_tIsoInfo.fCY = 128.f;
		m_tIsoInfo.fHeight = 30.f;
		Set_Frame(7, 100);
		break;

	case PLAYER_WALK:
		m_tIsoInfo.fCX = 160.f;
		m_tIsoInfo.fCY = 128.f;
		m_tIsoInfo.fHeight = 30.f;
		Set_Frame(7, 100);
		break;

	case PLAYER_ATTACK:
		m_tIsoInfo.fCX = 160.f;
		m_tIsoInfo.fCY = 128.f;
		m_tIsoInfo.fHeight = 30.f;
		Set_Frame(15, 30);
		break;

	case PLAYER_HIT:
		break;

	case PLAYER_DEAD:
		break;

	default:
		break;
	}
}

void CPlayer::Direction_Change(DIRECTION eDir)
{
	m_eDir = eDir;
	m_tFrame.iFrameStart = 0;
}

void CPlayer::Key_Input(float dt)
{
	if (!CInput_Manager::Get_Instance()->Is_GameMode()) return;

	CInput_Manager* pInput = CInput_Manager::Get_Instance();

	// ===== 매 프레임 마우스 위치로 타일 체크 =====
	POINT tMouse = pInput->Get_MousePos();
	float fWorldX, fWorldZ;
	CCamera::Get_Instance()->ScreenToIsoWorld(tMouse.x, tMouse.y, fWorldX, fWorldZ);

	int iTileX = (int)floorf(fWorldX);
	int iTileZ = (int)floorf(fWorldZ);

	bool bMovable = CMap_Manager::Get_Instance()->Is_Movable(iTileX, iTileZ);

	// 이동 불가 타일이면 미리 커서 변경
	if (!bMovable)
		pInput->Set_CursorMode(CURSOR_NON_ATTACK);

	// ===== 클릭 처리 =====
	if (pInput->Key_Down(VK_LBUTTON))
	{
		if (!bMovable) return;  // 이동 불가면 클릭 무시

		m_fDestWorldX = fWorldX;
		m_fDestWorldZ = fWorldZ;

		m_tClickEffect.fWorldX = fWorldX;
		m_tClickEffect.fWorldZ = fWorldZ;
		m_tClickEffect.fScale = 1.f;
		m_tClickEffect.bActive = true;
		m_tClickEffect.color = D2D1::ColorF(0.f, 1.f, 0.f, 1.f);

		m_bMoving = true;

		if (m_eCurState != PLAYER_WALK)
			Motion_Change(PLAYER_WALK);

#ifdef GAME_DEBUG
		m_iDebugTileX = (int)floorf(m_fDestWorldX);
		m_iDebugTileZ = (int)floorf(m_fDestWorldZ);
		m_fDebugLocalX = m_fDestWorldX - (float)m_iDebugTileX;
		m_fDebugLocalZ = m_fDestWorldZ - (float)m_iDebugTileZ;
#endif
	}

	if (m_bMoving)
		Move_To_Dest(dt);

	if (pInput->Key_Down('A'))
	{
		CItemData_Potion* p = new CItemData_Potion;
		p->Set_PotionType(POTION_HP_M);
		m_pInventory->Add_Item(p);
	}
	// 2 : HP 포션 대형 추가
	if (pInput->Key_Down('S'))
	{
		CItemData_Potion* p = new CItemData_Potion;
		p->Set_PotionType(POTION_HP_L);
		m_pInventory->Add_Item(p);
	}
	// 3 : MP 포션 중형 추가
	if (pInput->Key_Down('D'))
	{
		CItemData_Potion* p = new CItemData_Potion;
		p->Set_PotionType(POTION_MP_M);
		m_pInventory->Add_Item(p);
	}
	// 4 : 검 랜덤 추가
	if (pInput->Key_Down('F'))
	{
		CItemData_Equipment* p = new CItemData_Equipment;
		p->Set_EquipType((EQUIPMENT_TYPE)(EQUIP_SWORD_0 + rand() % 7));
		m_pInventory->Add_Item(p);
	}
	// 5 : 갑옷 랜덤 추가
	if (pInput->Key_Down('5'))
	{
		CItemData_Equipment* p = new CItemData_Equipment;
		p->Set_EquipType((EQUIPMENT_TYPE)(EQUIP_ARMOR_0 + rand() % 8));
		m_pInventory->Add_Item(p);
	}
	// 6 : 투구 랜덤 추가
	if (pInput->Key_Down('6'))
	{
		CItemData_Equipment* p = new CItemData_Equipment;
		p->Set_EquipType((EQUIPMENT_TYPE)(EQUIP_HELMET_0 + rand() % 7));
		m_pInventory->Add_Item(p);
	}
	// 7 : 방패 랜덤 추가
	if (pInput->Key_Down('7'))
	{
		CItemData_Equipment* p = new CItemData_Equipment;
		p->Set_EquipType((EQUIPMENT_TYPE)(EQUIP_SHIELD_0 + rand() % 7));
		m_pInventory->Add_Item(p);
	}
	// 8 : 반지 랜덤 추가
	if (pInput->Key_Down('8'))
	{
		CItemData_Equipment* p = new CItemData_Equipment;
		p->Set_EquipType((EQUIPMENT_TYPE)(EQUIP_RING_0 + rand() % 5));
		m_pInventory->Add_Item(p);
	}
	// 9 : 목걸이 랜덤 추가
	if (pInput->Key_Down('9'))
	{
		CItemData_Equipment* p = new CItemData_Equipment;
		p->Set_EquipType((EQUIPMENT_TYPE)(EQUIP_PENDANT_0 + rand() % 5));
		m_pInventory->Add_Item(p);
	}
	// 0 : 골드 100 추가
	if (pInput->Key_Down('0'))
	{
		m_pInventory->Add_Gold(100);
	}

}

void CPlayer::Move_To_Dest(float dt)
{
	float fDX = m_fDestWorldX - m_tIsoInfo.fWorldX;
	float fDZ = m_fDestWorldZ - m_tIsoInfo.fWorldZ;
	float fDist = sqrtf(fDX * fDX + fDZ * fDZ);

	float fFrameSpeed = m_fSpeed * dt;  // dt 적용

	if (fDist <= fFrameSpeed)
	{
		m_tIsoInfo.fWorldX = m_fDestWorldX;
		m_tIsoInfo.fWorldZ = m_fDestWorldZ;
		m_bMoving = false;
		Motion_Change(PLAYER_IDLE);
		return;
	}

	float fNX = fDX / fDist;
	float fNZ = fDZ / fDist;
	m_tIsoInfo.fWorldX += fNX * fFrameSpeed;
	m_tIsoInfo.fWorldZ += fNZ * fFrameSpeed;

	Decide_Direction(fNX, fNZ);
}
void CPlayer::Decide_Direction(float fNX, float fNZ)
{
	// 이동 벡터의 각도로 8방향 결정
	float fScreenDX = (fNX - fNZ) * TILE_HALF_W;  // 화면 X방향
	float fScreenDY = (fNX + fNZ) * TILE_HALF_H;  // 화면 Y방향

	// 화면상 방향벡터로 각도 계산
	float fAngle = atan2f(fScreenDY, fScreenDX) * 180.f / 3.14159f;

	DIRECTION eNewDir = m_eDir;

	if (fAngle >= -22.5f && fAngle < 22.5f)  eNewDir = DIR_R;
	else if (fAngle >= 22.5f && fAngle < 67.5f)  eNewDir = DIR_RB;
	else if (fAngle >= 67.5f && fAngle < 112.5f)  eNewDir = DIR_B;
	else if (fAngle >= 112.5f && fAngle < 157.5f)  eNewDir = DIR_LB;
	else if (fAngle >= 157.5f || fAngle < -157.5f) eNewDir = DIR_L;
	else if (fAngle >= -157.5f && fAngle < -112.5f) eNewDir = DIR_LT;
	else if (fAngle >= -112.5f && fAngle < -67.5f) eNewDir = DIR_T;
	else                                              eNewDir = DIR_RT;

	if (eNewDir != m_eDir)
		Direction_Change(eNewDir);
}
void CPlayer::Update_ClickEffect(float dt)
{
	if (!m_tClickEffect.bActive) return;

	m_tClickEffect.fScale -= dt * 2.f;  // 속도 조절
	if (m_tClickEffect.fScale <= 0.f)
	{
		m_tClickEffect.fScale = 0.f;
		m_tClickEffect.bActive = false;
	}
}

void CPlayer::Use_Item(int iSlot)
{
	CItemData* pItem = m_pInventory->Get_Item(iSlot);
	if (!pItem || pItem->Get_Type() != ITEM_USE) return;

	CItemData_UseItem* pUse = dynamic_cast<CItemData_UseItem*>(pItem);
	pUse->Use_Item(this);   // 효과 적용

	// 수량 감소 or 슬롯 제거
	if (m_pInventory->Get_StackCount(iSlot) > 1)
	{
		m_pInventory->Decrease_Stack(iSlot);
	}
	else
	{
		CItemData* pRemoved = m_pInventory->Remove_Item(iSlot);
		delete pRemoved;
	}
}

void CPlayer::Use_QuickSlot(int iSlot, CItemData_UseItem* pItem)
{

	for (int i = 0; i < INVEN_SIZE; ++i)
	{
		if (m_pInventory->Get_Item(i) == pItem)
		{
			Use_Item(i);
			return;
		}
	}
}

void CPlayer::Equip_Item(int iSlot)
{
	CItemData* pItem = m_pInventory->Get_Item(iSlot);
	if (!pItem || pItem->Get_Type() != ITEM_EQUIPMENT) return;

	CItemData_Equipment* pEquip = dynamic_cast<CItemData_Equipment*>(pItem);
	EQUIP_SLOT eSlot = pEquip->Get_EquipSlot();

	// 인벤토리에서 소유권 꺼냄
	m_pInventory->Remove_Item(iSlot);

	// 기존 아이템 있으면 인벤토리로 반환
	CItemData_Equipment* pPrev = m_pEquipment->Equip(eSlot, pEquip);
	if (pPrev)
		m_pInventory->Add_Item(pPrev);
}

void CPlayer::UnEquip_Item(EQUIP_SLOT eSlot)
{
	CItemData_Equipment* pItem = m_pEquipment->Get_Equipped(eSlot);
	if (!pItem) return;

	// 인벤토리 빈 슬롯 있는지 먼저 확인
	if (m_pInventory->Find_EmptySlot() == INVEN_FULL) return;  // 꽉 차있으면 해제 불가

	// 해제 후 인벤토리로
	pItem = m_pEquipment->UnEquip(eSlot);
	m_pInventory->Add_Item(pItem);
}

#ifdef GAME_DEBUG
void CPlayer::Debug_Render(ID2D1RenderTarget* pRT)
{
	Debug_DrawClickedTile(pRT);
	Debug_DrawClickPoint(pRT);
	Debug_DrawCollider(pRT);
	Debug_DrawText(pRT);
}

void CPlayer::Debug_DrawClickedTile(ID2D1RenderTarget* pRT)
{
	POINT tS = CCamera::Get_Instance()->IsoWorldToScreen(
		(float)m_iDebugTileX, (float)m_iDebugTileZ);

	ID2D1SolidColorBrush* pBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 0.f, 0.f), &pBrush);

	float cx = (float)tS.x, cy = (float)tS.y;
	float hw = TILE_WIDTH / 2.f, hh = TILE_HEIGHT / 2.f;

	pRT->DrawLine({ cx,      cy }, { cx + hw, cy + hh }, pBrush, 2.f);
	pRT->DrawLine({ cx + hw, cy + hh }, { cx,      cy + hh * 2 }, pBrush, 2.f);
	pRT->DrawLine({ cx,      cy + hh * 2 }, { cx - hw, cy + hh }, pBrush, 2.f);
	pRT->DrawLine({ cx - hw, cy + hh }, { cx,      cy }, pBrush, 2.f);

	pBrush->Release();
}

void CPlayer::Debug_DrawClickPoint(ID2D1RenderTarget* pRT)
{
	POINT tS = CCamera::Get_Instance()->IsoWorldToScreen(
		m_fDestWorldX, m_fDestWorldZ);

	ID2D1SolidColorBrush* pBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(0.f, 1.f, 0.f), &pBrush);

	float cx = (float)tS.x, cy = (float)tS.y;
	// 십자선
	pRT->DrawLine({ cx - 10.f, cy }, { cx + 10.f, cy }, pBrush, 2.f);
	pRT->DrawLine({ cx,        cy - 10.f }, { cx,        cy + 10.f }, pBrush, 2.f);

	pBrush->Release();
}

void CPlayer::Debug_DrawCollider(ID2D1RenderTarget* pRT)
{
	float fCX = Get_ColliderX();
	float fCZ = Get_ColliderZ();
	float fRX = m_tCollider.fRadiusX;
	float fRZ = m_tCollider.fRadiusZ;

	POINT tTL = CCamera::Get_Instance()->IsoWorldToScreen(fCX - fRX, fCZ - fRZ);
	POINT tTR = CCamera::Get_Instance()->IsoWorldToScreen(fCX + fRX, fCZ - fRZ);
	POINT tBR = CCamera::Get_Instance()->IsoWorldToScreen(fCX + fRX, fCZ + fRZ);
	POINT tBL = CCamera::Get_Instance()->IsoWorldToScreen(fCX - fRX, fCZ + fRZ);

	ID2D1SolidColorBrush* pBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(0.f, 1.f, 1.f), &pBrush);

	auto P = [](POINT p) { return D2D1::Point2F((float)p.x, (float)p.y); };

	pRT->DrawLine(P(tTL), P(tTR), pBrush, 2.f);
	pRT->DrawLine(P(tTR), P(tBR), pBrush, 2.f);
	pRT->DrawLine(P(tBR), P(tBL), pBrush, 2.f);
	pRT->DrawLine(P(tBL), P(tTL), pBrush, 2.f);

	pBrush->Release();
}

void CPlayer::Debug_DrawText(ID2D1RenderTarget* pRT)
{
	TCHAR szBuf[256];
	swprintf_s(szBuf, 256,
		L"클릭타일:[%d,%d] 내부위치:[%.2f,%.2f] 논리좌표:[%.2f,%.2f]",
		m_iDebugTileX, m_iDebugTileZ,
		m_fDebugLocalX, m_fDebugLocalZ,
		m_fDestWorldX, m_fDestWorldZ);

	TCHAR szPlayer[128];
	swprintf_s(szPlayer, 128,
		L"플레이어 월드:[%.2f, %.2f]  방향:%d  콜라이더 중심:[%.2f, %.2f]",
		m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ, (int)m_eDir,
		Get_ColliderX(), Get_ColliderZ());

	ID2D1SolidColorBrush* pBrush = nullptr;
	pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 0.f), &pBrush);

	IDWriteTextFormat* pFont = CImg_Manager::Get_Instance()->Get_DebugFont();

	pRT->DrawText(szBuf, wcslen(szBuf),
		pFont, D2D1::RectF(10.f, 10.f, 800.f, 30.f), pBrush);
	pRT->DrawText(szPlayer, wcslen(szPlayer),
		pFont, D2D1::RectF(10.f, 35.f, 800.f, 55.f), pBrush);

	TCHAR szInven[256];
	swprintf_s(szInven, 256,
		L"HP:%d/%d  MP:%d/%d  Gold:%d  ATK:%d  DEF:%d",
		m_iHp, m_iMaxHp,
		m_iMp, m_iMaxMp,
		m_pInventory->Get_Gold(),
		Get_TotalAtk(),
		Get_TotalDef());

	// 인벤토리 슬롯 0~4 상태
	TCHAR szSlot[256] = L"슬롯: ";
	for (int i = 0; i < 5; ++i)
	{
		CItemData* pItem = m_pInventory->Get_Item(i);
		if (pItem)
		{
			TCHAR szTemp[64];
			swprintf_s(szTemp, 64, L"[%d:%s x%d] ",
				i, pItem->Get_Name(),
				m_pInventory->Get_StackCount(i));
			lstrcat(szSlot, szTemp);
		}
		else
		{
			TCHAR szTemp[16];
			swprintf_s(szTemp, 16, L"[%d:빈슬롯] ", i);
			lstrcat(szSlot, szTemp);
		}
	}

	// 장착 상태
	TCHAR szEquip[128];
	CItemData_Equipment* pWeapon = m_pEquipment->Get_Equipped(SLOT_WEAPON);
	CItemData_Equipment* pArmor = m_pEquipment->Get_Equipped(SLOT_ARMOR);
	swprintf_s(szEquip, 128, L"무기:%s  갑옷:%s",
		pWeapon ? pWeapon->Get_Name() : L"없음",
		pArmor ? pArmor->Get_Name() : L"없음");



	pRT->DrawText(szInven, wcslen(szInven),
		pFont, D2D1::RectF(10.f, 60.f, 800.f, 80.f), pBrush);
	pRT->DrawText(szSlot, wcslen(szSlot),
		pFont, D2D1::RectF(10.f, 85.f, 1000.f, 105.f), pBrush);
	pRT->DrawText(szEquip, wcslen(szEquip),
		pFont, D2D1::RectF(10.f, 110.f, 800.f, 130.f), pBrush);

	pBrush->Release();
}
#endif


