#include "pch.h"
#include "Monster.h"
#include "Camera.h"
#include "Img_Manager.h"

void CMonster::Initialize()
{
    m_bDead = false;
    m_fSpeed = 0.f;
    m_iHp = m_iMaxHp;
    m_eDir = DIR_B;
    m_fServerX = m_tIsoInfo.fWorldX;
    m_fServerZ = m_tIsoInfo.fWorldZ;

}


int CMonster::Update(float dt)
{
    if (m_bDead) return OBJ_DEAD;


    __super::Update_Rect();
    __super::Move_Frame();

    return OBJ_NOEVENT;
}

void CMonster::Late_Update(float dt) {}

void CMonster::Render(ID2D1RenderTarget* pRT)
{
    Render_HpBar(pRT);
    Render_NameTag(pRT);

}

void CMonster::Release() {}

// 서버 위치로 선형 보간
// 서버 패킷이 띄엄띄엄 와도 부드럽게 이동
void CMonster::Update_Lerp(float dt)
{
    float fDX = m_fServerX - m_tIsoInfo.fWorldX;
    float fDZ = m_fServerZ - m_tIsoInfo.fWorldZ;

    // 충분히 가까우면 스냅
    if (fabsf(fDX) < 0.01f && fabsf(fDZ) < 0.01f)
    {
        m_tIsoInfo.fWorldX = m_fServerX;
        m_tIsoInfo.fWorldZ = m_fServerZ;
        return;
    }

    m_tIsoInfo.fWorldX += fDX * m_fLerpSpeed * dt;
    m_tIsoInfo.fWorldZ += fDZ * m_fLerpSpeed * dt;
}

void CMonster::Render_Sprite(ID2D1RenderTarget* pRT, ID2D1Bitmap* pBitmap)
{
    if (!pBitmap) return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

    float fWidth = m_tIsoInfo.fCX * m_fScale;
    float fHeight = m_tIsoInfo.fCY * m_fScale;

    float fDrawX = tScreen.x - fWidth / 2.f;
    float fDrawY = tScreen.y - fHeight - m_tIsoInfo.fHeight + TILE_HALF_H;

    float fSrcX = m_tIsoInfo.fCX * m_tFrame.iFrameStart;
    float fSrcY = m_tIsoInfo.fCY * (int)m_eDir;

    pRT->DrawBitmap(pBitmap,
        D2D1::RectF(fDrawX, fDrawY, fDrawX + fWidth, fDrawY + fHeight),
        1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF(fSrcX, fSrcY, fSrcX + m_tIsoInfo.fCX, fSrcY + m_tIsoInfo.fCY)
    );
}

void CMonster::Render_HpBar(ID2D1RenderTarget* pRT)
{
    // CGameObject의 m_iHp / m_iMaxHp 그대로 사용
    if (m_iHp <= 0 || m_iHp >= m_iMaxHp) return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

    float fBarW = 60.f;
    float fBarH = 6.f;
    float fBarX = tScreen.x - fBarW / 2.f;
    float fBarY = tScreen.y - m_tIsoInfo.fCY - m_tIsoInfo.fHeight - 10.f + TILE_HALF_H;
    float fRatio = (float)m_iHp / (float)m_iMaxHp;

    ID2D1SolidColorBrush* pBrush = nullptr;

    pRT->CreateSolidColorBrush(D2D1::ColorF(0.6f, 0.f, 0.f), &pBrush);
    pRT->FillRectangle(
        D2D1::RectF(fBarX, fBarY, fBarX + fBarW, fBarY + fBarH), pBrush);
    pBrush->Release();

    pRT->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.8f, 0.f), &pBrush);
    pRT->FillRectangle(
        D2D1::RectF(fBarX, fBarY, fBarX + fBarW * fRatio, fBarY + fBarH), pBrush);
    pBrush->Release();
}

void CMonster::Render_NameTag(ID2D1RenderTarget* pRT)
{
    if (m_szName[0] == '\0') return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

    float fNameY = tScreen.y - m_tIsoInfo.fCY - m_tIsoInfo.fHeight - 20.f + TILE_HALF_H;

    ID2D1SolidColorBrush* pBrush = nullptr;
    pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 0.3f, 0.3f), &pBrush);  // 몬스터 이름 빨간색
    pRT->DrawText(m_szName, lstrlen(m_szName),
        CImg_Manager::Get_Instance()->Get_DebugFont(),
        D2D1::RectF(
            (float)tScreen.x - 40.f, fNameY,
            (float)tScreen.x + 40.f, fNameY + 20.f),
        pBrush);
    pBrush->Release();
}