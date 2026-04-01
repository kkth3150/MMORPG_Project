#include "pch.h"
#include "Npc.h"
#include "Camera.h"
#include "Input_Manager.h"
#include "Img_Manager.h"

CNPC::CNPC() {}
CNPC::~CNPC() { Release(); }

void CNPC::Initialize()
{
    m_bDead = false;
    m_fSpeed = 0.5f;
}

int CNPC::Update(float dt)
{
    if (m_bInteractable)
    {
         POINT tMouse = CInput_Manager::Get_Instance()->Get_MousePos();
         if (Is_MouseCollide(tMouse)) {
             CInput_Manager::Get_Instance()->Set_CursorMode(CURSOR_TALK);
            if (CInput_Manager::Get_Instance()->Mouse_Down_Snap(MBUTTON_L))
                On_Click();
           
         }
         else {
             CInput_Manager::Get_Instance()->Set_CursorMode(CURSOR_NORMAL);
         }
    }


    if (m_bShowBubble)
    {
        m_fBubbleTimer -= dt;
        if (m_fBubbleTimer <= 0.f)
            m_bShowBubble = false;
    }
    
    __super::Update_Rect();
    __super::Move_Frame();
    Update_MouseRect();


    return OBJ_NOEVENT;
}

void CNPC::Late_Update(float dt)
{


}

void CNPC::Render(ID2D1RenderTarget* pRT)
{
    Render_NameTag(pRT);
    Render_Indicator(pRT);
}

void CNPC::Release() {}

void CNPC::Render_Sprite(ID2D1RenderTarget* pRT, ID2D1Bitmap* pBitmap)
{
    if (!pBitmap) return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

    float fDrawX = tScreen.x - m_tIsoInfo.fCX / 2.f;
    float fDrawY = tScreen.y - m_tIsoInfo.fCY - m_tIsoInfo.fHeight + TILE_HALF_H;
    float fSrcX = m_tIsoInfo.fCX * m_tFrame.iFrameStart;

    pRT->DrawBitmap(pBitmap,
        D2D1::RectF(fDrawX, fDrawY, fDrawX + m_tIsoInfo.fCX, fDrawY + m_tIsoInfo.fCY),
        1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF(fSrcX, 0.f, fSrcX + m_tIsoInfo.fCX, m_tIsoInfo.fCY)
    );
}

void CNPC::Render_NameTag(ID2D1RenderTarget* pRT)
{
    if (m_szName[0] == '\0') return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        m_tIsoInfo.fWorldX, m_tIsoInfo.fWorldZ);

    float fNameX = tScreen.x - 40.f;
    float fNameY = tScreen.y - m_tIsoInfo.fCY - m_tIsoInfo.fHeight - 20.f;

    ID2D1SolidColorBrush* pBg = nullptr;
    pRT->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 0.5f), &pBg);
    pRT->FillRoundedRectangle(
        D2D1::RoundedRect(
            D2D1::RectF(fNameX, fNameY, fNameX + 80.f, fNameY + 18.f), 4.f, 4.f),
        pBg);

    ID2D1SolidColorBrush* pText = nullptr;
    pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pText);
    pRT->DrawText(m_szName, lstrlen(m_szName),
        CImg_Manager::Get_Instance()->Get_DebugFont(),
        D2D1::RectF(fNameX, fNameY, fNameX + 80.f, fNameY + 18.f),
        pText);

    pBg->Release();
    pText->Release();
}

void CNPC::Render_Indicator(ID2D1RenderTarget* pRT)
{
    // 추후 구현
}

void CNPC::On_Collision(CGameObject* pOther)
{
    m_bInteractable = true;
}

void CNPC::On_CollisionEnd()
{
    m_bInteractable = false;
}

#ifdef GAME_DEBUG
void CNPC::Debug_Render(ID2D1RenderTarget* pRT)
{
    Debug_DrawCollider(pRT);
    Debug_DrawMouseCollider(pRT);
}

// 상호작용 범위 - 아이소메트릭 마름모
// 평소 초록, 인터렉티블 상태면 빨강
void CNPC::Debug_DrawCollider(ID2D1RenderTarget* pRT)
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
    // 인터렉티블 상태면 빨강, 아니면 초록
    if (m_bInteractable)
        pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 0.f, 0.f), &pBrush);
    else
        pRT->CreateSolidColorBrush(D2D1::ColorF(0.f, 1.f, 0.f), &pBrush);

    auto P = [](POINT p) { return D2D1::Point2F((float)p.x, (float)p.y); };

    pRT->DrawLine(P(tTL), P(tTR), pBrush, 2.f);
    pRT->DrawLine(P(tTR), P(tBR), pBrush, 2.f);
    pRT->DrawLine(P(tBR), P(tBL), pBrush, 2.f);
    pRT->DrawLine(P(tBL), P(tTL), pBrush, 2.f);

    pBrush->Release();
}

//마우스 클릭 박스 - 직교 사각형 (노란색)
void CNPC::Debug_DrawMouseCollider(ID2D1RenderTarget* pRT)
{
    ID2D1SolidColorBrush* pBrush = nullptr;
    pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 0.f), &pBrush);

    pRT->DrawRectangle(
        D2D1::RectF(
            (float)m_tMouseRect.left,
            (float)m_tMouseRect.top,
            (float)m_tMouseRect.right,
            (float)m_tMouseRect.bottom
        ),
        pBrush, 2.f
    );

    pBrush->Release();
}
#endif