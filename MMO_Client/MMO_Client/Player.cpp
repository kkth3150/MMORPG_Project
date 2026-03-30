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

void CPlayer::Render(HDC hDC)
{
	switch (m_eCurState)
	{
	case PLAYER_IDLE:	RenderIDLE(hDC);	break;
	case PLAYER_WALK:	RenderWALK(hDC);	break;
	case PLAYER_ATTACK:	RenderATTACK(hDC);	break;
	case PLAYER_HIT:	RenderHIT(hDC);		break;
	case PLAYER_DEAD:	RenderDEAD(hDC);	break;
	default: break;
	}

	Render_ClickEffect(hDC);
#ifdef GAME_DEBUG
	Debug_Render(hDC);
#endif
}

void CPlayer::Release(void)
{
}



void CPlayer::Render_Sprite(HDC hDC, HDC PngDC)
{
	if (!PngDC) return;

	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

	int iFootOffsetY = -(int)(m_tIsoInfo.fHeight);

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(hDC,
		tScreen.x - (int)(m_tIsoInfo.fCX / 2),
		tScreen.y - (int)(m_tIsoInfo.fCY) + iFootOffsetY + (int)TILE_HALF_H,
		(int)m_tIsoInfo.fCX,
		(int)m_tIsoInfo.fCY,
		PngDC,
		(int)(m_tIsoInfo.fCX * m_tFrame.iFrameStart),
		0,
		(int)m_tIsoInfo.fCX,
		(int)m_tIsoInfo.fCY,
		blend
	);
}
void CPlayer::RenderIDLE(HDC hDC)
{
	HDC PngDC = nullptr;
	switch (m_eDir)
	{
	case DIR_B:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_B");  break;
	case DIR_LB: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_LB"); break;
	case DIR_L:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_L");  break;
	case DIR_LT: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_LT"); break;
	case DIR_T:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_T");  break;
	case DIR_RT: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_RT"); break;
	case DIR_R:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_R");  break;
	case DIR_RB: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_IDLE_RB"); break;
	default: break;
	}
	Render_Sprite(hDC, PngDC);
}
void CPlayer::RenderWALK(HDC hDC)
{
	HDC PngDC = nullptr;
	switch (m_eDir)
	{
	case DIR_B:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_B");  break;
	case DIR_LB: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_LB"); break;
	case DIR_L:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_L");  break;
	case DIR_LT: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_LT"); break;
	case DIR_T:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_T");  break;
	case DIR_RT: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_RT"); break;
	case DIR_R:  PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_R");  break;
	case DIR_RB: PngDC = CImg_Manager::Get_Instance()->Find_Png(L"PLAYER_WALK_RB"); break;
	default: break;
	}
	Render_Sprite(hDC, PngDC);
}
void CPlayer::RenderATTACK(HDC hDC)
{
	// 추후 구현
}
void CPlayer::RenderHIT(HDC hDC)
{
	// 추후 구현
}
void CPlayer::RenderDEAD(HDC hDC)
{
	// 추후 구현
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
		m_tClickEffect.color = RGB(0,255,0);

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

#ifdef GAME_DEBUG
void CPlayer::Debug_Render(HDC hDC)
{
	Debug_DrawClickedTile(hDC);
	Debug_DrawClickPoint(hDC);
	Debug_DrawCollider(hDC);
	Debug_DrawText(hDC);
}

void CPlayer::Debug_DrawClickedTile(HDC hDC)
{
	POINT tTileScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		(float)m_iDebugTileX, (float)m_iDebugTileZ);

	HPEN   hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN   hOldPen = (HPEN)SelectObject(hDC, hRedPen);
	HBRUSH hNull = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hNull);

	POINT tDiamond[5] =
	{
		{ tTileScreen.x,                  tTileScreen.y                    },
		{ tTileScreen.x + TILE_WIDTH / 2, tTileScreen.y + TILE_HEIGHT / 2 },
		{ tTileScreen.x,                  tTileScreen.y + TILE_HEIGHT      },
		{ tTileScreen.x - TILE_WIDTH / 2, tTileScreen.y + TILE_HEIGHT / 2 },
		{ tTileScreen.x,                  tTileScreen.y                    },
	};
	Polyline(hDC, tDiamond, 5);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBr);
	DeleteObject(hRedPen);
}

void CPlayer::Debug_DrawClickPoint(HDC hDC)
{
	POINT tClickScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_fDestWorldX, m_fDestWorldZ);

	HPEN hGreenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hGreenPen);

	MoveToEx(hDC, tClickScreen.x - 10, tClickScreen.y, NULL);
	LineTo(hDC, tClickScreen.x + 10, tClickScreen.y);
	MoveToEx(hDC, tClickScreen.x, tClickScreen.y - 10, NULL);
	LineTo(hDC, tClickScreen.x, tClickScreen.y + 10);

	SelectObject(hDC, hOldPen);
	DeleteObject(hGreenPen);
}

void CPlayer::Debug_DrawCollider(HDC hDC)
{
	float fCX = Get_ColliderX();
	float fCZ = Get_ColliderZ();
	float fRX = m_tCollider.fRadiusX;
	float fRZ = m_tCollider.fRadiusZ;

	// AABB 네 모서리를 화면으로 변환 → 아이소메트릭에서 자동으로 마름모가 됨
	POINT tTL = CCamera::Get_Instance()->IsoWorldToScreen(fCX - fRX, fCZ - fRZ);
	POINT tTR = CCamera::Get_Instance()->IsoWorldToScreen(fCX + fRX, fCZ - fRZ);
	POINT tBR = CCamera::Get_Instance()->IsoWorldToScreen(fCX + fRX, fCZ + fRZ);
	POINT tBL = CCamera::Get_Instance()->IsoWorldToScreen(fCX - fRX, fCZ + fRZ);

	HPEN   hCyanPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	HPEN   hOldPen = (HPEN)SelectObject(hDC, hCyanPen);
	HBRUSH hNull = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hNull);

	POINT tCollider[5] = { tTL, tTR, tBR, tBL, tTL };
	Polyline(hDC, tCollider, 5);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBr);
	DeleteObject(hCyanPen);
}

void CPlayer::Debug_DrawText(HDC hDC)
{
	POINT tTileScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		(float)m_iDebugTileX, (float)m_iDebugTileZ);

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

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 0));
	TextOut(hDC, 10, 10, szBuf, lstrlen(szBuf));
	TextOut(hDC, 10, 30, szPlayer, lstrlen(szPlayer));
}
#endif 

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
void CPlayer::Render_ClickEffect(HDC hDC)
{
	if (!m_tClickEffect.bActive) return;

	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tClickEffect.fWorldX, m_tClickEffect.fWorldZ);

	// 타일 반경 기준으로 타원 크기 결정
	int iRX = (int)(TILE_HALF_W * 0.4f * m_tClickEffect.fScale);
	int iRY = (int)(TILE_HALF_H * 0.4f * m_tClickEffect.fScale);

	if (iRX <= 1 || iRY <= 1) return;

	HPEN hPen = CreatePen(PS_SOLID, 2, m_tClickEffect.color);
	HPEN   hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hNull = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hNull);

	Ellipse(hDC,
		tScreen.x - iRX,
		tScreen.y - iRY,
		tScreen.x + iRX,
		tScreen.y + iRY);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBr);
	DeleteObject(hPen);
}