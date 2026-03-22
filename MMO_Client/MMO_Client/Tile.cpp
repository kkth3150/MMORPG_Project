#include "pch.h"
#include "Tile.h"
#include "Img_Manager.h"
#include "Camera.h"

void CTile::Render(HDC hDC)
{
    // 컬링 체크
    if (!CCamera::Get_Instance()->Is_InViewport(
        (float)m_iTileX, (float)m_iTileZ,
        (float)TILE_WIDTH, (float)TILE_HEIGHT))
        return;

    // 논리좌표 → 스크린좌표
    POINT tScreen = CCamera::Get_Instance()->IsoWorldToScreen(
        (float)m_iTileX, (float)m_iTileZ);

    HDC hBmpDC = nullptr;

    // 타입별 다른 이미지 (나중에 타입 추가 용이)
    switch (m_eType)
    {
    case TILE_NORMAL:
        hBmpDC = CImg_Manager::Get_Instance()->Find_Bmp(L"TILE_NORMAL");
        break;
    case TILE_BLOCK:
        hBmpDC = CImg_Manager::Get_Instance()->Find_Bmp(L"TILE_BLOCK");
        break;
    default:
        break;
    }

    if (!hBmpDC) return;

    GdiTransparentBlt(hDC,
        tScreen.x - TILE_WIDTH / 2,
        tScreen.y - TILE_HEIGHT / 2,
        TILE_WIDTH,
        TILE_HEIGHT,
        hBmpDC,
        0, 0,
        TILE_WIDTH,
        TILE_HEIGHT,
        RGB(0, 255, 255));
}