#include "pch.h"
#include "Tile.h"
#include "Img_Manager.h"
#include "Camera.h"
#include "Input_Manager.h"

void CTile::Render(ID2D1RenderTarget* pRT)
{
    if (!CCamera::Get_Instance()->Is_InViewport(
        (float)m_iTileX, (float)m_iTileZ,
        (float)TILE_WIDTH, (float)TILE_HEIGHT))
        return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        (float)m_iTileX, (float)m_iTileZ);

    //HDC → ID2D1Bitmap*
    ID2D1Bitmap* pBitmap = nullptr;
    switch (m_eType)
    {
    case TILE_NORMAL: pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"TILE_NORMAL"); break;
    case TILE_BLOCK:  pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"TILE_BLOCK");  break;
    default: break;
    }
    if (!pBitmap) return;

    //GdiTransparentBlt → DrawBitmap (알파 자동 처리)
    float fLeft = tScreen.x - TILE_WIDTH / 2.f;
    float fTop = (float)tScreen.y;

    pRT->DrawBitmap(
        pBitmap,
        D2D1::RectF(fLeft, fTop, fLeft + TILE_WIDTH, fTop + TILE_HEIGHT),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
    );

#ifdef GAME_DEBUG
    POINT tMouse = CInput_Manager::Get_Instance()->Get_MousePos();
    float fHoverWorldX = 0.f, fHoverWorldZ = 0.f;
    CCamera::Get_Instance()->ScreenToIsoWorld(
        tMouse.x, tMouse.y, fHoverWorldX, fHoverWorldZ);
    int iHoverTileX = (int)floorf(fHoverWorldX);
    int iHoverTileZ = (int)floorf(fHoverWorldZ);

    if (iHoverTileX == m_iTileX && iHoverTileZ == m_iTileZ)
    {
        //마름모 하이라이트 (Polyline → DrawLine)
        ID2D1SolidColorBrush* pBrush = nullptr;
        pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 0.f), &pBrush);

        float cx = (float)tScreen.x;
        float cy = (float)tScreen.y;
        float hw = TILE_WIDTH / 2.f;
        float hh = TILE_HEIGHT / 2.f;

        pRT->DrawLine({ cx,      cy }, { cx + hw, cy + hh }, pBrush, 2.f);
        pRT->DrawLine({ cx + hw, cy + hh }, { cx,      cy + hh * 2 }, pBrush, 2.f);
        pRT->DrawLine({ cx,      cy + hh * 2 }, { cx - hw, cy + hh }, pBrush, 2.f);
        pRT->DrawLine({ cx - hw, cy + hh }, { cx,      cy }, pBrush, 2.f);

        //타일 좌표 텍스트 (TextOut → DrawText)
        //IDWriteTextFormat은 외부에서 전달받거나 임시 생성
        TCHAR szTile[64];
        swprintf_s(szTile, 64, L"[%d, %d]", m_iTileX, m_iTileZ);

        ID2D1SolidColorBrush* pTextBrush = nullptr;
        pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 0.f), &pTextBrush);

        pRT->DrawText(
            szTile, wcslen(szTile),
            CImg_Manager::Get_Instance()->Get_DebugFont(), // ← 아래 설명
            D2D1::RectF(cx - 20.f, cy + hh - 8.f, cx + 60.f, cy + hh + 20.f),
            pTextBrush
        );

        pBrush->Release();
        pTextBrush->Release();
    }
#endif
}