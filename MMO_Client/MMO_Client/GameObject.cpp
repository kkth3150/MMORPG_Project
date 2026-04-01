#include "pch.h"
#include "GameObject.h"
#include "Camera.h"

CGameObject::CGameObject() : m_fSpeed(0.f), m_eDir(DIR_END), m_bDead(false), m_fAngle(0.f), m_pFrameKey(L"")
{
	ZeroMemory(&m_tIsoInfo, sizeof(ISO_INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update_Rect()
{
	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

	m_tRect.left = tScreen.x - (LONG)(m_tIsoInfo.fCX * 0.5f);
	m_tRect.top = tScreen.y - (LONG)(m_tIsoInfo.fCY * 0.5f);
	m_tRect.right = tScreen.x + (LONG)(m_tIsoInfo.fCX * 0.5f);
	m_tRect.bottom = tScreen.y + (LONG)(m_tIsoInfo.fCY * 0.5f);
}

void CGameObject::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwTime = GetTickCount();
	}
}

void CGameObject::Update_MouseRect()
{
	// 스프라이트 스크린 위치 기준으로 직교 RECT 계산
	POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
		m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

	// 스프라이트 좌상단 기준
	float fSpriteLeft = tScreen.x - m_tIsoInfo.fCX / 2.f;
	float fSpriteTop = tScreen.y - m_tIsoInfo.fCY
		- m_tIsoInfo.fHeight + TILE_HALF_H;

	// 오프셋 적용
	m_tMouseRect.left = (LONG)(fSpriteLeft + m_tMouseCollider.fOffsetX);
	m_tMouseRect.top = (LONG)(fSpriteTop + m_tMouseCollider.fOffsetY);
	m_tMouseRect.right = (LONG)(m_tMouseRect.left + m_tMouseCollider.fWidth);
	m_tMouseRect.bottom = (LONG)(m_tMouseRect.top + m_tMouseCollider.fHeight);
}