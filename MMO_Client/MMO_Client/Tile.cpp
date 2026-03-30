#include "pch.h"
#include "Tile.h"
#include "Img_Manager.h"
#include "Camera.h"
#include "Input_Manager.h"

void CTile::Render(HDC hDC)
{
    if (!CCamera::Get_Instance()->Is_InViewport(
        (float)m_iTileX, (float)m_iTileZ,
        (float)TILE_WIDTH, (float)TILE_HEIGHT))
        return;

    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        (float)m_iTileX, (float)m_iTileZ);

    HDC hBmpDC = nullptr;
    switch (m_eType)
    {
    case TILE_NORMAL: hBmpDC = CImg_Manager::Get_Instance()->Find_Bmp(L"TILE_NORMAL"); break;
    case TILE_BLOCK:  hBmpDC = CImg_Manager::Get_Instance()->Find_Bmp(L"TILE_BLOCK");  break;
    default: break;
    }
    if (!hBmpDC) return;

    GdiTransparentBlt(hDC,
        tScreen.x - TILE_WIDTH / 2,
        tScreen.y,
        TILE_WIDTH, TILE_HEIGHT,
        hBmpDC,
        0, 0, TILE_WIDTH, TILE_HEIGHT,
        RGB(0, 255, 255));

#ifdef GAME_DEBUG
    // 마우스 호버 타일 좌표 표시
    POINT tMouse = CInput_Manager::Get_Instance()->Get_MousePos();
    float fHoverWorldX = 0.f, fHoverWorldZ = 0.f;
    CCamera::Get_Instance()->ScreenToIsoWorld(
        tMouse.x, tMouse.y, fHoverWorldX, fHoverWorldZ);

    int iHoverTileX = (int)floorf(fHoverWorldX);
    int iHoverTileZ = (int)floorf(fHoverWorldZ);

    if (iHoverTileX == m_iTileX && iHoverTileZ == m_iTileZ)
    {
        // 마름모 하이라이트
        HPEN hYellowPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
        HPEN hOldPen = (HPEN)SelectObject(hDC, hYellowPen);
        HBRUSH hNull = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hNull);

        POINT tDiamond[5] =
        {
            { tScreen.x,                  tScreen.y                    },
            { tScreen.x + TILE_WIDTH / 2, tScreen.y + TILE_HEIGHT / 2 },
            { tScreen.x,                  tScreen.y + TILE_HEIGHT      },
            { tScreen.x - TILE_WIDTH / 2, tScreen.y + TILE_HEIGHT / 2 },
            { tScreen.x,                  tScreen.y                    },
        };
        Polyline(hDC, tDiamond, 5);

        SelectObject(hDC, hOldPen);
        SelectObject(hDC, hOldBr);
        DeleteObject(hYellowPen);

        // 타일 월드좌표 텍스트 (타일 중앙에)
        TCHAR szTile[64];
        swprintf_s(szTile, 64, L"[%d, %d]", m_iTileX, m_iTileZ);

        POINT tCenter = { tScreen.x, tScreen.y + TILE_HEIGHT / 2 };
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, RGB(255, 255, 0));
        TextOut(hDC, tCenter.x - 20, tCenter.y - 8, szTile, lstrlen(szTile));
    }
#endif
}