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
	m_fSpeed = 1.f;
	Set_Collider(0.4f, 0.4f);
	m_tIsoInfo.fWorldX = 5.f;
	m_tIsoInfo.fWorldZ = 5.f;
}

int CPlayer::Update(float dt)
{
	Key_Input(dt);
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
}

void CPlayer::Release(void)
{
}


void CPlayer::Render_Sprite(HDC hDC, HDC PngDC)
{
	if (!PngDC) return;

	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

	// 발 위치가 논리좌표의 중심이므로
	// 스프라이트를 위로 올려서 발이 중심에 오게 함
	// fHeight만큼 픽셀로 변환해서 위로 올림
	int iFootOffsetY = (int)(m_tIsoInfo.fHeight);

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(hDC,
		tScreen.x - (int)(m_tIsoInfo.fCX / 2),
		tScreen.y - (int)(m_tIsoInfo.fCY) + iFootOffsetY, // 발 기준으로 위로 올림
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