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
