#include "pch.h"
#include "Tile.h"
#include "Img_Manager.h"
#include "Camera.h"
#include "Input_Manager.h"

void CTile::Render(ID2D1RenderTarget* pRT)
{
    // 뷰포트 컬링
    if (!CCamera::Get_Instance()->Is_InViewport(
        (float)m_iTileX, (float)m_iTileZ,
        (float)TILE_WIDTH, (float)TILE_HEIGHT))
        return;

    // ImgKey로 이미지 검색
    // 존이 세팅한 키를 그대로 사용 → 존마다 다른 비주얼 자동 적용
    //if (m_pImgKey.empty()) return;
    ID2D1Bitmap* pBitmap = CImg_Manager::Get_Instance()->Find_Png(m_pImgKey);
    if (!pBitmap) return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        (float)m_iTileX, (float)m_iTileZ);

    float fLeft = tScreen.x - TILE_WIDTH / 2.f;
    float fTop = (float)tScreen.y;

    pRT->DrawBitmap(
        pBitmap,
        D2D1::RectF(fLeft, fTop, fLeft + TILE_WIDTH, fTop + TILE_HEIGHT),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
    );

#ifdef GAME_DEBUG
    // 마우스 호버 타일 하이라이트
    POINT tMouse = CInput_Manager::Get_Instance()->Get_MousePos();
    float fHoverX = 0.f, fHoverZ = 0.f;
    CCamera::Get_Instance()->ScreenToIsoWorld(tMouse.x, tMouse.y, fHoverX, fHoverZ);

    if ((int)floorf(fHoverX) == m_iTileX && (int)floorf(fHoverZ) == m_iTileZ)
    {
        ID2D1SolidColorBrush* pBrush = nullptr;
        pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 0.f), &pBrush);

        float cx = (float)tScreen.x;
        float cy = (float)tScreen.y;
        float hw = TILE_WIDTH / 2.f;
        float hh = TILE_HEIGHT / 2.f;

        // 마름모 외곽선
        pRT->DrawLine({ cx,      cy }, { cx + hw, cy + hh }, pBrush, 2.f);
        pRT->DrawLine({ cx + hw, cy + hh }, { cx,      cy + hh * 2 }, pBrush, 2.f);
        pRT->DrawLine({ cx,      cy + hh * 2 }, { cx - hw, cy + hh }, pBrush, 2.f);
        pRT->DrawLine({ cx - hw, cy + hh }, { cx,      cy }, pBrush, 2.f);

        // 좌표 텍스트
        TCHAR szTile[64];
        swprintf_s(szTile, 64, L"[%d,%d]", m_iTileX, m_iTileZ);

        ID2D1SolidColorBrush* pTextBrush = nullptr;
        pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 0.f), &pTextBrush);
        pRT->DrawText(
            szTile, wcslen(szTile),
            CImg_Manager::Get_Instance()->Get_DebugFont(),
            D2D1::RectF(cx - 20.f, cy + hh - 8.f, cx + 60.f, cy + hh + 20.f),
            pTextBrush
        );

        pBrush->Release();
        pTextBrush->Release();
    }
#endif
}