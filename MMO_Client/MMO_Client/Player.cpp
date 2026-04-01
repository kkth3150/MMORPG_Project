#include "pch.h"
#include "Player.h"
#include "Img_Manager.h"
#include "Input_Manager.h"
#include "Camera.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	Motion_Change(PLAYER_IDLE);
	Direction_Change(DIR_B);
	Set_Collider(0.2f, 0.2f);
	m_fSpeed = 1.f;
	m_tIsoInfo.fWorldX = 5.f;
	m_tIsoInfo.fWorldZ = 5.f;

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

// ===================== 입력/이동 =====================

void CPlayer::Key_Input(float dt)
{
	if (CInput_Manager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT tMouse = CInput_Manager::Get_Instance()->Get_MousePos();
		CCamera::Get_Instance()->ScreenToIsoWorld(
			tMouse.x, tMouse.y,
			m_fDestWorldX, m_fDestWorldZ);

		m_tClickEffect.fWorldX = m_fDestWorldX;
		m_tClickEffect.fWorldZ = m_fDestWorldZ;
		m_tClickEffect.fScale = 1.f;
		m_tClickEffect.bActive = true;
		m_tClickEffect.color = D2D1::ColorF(0.f, 1.f, 0.f, 1.f); // R G B A

#ifdef GAME_DEBUG
		m_iDebugTileX = (int)floorf(m_fDestWorldX);
		m_iDebugTileZ = (int)floorf(m_fDestWorldZ);
		m_fDebugLocalX = m_fDestWorldX - (float)m_iDebugTileX;
		m_fDebugLocalZ = m_fDestWorldZ - (float)m_iDebugTileZ;
#endif

		m_bMoving = true;

		if (m_eCurState != PLAYER_WALK)
			Motion_Change(PLAYER_WALK);
	}

	if (m_bMoving)
		Move_To_Dest(dt);  // dt 전달
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

	pBrush->Release();
}
#endif


