#include "pch.h"
#include "Camera.h"
#include "GameObject.h"

CCamera* CCamera::m_pInstance = nullptr;

void CCamera::Update(float dt)
{
    if (!m_pTarget) return;

    ISO_INFO tInfo = m_pTarget->Get_IsoInfo(); // INFO  ISO_INFO

    // 플레이어 논리좌표 -> 스크린좌표 변환해서 카메라 추적
    POINT tScreen = IsoWorldToScreen(tInfo.fWorldX, tInfo.fWorldZ);

    // 플레이어를 화면 중앙으로
    float fTargetX = m_fX + tScreen.x - WINCX / 2.f;
    float fTargetY = m_fY + tScreen.y - WINCY / 2.f;

    // Lerp
    m_fX += (fTargetX - m_fX) * 8.f * dt;
    m_fY += (fTargetY - m_fY) * 8.f * dt;

    // 맵 범위 제한 (나중에 맵 크기 확정되면)
    // m_fX = max(0.f, min(m_fX, MAP_PIXEL_W - WINCX));
    // m_fY = max(0.f, min(m_fY, MAP_PIXEL_H - WINCY));
}

void CCamera::ScreenToIsoWorld(int iScreenX, int iScreenY,
    float& fOutWorldX, float& fOutWorldZ)
{
    float fMapX = iScreenX + m_fX;
    float fMapY = iScreenY + m_fY + TILE_HALF_H;  // 상단꼭짓점 기준 역변환

    fOutWorldX = (fMapX / TILE_HALF_W + fMapY / TILE_HALF_H) / 2.f;
    fOutWorldZ = (fMapY / TILE_HALF_H - fMapX / TILE_HALF_W) / 2.f;
}

POINT CCamera::IsoWorldToScreen(float fWorldX, float fWorldZ)
{
    POINT pt;
    pt.x = (LONG)((fWorldX - fWorldZ) * TILE_HALF_W - m_fX);
    // Y를 TILE_HALF_H만큼 올려서 상단 꼭짓점 기준으로
    pt.y = (LONG)((fWorldX + fWorldZ) * TILE_HALF_H - m_fY - TILE_HALF_H);
    return pt;
}

// 타일좌표 → 월드픽셀 (타일 렌더링 위치용)
POINT CCamera::TileToScreen(int iTileX, int iTileZ)
{
    return IsoWorldToScreen((float)iTileX, (float)iTileZ);
}

// 컬링 체크
bool CCamera::Is_InViewport(float fWorldX, float fWorldZ, float fCX, float fCY)
{
    POINT tScreen = IsoWorldToScreen(fWorldX, fWorldZ);

    if (tScreen.x + fCX / 2 < 0)     return false;
    if (tScreen.x - fCX / 2 > WINCX) return false;
    if (tScreen.y + fCY / 2 < 0)     return false;
    if (tScreen.y - fCY / 2 > WINCY) return false;

    return true;
}