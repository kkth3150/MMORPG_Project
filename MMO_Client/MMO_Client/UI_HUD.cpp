#include "pch.h"
#include "UI_HUD.h"
#include "Player.h"
#include "Img_Manager.h"

void CUI_HUD::Initialize()
{
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/UI/FrameFinal.png", L"HUD_FRAME");
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/UI/Hp_Bar.png", L"HUD_HP");
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/UI/Mp_Bar.png", L"HUD_MP");
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/UI/Exp_Bar.png", L"HUD_EXP");

    // 좌측 하단 고정
    Set_Pos(PANEL_W * 0.5f + 10.f, WINCY - PANEL_H * 0.5f - 10.f);
    Set_Size(PANEL_W, PANEL_H);
    Update_Rect();
}

int CUI_HUD::Update(float dt) { return OBJ_NOEVENT; }
void CUI_HUD::Late_Update(float dt) {}
void CUI_HUD::Release() {}

void CUI_HUD::Render(ID2D1RenderTarget* pRT)
{
    if (!m_pPlayer) return;

    float fL = (float)m_tRect.left;
    float fT = (float)m_tRect.top;


    ID2D1Bitmap* pFrame = CImg_Manager::Get_Instance()->Find_Png(L"HUD_FRAME");
    if (pFrame)
        pRT->DrawBitmap(pFrame,
            D2D1::RectF(fL, fT, fL + PANEL_W, fT + PANEL_H),
            1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

    // EXP 바
    float fExpRatio = (m_pPlayer->Get_MaxExp() > 0)
        ? (float)m_pPlayer->Get_Exp() / m_pPlayer->Get_MaxExp() : 0.f;
    Render_Bar(pRT, L"HUD_EXP", fL + EXP_X, fT + EXP_Y, BAR_W, BAR_H, fExpRatio);

    // HP 바
    float fHpRatio = (m_pPlayer->Get_MaxHP() > 0)
        ? (float)m_pPlayer->Get_HP() / m_pPlayer->Get_MaxHP() : 0.f;
    Render_Bar(pRT, L"HUD_HP", fL + HP_X, fT + HP_Y, BAR_W, BAR_H, fHpRatio);

    // MP 바
    float fMpRatio = (m_pPlayer->Get_MaxMP() > 0)
        ? (float)m_pPlayer->Get_MP() / m_pPlayer->Get_MaxMP() : 0.f;
    Render_Bar(pRT, L"HUD_MP", fL + MP_X, fT + MP_Y, BAR_W, BAR_H, fMpRatio);

 
}

void CUI_HUD::Render_Bar(ID2D1RenderTarget* pRT, const TCHAR* szKey,
    float fX, float fY, float fW, float fH, float fRatio)
{
    ID2D1Bitmap* pBitmap = CImg_Manager::Get_Instance()->Find_Png(szKey);
    if (!pBitmap) return;

    fRatio = max(0.f, min(1.f, fRatio));

    // 바를 비율만큼 잘라서 렌더
    float fBitmapW = pBitmap->GetSize().width;
    float fBitmapH = pBitmap->GetSize().height;

    pRT->DrawBitmap(pBitmap,
        D2D1::RectF(fX, fY, fX + fW * fRatio, fY + fH),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF(0.f, 0.f, fBitmapW * fRatio, fBitmapH));
}